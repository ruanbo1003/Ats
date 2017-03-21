/*
 * OrderMngr.cpp
 *
 *  Created on: Mar 16, 2017
 *      Author: ruanbo
 */

#include <AtsMain/trader/data/OrderMngr.hpp>

OrderMngr::OrderMngr()
{
    init();
}

OrderMngr::~OrderMngr()
{
    _orders.clear();
    _pools.clear();
}

void OrderMngr::init()
{
    for(int i=0; i<DEFAULT_ORDER_SIZE; ++i)
    {
        CtpOrderPtr one_order = CtpOrderPtr(new CThostFtdcInputOrderField());
        zero_inst(one_order);

        _pools.push_back(one_order);
    }
}

void OrderMngr::zero_inst(CtpOrderPtr& order)
{
    if(!order)
    {
        return;
    }

    bzero(order->BrokerID, sizeof(order->BrokerID));
    bzero(order->InvestorID, sizeof(order->InvestorID));
    bzero(order->InstrumentID, sizeof(order->InstrumentID));
    bzero(order->OrderRef, sizeof(order->OrderRef));
    bzero(order->UserID, sizeof(order->UserID));
    order->OrderPriceType = 0;
    order->Direction = 0;
    bzero(order->CombOffsetFlag, sizeof(order->CombOffsetFlag));
    bzero(order->CombHedgeFlag, sizeof(order->CombHedgeFlag));
    order->LimitPrice = 0;
    order->VolumeTotalOriginal = 0;
    order->TimeCondition = 0;
    bzero(order->GTDDate, sizeof(order->GTDDate));
    order->VolumeCondition = 0;
    order->MinVolume = 0;
    order->ContingentCondition = 0;
    order->StopPrice = 0;
    order->ForceCloseReason = 0;
    order->IsAutoSuspend = 0;
    bzero(order->BusinessUnit, sizeof(order->BusinessUnit));
    order->RequestID = 0;
    order->UserForceClose = 0;
    order->IsSwapOrder = 0;
    bzero(order->ExchangeID, sizeof(order->ExchangeID));
    bzero(order->InvestUnitID, sizeof(order->InvestUnitID));
    bzero(order->AccountID, sizeof(order->AccountID));
    bzero(order->CurrencyID, sizeof(order->CurrencyID));
    bzero(order->ClientID, sizeof(order->ClientID));
    bzero(order->IPAddress, sizeof(order->IPAddress));
    bzero(order->MacAddress, sizeof(order->MacAddress));
}

CtpOrderPtr OrderMngr::instance()
{
    if(_pools.empty() == false)
    {
        CtpOrderPtr inst = _pools.front();
        _pools.pop_front();
        return inst;
    }

    CtpOrderPtr inst = CtpOrderPtr(new CThostFtdcInputOrderField());

    return inst;
}

void OrderMngr::new_order(const CtpOrderPtr& order)
{
    _orders.push_back(order);
}

void OrderMngr::back_pool(CtpOrderPtr& inst)
{
    zero_inst(inst);

    _pools.push_back(inst);
}

CtpOrderPtr OrderMngr::get_one()
{
    CtpOrderPtr order = CtpOrderPtr();   // OrderFieldPtr(
    if(_orders.empty() == false)
    {
        order = _orders.front();
        _orders.pop_front();
    }

    return order;
}

