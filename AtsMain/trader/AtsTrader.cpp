/*
 * AtsTrader.cpp
 *
 *  Created on: Feb 6, 2017
 *      Author: ruanbo
 */

#include <AtsMain/trader/AtsTrader.hpp>
#include "comm/comm.hpp"


AtsTrader::AtsTrader(const string& front)
{
    _front_addr = front;
    init_vals();
}

AtsTrader::~AtsTrader()
{

}

void AtsTrader::init_vals()
{
    _broker_id = "";
    _investor_id = "";
    _passwd = "";

    _pUserApi = NULL;
    _requestId = 0;

    _ats_status = ATS_Null;
    _is_stop = false;   // 系统开始运行，

    _run_secs = 0;

    _time_op = TimeOpPtr(new TimeOp());
    _cc = CodeConvertPtr(new CodeConvert("gb2312", "utf-8"));
    _order_mngr = OrderMngrPtr(new OrderMngr());



    _mysql = MysqlDbPtr(new MysqlDb());
}

void AtsTrader::update_status(AtsStatus status)
{
    _ats_status = status;
}
AtsStatus AtsTrader::status()const
{
    return _ats_status;
}

// 初始化及运行，停止接口
bool AtsTrader::init()
{
    if(_mysql->init() == false)
    {
        Log("MysqlDb init failed");
        return false;
    }

    _pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("traderData/");
    if(!_pUserApi)
    {
        LogError("创建 TraderApi出错");
        return false;
    }

    _broker_id = "9999";
    _investor_id = "082609";
    _passwd = "ruanbo1003";

    Log("AtsTrader init success");

    return true;
}

bool AtsTrader::uninit()
{
    Log("AtsTrader::uninit");

//    _pUserApi->RegisterFront(NULL);
    _pUserApi->RegisterSpi(NULL);

    _pUserApi->Release();
    _pUserApi = NULL;

    return true;
}

bool AtsTrader::run()
{
    _pUserApi->RegisterSpi(this);
    _pUserApi->RegisterFront((char*)_front_addr.data());
    _pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);
    _pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);

    _pUserApi->Init();

    Log("after trader api init");

    return true;
}

bool AtsTrader::stop()
{
    Log("AtsTrader::stop");

    _is_stop = true;

    uninit();

    return true;
}

bool AtsTrader::is_stop()const
{
    return _is_stop;
}

void AtsTrader::on_next_second()
{
    _run_secs++;
    Log("on_next_second:%d, status:%d", _run_secs, _ats_status);

    check_settlement();
    check_login();

    if(_run_secs == 10)
    {
        onOrderInsert();
    }
}

void AtsTrader::on_next_minute()
{
    Log("on_next_minute");

//    reqSettlementInfo();
}

void AtsTrader::on_next_hour()
{

}


void AtsTrader::check_login()
{
    if(status() != ATS_Connected)
    {
        return;
    }

    reqLogin();
}

void AtsTrader::reqLogin()
{
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, _broker_id.data());
    strcpy(req.UserID, _investor_id.data());  // 18575651049
    strcpy(req.Password, _passwd.data());

    int ret = _pUserApi->ReqUserLogin(&req, ++_requestId);

    if(ret == 0)
    {
        Log("send login request ok.");
    }
    else if(ret == -1)
    {
        LogError("send long request failed because of network");
    }
    else if(ret == -2)
    {
        LogError("send login request failed because of too many requests");
    }
    else if(ret == -3)
    {
        LogError("send login request failed because of too many requests per second");
    }
    else
    {
        LogError("send login request, Unknow return parameter");
    }
}

void AtsTrader::reqAuthenticate()
{
    CThostFtdcReqAuthenticateField req;
    memset(&req, 0, sizeof(CThostFtdcReqAuthenticateField));

    strcpy(req.BrokerID, _broker_id.data());
    strcpy(req.UserID, _investor_id.data());
    strcpy(req.UserProductInfo, "");
    strcpy(req.AuthCode, "");

    int ret = _pUserApi->ReqAuthenticate(&req, ++_requestId);

    Log("AtsTrader::reqAuthenticate, ret:%d", ret);
}

void AtsTrader::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    Log("AtsTrader::OnRspAuthenticate. ErrorId:%d, bIsLast:%d", pRspInfo->ErrorID, bIsLast);
    if(pRspAuthenticateField)
    {
        Log("BrokerID:%s", pRspAuthenticateField->BrokerID);
        Log("UserID:%s", pRspAuthenticateField->UserID);
        Log("UserProductInfo:%s", pRspAuthenticateField->UserProductInfo);
    }

}

void AtsTrader::reqLogout()
{
    CThostFtdcUserLogoutField req;
    memset(&req, 0, sizeof(CThostFtdcUserLogoutField));

    strcpy(req.BrokerID, _broker_id.data());
    strcpy(req.UserID, _investor_id.data());   // 18575651049

    int ret = _pUserApi->ReqUserLogout(&req, ++_requestId);

    Log("AtsTrader::reqLogout. ret:%d", ret);
}

void AtsTrader::OnFrontConnected()
{
    Log("AtsTrader::OnFrontConnected");

    update_status(ATS_Connected);

    if(is_stop() == false)
    {
        //reqAuthenticate();
        reqLogin();
    }

}

void AtsTrader::OnFrontDisconnected(int nReason)
{
    update_status(ATS_Null);

    Log("AtsTrader::OnFrontDisconnected:%d", nReason);

    if(nReason == 0x1001)
    {
        Log("网络读失败");
    }
    else if(nReason == 0x1002)
    {
        Log("网络写失败");
    }
    else if(nReason == 0x2001)
    {
        Log("接收心跳超时");
    }
    else if(nReason == 0x2002)
    {
        Log("发送心跳失败");
    }
    else if(nReason == 0x2003)
    {
        Log("收到错误报文");
    }
    else
    {
        Log("未知原因断开连接。")
        LogError("未知原因断开连接。")
    }
}

void AtsTrader::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(pRspUserLogin)
    {
        Log("TradingDay:%s", pRspUserLogin->TradingDay);
        Log("LoginTime:%s", pRspUserLogin->LoginTime);
        Log("BrokerID:%s", pRspUserLogin->BrokerID);
        Log("UserID:%s", pRspUserLogin->UserID);
        Log("SystemName:%s", pRspUserLogin->SystemName);
        Log("FrontID:%d", pRspUserLogin->FrontID);
        Log("SessionID:%d", pRspUserLogin->SessionID);
        Log("MaxOrderRef:%s", pRspUserLogin->MaxOrderRef);
        Log("SHFETime:%s", pRspUserLogin->SHFETime);
        Log("DCETime:%s", pRspUserLogin->DCETime);
        Log("CZCETime:%s", pRspUserLogin->CZCETime);
        Log("FFEXTime:%s", pRspUserLogin->FFEXTime);
        Log("INETime:%s", pRspUserLogin->INETime);
    }

    Log("AtsTrader::OnRspUserLogin, ErrorId:%d", pRspInfo->ErrorID);

    if(pRspInfo->ErrorID == 0 && bIsLast == true)
    {
        update_status(ATS_Logined);

        Log("Ats Trader Login success");
    }
    else
    {
        Log("Ats Trader Login failed");
        return;
    }

    reqTradingAccount();

//    reqSettlementInfo();
//    loop();

//    showRspInfo(pRspInfo, nRequestID, bIsLast);
}

void AtsTrader::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    Log("AtsTrader::OnRspUserLogout. ErrorId:%d, bIsLast:%d", pRspInfo->ErrorID, bIsLast);

    if(pRspInfo->ErrorID == 0 && bIsLast == true)
    {
        update_status(ATS_Connected);
    }
}

bool AtsTrader::is_login()const
{
    return (status() == ATS_Logined);
}



// 查询资金账户
void AtsTrader::reqTradingAccount()
{
    CThostFtdcQryTradingAccountField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, _broker_id.data());
    strcpy(req.InvestorID, _investor_id.data());
    strcpy(req.CurrencyID, "CNY");

    int ret = _pUserApi->ReqQryTradingAccount(&req, ++_requestId);
    LogFunc;
    Log("ret:%d", ret);
}
void AtsTrader::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(pRspInfo && pRspInfo->ErrorID != 0)
    {
        LogError("OnRspQryTradingAccount failed");
        return;
    }

    if(!pTradingAccount)
    {
        LogError("OnRspQryTradingAccount rsp failed");
        return;
    }

    Log("===OnRspQryTradingAccount查询资金账户 begin===");
    Log("BrokerID %s", pTradingAccount->BrokerID);
    Log("AccountID %s", pTradingAccount->AccountID);
    Log("上次存款额:%f", pTradingAccount->PreDeposit);
    Log("期货结算准备金:%f", pTradingAccount->Balance);
    Log("可用资金:%f", pTradingAccount->Available);
    Log("基本准备金:%f", pTradingAccount->Reserve);
    Log("===OnRspQryTradingAccount查询资金账户 end===\n");
}

// 持仓
void AtsTrader::reqInvestorPosition()
{

}


//错误应答
void AtsTrader::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    Log("OnRspError 错误应答:[%d,%s], nRequestId:%d, bIsLast:%d\n", pRspInfo->ErrorID, onCC(pRspInfo->ErrorMsg), nRequestID, bIsLast);
}
void AtsTrader::showRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    Log("ErrorID:%d, ErrorMsg:%s, nRequestID:%d, bIsLast:%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
}









