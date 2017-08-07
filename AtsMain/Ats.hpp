/*
 * Ats.hpp
 *
 *  Created on: Feb 6, 2017
 *      Author: ruanbo
 */

#ifndef ATS_HPP_
#define ATS_HPP_

#include <AtsMain/quote/AtsQuote.hpp>
#include <AtsMain/trader/AtsTrader.hpp>
#include "comm/comm.hpp"
#include "utils/IThread.hpp"
#include "AtsConfig.hpp"


class Ats : public IThread
{
private:
    CThostFtdcMdApi* _md_api;
    AtsQuote* _md_spi;

    AtsTrader* _trader_spi;

    AtsConfigPtr _config;

public:
    Ats();
    virtual ~Ats();

private:

private:
    void thread_loop();

    void on_next_second();
    void on_next_minute();
    void on_next_hour();

private:
    bool quote_tests();
    bool trader_tests();

private:
    void onLogout();
public:
    bool init();
    void unit();

    void run();
};

using AtsPtr = tr1::shared_ptr<Ats>;


#endif /* ATS_HPP_ */



