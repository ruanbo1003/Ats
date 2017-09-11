/*
 * Investor.cpp
 *
 *  Created on: Sep 3, 2017
 *      Author: ruanbo
 */

#include <AtsMain/trader/AtsTrader.hpp>


void AtsTrader::reqInvector()
{
	CThostFtdcQryInvestorField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, _broker_id.data());
	strcpy(req.InvestorID, _investor_id.data());
//    strcpy(req.CurrencyID, "CNY");

	int ret = _pUserApi->ReqQryInvestor(&req, ++_requestId);
	if(ret == 0)
	{
		Log("查询投资者ret ok");
	}
	else
	{
		Log("查询投资者ret(%d) failed", ret);
	}
}

void AtsTrader::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pRspInfo->ErrorID == 0 && bIsLast == 1 and pInvestor)
	{
		Log("查询投资者返回ok\n");
	}
}











