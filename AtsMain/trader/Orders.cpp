/*
 * Orders.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: ruanbo
 */

#include <AtsMain/trader/AtsTrader.hpp>

//报单
void AtsTrader::onOrderInsert()
{
	if(status() != ATS_Confirmed)
	{
		return;
	}

	CThostFtdcInputOrderField ord;
	memset(&ord, 0, sizeof(ord));

	strcpy(ord.BrokerID, _broker_id.data());
	strcpy(ord.InvestorID, _investor_id.data());
	strcpy(ord.InstrumentID, "IF1712");
	strcpy(ord.OrderRef, "");

	ord.Direction = THOST_FTDC_D_Buy;
	ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	ord.VolumeTotalOriginal = 1;
	ord.ContingentCondition = THOST_FTDC_CC_Immediately;
	ord.VolumeCondition = THOST_FTDC_VC_MV;
	ord.MinVolume = 1;  // 1000000000
	ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	ord.IsAutoSuspend = 0;
	ord.UserForceClose = 0;

	if(true)
	{
		//市价单
		ord.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
		ord.LimitPrice = 0;
		ord.TimeCondition = THOST_FTDC_TC_IOC;
	}

	if(false)
	{
		//限价单
		ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		ord.LimitPrice = 1900.00;
		ord.TimeCondition = THOST_FTDC_TC_GFD;
	}
	if(false)
	{
		//条件单
		ord.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterThanStopPrice;
		ord.StopPrice = 2150;
		ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		ord.LimitPrice = 2100;
		ord.TimeCondition = THOST_FTDC_TC_GFD;
	}

	int ret = _pUserApi->ReqOrderInsert(&ord, ++_requestId);

	Log("\n\n\nonOrderInsert 报单 ret:%d\n", ret);
}

//保单状态
void AtsTrader::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	if(!pOrder)
	{
		Log("===OnRtnOrder 报单状态 null \n");
		return;
	}

	Log("===OnRtnOrder 报单状态 begin====");

	Log("报单编号:%s", pOrder->OrderSysID);
	Log("报单状态:%c", pOrder->OrderStatus);
	Log("今成交数量:%d", pOrder->VolumeTraded);
	Log("状态信息:%s", _cc->convert2(pOrder->StatusMsg, strlen(pOrder->StatusMsg)));
//	Log("状态信息:%s", pOrder->StatusMsg);

	Log("===OnRtnOrder 报单状态 end====\n");
}

//包含错误信息的报单录入请求响应
void AtsTrader::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!pInputOrder)
	{
		Log("===OnRspOrderInsert 报单录入请求响应 null \n");
		return;
	}

	Log("===OnRspOrderInsert 报单错误信息 begin====");

	Log("错误代码:%d", pRspInfo->ErrorID);
	Log("错误信息:%s", pRspInfo->ErrorMsg);

	Log("===OnRspOrderInsert 报单错误信息 end====\n");
}

///报单录入错误回报, 报单被ctp拒绝是返回
void AtsTrader::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	if(!pInputOrder)
	{
		Log("===OnRspOrderInsert 报单录入请求响应 null \n");
		return;
	}

	Log("===OnErrRtnOrderInsert 报单录入错误回报 begin====");

	Log("错误代码:%d", pRspInfo->ErrorID);
	Log("错误信息:%s", pRspInfo->ErrorMsg);

	Log("===OnErrRtnOrderInsert 报单录入错误回报 end====\n");
}

//撮合成功通知
void AtsTrader::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	Log("===OnRtnTrade 成交通知 begin====");

	Log("合约代码:%s", pTrade->InstrumentID);
	Log("成交编号:%s", pTrade->TradeID);
	Log("价格:%f", pTrade->Price);
	Log("数量:%d", pTrade->Volume);
	Log("成交时期:%s", pTrade->TradeDate);
	Log("成交时间:%s", pTrade->TradeTime);
	Log("成交类型:%c", pTrade->TradeType);
	Log("成交价来源:%c", pTrade->PriceSource);
	Log("成交来源:%c", pTrade->TradeSource);

	Log("===OnRtnTrade 成交通知 end====\n");
}

//
bool AtsTrader::make_order()
{
	CtpOrderPtr order = _order_mngr->instance();

	_order_mngr->new_order(order);

	return true;
}

