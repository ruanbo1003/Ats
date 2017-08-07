/*
 * InvestorPotion.cpp
 *
 *  Created on: Aug 1, 2017
 *      Author: ruanbo
 */

#include <AtsMain/trader/AtsTrader.hpp>

/*
 *  �ֲ�
 */

// �ֲ�
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
		Log("��ѯͶ���ֲ߳�ret ok");
	}
	else
	{
		Log("��ѯͶ���ֲ߳�ret(%d) failed", ret);
	}
}

void AtsTrader::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pRspInfo && pRspInfo->ErrorID != 0)
	{
		LogError("��ѯͶ���ֲ߳� error(%d)", pRspInfo->ErrorID);
		return;
	}

	if(!pInvestorPosition)
	{
		Log("��ѯͶ���ֲַ߳��� null\n");
		return;
	}

	Log("��ѯͶ���ֲַ߳���");
	Log("��Լ����:%s", pInvestorPosition->InstrumentID);
	Log("�ֲֶ�շ���:%d", pInvestorPosition->PosiDirection);
	Log("�ֲ�����:%d", pInvestorPosition->PositionDate);
	Log("�ֲֳɱ�:%lf", pInvestorPosition->PositionCost);
	Log("������:%lf", pInvestorPosition->Commission);

	if(bIsLast)
	{
		Log("��ѯͶ���ֲֽ߳���=============\n");
	}
}

