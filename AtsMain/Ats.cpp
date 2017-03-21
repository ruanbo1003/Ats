/*
 * Ats.cpp
 *
 *  Created on: Feb 6, 2017
 *      Author: ruanbo
 */

#include <AtsMain/Ats.hpp>
#include <sys/resource.h>
#include <thread>

Ats::Ats()
{
    _md_api = NULL;
    _md_spi = NULL;
    _md_front = "tcp://218.202.237.33:10012";  // SimNow的接口

    _trader_spi = NULL;
//    _trader_front = "tcp://218.202.237.33:10002";  // SimNow的接口
    _trader_front = "tcp://180.168.146.187:10001";

    set_tname("Ats main");
}

Ats::~Ats()
{

}

void Ats::thread_loop()
{
    tmsleep(10);
}

void Ats::on_next_second()
{
    if(_trader_spi)
    {
        _trader_spi->on_next_second();
    }
}
void Ats::on_next_minute()
{

}
void Ats::on_next_hour()
{

}

bool Ats::quote_tests()
{
    _md_api = CThostFtdcMdApi::CreateFtdcMdApi("quoteData/", false);
    if(!_md_api)
    {
        LogError("创建MdApi出错");
        return false;
    }

    Log("MdApi version:%s", _md_api->GetApiVersion());

    _md_spi = new AtsQuote(_md_api);

    _md_api->RegisterSpi(_md_spi);
    _md_api->RegisterFront((char*)_md_front.data());

    return true;
}

bool Ats::trader_tests()
{
    _trader_spi = new AtsTrader(_trader_front);

    if(_trader_spi->init() == false)
    {
        LogError("AtsTrader::init failed");
        return false;
    }

    return true;
}

void Ats::onLogout()
{
    if(_md_spi)
    {

    }

    if(_trader_spi)
    {
        while(true)
        {
            if(_trader_spi->is_login() == false)
            {
                break;
            }

            _trader_spi->reqLogout();
            usleep(100000);
        }
    }

    Log("Ats::onLogout end");
}

bool Ats::init()
{
    if(false)
    {
        if(quote_tests() == false)
        {
            LogError("quote_tests failed");
            return false;
        }
    }

    if(true)
    {
        if(trader_tests() == false)
        {
            LogError("trader_tests failed");
            return false;
        }
    }
    return true;
}


void Ats::unit()
{
    Log("Ats::unit()");

    onLogout();

    if(_md_api)
    {
        _md_spi->ReqUserLogout();

        while(_md_spi->is_login() == true)
        {
            usleep(10000);   // 微秒
        }
    }

    if(_md_api)
    {
        _md_api->RegisterSpi(NULL);

        delete _md_spi;
        _md_spi = NULL;

        _md_api->Release();
//        _md_api->Join();
        _md_api = NULL;
    }

    if(_trader_spi)
    {
        _trader_spi->stop();

        delete _trader_spi;
        _trader_spi = NULL;
    }

    return;
}


void Ats::run()
{
    if(_md_api)
    {
        _md_api->Init();
    }

    if(_trader_spi)
    {
        _trader_spi->run();
    }

    start();
}





