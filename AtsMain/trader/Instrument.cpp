/*
 * Instrument.cpp
 *
 *  Created on: Aug 1, 2017
 *      Author: ruanbo
 */

#include <AtsMain/trader/AtsTrader.hpp>

/*
 * 合约
 */


void AtsTrader::reqAllInstrument()
{
    CThostFtdcQryInstrumentField req;
    memset(&req, 0, sizeof(req));

    int ret = _pUserApi->ReqQryInstrument(&req, ++_requestId);
    if(ret == 0)
    {
        Log("请求所有合约ret ok");
    }
}

void AtsTrader::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pInstrument)
    {
        Log("OnRspQryInstrument合约信息null\n");
        return;
    }

    Log("一个合约信息,合约代码:%s", pInstrument->InstrumentID);

    if(bIsLast)
    {
        Log("\n");
    }

}


















