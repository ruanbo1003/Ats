/*
 * Orders.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: ruanbo
 */

#include <AtsMain/trader/AtsTrader.hpp>

//����
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
		//�м۵�
		ord.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
		ord.LimitPrice = 0;
		ord.TimeCondition = THOST_FTDC_TC_IOC;
	}

	if(false)
	{
		//�޼۵�
		ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		ord.LimitPrice = 1900.00;
		ord.TimeCondition = THOST_FTDC_TC_GFD;
	}
	if(false)
	{
		//������
		ord.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterThanStopPrice;
		ord.StopPrice = 2150;
		ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		ord.LimitPrice = 2100;
		ord.TimeCondition = THOST_FTDC_TC_GFD;
	}

	int ret = _pUserApi->ReqOrderInsert(&ord, ++_requestId);

	Log("\n\n\nonOrderInsert ���� ret:%d\n", ret);
}

//����״̬
void AtsTrader::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	if(!pOrder)
	{
		Log("===OnRtnOrder ����״̬ null \n");
		return;
	}

	Log("===OnRtnOrder ����״̬ begin====");

	Log("�������:%s", pOrder->OrderSysID);
	Log("����״̬:%c", pOrder->OrderStatus);
	Log("��ɽ�����:%d", pOrder->VolumeTraded);
//	Log("״̬��Ϣ:%s", _cc->convert2(pOrder->StatusMsg, strlen(pOrder->StatusMsg)));
	Log("״̬��Ϣ:%s", pOrder->StatusMsg);

	Log("===OnRtnOrder ����״̬ end====\n");
}

//����������Ϣ�ı���¼��������Ӧ
void AtsTrader::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!pInputOrder)
	{
		Log("===OnRspOrderInsert ����¼��������Ӧ null \n");
		return;
	}

	Log("===OnRspOrderInsert ����������Ϣ begin====");

	Log("�������:%d", pRspInfo->ErrorID);
	Log("������Ϣ:%s", pRspInfo->ErrorMsg);

	Log("===OnRspOrderInsert ����������Ϣ end====\n");
}

///����¼�����ر�, ������ctp�ܾ��Ƿ���
void AtsTrader::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	if(!pInputOrder)
	{
		Log("===OnRspOrderInsert ����¼��������Ӧ null \n");
		return;
	}

	Log("===OnErrRtnOrderInsert ����¼�����ر� begin====");

	Log("�������:%d", pRspInfo->ErrorID);
	Log("������Ϣ:%s", pRspInfo->ErrorMsg);

	Log("===OnErrRtnOrderInsert ����¼�����ر� end====\n");
}

//��ϳɹ�֪ͨ
void AtsTrader::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	Log("===OnRtnTrade �ɽ�֪ͨ begin====");

	Log("��Լ����:%s", pTrade->InstrumentID);
	Log("�ɽ����:%s", pTrade->TradeID);
	Log("�۸�:%f", pTrade->Price);
	Log("����:%d", pTrade->Volume);
	Log("�ɽ�ʱ��:%s", pTrade->TradeDate);
	Log("�ɽ�ʱ��:%s", pTrade->TradeTime);
	Log("�ɽ�����:%c", pTrade->TradeType);
	Log("�ɽ�����Դ:%c", pTrade->PriceSource);
	Log("�ɽ���Դ:%c", pTrade->TradeSource);

	Log("===OnRtnTrade �ɽ�֪ͨ end====\n");
}

//
bool AtsTrader::make_order()
{
	CtpOrderPtr order = _order_mngr->instance();

	_order_mngr->new_order(order);

	return true;
}

