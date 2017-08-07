/*
 * TradingAccount.cpp
 *
 *  Created on: Aug 1, 2017
 *      Author: ruanbo
 */


#include <AtsMain/trader/AtsTrader.hpp>

/*
 * �ʽ��˻�
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
        Log("��ѯ�ʽ��˻�ret ok");
    }
    else
    {
        Log("��ѯ�ʽ��˻�ret(%d) failed", ret);
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
        Log("��ѯ�ʽ��˻�����NULL");
        return;
    }

    Log("��ѯ�ʽ��˻�����");
    Log("���͹�˾����:%s", pTradingAccount->BrokerID);
    Log("Ͷ�����ʺ�:%s", pTradingAccount->AccountID);
    Log("�ϴδ���:%lf", pTradingAccount->PreDeposit);
    Log("��Ϣ����:%lf", pTradingAccount->InterestBase);
    Log("��Ϣ����:%lf", pTradingAccount->Interest);
    Log("�����:%lf", pTradingAccount->Deposit);
    Log("������:%lf", pTradingAccount->Withdraw);
    Log("����ı�֤��:%lf", pTradingAccount->FrozenMargin);
    Log("������ʽ�:%lf", pTradingAccount->FrozenCash);
    Log("������:%lf", pTradingAccount->Commission);
    Log("ƽ��ӯ��:%lf", pTradingAccount->CloseProfit);
    Log("�ֲ�ӯ��:%lf", pTradingAccount->PositionProfit);
    Log("�����ʽ�:%lf", pTradingAccount->Available);
    Log("��ȡ�ʽ�:%lf", pTradingAccount->WithdrawQuota);
    Log("������:%s", pTradingAccount->TradingDay);
    Log("���ö��:%lf", pTradingAccount->Credit);
    Log("���ִ���:%s\n", pTradingAccount->CurrencyID);

    if(bIsLast)
    {
        Log("�ʽ��˻���ѯ����===============\n");
    }
}



