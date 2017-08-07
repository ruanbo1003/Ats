/*
 * TradingAccount.cpp
 *
 *  Created on: Aug 1, 2017
 *      Author: ruanbo
 */


#include <AtsMain/trader/AtsTrader.hpp>

/*
 * 资金，账户
 */

void AtsTrader::reqTradingAccount()
{
    CThostFtdcQryTradingAccountField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, _broker_id.data());
    strcpy(req.InvestorID, _investor_id.data());
//    strcpy(req.CurrencyID, "CNY");

    int ret = _pUserApi->ReqQryTradingAccount(&req, ++_requestId);
    if(ret == 0)
    {
        Log("查询资金账户ret ok");
    }
    else
    {
        Log("查询资金账户ret(%d) failed", ret);
    }
}


void AtsTrader::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(pRspInfo && pRspInfo->ErrorID != 0)
    {
        LogError("OnRspQryTradingAccount failed");
        return;
    }

    if(!pTradingAccount)
    {
        Log("查询资金账户返回NULL");
        return;
    }

    Log("查询资金账户返回");
    Log("经纪公司代码:%s", pTradingAccount->BrokerID);
    Log("投资者帐号:%s", pTradingAccount->AccountID);
    Log("上次存款额:%lf", pTradingAccount->PreDeposit);
    Log("利息基数:%lf", pTradingAccount->InterestBase);
    Log("利息收入:%lf", pTradingAccount->Interest);
    Log("入金金额:%lf", pTradingAccount->Deposit);
    Log("出金金额:%lf", pTradingAccount->Withdraw);
    Log("冻结的保证金:%lf", pTradingAccount->FrozenMargin);
    Log("冻结的资金:%lf", pTradingAccount->FrozenCash);
    Log("手续费:%lf", pTradingAccount->Commission);
    Log("平仓盈亏:%lf", pTradingAccount->CloseProfit);
    Log("持仓盈亏:%lf", pTradingAccount->PositionProfit);
    Log("可用资金:%lf", pTradingAccount->Available);
    Log("可取资金:%lf", pTradingAccount->WithdrawQuota);
    Log("交易日:%s", pTradingAccount->TradingDay);
    Log("信用额度:%lf", pTradingAccount->Credit);
    Log("币种代码:%s\n", pTradingAccount->CurrencyID);

    if(bIsLast)
    {
        Log("资金账户查询结束===============\n");
    }
}



