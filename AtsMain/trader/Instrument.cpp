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
	else
	{
		Log("请求所有合约ret(%d) failed", ret);
	}
}

void AtsTrader::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!pInstrument)
	{
		Log("OnRspQryInstrument合约信息null\n");
		return;
	}

//	Log("一个合约信息,合约代码:%s", pInstrument->InstrumentID);

	if(bIsLast)
	{
		Log("全部合约信息结束\n");
	}

}

//查询合约详情
void AtsTrader::reqOneDepthMarketData()
{
	CThostFtdcQryDepthMarketDataField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, "IF1712");

	int ret = _pUserApi->ReqQryDepthMarketData(&req, ++_requestId);
	if(ret == 0)
	{
		Log("查询合约详情ret ok");
	}
}

void AtsTrader::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!pDepthMarketData)
	{
		Log("OnRspQryDepthMarketData合约详情 null\n");
		return;
	}

	Log("合约:%s 详情返回", pDepthMarketData->InstrumentID);
	Log("交易日:%s", pDepthMarketData->TradingDay);
	Log("交易所代码:%s", pDepthMarketData->ExchangeID);
	Log("合约在交易所的代码:%s", pDepthMarketData->ExchangeInstID);
	Log("最新价:%lf", pDepthMarketData->LastPrice);
	Log("上次结算价:%lf", pDepthMarketData->PreSettlementPrice);
	Log("昨收盘:%lf", pDepthMarketData->PreClosePrice);
	Log("昨持仓量:%lf", pDepthMarketData->PreOpenInterest);

	Log("今开盘:%lf", pDepthMarketData->OpenPrice);
	Log("最高价:%lf", pDepthMarketData->HighestPrice);
	Log("最低价:%lf", pDepthMarketData->LowestPrice);

	Log("数量:%d", pDepthMarketData->Volume);
	Log("成交金额:%lf", pDepthMarketData->Turnover);
	Log("持仓量:%lf", pDepthMarketData->OpenInterest);

	Log("今收盘:%lf", pDepthMarketData->ClosePrice);
	Log("本次结算价:%lf", pDepthMarketData->SettlementPrice);

	Log("最后修改时间:%s", pDepthMarketData->UpdateTime);
	Log("最后修改毫秒:%d", pDepthMarketData->UpdateMillisec);

	Log("当日均价:%lf", pDepthMarketData->AveragePrice);
	Log("业务日期:%s", pDepthMarketData->ActionDay);

	if(bIsLast)
	{
		Log("合约%s详情end=======\n\n", pDepthMarketData->InstrumentID);
	}

}

