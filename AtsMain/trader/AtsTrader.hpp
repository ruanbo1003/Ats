/*
 * AtsTrader.hpp
 *
 *  Created on: Feb 6, 2017
 *      Author: ruanbo
 */

#ifndef ATSTRADER_HPP_
#define ATSTRADER_HPP_

#include <utils/TimeOp.hpp>
#include "ctp/ThostFtdcTraderApi.h"
#include "utils/FileOp.hpp"
#include "utils/CodeConvert.hpp"
#include "utils/TimeOp.hpp"
#include "data/OrderMngr.hpp"
#include "AtsMain/DbDriver/MysqlDb.hpp"
#include "AtsMain/AtsConfig.hpp"

enum AtsStatus
{
	ATS_Null = 0,

	ATS_Connected = 1,
	ATS_Logined = 2,
	ATS_Confirming = 3,
	ATS_Confirmed = 4,
	ATS_Stoping = 5,
	ATS_Quit = 6,

	ATS_End = 9999,
};

typedef void (*FuncPtr)(void);

class AtsTrader: public CThostFtdcTraderSpi
{
private:
	string _broker_id;
	string _investor_id;
	string _passwd;

	CThostFtdcTraderApi* _pUserApi;
	int _requestId;

	AtsStatus _ats_status;

	AtsConfigPtr _config;

	unsigned long int _run_secs;   // 运行的秒数

	string _settlement_path;  // 结算信息文件根目录
	string _run_path; // 当前路径

	//时间操作
	TimeOpPtr _time_op;
	// 结算单log文件
	FileOpPtr _settlement_f;
	// 编码转换
	CodeConvertPtr _cc;
	// 订单管理
	OrderMngrPtr _order_mngr;
	// MySql数据库
	MysqlDbPtr _mysql;

	map<int, std::function<void(void)>> _init_funcs;

public:
	AtsTrader();
	virtual ~AtsTrader();

private:
	void init_func();
	void init_vals();

	void setstatus(AtsStatus status);
	AtsStatus status() const;

public:
	// 初始化及运行，停止接口
	bool init(const AtsConfigPtr& config);

	bool uninit();

	bool run();

	bool stop();

	bool can_quit()const;

	void quit();

public:
	void on_next_second();
	void on_next_minute();
	void on_next_hour();

private:
	void check_login();

	void reqLogin();

	void reqAuthenticate();

	void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

public:
	void OnFrontConnected();

	void OnFrontDisconnected(int nReason);

	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	bool is_login() const;

	void reqLogout();

public:
	/* 为了使投资者及时准确的了解自己的交易状况，如可用资金，持仓，保证金占用等，从而及时了解自己的风
	 险状况，综合交易平台要求投资者在每一个交易日进行交易前都必须对前一交易日的结算结果进行确认。
	 */
	void check_settlement();

	// 查询投资者结算单
	void reqSettlementInfo();
	void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// 确认结算单
	void SettlementInfoConfirm();
	void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// 查询结算单确认的日期
	void reqSettlementInfoConfrm();
	void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	void on_settlement_confirmed(bool directConfirm = false);

public:
	//持仓
	void reqInvestorPosition();
	void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

public:
	//报单
	void onOrderInsert();
	//保单状态
	void OnRtnOrder(CThostFtdcOrderField *pOrder);

	//包含错误信息的报单录入请求响应
	void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//报单录入错误回报,　报单被ctp拒绝是返回
	void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
	//撮合成功通知
	void OnRtnTrade(CThostFtdcTradeField *pTrade);

	//创建一个报单
	bool make_order();

public:
	// 合约
	void reqAllInstrument();
	void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询合约详情
	void reqOneDepthMarketData();
	void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//查询合约保证金率
	void reqInstrumentMarginRate();
	void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
public:
	//　资金账户
	void reqTradingAccount();
	void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

public:
	//
	void reqInvector();
	void OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
	//错误应答
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
	void showRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//编码转换，CodeConvert
	const char* onCC(char* pSrc, size_t len = 0);

};

#endif /* ATSTRADER_HPP_ */
