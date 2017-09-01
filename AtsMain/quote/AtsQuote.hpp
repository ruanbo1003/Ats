/*
 * AtsQuote.hpp
 *
 *  Created on: Feb 4, 2017
 *      Author: ruanbo
 */

#ifndef ATSQUOTE_HPP_
#define ATSQUOTE_HPP_

#include "ctp/ThostFtdcMdApi.h"
#include "AtsMain/DbDriver/MysqlDb.hpp"

#include "AtsMain/AtsConfig.hpp"

class AtsQuote : public CThostFtdcMdSpi
{
private:
    CThostFtdcMdApi *_pUserApi;
    int _requestId;
    bool _is_login;  //

    AtsConfigPtr _config;

    // MySql数据库
	MysqlDbPtr _mysql;

public:
    AtsQuote();
    virtual ~AtsQuote();

public:
    bool init(const AtsConfigPtr& config);
    void unit();

    void run();
    void stop();

public:
    void OnFrontConnected();
    void OnFrontDisconnected(int nReason);

    void ReqUserLogin();
    void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    void ReqUserLogout();
    void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    bool is_login()const;


private:
    void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    void OnHeartBeatWarning(int nTimeLapse);

    void showRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


public:
    void OnReqAllInstrument();

    void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

private:
    void clean_tick_data(CThostFtdcDepthMarketDataField *data);

};



#endif /* ATSQUOTE_HPP_ */
