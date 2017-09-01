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

// 查询投资者结算单
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
		setstatus(ATS_Confirming);
	}

	Log("reqSettlementInfo 查询投资者结算单 ret:%d\n", ret);
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

	Log("===OnRspQrySettlementInfo结算结果 begin===bIsLast:%d", bIsLast);
	Log("TradingDay交易日:%s", pSettlementInfo->TradingDay);
	Log("SequenceNo序号:%d", pSettlementInfo->SequenceNo);
//    Log("Content消息正文:%s", pSettlementInfo->Content);
//    Log("strlen(pSettlementInfo->Content):%ld", strlen(pSettlementInfo->Content));
//    Log("Content消息正文:%s", onCC(pSettlementInfo->Content, strlen(pSettlementInfo->Content)));

//    _settlement_f->write(onCC(pSettlementInfo->Content, 501));
	_settlement_f->write(pSettlementInfo->Content, 501);

	Log("===OnRspQrySettlementInfo结算结果 end===\n");

	if(bIsLast == true)
	{
		SettlementInfoConfirm();
	}
}

// 确认结算单
void AtsTrader::SettlementInfoConfirm()
{
	std::pair<string, string> dt = _time_op->date_time();

	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, _broker_id.data());
	strcpy(req.InvestorID, _investor_id.data());
	strcpy(req.ConfirmDate, dt.first.data());
	strcpy(req.ConfirmTime, dt.second.data()); //10点30分01秒

	int ret = _pUserApi->ReqSettlementInfoConfirm(&req, ++_requestId);

	Log("reqSettlementInfoConfirm 确认结算单，ret:%d\n", ret);
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

	Log("===OnRspQrySettlementInfo 结算信息确认 begin===bIsLast:%d", bIsLast);
	Log("ConfirmDate 确认日期:%s", pSettlementInfoConfirm->ConfirmDate);
	Log("ConfirmTime 确认时间:%s", pSettlementInfoConfirm->ConfirmTime);
	Log("===OnRspQrySettlementInfo 结算信息确认 end===\n");

	on_settlement_confirmed();
}

// 查询结算单确认的日期
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

	Log("===OnRspQrySettlementInfo 查询结算单确认的日期 begin===bIsLast:%d", bIsLast);
	Log("ConfirmDate 确认日期:%s", pSettlementInfoConfirm->ConfirmDate);
	Log("ConfirmTime 确认时间:%s", pSettlementInfoConfirm->ConfirmTime);
	Log("===OnRspQrySettlementInfo 查询结算单确认的日期 end===\n");

	on_settlement_confirmed();
}

void AtsTrader::on_settlement_confirmed(bool directConfirm)
{
	setstatus(ATS_Confirmed);

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
		Log("直接确认。\n");
	}
}

