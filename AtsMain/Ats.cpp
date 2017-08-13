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
    _quote_spi = NULL;
    _trader_spi = NULL;

    _config = tr1::shared_ptr<AtsConfig>(new AtsConfig("/home/ruanbo/Codes/Ats/Config/ats.xml"));

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
	LogTid();
}
void Ats::on_next_hour()
{

}

bool Ats::quote_tests()
{
	Log("quote test");

    _quote_spi = new AtsQuote();
    if(_quote_spi->init(_config) == false)
    {
    	LogError("AtsQuote::init failed");
    	return false;
    }

    return true;
}

bool Ats::trader_tests()
{
	Log("trader test");

    _trader_spi = new AtsTrader();

    if(_trader_spi->init(_config) == false)
    {
        LogError("AtsTrader::init failed");
        return false;
    }

    return true;
}

void Ats::onLogout()
{
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
	Log("Ats class thread id:%ld\n", gettid());

	if(_config->read_config() == false)
	{
		Log("read config file failed");
		return false;
	}

    if(_config->_quote_run)
    {
        if(quote_tests() == false)
        {
            LogError("quote_tests failed");
            return false;
        }
    }

    if(_config->_trader_run)
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
//    Log("Ats::unit()");

    LogTid();

//    onLogout();
//
//    if(_quote_spi)
//    {
//        _quote_spi->ReqUserLogout();
//
//        while(_quote_spi->is_login() == true)
//        {
//            usleep(10000);   // Î¢Ãë
//        }
//    }

    if(_quote_spi)
    {
    	_quote_spi->stop();

    	delete _quote_spi;
    	_quote_spi = NULL;
    }

    while(true)
    {
        if(_trader_spi)
        {
            if(_trader_spi->can_quit() == false)
            {
            	_trader_spi->stop();
            	sleep(1);
            	continue;
            }

            _trader_spi->quit();

            delete _trader_spi;
            _trader_spi = NULL;

            break;
        }
    }


    return;
}


void Ats::run()
{
	LogTid();

    if(_quote_spi)
    {
        _quote_spi->run();
    }

    if(_trader_spi)
    {
        _trader_spi->run();
    }

    start();
}





