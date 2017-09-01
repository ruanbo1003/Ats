/*
 * Subscribe.cpp
 *
 *  Created on: 2017年8月11日
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
        strcpy(instrumentIds[i], "IF1712");
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

    Log("合约id：%s", pSpecificInstrument->InstrumentID);
}

void AtsQuote::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LogFunc;
}

void AtsQuote::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if(!pDepthMarketData)
	{
		return;
	}

    Log("AtsQuote::OnRtnDepthMarketData:%s", pDepthMarketData->InstrumentID);

    clean_tick_data(pDepthMarketData);
    _mysql->tick_data(pDepthMarketData);
}

void AtsQuote::clean_tick_data(CThostFtdcDepthMarketDataField *data)
{
	DoubleCheck(data->LastPrice);
	DoubleCheck(data->PreSettlementPrice);
	DoubleCheck(data->PreClosePrice);
	DoubleCheck(data->PreOpenInterest);
	DoubleCheck(data->OpenPrice);
	DoubleCheck(data->HighestPrice);
	DoubleCheck(data->LowestPrice);
	DoubleCheck(data->Turnover);
	DoubleCheck(data->OpenInterest);
	DoubleCheck(data->ClosePrice);
	DoubleCheck(data->SettlementPrice);
	DoubleCheck(data->UpperLimitPrice);
	DoubleCheck(data->LowerLimitPrice);
	DoubleCheck(data->PreDelta);
	DoubleCheck(data->CurrDelta);

	DoubleCheck(data->BidPrice1);
	DoubleCheck(data->AskPrice1);
	DoubleCheck(data->BidPrice2);
	DoubleCheck(data->AskPrice2);
	DoubleCheck(data->BidPrice3);
	DoubleCheck(data->AskPrice3);
	DoubleCheck(data->BidPrice4);
	DoubleCheck(data->AskPrice4);
	DoubleCheck(data->BidPrice5);
	DoubleCheck(data->AskPrice5);
}



