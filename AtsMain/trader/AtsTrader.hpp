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

	unsigned long int _run_secs;   // ���е�����

	string _settlement_path;  // ������Ϣ�ļ���Ŀ¼
	string _run_path; // ��ǰ·��

	//ʱ�����
	TimeOpPtr _time_op;
	// ���㵥log�ļ�
	FileOpPtr _settlement_f;
	// ����ת��
	CodeConvertPtr _cc;
	// ��������
	OrderMngrPtr _order_mngr;
	// MySql���ݿ�
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
	// ��ʼ�������У�ֹͣ�ӿ�
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
	/* Ϊ��ʹͶ���߼�ʱ׼ȷ���˽��Լ��Ľ���״����������ʽ𣬳ֲ֣���֤��ռ�õȣ��Ӷ���ʱ�˽��Լ��ķ�
	 ��״�����ۺϽ���ƽ̨Ҫ��Ͷ������ÿһ�������ս��н���ǰ�������ǰһ�����յĽ���������ȷ�ϡ�
	 */
	void check_settlement();

	// ��ѯͶ���߽��㵥
	void reqSettlementInfo();
	void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// ȷ�Ͻ��㵥
	void SettlementInfoConfirm();
	void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// ��ѯ���㵥ȷ�ϵ�����
	void reqSettlementInfoConfrm();
	void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	void on_settlement_confirmed(bool directConfirm = false);

public:
	//�ֲ�
	void reqInvestorPosition();
	void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

public:
	//����
	void onOrderInsert();
	//����״̬
	void OnRtnOrder(CThostFtdcOrderField *pOrder);

	//����������Ϣ�ı���¼��������Ӧ
	void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//����¼�����ر�,��������ctp�ܾ��Ƿ���
	void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
	//��ϳɹ�֪ͨ
	void OnRtnTrade(CThostFtdcTradeField *pTrade);

	//����һ������
	bool make_order();

public:
	// ��Լ
	void reqAllInstrument();

	void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//��ѯ��Լ����
	void reqOneDepthMarketData();

	void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

public:
	//���ʽ��˻�
	void reqTradingAccount();
	void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
	//����Ӧ��
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
	void showRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//����ת����CodeConvert
	const char* onCC(char* pSrc, size_t len = 0);

};

#endif /* ATSTRADER_HPP_ */
