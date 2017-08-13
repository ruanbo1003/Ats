/*
 * Subscribe.cpp
 *
 *  Created on: 2017��8��11��
 *      Author: ruanbo
 */

#include "AtsQuote.hpp"


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

void AtsQuote::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LogLine("OnRspSubMarketData ");
    showRspInfo(pRspInfo, nRequestID, bIsLast);

    Log("��Լid��%s", pSpecificInstrument->InstrumentID);
}

void AtsQuote::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LogFunc;
}

void AtsQuote::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    Log("AtsQuote::OnRtnDepthMarketData:%s", pDepthMarketData->InstrumentID);
}



