/*
 * AtsQuote.cpp
 *
 *  Created on: Feb 4, 2017
 *      Author: ruanbo
 */

#include <AtsMain/quote/AtsQuote.hpp>
#include "comm/comm.hpp"

AtsQuote::AtsQuote()
{
    _pUserApi = NULL;
    _requestId = 0;
    _is_login = false;

    LogFunc;
}

AtsQuote::~AtsQuote()
{
}


bool AtsQuote::init(const AtsConfigPtr& config)
{
	_config = config;

	_pUserApi = CThostFtdcMdApi::CreateFtdcMdApi("quoteData/", false);
    if(!_pUserApi)
    {
        LogError("创建MdApi出错");
        return false;
    }

    _pUserApi->RegisterSpi(this);

    Log("quote front:%s", _config->_quote_front.data());

    _pUserApi->RegisterFront((char*)_config->_quote_front.data());
    _pUserApi->Init();

	return true;
}

void AtsQuote::unit()
{
	Log("AtsQuote::unit thread id:%ld\n", gettid());

	_pUserApi->RegisterSpi(NULL);
	_pUserApi->Release();
	_pUserApi = NULL;
}

void AtsQuote::run()
{
//	sleep(30);
//	OnReqAllInstrument();
}

void AtsQuote::stop()
{
	unit();
}

void AtsQuote::OnFrontConnected()
{
    Log("AtsQuote::OnFrontConnected");

    Log("AtsQuote thread id:%ld\n", gettid());

//    ReqUserLogin();
}

void AtsQuote::OnFrontDisconnected(int nReason)
{
    LogFunc;
}



void AtsQuote::ReqUserLogin()
{
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, "9999");
    strcpy(req.UserID, "082609");   //
    strcpy(req.Password, "ruanbo1003");

    int ret = _pUserApi->ReqUserLogin(&req, ++_requestId);

    Log("ReqUserLogin ret:%d", ret);
}

void AtsQuote::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LogFunc;

    if(pRspInfo && pRspInfo->ErrorID != 0)
    {
        Log("ctp trader user login failed, ErrorId:%d.\n", pRspInfo->ErrorID);
        _is_login = true;
    }

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

    Log("nRequestID:%d", nRequestID);
    Log("bIsLast:%d", bIsLast);

    if(pRspInfo && pRspInfo->ErrorID == 0)
    {
        Log("ctp trader user login success.\n");
        _is_login = true;
    }
}

void AtsQuote::ReqUserLogout()
{
    if(is_login() == false)
    {
        return;
    }

    CThostFtdcUserLogoutField req;
    memset(&req, 0, sizeof(req));

    strcpy(req.BrokerID, "9999");   //9999
    strcpy(req.UserID, "082609");   //
//    strcpy(req.BrokerID, "");   //9999
//    strcpy(req.UserID, "");

    int ret = _pUserApi->ReqUserLogout(&req, ++_requestId);

    Log("AtsQuote::ReqUserLogout, ret:%d", ret);
}

void AtsQuote::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(pRspInfo && pRspInfo->ErrorID != 0)
    {
    	Log("AtsQuote Logout返回错误，ErrorId:%d \n", pRspInfo->ErrorID);
    	return;
    }

    Log("AtsQuote Logout successed");

    _is_login = false;
}

bool AtsQuote::is_login()const
{
    return _is_login;
}


void AtsQuote::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    Log("AtsQuote::OnRspError, ErrorId:%d, %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
}

void AtsQuote::OnHeartBeatWarning(int nTimeLapse)
{
    LogFunc;
}

void AtsQuote::showRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    Log("ErrorID:%d, ErrorMsg:%s, nRequestID:%d, bIsLast:%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
}
















