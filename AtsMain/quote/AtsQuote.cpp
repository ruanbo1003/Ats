/*
 * AtsQuote.cpp
 *
 *  Created on: Feb 4, 2017
 *      Author: ruanbo
 */

#include <AtsMain/quote/AtsQuote.hpp>
#include "comm/comm.hpp"

AtsQuote::AtsQuote(CThostFtdcMdApi *api)
{
    _pUserApi = api;
    _requestId = 0;
    _is_login = false;

    LogFunc;
}

AtsQuote::~AtsQuote()
{
}

void AtsQuote::OnFrontConnected()
{
    Log("AtsQuote::OnFrontConnected");

    ReqUserLogin();
}

void AtsQuote::ReqUserLogin()
{
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, "9999");
    strcpy(req.UserID, "082609");   // 18575651049
    strcpy(req.Password, "ruanbo1003");

    int ret = _pUserApi->ReqUserLogin(&req, ++_requestId);

    Log("ReqUserLogin ret:%d", ret);
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
    strcpy(req.UserID, "082609");   // 18575651049  082609
//    strcpy(req.BrokerID, "");   //9999
//    strcpy(req.UserID, "");

    int ret = _pUserApi->ReqUserLogout(&req, ++_requestId);

    Log("AtsQuote::ReqUserLogout, ret:%d", ret);
}

void AtsQuote::OnFrontDisconnected(int nReason)
{
    LogFunc;
}

void AtsQuote::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LogFunc;

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

    if(pRspInfo)
    {
        Log("ErrorID:%d", pRspInfo->ErrorID);
//        Log("ErrorMsg:%s", pRspInfo->ErrorMsg);
        std::cout << "ErrorMsg:" << pRspInfo->ErrorMsg << std::endl;
    }

    Log("nRequestID:%d", nRequestID);
    Log("bIsLast:%d", bIsLast);

    if(pRspInfo && pRspInfo->ErrorID == 0)
    {
        Log("ctp trader user login success.\n");
        _is_login = true;
        OnReqAllInstrument();
    }
}

void AtsQuote::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    _is_login = false;
    LogFunc;
}

bool AtsQuote::is_login()const
{
    return _is_login;
}

void AtsQuote::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LogLine("OnRspSubMarketData ");
    showRspInfo(pRspInfo, nRequestID, bIsLast);

    Log("合约id：%s", pSpecificInstrument->InstrumentID);
}

void AtsQuote::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LogFunc;
}

void AtsQuote::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    Log("AtsQuote::OnRtnDepthMarketData:%s", pDepthMarketData->InstrumentID);
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

void AtsQuote::OnReqAllInstrument()
{
    int nCount = 1;
    char* instrumentIds[nCount];

    for(int i=0; i<nCount; ++i)
    {
        instrumentIds[i] = new char[32];
        memset(instrumentIds[i], 0, 32);
        strcpy(instrumentIds[i], "IF1709");
    }

    int ret = _pUserApi->SubscribeMarketData(instrumentIds, 1);

    Log("OnReqAllInstrument ret:%d", ret);

    for(int i=0; i<nCount; ++i)
    {
        delete[] instrumentIds[i];
    }

}














