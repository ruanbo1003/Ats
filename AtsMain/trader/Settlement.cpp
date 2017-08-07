/*
 * Settlement.cpp
 *
 *  Created on: Mar 15, 2017
 *      Author: ruanbo
 */

#include <AtsMain/trader/AtsTrader.hpp>

void AtsTrader::check_settlement()
{
	if(status() != ATS_Logined)
	{
		return;
	}

	Log("check check_settlement()");

	if(_mysql->is_settlement_confirmed(_time_op->date()) == true)
	{
		on_settlement_confirmed(true);
		return;
	}

	if(!_settlement_f)
	{
		_settlement_f = FileOpPtr(new FileOp(_settlement_path + _time_op->date() + ".txt"));
	}

	reqSettlementInfo();
}

// ��ѯͶ���߽��㵥
void AtsTrader::reqSettlementInfo()
{
	if(status() >= ATS_Confirming)
	{
		return;
	}

	CThostFtdcQrySettlementInfoField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, _broker_id.data());
	strcpy(req.InvestorID, _investor_id.data());
//    strcpy(req.TradingDay, "20170731");

	int ret = _pUserApi->ReqQrySettlementInfo(&req, ++_requestId);
	if(ret == 0)
	{
		update_status(ATS_Confirming);
	}

	Log("reqSettlementInfo ��ѯͶ���߽��㵥 ret:%d\n", ret);
}
void AtsTrader::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pRspInfo && pRspInfo->ErrorID != 0)
	{
		LogError("OnRspQrySettlementInfo failed");
		return;
	}

	if(!pSettlementInfo)
	{
		LogError("OnRsqQrySettlementInfo failed:pSettlementInfo is NULL");
		if(pRspInfo)
		{
			Log("errerid:%d", pRspInfo->ErrorID);
		}
		return;
	}

	Log("===OnRspQrySettlementInfo������ begin===bIsLast:%d", bIsLast);
	Log("TradingDay������:%s", pSettlementInfo->TradingDay);
	Log("SequenceNo���:%d", pSettlementInfo->SequenceNo);
//    Log("Content��Ϣ����:%s", pSettlementInfo->Content);
//    Log("strlen(pSettlementInfo->Content):%ld", strlen(pSettlementInfo->Content));
//    Log("Content��Ϣ����:%s", onCC(pSettlementInfo->Content, strlen(pSettlementInfo->Content)));

//    _settlement_f->write(onCC(pSettlementInfo->Content, 501));
	_settlement_f->write(pSettlementInfo->Content, 501);

	Log("===OnRspQrySettlementInfo������ end===\n");

	if(bIsLast == true)
	{
		SettlementInfoConfirm();
	}
}

// ȷ�Ͻ��㵥
void AtsTrader::SettlementInfoConfirm()
{
	std::pair<string, string> dt = _time_op->date_time();

	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, _broker_id.data());
	strcpy(req.InvestorID, _investor_id.data());
	strcpy(req.ConfirmDate, dt.first.data());
	strcpy(req.ConfirmTime, dt.second.data()); //10��30��01��

	int ret = _pUserApi->ReqSettlementInfoConfirm(&req, ++_requestId);

	Log("reqSettlementInfoConfirm ȷ�Ͻ��㵥��ret:%d\n", ret);
}

void AtsTrader::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pRspInfo && pRspInfo->ErrorID != 0)
	{
		LogError("OnRspQrySettlementInfo failed");
		return;
	}

	if(!pSettlementInfoConfirm)
	{
		LogError("OnRsqQrySettlementInfo failed:pSettlementInfo is NULL");
		return;
	}

	Log("===OnRspQrySettlementInfo ������Ϣȷ�� begin===bIsLast:%d", bIsLast);
	Log("ConfirmDate ȷ������:%s", pSettlementInfoConfirm->ConfirmDate);
	Log("ConfirmTime ȷ��ʱ��:%s", pSettlementInfoConfirm->ConfirmTime);
	Log("===OnRspQrySettlementInfo ������Ϣȷ�� end===\n");

	on_settlement_confirmed();
}

// ��ѯ���㵥ȷ�ϵ�����
void AtsTrader::reqSettlementInfoConfrm()
{
	CThostFtdcQrySettlementInfoConfirmField req;
	bzero(&req, sizeof(req));
	strcpy(req.BrokerID, _broker_id.data());
	strcpy(req.InvestorID, _investor_id.data());

	int ret = _pUserApi->ReqQrySettlementInfoConfirm(&req, ++_requestId);

	Log("reqSettlementInfoConfrm ret:%d", ret);
}
void AtsTrader::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pRspInfo && pRspInfo->ErrorID != 0)
	{
		LogError("OnRspQrySettlementInfo failed");
		return;
	}

	if(!pSettlementInfoConfirm)
	{
		LogError("OnRsqQrySettlementInfo failed:pSettlementInfo is NULL");
		return;
	}

	Log("===OnRspQrySettlementInfo ��ѯ���㵥ȷ�ϵ����� begin===bIsLast:%d", bIsLast);
	Log("ConfirmDate ȷ������:%s", pSettlementInfoConfirm->ConfirmDate);
	Log("ConfirmTime ȷ��ʱ��:%s", pSettlementInfoConfirm->ConfirmTime);
	Log("===OnRspQrySettlementInfo ��ѯ���㵥ȷ�ϵ����� end===\n");

	on_settlement_confirmed();
}

void AtsTrader::on_settlement_confirmed(bool directConfirm)
{
	update_status(ATS_Confirmed);

//    reqAllInstrument();
//
//    reqTradingAccount();
//
//    reqInvestorPosition();

	if(directConfirm == false)
	{
		_mysql->setttlement_confirm(_time_op->date(), _time_op->nowt());
	}
	else
	{
		Log("ֱ��ȷ�ϡ�\n");
	}
}

