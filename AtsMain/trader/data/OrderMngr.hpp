/*
 * OrderMngr.hpp
 *
 *  Created on: Mar 16, 2017
 *      Author: ruanbo
 */

#ifndef TRADER_DATA_ORDERMNGR_HPP_
#define TRADER_DATA_ORDERMNGR_HPP_

#include "ctp/ThostFtdcUserApiStruct.h"
#include "comm/comm.hpp"

using CtpOrderPtr = tr1::shared_ptr<CThostFtdcInputOrderField>;

class OrderMngr : public NonCopyable
{
#define DEFAULT_ORDER_SIZE 32
private:
    list<CtpOrderPtr> _orders;
    list<CtpOrderPtr> _pools;

public:
    OrderMngr();
    virtual ~OrderMngr();

private:
    void init();

    void zero_inst(CtpOrderPtr& order);

public:
    CtpOrderPtr instance();

    void new_order(const CtpOrderPtr& order);

    void back_pool(CtpOrderPtr& inst);

    CtpOrderPtr get_one();
};

using OrderMngrPtr = tr1::shared_ptr<OrderMngr>;

#endif /* TRADER_DATA_ORDERMNGR_HPP_ */
