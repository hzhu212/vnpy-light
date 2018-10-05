//说明部分
/*
由于CTP API中和资金转账相关的函数太多，
同时也已经有大量的客户端支持这些功能，
因此在这个Python封装中暂时选择只支持交易功能
*/



#include <string>
#include <queue>

//Boost
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python/module.hpp>	//python封装
#include <boost/python/def.hpp>		//python封装
#include <boost/python/dict.hpp>	//python封装
#include <boost/python/object.hpp>	//python封装
#include <boost/python.hpp>			//python封装
#include <boost/thread.hpp>			//任务队列的线程功能
#include <boost/bind.hpp>			//任务队列的线程功能
#include <boost/any.hpp>			//任务队列的任务实现
#include <boost/locale.hpp>			//字符集转换

//API
#include "ThostFtdcTraderApi.h"

//命名空间
using namespace std;
using namespace boost::python;
using namespace boost;

//常量
// render_data.ctp_td_header_define start
#define ONFRONTCONNECTED 1
#define ONFRONTDISCONNECTED 2
#define ONHEARTBEATWARNING 3
#define ONRSPAUTHENTICATE 4
#define ONRSPUSERLOGIN 5
#define ONRSPUSERLOGOUT 6
#define ONRSPUSERPASSWORDUPDATE 7
#define ONRSPTRADINGACCOUNTPASSWORDUPDATE 8
#define ONRSPORDERINSERT 9
#define ONRSPPARKEDORDERINSERT 10
#define ONRSPPARKEDORDERACTION 11
#define ONRSPORDERACTION 12
#define ONRSPQUERYMAXORDERVOLUME 13
#define ONRSPSETTLEMENTINFOCONFIRM 14
#define ONRSPREMOVEPARKEDORDER 15
#define ONRSPREMOVEPARKEDORDERACTION 16
#define ONRSPEXECORDERINSERT 17
#define ONRSPEXECORDERACTION 18
#define ONRSPFORQUOTEINSERT 19
#define ONRSPQUOTEINSERT 20
#define ONRSPQUOTEACTION 21
#define ONRSPBATCHORDERACTION 22
#define ONRSPOPTIONSELFCLOSEINSERT 23
#define ONRSPOPTIONSELFCLOSEACTION 24
#define ONRSPCOMBACTIONINSERT 25
#define ONRSPQRYORDER 26
#define ONRSPQRYTRADE 27
#define ONRSPQRYINVESTORPOSITION 28
#define ONRSPQRYTRADINGACCOUNT 29
#define ONRSPQRYINVESTOR 30
#define ONRSPQRYTRADINGCODE 31
#define ONRSPQRYINSTRUMENTMARGINRATE 32
#define ONRSPQRYINSTRUMENTCOMMISSIONRATE 33
#define ONRSPQRYEXCHANGE 34
#define ONRSPQRYPRODUCT 35
#define ONRSPQRYINSTRUMENT 36
#define ONRSPQRYDEPTHMARKETDATA 37
#define ONRSPQRYSETTLEMENTINFO 38
#define ONRSPQRYTRANSFERBANK 39
#define ONRSPQRYINVESTORPOSITIONDETAIL 40
#define ONRSPQRYNOTICE 41
#define ONRSPQRYSETTLEMENTINFOCONFIRM 42
#define ONRSPQRYINVESTORPOSITIONCOMBINEDETAIL 43
#define ONRSPQRYCFMMCTRADINGACCOUNTKEY 44
#define ONRSPQRYEWARRANTOFFSET 45
#define ONRSPQRYINVESTORPRODUCTGROUPMARGIN 46
#define ONRSPQRYEXCHANGEMARGINRATE 47
#define ONRSPQRYEXCHANGEMARGINRATEADJUST 48
#define ONRSPQRYEXCHANGERATE 49
#define ONRSPQRYSECAGENTACIDMAP 50
#define ONRSPQRYPRODUCTEXCHRATE 51
#define ONRSPQRYPRODUCTGROUP 52
#define ONRSPQRYMMINSTRUMENTCOMMISSIONRATE 53
#define ONRSPQRYMMOPTIONINSTRCOMMRATE 54
#define ONRSPQRYINSTRUMENTORDERCOMMRATE 55
#define ONRSPQRYOPTIONINSTRTRADECOST 56
#define ONRSPQRYOPTIONINSTRCOMMRATE 57
#define ONRSPQRYEXECORDER 58
#define ONRSPQRYFORQUOTE 59
#define ONRSPQRYQUOTE 60
#define ONRSPQRYOPTIONSELFCLOSE 61
#define ONRSPQRYINVESTUNIT 62
#define ONRSPQRYCOMBINSTRUMENTGUARD 63
#define ONRSPQRYCOMBACTION 64
#define ONRSPQRYTRANSFERSERIAL 65
#define ONRSPQRYACCOUNTREGISTER 66
#define ONRSPERROR 67
#define ONRTNORDER 68
#define ONRTNTRADE 69
#define ONERRRTNORDERINSERT 70
#define ONERRRTNORDERACTION 71
#define ONRTNINSTRUMENTSTATUS 72
#define ONRTNBULLETIN 73
#define ONRTNTRADINGNOTICE 74
#define ONRTNERRORCONDITIONALORDER 75
#define ONRTNEXECORDER 76
#define ONERRRTNEXECORDERINSERT 77
#define ONERRRTNEXECORDERACTION 78
#define ONERRRTNFORQUOTEINSERT 79
#define ONRTNQUOTE 80
#define ONERRRTNQUOTEINSERT 81
#define ONERRRTNQUOTEACTION 82
#define ONRTNFORQUOTERSP 83
#define ONRTNCFMMCTRADINGACCOUNTTOKEN 84
#define ONERRRTNBATCHORDERACTION 85
#define ONRTNOPTIONSELFCLOSE 86
#define ONERRRTNOPTIONSELFCLOSEINSERT 87
#define ONERRRTNOPTIONSELFCLOSEACTION 88
#define ONRTNCOMBACTION 89
#define ONERRRTNCOMBACTIONINSERT 90
#define ONRSPQRYCONTRACTBANK 91
#define ONRSPQRYPARKEDORDER 92
#define ONRSPQRYPARKEDORDERACTION 93
#define ONRSPQRYTRADINGNOTICE 94
#define ONRSPQRYBROKERTRADINGPARAMS 95
#define ONRSPQRYBROKERTRADINGALGOS 96
#define ONRSPQUERYCFMMCTRADINGACCOUNTTOKEN 97
#define ONRTNFROMBANKTOFUTUREBYBANK 98
#define ONRTNFROMFUTURETOBANKBYBANK 99
#define ONRTNREPEALFROMBANKTOFUTUREBYBANK 100
#define ONRTNREPEALFROMFUTURETOBANKBYBANK 101
#define ONRTNFROMBANKTOFUTUREBYFUTURE 102
#define ONRTNFROMFUTURETOBANKBYFUTURE 103
#define ONRTNREPEALFROMBANKTOFUTUREBYFUTUREMANUAL 104
#define ONRTNREPEALFROMFUTURETOBANKBYFUTUREMANUAL 105
#define ONRTNQUERYBANKBALANCEBYFUTURE 106
#define ONERRRTNBANKTOFUTUREBYFUTURE 107
#define ONERRRTNFUTURETOBANKBYFUTURE 108
#define ONERRRTNREPEALBANKTOFUTUREBYFUTUREMANUAL 109
#define ONERRRTNREPEALFUTURETOBANKBYFUTUREMANUAL 110
#define ONERRRTNQUERYBANKBALANCEBYFUTURE 111
#define ONRTNREPEALFROMBANKTOFUTUREBYFUTURE 112
#define ONRTNREPEALFROMFUTURETOBANKBYFUTURE 113
#define ONRSPFROMBANKTOFUTUREBYFUTURE 114
#define ONRSPFROMFUTURETOBANKBYFUTURE 115
#define ONRSPQUERYBANKACCOUNTMONEYBYFUTURE 116
#define ONRTNOPENACCOUNTBYBANK 117
#define ONRTNCANCELACCOUNTBYBANK 118
#define ONRTNCHANGEACCOUNTBYBANK 119

// render_data.ctp_td_header_define end



///-------------------------------------------------------------------------------------
///API中的部分组件
///-------------------------------------------------------------------------------------

//GIL全局锁简化获取用，
//用于帮助C++线程获得GIL锁，从而防止python崩溃
class PyLock
{
private:
	PyGILState_STATE gil_state;

public:
	//在某个函数方法中创建该对象时，获得GIL锁
	PyLock()
	{
		gil_state = PyGILState_Ensure();
	}

	//在某个函数完成后销毁该对象时，解放GIL锁
	~PyLock()
	{
		PyGILState_Release(gil_state);
	}
};


//任务结构体
struct Task
{
	int task_name;		//回调函数名称对应的常量
	any task_data;		//数据结构体
	any task_error;		//错误结构体
	int task_id;		//请求id
	bool task_last;		//是否为最后返回
};


///线程安全的队列
template<typename Data>

class ConcurrentQueue
{
private:
	queue<Data> the_queue;								//标准库队列
	mutable mutex the_mutex;							//boost互斥锁
	condition_variable the_condition_variable;			//boost条件变量

public:

	//存入新的任务
	void push(Data const& data)
	{
		mutex::scoped_lock lock(the_mutex);				//获取互斥锁
		the_queue.push(data);							//向队列中存入数据
		lock.unlock();									//释放锁
		the_condition_variable.notify_one();			//通知正在阻塞等待的线程
	}

	//检查队列是否为空
	bool empty() const
	{
		mutex::scoped_lock lock(the_mutex);
		return the_queue.empty();
	}

	//取出
	Data wait_and_pop()
	{
		mutex::scoped_lock lock(the_mutex);

		while (the_queue.empty())						//当队列为空时
		{
			the_condition_variable.wait(lock);			//等待条件变量通知
		}

		Data popped_value = the_queue.front();			//获取队列中的最后一个任务
		the_queue.pop();								//删除该任务
		return popped_value;							//返回该任务
	}

};


//从字典中获取某个建值对应的整数，并赋值到请求结构体对象的值上
void getInt(dict d, string key, int* value);


//从字典中获取某个建值对应的浮点数，并赋值到请求结构体对象的值上
void getDouble(dict d, string key, double* value);


//从字典中获取某个建值对应的字符，并赋值到请求结构体对象的值上
void getChar(dict d, string key, char* value);


//从字典中获取某个建值对应的字符串，并赋值到请求结构体对象的值上
void getStr(dict d, string key, char* value);


///-------------------------------------------------------------------------------------
///C++ SPI的回调函数方法实现
///-------------------------------------------------------------------------------------

//API的继承实现
class TdApi : public CThostFtdcTraderSpi
{
private:
	CThostFtdcTraderApi* api;			//API对象
	thread *task_thread;				//工作线程指针（向python中推送数据）
	ConcurrentQueue<Task> task_queue;	//任务队列

public:
	TdApi()
	{
		function0<void> f = boost::bind(&TdApi::processTask, this);
		thread t(f);
		this->task_thread = &t;
	};

	~TdApi()
	{
	};

	//-------------------------------------------------------------------------------------
	//API回调函数
	//-------------------------------------------------------------------------------------

	// render_data.ctp_td_header_on_orig start
	virtual void OnFrontConnected();
    virtual void OnFrontDisconnected(int nReason);
    virtual void OnHeartBeatWarning(int nTimeLapse);
    virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField *pInputOptionSelfClose, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspOptionSelfCloseAction(CThostFtdcInputOptionSelfCloseActionField *pInputOptionSelfCloseAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryProductExchRate(CThostFtdcProductExchRateField *pProductExchRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryProductGroup(CThostFtdcProductGroupField *pProductGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField *pMMInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField *pMMOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryInvestUnit(CThostFtdcInvestUnitField *pInvestUnit, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField *pCombInstrumentGuard, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryCombAction(CThostFtdcCombActionField *pCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) ;
    virtual void OnRtnTrade(CThostFtdcTradeField *pTrade) ;
    virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) ;
    virtual void OnRtnBulletin(CThostFtdcBulletinField *pBulletin) ;
    virtual void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) ;
    virtual void OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) ;
    virtual void OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) ;
    virtual void OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnRtnQuote(CThostFtdcQuoteField *pQuote) ;
    virtual void OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) ;
    virtual void OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken) ;
    virtual void OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *pBatchOrderAction, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnRtnOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose) ;
    virtual void OnErrRtnOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField *pInputOptionSelfClose, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnErrRtnOptionSelfCloseAction(CThostFtdcOptionSelfCloseActionField *pOptionSelfCloseAction, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnRtnCombAction(CThostFtdcCombActionField *pCombAction) ;
    virtual void OnErrRtnCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer) ;
    virtual void OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer) ;
    virtual void OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal) ;
    virtual void OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal) ;
    virtual void OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer) ;
    virtual void OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer) ;
    virtual void OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal) ;
    virtual void OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal) ;
    virtual void OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) ;
    virtual void OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo) ;
    virtual void OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal) ;
    virtual void OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal) ;
    virtual void OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    virtual void OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount) ;
    virtual void OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount) ;
    virtual void OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount) ;

	// render_data.ctp_td_header_on_orig end

	//-------------------------------------------------------------------------------------
	//task：任务
	//-------------------------------------------------------------------------------------

	void processTask();

	// render_data.ctp_td_header_process start
	void processFrontConnected(Task task);

    void processFrontDisconnected(Task task);

    void processHeartBeatWarning(Task task);

    void processRspAuthenticate(Task task);

    void processRspUserLogin(Task task);

    void processRspUserLogout(Task task);

    void processRspUserPasswordUpdate(Task task);

    void processRspTradingAccountPasswordUpdate(Task task);

    void processRspOrderInsert(Task task);

    void processRspParkedOrderInsert(Task task);

    void processRspParkedOrderAction(Task task);

    void processRspOrderAction(Task task);

    void processRspQueryMaxOrderVolume(Task task);

    void processRspSettlementInfoConfirm(Task task);

    void processRspRemoveParkedOrder(Task task);

    void processRspRemoveParkedOrderAction(Task task);

    void processRspExecOrderInsert(Task task);

    void processRspExecOrderAction(Task task);

    void processRspForQuoteInsert(Task task);

    void processRspQuoteInsert(Task task);

    void processRspQuoteAction(Task task);

    void processRspBatchOrderAction(Task task);

    void processRspOptionSelfCloseInsert(Task task);

    void processRspOptionSelfCloseAction(Task task);

    void processRspCombActionInsert(Task task);

    void processRspQryOrder(Task task);

    void processRspQryTrade(Task task);

    void processRspQryInvestorPosition(Task task);

    void processRspQryTradingAccount(Task task);

    void processRspQryInvestor(Task task);

    void processRspQryTradingCode(Task task);

    void processRspQryInstrumentMarginRate(Task task);

    void processRspQryInstrumentCommissionRate(Task task);

    void processRspQryExchange(Task task);

    void processRspQryProduct(Task task);

    void processRspQryInstrument(Task task);

    void processRspQryDepthMarketData(Task task);

    void processRspQrySettlementInfo(Task task);

    void processRspQryTransferBank(Task task);

    void processRspQryInvestorPositionDetail(Task task);

    void processRspQryNotice(Task task);

    void processRspQrySettlementInfoConfirm(Task task);

    void processRspQryInvestorPositionCombineDetail(Task task);

    void processRspQryCFMMCTradingAccountKey(Task task);

    void processRspQryEWarrantOffset(Task task);

    void processRspQryInvestorProductGroupMargin(Task task);

    void processRspQryExchangeMarginRate(Task task);

    void processRspQryExchangeMarginRateAdjust(Task task);

    void processRspQryExchangeRate(Task task);

    void processRspQrySecAgentACIDMap(Task task);

    void processRspQryProductExchRate(Task task);

    void processRspQryProductGroup(Task task);

    void processRspQryMMInstrumentCommissionRate(Task task);

    void processRspQryMMOptionInstrCommRate(Task task);

    void processRspQryInstrumentOrderCommRate(Task task);

    void processRspQryOptionInstrTradeCost(Task task);

    void processRspQryOptionInstrCommRate(Task task);

    void processRspQryExecOrder(Task task);

    void processRspQryForQuote(Task task);

    void processRspQryQuote(Task task);

    void processRspQryOptionSelfClose(Task task);

    void processRspQryInvestUnit(Task task);

    void processRspQryCombInstrumentGuard(Task task);

    void processRspQryCombAction(Task task);

    void processRspQryTransferSerial(Task task);

    void processRspQryAccountregister(Task task);

    void processRspError(Task task);

    void processRtnOrder(Task task);

    void processRtnTrade(Task task);

    void processErrRtnOrderInsert(Task task);

    void processErrRtnOrderAction(Task task);

    void processRtnInstrumentStatus(Task task);

    void processRtnBulletin(Task task);

    void processRtnTradingNotice(Task task);

    void processRtnErrorConditionalOrder(Task task);

    void processRtnExecOrder(Task task);

    void processErrRtnExecOrderInsert(Task task);

    void processErrRtnExecOrderAction(Task task);

    void processErrRtnForQuoteInsert(Task task);

    void processRtnQuote(Task task);

    void processErrRtnQuoteInsert(Task task);

    void processErrRtnQuoteAction(Task task);

    void processRtnForQuoteRsp(Task task);

    void processRtnCFMMCTradingAccountToken(Task task);

    void processErrRtnBatchOrderAction(Task task);

    void processRtnOptionSelfClose(Task task);

    void processErrRtnOptionSelfCloseInsert(Task task);

    void processErrRtnOptionSelfCloseAction(Task task);

    void processRtnCombAction(Task task);

    void processErrRtnCombActionInsert(Task task);

    void processRspQryContractBank(Task task);

    void processRspQryParkedOrder(Task task);

    void processRspQryParkedOrderAction(Task task);

    void processRspQryTradingNotice(Task task);

    void processRspQryBrokerTradingParams(Task task);

    void processRspQryBrokerTradingAlgos(Task task);

    void processRspQueryCFMMCTradingAccountToken(Task task);

    void processRtnFromBankToFutureByBank(Task task);

    void processRtnFromFutureToBankByBank(Task task);

    void processRtnRepealFromBankToFutureByBank(Task task);

    void processRtnRepealFromFutureToBankByBank(Task task);

    void processRtnFromBankToFutureByFuture(Task task);

    void processRtnFromFutureToBankByFuture(Task task);

    void processRtnRepealFromBankToFutureByFutureManual(Task task);

    void processRtnRepealFromFutureToBankByFutureManual(Task task);

    void processRtnQueryBankBalanceByFuture(Task task);

    void processErrRtnBankToFutureByFuture(Task task);

    void processErrRtnFutureToBankByFuture(Task task);

    void processErrRtnRepealBankToFutureByFutureManual(Task task);

    void processErrRtnRepealFutureToBankByFutureManual(Task task);

    void processErrRtnQueryBankBalanceByFuture(Task task);

    void processRtnRepealFromBankToFutureByFuture(Task task);

    void processRtnRepealFromFutureToBankByFuture(Task task);

    void processRspFromBankToFutureByFuture(Task task);

    void processRspFromFutureToBankByFuture(Task task);

    void processRspQueryBankAccountMoneyByFuture(Task task);

    void processRtnOpenAccountByBank(Task task);

    void processRtnCancelAccountByBank(Task task);

    void processRtnChangeAccountByBank(Task task);


	// render_data.ctp_td_header_process end

	//-------------------------------------------------------------------------------------
	//data：回调函数的数据字典
	//error：回调函数的错误字典
	//id：请求id
	//last：是否为最后返回
	//i：整数
	//-------------------------------------------------------------------------------------

	virtual void onFrontConnected(){};

	virtual void onFrontDisconnected(int i){};

	virtual void onHeartBeatWarning(int i){};

	// render_data.ctp_td_header_on start
	virtual void onRspAuthenticate(dict data, dict error, int id, bool last) {};
    virtual void onRspUserLogin(dict data, dict error, int id, bool last) {};
    virtual void onRspUserLogout(dict data, dict error, int id, bool last) {};
    virtual void onRspUserPasswordUpdate(dict data, dict error, int id, bool last) {};
    virtual void onRspTradingAccountPasswordUpdate(dict data, dict error, int id, bool last) {};
    virtual void onRspOrderInsert(dict data, dict error, int id, bool last) {};
    virtual void onRspParkedOrderInsert(dict data, dict error, int id, bool last) {};
    virtual void onRspParkedOrderAction(dict data, dict error, int id, bool last) {};
    virtual void onRspOrderAction(dict data, dict error, int id, bool last) {};
    virtual void onRspQueryMaxOrderVolume(dict data, dict error, int id, bool last) {};
    virtual void onRspSettlementInfoConfirm(dict data, dict error, int id, bool last) {};
    virtual void onRspRemoveParkedOrder(dict data, dict error, int id, bool last) {};
    virtual void onRspRemoveParkedOrderAction(dict data, dict error, int id, bool last) {};
    virtual void onRspExecOrderInsert(dict data, dict error, int id, bool last) {};
    virtual void onRspExecOrderAction(dict data, dict error, int id, bool last) {};
    virtual void onRspForQuoteInsert(dict data, dict error, int id, bool last) {};
    virtual void onRspQuoteInsert(dict data, dict error, int id, bool last) {};
    virtual void onRspQuoteAction(dict data, dict error, int id, bool last) {};
    virtual void onRspBatchOrderAction(dict data, dict error, int id, bool last) {};
    virtual void onRspOptionSelfCloseInsert(dict data, dict error, int id, bool last) {};
    virtual void onRspOptionSelfCloseAction(dict data, dict error, int id, bool last) {};
    virtual void onRspCombActionInsert(dict data, dict error, int id, bool last) {};
    virtual void onRspQryOrder(dict data, dict error, int id, bool last) {};
    virtual void onRspQryTrade(dict data, dict error, int id, bool last) {};
    virtual void onRspQryInvestorPosition(dict data, dict error, int id, bool last) {};
    virtual void onRspQryTradingAccount(dict data, dict error, int id, bool last) {};
    virtual void onRspQryInvestor(dict data, dict error, int id, bool last) {};
    virtual void onRspQryTradingCode(dict data, dict error, int id, bool last) {};
    virtual void onRspQryInstrumentMarginRate(dict data, dict error, int id, bool last) {};
    virtual void onRspQryInstrumentCommissionRate(dict data, dict error, int id, bool last) {};
    virtual void onRspQryExchange(dict data, dict error, int id, bool last) {};
    virtual void onRspQryProduct(dict data, dict error, int id, bool last) {};
    virtual void onRspQryInstrument(dict data, dict error, int id, bool last) {};
    virtual void onRspQryDepthMarketData(dict data, dict error, int id, bool last) {};
    virtual void onRspQrySettlementInfo(dict data, dict error, int id, bool last) {};
    virtual void onRspQryTransferBank(dict data, dict error, int id, bool last) {};
    virtual void onRspQryInvestorPositionDetail(dict data, dict error, int id, bool last) {};
    virtual void onRspQryNotice(dict data, dict error, int id, bool last) {};
    virtual void onRspQrySettlementInfoConfirm(dict data, dict error, int id, bool last) {};
    virtual void onRspQryInvestorPositionCombineDetail(dict data, dict error, int id, bool last) {};
    virtual void onRspQryCFMMCTradingAccountKey(dict data, dict error, int id, bool last) {};
    virtual void onRspQryEWarrantOffset(dict data, dict error, int id, bool last) {};
    virtual void onRspQryInvestorProductGroupMargin(dict data, dict error, int id, bool last) {};
    virtual void onRspQryExchangeMarginRate(dict data, dict error, int id, bool last) {};
    virtual void onRspQryExchangeMarginRateAdjust(dict data, dict error, int id, bool last) {};
    virtual void onRspQryExchangeRate(dict data, dict error, int id, bool last) {};
    virtual void onRspQrySecAgentACIDMap(dict data, dict error, int id, bool last) {};
    virtual void onRspQryProductExchRate(dict data, dict error, int id, bool last) {};
    virtual void onRspQryProductGroup(dict data, dict error, int id, bool last) {};
    virtual void onRspQryMMInstrumentCommissionRate(dict data, dict error, int id, bool last) {};
    virtual void onRspQryMMOptionInstrCommRate(dict data, dict error, int id, bool last) {};
    virtual void onRspQryInstrumentOrderCommRate(dict data, dict error, int id, bool last) {};
    virtual void onRspQryOptionInstrTradeCost(dict data, dict error, int id, bool last) {};
    virtual void onRspQryOptionInstrCommRate(dict data, dict error, int id, bool last) {};
    virtual void onRspQryExecOrder(dict data, dict error, int id, bool last) {};
    virtual void onRspQryForQuote(dict data, dict error, int id, bool last) {};
    virtual void onRspQryQuote(dict data, dict error, int id, bool last) {};
    virtual void onRspQryOptionSelfClose(dict data, dict error, int id, bool last) {};
    virtual void onRspQryInvestUnit(dict data, dict error, int id, bool last) {};
    virtual void onRspQryCombInstrumentGuard(dict data, dict error, int id, bool last) {};
    virtual void onRspQryCombAction(dict data, dict error, int id, bool last) {};
    virtual void onRspQryTransferSerial(dict data, dict error, int id, bool last) {};
    virtual void onRspQryAccountregister(dict data, dict error, int id, bool last) {};
    virtual void onRspError(dict error, int id, bool last) {};
    virtual void onRtnOrder(dict data) {};
    virtual void onRtnTrade(dict data) {};
    virtual void onErrRtnOrderInsert(dict data, dict error) {};
    virtual void onErrRtnOrderAction(dict data, dict error) {};
    virtual void onRtnInstrumentStatus(dict data) {};
    virtual void onRtnBulletin(dict data) {};
    virtual void onRtnTradingNotice(dict data) {};
    virtual void onRtnErrorConditionalOrder(dict data) {};
    virtual void onRtnExecOrder(dict data) {};
    virtual void onErrRtnExecOrderInsert(dict data, dict error) {};
    virtual void onErrRtnExecOrderAction(dict data, dict error) {};
    virtual void onErrRtnForQuoteInsert(dict data, dict error) {};
    virtual void onRtnQuote(dict data) {};
    virtual void onErrRtnQuoteInsert(dict data, dict error) {};
    virtual void onErrRtnQuoteAction(dict data, dict error) {};
    virtual void onRtnForQuoteRsp(dict data) {};
    virtual void onRtnCFMMCTradingAccountToken(dict data) {};
    virtual void onErrRtnBatchOrderAction(dict data, dict error) {};
    virtual void onRtnOptionSelfClose(dict data) {};
    virtual void onErrRtnOptionSelfCloseInsert(dict data, dict error) {};
    virtual void onErrRtnOptionSelfCloseAction(dict data, dict error) {};
    virtual void onRtnCombAction(dict data) {};
    virtual void onErrRtnCombActionInsert(dict data, dict error) {};
    virtual void onRspQryContractBank(dict data, dict error, int id, bool last) {};
    virtual void onRspQryParkedOrder(dict data, dict error, int id, bool last) {};
    virtual void onRspQryParkedOrderAction(dict data, dict error, int id, bool last) {};
    virtual void onRspQryTradingNotice(dict data, dict error, int id, bool last) {};
    virtual void onRspQryBrokerTradingParams(dict data, dict error, int id, bool last) {};
    virtual void onRspQryBrokerTradingAlgos(dict data, dict error, int id, bool last) {};
    virtual void onRspQueryCFMMCTradingAccountToken(dict data, dict error, int id, bool last) {};
    virtual void onRtnFromBankToFutureByBank(dict data) {};
    virtual void onRtnFromFutureToBankByBank(dict data) {};
    virtual void onRtnRepealFromBankToFutureByBank(dict data) {};
    virtual void onRtnRepealFromFutureToBankByBank(dict data) {};
    virtual void onRtnFromBankToFutureByFuture(dict data) {};
    virtual void onRtnFromFutureToBankByFuture(dict data) {};
    virtual void onRtnRepealFromBankToFutureByFutureManual(dict data) {};
    virtual void onRtnRepealFromFutureToBankByFutureManual(dict data) {};
    virtual void onRtnQueryBankBalanceByFuture(dict data) {};
    virtual void onErrRtnBankToFutureByFuture(dict data, dict error) {};
    virtual void onErrRtnFutureToBankByFuture(dict data, dict error) {};
    virtual void onErrRtnRepealBankToFutureByFutureManual(dict data, dict error) {};
    virtual void onErrRtnRepealFutureToBankByFutureManual(dict data, dict error) {};
    virtual void onErrRtnQueryBankBalanceByFuture(dict data, dict error) {};
    virtual void onRtnRepealFromBankToFutureByFuture(dict data) {};
    virtual void onRtnRepealFromFutureToBankByFuture(dict data) {};
    virtual void onRspFromBankToFutureByFuture(dict data, dict error, int id, bool last) {};
    virtual void onRspFromFutureToBankByFuture(dict data, dict error, int id, bool last) {};
    virtual void onRspQueryBankAccountMoneyByFuture(dict data, dict error, int id, bool last) {};
    virtual void onRtnOpenAccountByBank(dict data) {};
    virtual void onRtnCancelAccountByBank(dict data) {};
    virtual void onRtnChangeAccountByBank(dict data) {};

	// render_data.ctp_td_header_on end

	//-------------------------------------------------------------------------------------
	//req:主动函数的请求字典
	//-------------------------------------------------------------------------------------

	void createFtdcTraderApi(string pszFlowPath = "");

	void release();

	void init();

	int join();

	int exit();

	string getTradingDay();

	void registerFront(string pszFrontAddress);

	void subscribePrivateTopic(int nType);

	void subscribePublicTopic(int nType);

	// render_data.ctp_td_header_function start
	int reqAuthenticate(dict req, int nRequestID);

    int reqUserLogin(dict req, int nRequestID);

    int reqUserLogout(dict req, int nRequestID);

    int reqUserPasswordUpdate(dict req, int nRequestID);

    int reqTradingAccountPasswordUpdate(dict req, int nRequestID);

    int reqUserLogin2(dict req, int nRequestID);

    int reqUserPasswordUpdate2(dict req, int nRequestID);

    int reqOrderInsert(dict req, int nRequestID);

    int reqParkedOrderInsert(dict req, int nRequestID);

    int reqParkedOrderAction(dict req, int nRequestID);

    int reqOrderAction(dict req, int nRequestID);

    int reqQueryMaxOrderVolume(dict req, int nRequestID);

    int reqSettlementInfoConfirm(dict req, int nRequestID);

    int reqRemoveParkedOrder(dict req, int nRequestID);

    int reqRemoveParkedOrderAction(dict req, int nRequestID);

    int reqExecOrderInsert(dict req, int nRequestID);

    int reqExecOrderAction(dict req, int nRequestID);

    int reqForQuoteInsert(dict req, int nRequestID);

    int reqQuoteInsert(dict req, int nRequestID);

    int reqQuoteAction(dict req, int nRequestID);

    int reqBatchOrderAction(dict req, int nRequestID);

    int reqOptionSelfCloseInsert(dict req, int nRequestID);

    int reqOptionSelfCloseAction(dict req, int nRequestID);

    int reqCombActionInsert(dict req, int nRequestID);

    int reqQryOrder(dict req, int nRequestID);

    int reqQryTrade(dict req, int nRequestID);

    int reqQryInvestorPosition(dict req, int nRequestID);

    int reqQryTradingAccount(dict req, int nRequestID);

    int reqQryInvestor(dict req, int nRequestID);

    int reqQryTradingCode(dict req, int nRequestID);

    int reqQryInstrumentMarginRate(dict req, int nRequestID);

    int reqQryInstrumentCommissionRate(dict req, int nRequestID);

    int reqQryExchange(dict req, int nRequestID);

    int reqQryProduct(dict req, int nRequestID);

    int reqQryInstrument(dict req, int nRequestID);

    int reqQryDepthMarketData(dict req, int nRequestID);

    int reqQrySettlementInfo(dict req, int nRequestID);

    int reqQryTransferBank(dict req, int nRequestID);

    int reqQryInvestorPositionDetail(dict req, int nRequestID);

    int reqQryNotice(dict req, int nRequestID);

    int reqQrySettlementInfoConfirm(dict req, int nRequestID);

    int reqQryInvestorPositionCombineDetail(dict req, int nRequestID);

    int reqQryCFMMCTradingAccountKey(dict req, int nRequestID);

    int reqQryEWarrantOffset(dict req, int nRequestID);

    int reqQryInvestorProductGroupMargin(dict req, int nRequestID);

    int reqQryExchangeMarginRate(dict req, int nRequestID);

    int reqQryExchangeMarginRateAdjust(dict req, int nRequestID);

    int reqQryExchangeRate(dict req, int nRequestID);

    int reqQrySecAgentACIDMap(dict req, int nRequestID);

    int reqQryProductExchRate(dict req, int nRequestID);

    int reqQryProductGroup(dict req, int nRequestID);

    int reqQryMMInstrumentCommissionRate(dict req, int nRequestID);

    int reqQryMMOptionInstrCommRate(dict req, int nRequestID);

    int reqQryInstrumentOrderCommRate(dict req, int nRequestID);

    int reqQryOptionInstrTradeCost(dict req, int nRequestID);

    int reqQryOptionInstrCommRate(dict req, int nRequestID);

    int reqQryExecOrder(dict req, int nRequestID);

    int reqQryForQuote(dict req, int nRequestID);

    int reqQryQuote(dict req, int nRequestID);

    int reqQryOptionSelfClose(dict req, int nRequestID);

    int reqQryInvestUnit(dict req, int nRequestID);

    int reqQryCombInstrumentGuard(dict req, int nRequestID);

    int reqQryCombAction(dict req, int nRequestID);

    int reqQryTransferSerial(dict req, int nRequestID);

    int reqQryAccountregister(dict req, int nRequestID);

    int reqQryContractBank(dict req, int nRequestID);

    int reqQryParkedOrder(dict req, int nRequestID);

    int reqQryParkedOrderAction(dict req, int nRequestID);

    int reqQryTradingNotice(dict req, int nRequestID);

    int reqQryBrokerTradingParams(dict req, int nRequestID);

    int reqQryBrokerTradingAlgos(dict req, int nRequestID);

    int reqQueryCFMMCTradingAccountToken(dict req, int nRequestID);

    int reqFromBankToFutureByFuture(dict req, int nRequestID);

    int reqFromFutureToBankByFuture(dict req, int nRequestID);

    int reqQueryBankAccountMoneyByFuture(dict req, int nRequestID);


	// render_data.ctp_td_header_function end

};
