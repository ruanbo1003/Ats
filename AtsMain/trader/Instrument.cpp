/*
 * Instrument.cpp
 *
 *  Created on: Aug 1, 2017
 *      Author: ruanbo
 */

#include <AtsMain/trader/AtsTrader.hpp>

/*
 * ��Լ
 */

void AtsTrader::reqAllInstrument()
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));

	int ret = _pUserApi->ReqQryInstrument(&req, ++_requestId);
	if(ret == 0)
	{
		Log("�������к�Լret ok");
	}
	else
	{
		Log("�������к�Լret(%d) failed", ret);
	}
}

void AtsTrader::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!pInstrument)
	{
		Log("OnRspQryInstrument��Լ��Ϣnull\n");
		return;
	}

//	Log("һ����Լ��Ϣ,��Լ����:%s", pInstrument->InstrumentID);

	if(bIsLast)
	{
		Log("ȫ����Լ��Ϣ����\n");
	}

}

//��ѯ��Լ����
void AtsTrader::reqOneDepthMarketData()
{
	CThostFtdcQryDepthMarketDataField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, "IF1712");

	int ret = _pUserApi->ReqQryDepthMarketData(&req, ++_requestId);
	if(ret == 0)
	{
		Log("��ѯ��Լ����ret ok");
	}
}

void AtsTrader::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!pDepthMarketData)
	{
		Log("OnRspQryDepthMarketData��Լ���� null\n");
		return;
	}

	Log("��Լ:%s ���鷵��", pDepthMarketData->InstrumentID);
	Log("������:%s", pDepthMarketData->TradingDay);
	Log("����������:%s", pDepthMarketData->ExchangeID);
	Log("��Լ�ڽ������Ĵ���:%s", pDepthMarketData->ExchangeInstID);
	Log("���¼�:%lf", pDepthMarketData->LastPrice);
	Log("�ϴν����:%lf", pDepthMarketData->PreSettlementPrice);
	Log("������:%lf", pDepthMarketData->PreClosePrice);
	Log("��ֲ���:%lf", pDepthMarketData->PreOpenInterest);

	Log("����:%lf", pDepthMarketData->OpenPrice);
	Log("��߼�:%lf", pDepthMarketData->HighestPrice);
	Log("��ͼ�:%lf", pDepthMarketData->LowestPrice);

	Log("����:%d", pDepthMarketData->Volume);
	Log("�ɽ����:%lf", pDepthMarketData->Turnover);
	Log("�ֲ���:%lf", pDepthMarketData->OpenInterest);

	Log("������:%lf", pDepthMarketData->ClosePrice);
	Log("���ν����:%lf", pDepthMarketData->SettlementPrice);

	Log("����޸�ʱ��:%s", pDepthMarketData->UpdateTime);
	Log("����޸ĺ���:%d", pDepthMarketData->UpdateMillisec);

	Log("���վ���:%lf", pDepthMarketData->AveragePrice);
	Log("ҵ������:%s", pDepthMarketData->ActionDay);

	if(bIsLast)
	{
		Log("��Լ%s����end=======\n\n", pDepthMarketData->InstrumentID);
	}

}

