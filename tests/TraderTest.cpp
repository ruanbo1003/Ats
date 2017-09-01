/*
 * TraderTest.cpp
 *
 *  Created on: 2017年8月11日
 *      Author: ruanbo
 */

#include "comm/comm.hpp"
#include "ctp/ThostFtdcTraderApi.h"


void trader_test()
{
	CThostFtdcTraderApi* api = CThostFtdcTraderApi::CreateFtdcTraderApi("traderData/");
	if(!api)
	{
		LogError("创建 TraderApi出错");
		return;
	}

	sleep(60);

	api->Release();
}















