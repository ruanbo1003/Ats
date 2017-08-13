/*
 * TraderTest.cpp
 *
 *  Created on: 2017��8��11��
 *      Author: ruanbo
 */

#include "comm/comm.hpp"
#include "ctp/ThostFtdcTraderApi.h"


void trader_test()
{
	CThostFtdcTraderApi* api = CThostFtdcTraderApi::CreateFtdcTraderApi("traderData/");
	if(!api)
	{
		LogError("���� TraderApi����");
		return;
	}

	sleep(60);

	api->Release();
}















