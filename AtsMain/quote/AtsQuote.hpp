/*
 * AtsQuote.hpp
 *
 *  Created on: Feb 4, 2017
 *      Author: ruanbo
 */

#ifndef ATSQUOTE_HPP_
#define ATSQUOTE_HPP_

#include "ctp/ThostFtdcMdApi.h"

class AtsQuote : public CThostFtdcMdSpi
{
private:
    CThostFtdcMdApi *_pUserApi;
    int _requestId;
    bool _is_login;  // 是否已经登陆了

public:
    AtsQuote(CThostFtdcMdApi *api);
    virtual ~AtsQuote();

public:
    void OnFrontConnected();

    void ReqUserLogin();

    void ReqUserLogout();

    void OnFrontDisconnected(int nReason);

    void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    bool is_login()const;

public:
    void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

    void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    void OnHeartBeatWarning(int nTimeLapse);

private:
    //显示回应信息
    void showRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
private:
    //查询所有合约
    void OnReqAllInstrument();
};



#endif /* ATSQUOTE_HPP_ */
