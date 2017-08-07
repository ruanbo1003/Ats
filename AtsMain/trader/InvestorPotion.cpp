/*
 * InvestorPotion.cpp
 *
 *  Created on: Aug 1, 2017
 *      Author: ruanbo
 */

#include <AtsMain/trader/AtsTrader.hpp>

/*
 *  持仓
 */

// 持仓
void AtsTrader::reqInvestorPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, _broker_id.data());
	strcpy(req.InvestorID, _investor_id.data());
//    strcpy(req.CurrencyID, "CNY");

	int ret = _pUserApi->ReqQryInvestorPosition(&req, ++_requestId);
	if(ret == 0)
	{
		Log("查询投资者持仓ret ok");
	}
	else
	{
		Log("查询投资者持仓ret(%d) failed", ret);
	}
}

void AtsTrader::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pRspInfo && pRspInfo->ErrorID != 0)
	{
		LogError("查询投资者持仓 error(%d)", pRspInfo->ErrorID);
		return;
	}

	if(!pInvestorPosition)
	{
		Log("查询投资者持仓返回 null\n");
		return;
	}

	Log("查询投资者持仓返回");
	Log("合约代码:%s", pInvestorPosition->InstrumentID);
	Log("持仓多空方向:%d", pInvestorPosition->PosiDirection);
	Log("持仓日期:%d", pInvestorPosition->PositionDate);
	Log("持仓成本:%lf", pInvestorPosition->PositionCost);
	Log("手续费:%lf", pInvestorPosition->Commission);

	if(bIsLast)
	{
		Log("查询投资者持仓结束=============\n");
	}
}

