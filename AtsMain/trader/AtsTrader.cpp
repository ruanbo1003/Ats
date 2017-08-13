/*
 * AtsTrader.cpp
 *
 *  Created on: Feb 6, 2017
 *      Author: ruanbo
 */

#include <AtsMain/trader/AtsTrader.hpp>
#include "comm/comm.hpp"
#include "utils/utils.hpp"

AtsTrader::AtsTrader()
{

}

AtsTrader::~AtsTrader()
{

}

void AtsTrader::init_func()
{
//    _init_funcs[1] = reqAllInstrument;
//    _init_funcs[2] = reqTradingAccount;
//    _init_funcs[3] = reqInvestorPosition;

	_init_funcs[1] = std::bind(&AtsTrader::reqAllInstrument, this);
	_init_funcs[2] = std::bind(&AtsTrader::reqTradingAccount, this);
	_init_funcs[3] = std::bind(&AtsTrader::reqInvestorPosition, this);
	_init_funcs[4] = std::bind(&AtsTrader::reqOneDepthMarketData, this);
	_init_funcs[5] = std::bind(&AtsTrader::onOrderInsert, this);
}
void AtsTrader::init_vals()
{
	_broker_id = "";
	_investor_id = "";
	_passwd = "";

	_pUserApi = NULL;
	_requestId = 0;

	_ats_status = ATS_Null;

	_run_secs = 0;
	_settlement_path = "/home/ruanbo/Codes/Ats/AtsData/Settlement/";
	_run_path = utils::get_path();

	_time_op = TimeOpPtr(new TimeOp());
	_cc = CodeConvertPtr(new CodeConvert("latin1", "utf-8"));
	_order_mngr = OrderMngrPtr(new OrderMngr());

	_mysql = MysqlDbPtr(new MysqlDb(_config->_mysql_addr, _config->_mysql_user, _config->_mysql_passwd));

	init_func();
}

void AtsTrader::setstatus(AtsStatus status)
{
	_ats_status = status;
}
AtsStatus AtsTrader::status() const
{
	return _ats_status;
}

// 初始化及运行，停止接口
bool AtsTrader::init(const AtsConfigPtr& config)
{
	LogTid();

	_config = config;

	init_vals();

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
//	Log("AtsTrader::uninit");

	LogTid();

//    _pUserApi->RegisterFront(NULL);
	_pUserApi->RegisterSpi(NULL);

	_pUserApi->Release();
	_pUserApi = NULL;

	return true;
}

bool AtsTrader::run()
{
	LogTid();

	_pUserApi->RegisterSpi(this);
	_pUserApi->RegisterFront((char*) _config->_trader_front.data());
	_pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);
	_pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);

	_pUserApi->Init();

	Log("after trader api init");

	return true;
}


bool AtsTrader::stop()
{
//	Log("AtsTrader::stop");

	LogTid();

	if(ATS_Logined <= status() && status() <= ATS_Confirmed)
	{
		reqLogout();
	}

	return true;
}


bool AtsTrader::can_quit() const
{
	return status() == ATS_Quit;
}

void AtsTrader::quit()
{
	uninit();
}

void AtsTrader::on_next_second()
{
	check_login();

	check_settlement();

	if(status() < ATS_Confirmed)
	{
		return;
	}
	_run_secs++;

//	Log("on_next_second:%ld, status:%d", _run_secs, _ats_status);

	auto func_it = _init_funcs.find(_run_secs);
	if(func_it != _init_funcs.end())
	{
		auto one_init_f = func_it->second;
		one_init_f();
	}

	if(_mysql && _mysql->isOk() == false)
	{
		_mysql->reconnect();
	}
}

void AtsTrader::on_next_minute()
{
	Log("on_next_minute, status:%d", _ats_status);

	LogTid();

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
	strcpy(req.UserID, _investor_id.data());  //
	strcpy(req.Password, _passwd.data());

	int ret = _pUserApi->ReqUserLogin(&req, ++_requestId);

	if(ret == 0)
	{
		Log("send login request ok.\n");
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
	strcpy(req.UserID, _investor_id.data());   //

	int ret = _pUserApi->ReqUserLogout(&req, ++_requestId);

	Log("AtsTrader::reqLogout. ret:%d", ret);
	if(ret == 0)
	{
		setstatus(ATS_Stoping);
	}
}

void AtsTrader::OnFrontConnected()
{
	LogTid();

	Log("AtsTrader::OnFrontConnected\n");

	if(status() == ATS_Stoping || status() == ATS_Quit)
	{
		return;
	}

	setstatus(ATS_Connected);
}

void AtsTrader::OnFrontDisconnected(int nReason)
{
	LogTid();

	if(status() == ATS_Stoping || status() == ATS_Quit)
	{
		return;
	}

	setstatus(ATS_Null);

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
		Log("FrontID前置编号:%d", pRspUserLogin->FrontID);
		Log("SessionID会话编号:%d", pRspUserLogin->SessionID);
		Log("MaxOrderRef最大报单编号:%s", pRspUserLogin->MaxOrderRef);
		Log("SHFETime:%s", pRspUserLogin->SHFETime);
		Log("DCETime:%s", pRspUserLogin->DCETime);
		Log("CZCETime:%s", pRspUserLogin->CZCETime);
		Log("FFEXTime:%s", pRspUserLogin->FFEXTime);
		Log("INETime:%s", pRspUserLogin->INETime);
	}

	Log("AtsTrader::OnRspUserLogin, ErrorId:%d", pRspInfo->ErrorID);

	if(pRspInfo->ErrorID == 0 && bIsLast == true)
	{
		setstatus(ATS_Logined);

		Log("Ats Trader Login success\n");
	}
	else
	{
		Log("Ats Trader Login failed\n");
		return;
	}

//    reqSettlementInfo();
//    loop();

//    showRspInfo(pRspInfo, nRequestID, bIsLast);
}

void AtsTrader::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	Log("AtsTrader::OnRspUserLogout. ErrorId:%d, bIsLast:%d", pRspInfo->ErrorID, bIsLast);

	if(pRspInfo->ErrorID == 0 && bIsLast == true)
	{
		setstatus(ATS_Quit);
	}
}

bool AtsTrader::is_login() const
{
	return (status() == ATS_Logined);
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

