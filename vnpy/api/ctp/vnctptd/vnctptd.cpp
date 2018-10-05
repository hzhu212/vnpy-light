// vnctptd.cpp : 定义 DLL 应用程序的导出函数。
//

#include "vnctptd.h"


///-------------------------------------------------------------------------------------
///从Python对象到C++类型转换用的函数
///-------------------------------------------------------------------------------------

void getInt(dict d, string key, int *value)
{
	if (d.has_key(key))		//检查字典中是否存在该键值
	{
		object o = d[key];	//获取该键值
		extract<int> x(o);	//创建提取器
		if (x.check())		//如果可以提取
		{
			*value = x();	//对目标整数指针赋值
		}
	}
}

void getDouble(dict d, string key, double *value)
{
	if (d.has_key(key))
	{
		object o = d[key];
		extract<double> x(o);
		if (x.check())
		{
			*value = x();
		}
	}
}

void getStr(dict d, string key, char *value)
{
	if (d.has_key(key))
	{
		object o = d[key];
		extract<string> x(o);
		if (x.check())
		{
			string s = x();
			const char *buffer = s.c_str();
			//对字符串指针赋值必须使用strcpy_s, vs2013使用strcpy编译通不过
			//+1应该是因为C++字符串的结尾符号？不是特别确定，不加这个1会出错
#ifdef _MSC_VER //WIN32
			strcpy_s(value, strlen(buffer) + 1, buffer);
#elif __GNUC__
			strncpy(value, buffer, strlen(buffer) + 1);
#endif
		}
	}
}

void getChar(dict d, string key, char *value)
{
	if (d.has_key(key))
	{
		object o = d[key];
		extract<string> x(o);
		if (x.check())
		{
			string s = x();
			const char *buffer = s.c_str();
			*value = *buffer;
		}
	}
}


///-------------------------------------------------------------------------------------
///C++的回调函数将数据保存到队列中
///-------------------------------------------------------------------------------------

// render_data.ctp_td_task start
void TdApi::OnFrontConnected()
{
	Task task = Task();
	task.task_name = ONFRONTCONNECTED;
	this->task_queue.push(task);
};

void TdApi::OnFrontDisconnected(int nReason)
{
	Task task = Task();
	task.task_name = ONFRONTDISCONNECTED;
	task.task_id = nReason;
	this->task_queue.push(task);
};

void TdApi::OnHeartBeatWarning(int nTimeLapse)
{
	Task task = Task();
	task.task_name = ONHEARTBEATWARNING;
	task.task_id = nTimeLapse;
	this->task_queue.push(task);
};

void TdApi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPAUTHENTICATE;

	if (pRspAuthenticateField)
	{
		task.task_data = *pRspAuthenticateField;
	}
	else
	{
		CThostFtdcRspAuthenticateField empty_data = CThostFtdcRspAuthenticateField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPUSERLOGIN;

	if (pRspUserLogin)
	{
		task.task_data = *pRspUserLogin;
	}
	else
	{
		CThostFtdcRspUserLoginField empty_data = CThostFtdcRspUserLoginField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPUSERLOGOUT;

	if (pUserLogout)
	{
		task.task_data = *pUserLogout;
	}
	else
	{
		CThostFtdcUserLogoutField empty_data = CThostFtdcUserLogoutField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPUSERPASSWORDUPDATE;

	if (pUserPasswordUpdate)
	{
		task.task_data = *pUserPasswordUpdate;
	}
	else
	{
		CThostFtdcUserPasswordUpdateField empty_data = CThostFtdcUserPasswordUpdateField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPTRADINGACCOUNTPASSWORDUPDATE;

	if (pTradingAccountPasswordUpdate)
	{
		task.task_data = *pTradingAccountPasswordUpdate;
	}
	else
	{
		CThostFtdcTradingAccountPasswordUpdateField empty_data = CThostFtdcTradingAccountPasswordUpdateField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPORDERINSERT;

	if (pInputOrder)
	{
		task.task_data = *pInputOrder;
	}
	else
	{
		CThostFtdcInputOrderField empty_data = CThostFtdcInputOrderField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPPARKEDORDERINSERT;

	if (pParkedOrder)
	{
		task.task_data = *pParkedOrder;
	}
	else
	{
		CThostFtdcParkedOrderField empty_data = CThostFtdcParkedOrderField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPPARKEDORDERACTION;

	if (pParkedOrderAction)
	{
		task.task_data = *pParkedOrderAction;
	}
	else
	{
		CThostFtdcParkedOrderActionField empty_data = CThostFtdcParkedOrderActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPORDERACTION;

	if (pInputOrderAction)
	{
		task.task_data = *pInputOrderAction;
	}
	else
	{
		CThostFtdcInputOrderActionField empty_data = CThostFtdcInputOrderActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQUERYMAXORDERVOLUME;

	if (pQueryMaxOrderVolume)
	{
		task.task_data = *pQueryMaxOrderVolume;
	}
	else
	{
		CThostFtdcQueryMaxOrderVolumeField empty_data = CThostFtdcQueryMaxOrderVolumeField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPSETTLEMENTINFOCONFIRM;

	if (pSettlementInfoConfirm)
	{
		task.task_data = *pSettlementInfoConfirm;
	}
	else
	{
		CThostFtdcSettlementInfoConfirmField empty_data = CThostFtdcSettlementInfoConfirmField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPREMOVEPARKEDORDER;

	if (pRemoveParkedOrder)
	{
		task.task_data = *pRemoveParkedOrder;
	}
	else
	{
		CThostFtdcRemoveParkedOrderField empty_data = CThostFtdcRemoveParkedOrderField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPREMOVEPARKEDORDERACTION;

	if (pRemoveParkedOrderAction)
	{
		task.task_data = *pRemoveParkedOrderAction;
	}
	else
	{
		CThostFtdcRemoveParkedOrderActionField empty_data = CThostFtdcRemoveParkedOrderActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPEXECORDERINSERT;

	if (pInputExecOrder)
	{
		task.task_data = *pInputExecOrder;
	}
	else
	{
		CThostFtdcInputExecOrderField empty_data = CThostFtdcInputExecOrderField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPEXECORDERACTION;

	if (pInputExecOrderAction)
	{
		task.task_data = *pInputExecOrderAction;
	}
	else
	{
		CThostFtdcInputExecOrderActionField empty_data = CThostFtdcInputExecOrderActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPFORQUOTEINSERT;

	if (pInputForQuote)
	{
		task.task_data = *pInputForQuote;
	}
	else
	{
		CThostFtdcInputForQuoteField empty_data = CThostFtdcInputForQuoteField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQUOTEINSERT;

	if (pInputQuote)
	{
		task.task_data = *pInputQuote;
	}
	else
	{
		CThostFtdcInputQuoteField empty_data = CThostFtdcInputQuoteField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQUOTEACTION;

	if (pInputQuoteAction)
	{
		task.task_data = *pInputQuoteAction;
	}
	else
	{
		CThostFtdcInputQuoteActionField empty_data = CThostFtdcInputQuoteActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPBATCHORDERACTION;

	if (pInputBatchOrderAction)
	{
		task.task_data = *pInputBatchOrderAction;
	}
	else
	{
		CThostFtdcInputBatchOrderActionField empty_data = CThostFtdcInputBatchOrderActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField *pInputOptionSelfClose, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPOPTIONSELFCLOSEINSERT;

	if (pInputOptionSelfClose)
	{
		task.task_data = *pInputOptionSelfClose;
	}
	else
	{
		CThostFtdcInputOptionSelfCloseField empty_data = CThostFtdcInputOptionSelfCloseField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspOptionSelfCloseAction(CThostFtdcInputOptionSelfCloseActionField *pInputOptionSelfCloseAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPOPTIONSELFCLOSEACTION;

	if (pInputOptionSelfCloseAction)
	{
		task.task_data = *pInputOptionSelfCloseAction;
	}
	else
	{
		CThostFtdcInputOptionSelfCloseActionField empty_data = CThostFtdcInputOptionSelfCloseActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPCOMBACTIONINSERT;

	if (pInputCombAction)
	{
		task.task_data = *pInputCombAction;
	}
	else
	{
		CThostFtdcInputCombActionField empty_data = CThostFtdcInputCombActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYORDER;

	if (pOrder)
	{
		task.task_data = *pOrder;
	}
	else
	{
		CThostFtdcOrderField empty_data = CThostFtdcOrderField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYTRADE;

	if (pTrade)
	{
		task.task_data = *pTrade;
	}
	else
	{
		CThostFtdcTradeField empty_data = CThostFtdcTradeField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYINVESTORPOSITION;

	if (pInvestorPosition)
	{
		task.task_data = *pInvestorPosition;
	}
	else
	{
		CThostFtdcInvestorPositionField empty_data = CThostFtdcInvestorPositionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYTRADINGACCOUNT;

	if (pTradingAccount)
	{
		task.task_data = *pTradingAccount;
	}
	else
	{
		CThostFtdcTradingAccountField empty_data = CThostFtdcTradingAccountField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYINVESTOR;

	if (pInvestor)
	{
		task.task_data = *pInvestor;
	}
	else
	{
		CThostFtdcInvestorField empty_data = CThostFtdcInvestorField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYTRADINGCODE;

	if (pTradingCode)
	{
		task.task_data = *pTradingCode;
	}
	else
	{
		CThostFtdcTradingCodeField empty_data = CThostFtdcTradingCodeField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYINSTRUMENTMARGINRATE;

	if (pInstrumentMarginRate)
	{
		task.task_data = *pInstrumentMarginRate;
	}
	else
	{
		CThostFtdcInstrumentMarginRateField empty_data = CThostFtdcInstrumentMarginRateField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYINSTRUMENTCOMMISSIONRATE;

	if (pInstrumentCommissionRate)
	{
		task.task_data = *pInstrumentCommissionRate;
	}
	else
	{
		CThostFtdcInstrumentCommissionRateField empty_data = CThostFtdcInstrumentCommissionRateField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYEXCHANGE;

	if (pExchange)
	{
		task.task_data = *pExchange;
	}
	else
	{
		CThostFtdcExchangeField empty_data = CThostFtdcExchangeField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYPRODUCT;

	if (pProduct)
	{
		task.task_data = *pProduct;
	}
	else
	{
		CThostFtdcProductField empty_data = CThostFtdcProductField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYINSTRUMENT;

	if (pInstrument)
	{
		task.task_data = *pInstrument;
	}
	else
	{
		CThostFtdcInstrumentField empty_data = CThostFtdcInstrumentField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYDEPTHMARKETDATA;

	if (pDepthMarketData)
	{
		task.task_data = *pDepthMarketData;
	}
	else
	{
		CThostFtdcDepthMarketDataField empty_data = CThostFtdcDepthMarketDataField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYSETTLEMENTINFO;

	if (pSettlementInfo)
	{
		task.task_data = *pSettlementInfo;
	}
	else
	{
		CThostFtdcSettlementInfoField empty_data = CThostFtdcSettlementInfoField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYTRANSFERBANK;

	if (pTransferBank)
	{
		task.task_data = *pTransferBank;
	}
	else
	{
		CThostFtdcTransferBankField empty_data = CThostFtdcTransferBankField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYINVESTORPOSITIONDETAIL;

	if (pInvestorPositionDetail)
	{
		task.task_data = *pInvestorPositionDetail;
	}
	else
	{
		CThostFtdcInvestorPositionDetailField empty_data = CThostFtdcInvestorPositionDetailField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYNOTICE;

	if (pNotice)
	{
		task.task_data = *pNotice;
	}
	else
	{
		CThostFtdcNoticeField empty_data = CThostFtdcNoticeField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYSETTLEMENTINFOCONFIRM;

	if (pSettlementInfoConfirm)
	{
		task.task_data = *pSettlementInfoConfirm;
	}
	else
	{
		CThostFtdcSettlementInfoConfirmField empty_data = CThostFtdcSettlementInfoConfirmField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYINVESTORPOSITIONCOMBINEDETAIL;

	if (pInvestorPositionCombineDetail)
	{
		task.task_data = *pInvestorPositionCombineDetail;
	}
	else
	{
		CThostFtdcInvestorPositionCombineDetailField empty_data = CThostFtdcInvestorPositionCombineDetailField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYCFMMCTRADINGACCOUNTKEY;

	if (pCFMMCTradingAccountKey)
	{
		task.task_data = *pCFMMCTradingAccountKey;
	}
	else
	{
		CThostFtdcCFMMCTradingAccountKeyField empty_data = CThostFtdcCFMMCTradingAccountKeyField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYEWARRANTOFFSET;

	if (pEWarrantOffset)
	{
		task.task_data = *pEWarrantOffset;
	}
	else
	{
		CThostFtdcEWarrantOffsetField empty_data = CThostFtdcEWarrantOffsetField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYINVESTORPRODUCTGROUPMARGIN;

	if (pInvestorProductGroupMargin)
	{
		task.task_data = *pInvestorProductGroupMargin;
	}
	else
	{
		CThostFtdcInvestorProductGroupMarginField empty_data = CThostFtdcInvestorProductGroupMarginField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYEXCHANGEMARGINRATE;

	if (pExchangeMarginRate)
	{
		task.task_data = *pExchangeMarginRate;
	}
	else
	{
		CThostFtdcExchangeMarginRateField empty_data = CThostFtdcExchangeMarginRateField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYEXCHANGEMARGINRATEADJUST;

	if (pExchangeMarginRateAdjust)
	{
		task.task_data = *pExchangeMarginRateAdjust;
	}
	else
	{
		CThostFtdcExchangeMarginRateAdjustField empty_data = CThostFtdcExchangeMarginRateAdjustField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYEXCHANGERATE;

	if (pExchangeRate)
	{
		task.task_data = *pExchangeRate;
	}
	else
	{
		CThostFtdcExchangeRateField empty_data = CThostFtdcExchangeRateField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYSECAGENTACIDMAP;

	if (pSecAgentACIDMap)
	{
		task.task_data = *pSecAgentACIDMap;
	}
	else
	{
		CThostFtdcSecAgentACIDMapField empty_data = CThostFtdcSecAgentACIDMapField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryProductExchRate(CThostFtdcProductExchRateField *pProductExchRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYPRODUCTEXCHRATE;

	if (pProductExchRate)
	{
		task.task_data = *pProductExchRate;
	}
	else
	{
		CThostFtdcProductExchRateField empty_data = CThostFtdcProductExchRateField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryProductGroup(CThostFtdcProductGroupField *pProductGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYPRODUCTGROUP;

	if (pProductGroup)
	{
		task.task_data = *pProductGroup;
	}
	else
	{
		CThostFtdcProductGroupField empty_data = CThostFtdcProductGroupField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField *pMMInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYMMINSTRUMENTCOMMISSIONRATE;

	if (pMMInstrumentCommissionRate)
	{
		task.task_data = *pMMInstrumentCommissionRate;
	}
	else
	{
		CThostFtdcMMInstrumentCommissionRateField empty_data = CThostFtdcMMInstrumentCommissionRateField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField *pMMOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYMMOPTIONINSTRCOMMRATE;

	if (pMMOptionInstrCommRate)
	{
		task.task_data = *pMMOptionInstrCommRate;
	}
	else
	{
		CThostFtdcMMOptionInstrCommRateField empty_data = CThostFtdcMMOptionInstrCommRateField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYINSTRUMENTORDERCOMMRATE;

	if (pInstrumentOrderCommRate)
	{
		task.task_data = *pInstrumentOrderCommRate;
	}
	else
	{
		CThostFtdcInstrumentOrderCommRateField empty_data = CThostFtdcInstrumentOrderCommRateField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYOPTIONINSTRTRADECOST;

	if (pOptionInstrTradeCost)
	{
		task.task_data = *pOptionInstrTradeCost;
	}
	else
	{
		CThostFtdcOptionInstrTradeCostField empty_data = CThostFtdcOptionInstrTradeCostField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYOPTIONINSTRCOMMRATE;

	if (pOptionInstrCommRate)
	{
		task.task_data = *pOptionInstrCommRate;
	}
	else
	{
		CThostFtdcOptionInstrCommRateField empty_data = CThostFtdcOptionInstrCommRateField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYEXECORDER;

	if (pExecOrder)
	{
		task.task_data = *pExecOrder;
	}
	else
	{
		CThostFtdcExecOrderField empty_data = CThostFtdcExecOrderField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYFORQUOTE;

	if (pForQuote)
	{
		task.task_data = *pForQuote;
	}
	else
	{
		CThostFtdcForQuoteField empty_data = CThostFtdcForQuoteField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYQUOTE;

	if (pQuote)
	{
		task.task_data = *pQuote;
	}
	else
	{
		CThostFtdcQuoteField empty_data = CThostFtdcQuoteField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYOPTIONSELFCLOSE;

	if (pOptionSelfClose)
	{
		task.task_data = *pOptionSelfClose;
	}
	else
	{
		CThostFtdcOptionSelfCloseField empty_data = CThostFtdcOptionSelfCloseField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryInvestUnit(CThostFtdcInvestUnitField *pInvestUnit, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYINVESTUNIT;

	if (pInvestUnit)
	{
		task.task_data = *pInvestUnit;
	}
	else
	{
		CThostFtdcInvestUnitField empty_data = CThostFtdcInvestUnitField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField *pCombInstrumentGuard, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYCOMBINSTRUMENTGUARD;

	if (pCombInstrumentGuard)
	{
		task.task_data = *pCombInstrumentGuard;
	}
	else
	{
		CThostFtdcCombInstrumentGuardField empty_data = CThostFtdcCombInstrumentGuardField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryCombAction(CThostFtdcCombActionField *pCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYCOMBACTION;

	if (pCombAction)
	{
		task.task_data = *pCombAction;
	}
	else
	{
		CThostFtdcCombActionField empty_data = CThostFtdcCombActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYTRANSFERSERIAL;

	if (pTransferSerial)
	{
		task.task_data = *pTransferSerial;
	}
	else
	{
		CThostFtdcTransferSerialField empty_data = CThostFtdcTransferSerialField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYACCOUNTREGISTER;

	if (pAccountregister)
	{
		task.task_data = *pAccountregister;
	}
	else
	{
		CThostFtdcAccountregisterField empty_data = CThostFtdcAccountregisterField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPERROR;

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRtnOrder(CThostFtdcOrderField *pOrder) 
{
	Task task = Task();
	task.task_name = ONRTNORDER;

	if (pOrder)
	{
		task.task_data = *pOrder;
	}
	else
	{
		CThostFtdcOrderField empty_data = CThostFtdcOrderField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnTrade(CThostFtdcTradeField *pTrade) 
{
	Task task = Task();
	task.task_name = ONRTNTRADE;

	if (pTrade)
	{
		task.task_data = *pTrade;
	}
	else
	{
		CThostFtdcTradeField empty_data = CThostFtdcTradeField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNORDERINSERT;

	if (pInputOrder)
	{
		task.task_data = *pInputOrder;
	}
	else
	{
		CThostFtdcInputOrderField empty_data = CThostFtdcInputOrderField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNORDERACTION;

	if (pOrderAction)
	{
		task.task_data = *pOrderAction;
	}
	else
	{
		CThostFtdcOrderActionField empty_data = CThostFtdcOrderActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) 
{
	Task task = Task();
	task.task_name = ONRTNINSTRUMENTSTATUS;

	if (pInstrumentStatus)
	{
		task.task_data = *pInstrumentStatus;
	}
	else
	{
		CThostFtdcInstrumentStatusField empty_data = CThostFtdcInstrumentStatusField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnBulletin(CThostFtdcBulletinField *pBulletin) 
{
	Task task = Task();
	task.task_name = ONRTNBULLETIN;

	if (pBulletin)
	{
		task.task_data = *pBulletin;
	}
	else
	{
		CThostFtdcBulletinField empty_data = CThostFtdcBulletinField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) 
{
	Task task = Task();
	task.task_name = ONRTNTRADINGNOTICE;

	if (pTradingNoticeInfo)
	{
		task.task_data = *pTradingNoticeInfo;
	}
	else
	{
		CThostFtdcTradingNoticeInfoField empty_data = CThostFtdcTradingNoticeInfoField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) 
{
	Task task = Task();
	task.task_name = ONRTNERRORCONDITIONALORDER;

	if (pErrorConditionalOrder)
	{
		task.task_data = *pErrorConditionalOrder;
	}
	else
	{
		CThostFtdcErrorConditionalOrderField empty_data = CThostFtdcErrorConditionalOrderField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) 
{
	Task task = Task();
	task.task_name = ONRTNEXECORDER;

	if (pExecOrder)
	{
		task.task_data = *pExecOrder;
	}
	else
	{
		CThostFtdcExecOrderField empty_data = CThostFtdcExecOrderField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNEXECORDERINSERT;

	if (pInputExecOrder)
	{
		task.task_data = *pInputExecOrder;
	}
	else
	{
		CThostFtdcInputExecOrderField empty_data = CThostFtdcInputExecOrderField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNEXECORDERACTION;

	if (pExecOrderAction)
	{
		task.task_data = *pExecOrderAction;
	}
	else
	{
		CThostFtdcExecOrderActionField empty_data = CThostFtdcExecOrderActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNFORQUOTEINSERT;

	if (pInputForQuote)
	{
		task.task_data = *pInputForQuote;
	}
	else
	{
		CThostFtdcInputForQuoteField empty_data = CThostFtdcInputForQuoteField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnQuote(CThostFtdcQuoteField *pQuote) 
{
	Task task = Task();
	task.task_name = ONRTNQUOTE;

	if (pQuote)
	{
		task.task_data = *pQuote;
	}
	else
	{
		CThostFtdcQuoteField empty_data = CThostFtdcQuoteField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNQUOTEINSERT;

	if (pInputQuote)
	{
		task.task_data = *pInputQuote;
	}
	else
	{
		CThostFtdcInputQuoteField empty_data = CThostFtdcInputQuoteField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNQUOTEACTION;

	if (pQuoteAction)
	{
		task.task_data = *pQuoteAction;
	}
	else
	{
		CThostFtdcQuoteActionField empty_data = CThostFtdcQuoteActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) 
{
	Task task = Task();
	task.task_name = ONRTNFORQUOTERSP;

	if (pForQuoteRsp)
	{
		task.task_data = *pForQuoteRsp;
	}
	else
	{
		CThostFtdcForQuoteRspField empty_data = CThostFtdcForQuoteRspField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken) 
{
	Task task = Task();
	task.task_name = ONRTNCFMMCTRADINGACCOUNTTOKEN;

	if (pCFMMCTradingAccountToken)
	{
		task.task_data = *pCFMMCTradingAccountToken;
	}
	else
	{
		CThostFtdcCFMMCTradingAccountTokenField empty_data = CThostFtdcCFMMCTradingAccountTokenField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *pBatchOrderAction, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNBATCHORDERACTION;

	if (pBatchOrderAction)
	{
		task.task_data = *pBatchOrderAction;
	}
	else
	{
		CThostFtdcBatchOrderActionField empty_data = CThostFtdcBatchOrderActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose) 
{
	Task task = Task();
	task.task_name = ONRTNOPTIONSELFCLOSE;

	if (pOptionSelfClose)
	{
		task.task_data = *pOptionSelfClose;
	}
	else
	{
		CThostFtdcOptionSelfCloseField empty_data = CThostFtdcOptionSelfCloseField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField *pInputOptionSelfClose, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNOPTIONSELFCLOSEINSERT;

	if (pInputOptionSelfClose)
	{
		task.task_data = *pInputOptionSelfClose;
	}
	else
	{
		CThostFtdcInputOptionSelfCloseField empty_data = CThostFtdcInputOptionSelfCloseField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnOptionSelfCloseAction(CThostFtdcOptionSelfCloseActionField *pOptionSelfCloseAction, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNOPTIONSELFCLOSEACTION;

	if (pOptionSelfCloseAction)
	{
		task.task_data = *pOptionSelfCloseAction;
	}
	else
	{
		CThostFtdcOptionSelfCloseActionField empty_data = CThostFtdcOptionSelfCloseActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnCombAction(CThostFtdcCombActionField *pCombAction) 
{
	Task task = Task();
	task.task_name = ONRTNCOMBACTION;

	if (pCombAction)
	{
		task.task_data = *pCombAction;
	}
	else
	{
		CThostFtdcCombActionField empty_data = CThostFtdcCombActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNCOMBACTIONINSERT;

	if (pInputCombAction)
	{
		task.task_data = *pInputCombAction;
	}
	else
	{
		CThostFtdcInputCombActionField empty_data = CThostFtdcInputCombActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYCONTRACTBANK;

	if (pContractBank)
	{
		task.task_data = *pContractBank;
	}
	else
	{
		CThostFtdcContractBankField empty_data = CThostFtdcContractBankField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYPARKEDORDER;

	if (pParkedOrder)
	{
		task.task_data = *pParkedOrder;
	}
	else
	{
		CThostFtdcParkedOrderField empty_data = CThostFtdcParkedOrderField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYPARKEDORDERACTION;

	if (pParkedOrderAction)
	{
		task.task_data = *pParkedOrderAction;
	}
	else
	{
		CThostFtdcParkedOrderActionField empty_data = CThostFtdcParkedOrderActionField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYTRADINGNOTICE;

	if (pTradingNotice)
	{
		task.task_data = *pTradingNotice;
	}
	else
	{
		CThostFtdcTradingNoticeField empty_data = CThostFtdcTradingNoticeField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYBROKERTRADINGPARAMS;

	if (pBrokerTradingParams)
	{
		task.task_data = *pBrokerTradingParams;
	}
	else
	{
		CThostFtdcBrokerTradingParamsField empty_data = CThostFtdcBrokerTradingParamsField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQRYBROKERTRADINGALGOS;

	if (pBrokerTradingAlgos)
	{
		task.task_data = *pBrokerTradingAlgos;
	}
	else
	{
		CThostFtdcBrokerTradingAlgosField empty_data = CThostFtdcBrokerTradingAlgosField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQUERYCFMMCTRADINGACCOUNTTOKEN;

	if (pQueryCFMMCTradingAccountToken)
	{
		task.task_data = *pQueryCFMMCTradingAccountToken;
	}
	else
	{
		CThostFtdcQueryCFMMCTradingAccountTokenField empty_data = CThostFtdcQueryCFMMCTradingAccountTokenField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer) 
{
	Task task = Task();
	task.task_name = ONRTNFROMBANKTOFUTUREBYBANK;

	if (pRspTransfer)
	{
		task.task_data = *pRspTransfer;
	}
	else
	{
		CThostFtdcRspTransferField empty_data = CThostFtdcRspTransferField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer) 
{
	Task task = Task();
	task.task_name = ONRTNFROMFUTURETOBANKBYBANK;

	if (pRspTransfer)
	{
		task.task_data = *pRspTransfer;
	}
	else
	{
		CThostFtdcRspTransferField empty_data = CThostFtdcRspTransferField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal) 
{
	Task task = Task();
	task.task_name = ONRTNREPEALFROMBANKTOFUTUREBYBANK;

	if (pRspRepeal)
	{
		task.task_data = *pRspRepeal;
	}
	else
	{
		CThostFtdcRspRepealField empty_data = CThostFtdcRspRepealField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal) 
{
	Task task = Task();
	task.task_name = ONRTNREPEALFROMFUTURETOBANKBYBANK;

	if (pRspRepeal)
	{
		task.task_data = *pRspRepeal;
	}
	else
	{
		CThostFtdcRspRepealField empty_data = CThostFtdcRspRepealField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer) 
{
	Task task = Task();
	task.task_name = ONRTNFROMBANKTOFUTUREBYFUTURE;

	if (pRspTransfer)
	{
		task.task_data = *pRspTransfer;
	}
	else
	{
		CThostFtdcRspTransferField empty_data = CThostFtdcRspTransferField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer) 
{
	Task task = Task();
	task.task_name = ONRTNFROMFUTURETOBANKBYFUTURE;

	if (pRspTransfer)
	{
		task.task_data = *pRspTransfer;
	}
	else
	{
		CThostFtdcRspTransferField empty_data = CThostFtdcRspTransferField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal) 
{
	Task task = Task();
	task.task_name = ONRTNREPEALFROMBANKTOFUTUREBYFUTUREMANUAL;

	if (pRspRepeal)
	{
		task.task_data = *pRspRepeal;
	}
	else
	{
		CThostFtdcRspRepealField empty_data = CThostFtdcRspRepealField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal) 
{
	Task task = Task();
	task.task_name = ONRTNREPEALFROMFUTURETOBANKBYFUTUREMANUAL;

	if (pRspRepeal)
	{
		task.task_data = *pRspRepeal;
	}
	else
	{
		CThostFtdcRspRepealField empty_data = CThostFtdcRspRepealField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) 
{
	Task task = Task();
	task.task_name = ONRTNQUERYBANKBALANCEBYFUTURE;

	if (pNotifyQueryAccount)
	{
		task.task_data = *pNotifyQueryAccount;
	}
	else
	{
		CThostFtdcNotifyQueryAccountField empty_data = CThostFtdcNotifyQueryAccountField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNBANKTOFUTUREBYFUTURE;

	if (pReqTransfer)
	{
		task.task_data = *pReqTransfer;
	}
	else
	{
		CThostFtdcReqTransferField empty_data = CThostFtdcReqTransferField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNFUTURETOBANKBYFUTURE;

	if (pReqTransfer)
	{
		task.task_data = *pReqTransfer;
	}
	else
	{
		CThostFtdcReqTransferField empty_data = CThostFtdcReqTransferField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNREPEALBANKTOFUTUREBYFUTUREMANUAL;

	if (pReqRepeal)
	{
		task.task_data = *pReqRepeal;
	}
	else
	{
		CThostFtdcReqRepealField empty_data = CThostFtdcReqRepealField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNREPEALFUTURETOBANKBYFUTUREMANUAL;

	if (pReqRepeal)
	{
		task.task_data = *pReqRepeal;
	}
	else
	{
		CThostFtdcReqRepealField empty_data = CThostFtdcReqRepealField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo) 
{
	Task task = Task();
	task.task_name = ONERRRTNQUERYBANKBALANCEBYFUTURE;

	if (pReqQueryAccount)
	{
		task.task_data = *pReqQueryAccount;
	}
	else
	{
		CThostFtdcReqQueryAccountField empty_data = CThostFtdcReqQueryAccountField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal) 
{
	Task task = Task();
	task.task_name = ONRTNREPEALFROMBANKTOFUTUREBYFUTURE;

	if (pRspRepeal)
	{
		task.task_data = *pRspRepeal;
	}
	else
	{
		CThostFtdcRspRepealField empty_data = CThostFtdcRspRepealField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal) 
{
	Task task = Task();
	task.task_name = ONRTNREPEALFROMFUTURETOBANKBYFUTURE;

	if (pRspRepeal)
	{
		task.task_data = *pRspRepeal;
	}
	else
	{
		CThostFtdcRspRepealField empty_data = CThostFtdcRspRepealField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPFROMBANKTOFUTUREBYFUTURE;

	if (pReqTransfer)
	{
		task.task_data = *pReqTransfer;
	}
	else
	{
		CThostFtdcReqTransferField empty_data = CThostFtdcReqTransferField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPFROMFUTURETOBANKBYFUTURE;

	if (pReqTransfer)
	{
		task.task_data = *pReqTransfer;
	}
	else
	{
		CThostFtdcReqTransferField empty_data = CThostFtdcReqTransferField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	Task task = Task();
	task.task_name = ONRSPQUERYBANKACCOUNTMONEYBYFUTURE;

	if (pReqQueryAccount)
	{
		task.task_data = *pReqQueryAccount;
	}
	else
	{
		CThostFtdcReqQueryAccountField empty_data = CThostFtdcReqQueryAccountField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	task.task_id = nRequestID;
	task.task_last = bIsLast;
	this->task_queue.push(task);
};

void TdApi::OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount) 
{
	Task task = Task();
	task.task_name = ONRTNOPENACCOUNTBYBANK;

	if (pOpenAccount)
	{
		task.task_data = *pOpenAccount;
	}
	else
	{
		CThostFtdcOpenAccountField empty_data = CThostFtdcOpenAccountField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount) 
{
	Task task = Task();
	task.task_name = ONRTNCANCELACCOUNTBYBANK;

	if (pCancelAccount)
	{
		task.task_data = *pCancelAccount;
	}
	else
	{
		CThostFtdcCancelAccountField empty_data = CThostFtdcCancelAccountField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};

void TdApi::OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount) 
{
	Task task = Task();
	task.task_name = ONRTNCHANGEACCOUNTBYBANK;

	if (pChangeAccount)
	{
		task.task_data = *pChangeAccount;
	}
	else
	{
		CThostFtdcChangeAccountField empty_data = CThostFtdcChangeAccountField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
};


// render_data.ctp_td_task end


///-------------------------------------------------------------------------------------
///工作线程从队列中取出数据，转化为python对象后，进行推送
///-------------------------------------------------------------------------------------

void TdApi::processTask()
{
	while (1)
	{
		Task task = this->task_queue.wait_and_pop();

		switch (task.task_name)
		{
			// render_data.ctp_td_switch start
			case ONFRONTCONNECTED:
            {
            	this->processFrontConnected(task);
            	break;
            }

            case ONFRONTDISCONNECTED:
            {
            	this->processFrontDisconnected(task);
            	break;
            }

            case ONHEARTBEATWARNING:
            {
            	this->processHeartBeatWarning(task);
            	break;
            }

            case ONRSPAUTHENTICATE:
            {
            	this->processRspAuthenticate(task);
            	break;
            }

            case ONRSPUSERLOGIN:
            {
            	this->processRspUserLogin(task);
            	break;
            }

            case ONRSPUSERLOGOUT:
            {
            	this->processRspUserLogout(task);
            	break;
            }

            case ONRSPUSERPASSWORDUPDATE:
            {
            	this->processRspUserPasswordUpdate(task);
            	break;
            }

            case ONRSPTRADINGACCOUNTPASSWORDUPDATE:
            {
            	this->processRspTradingAccountPasswordUpdate(task);
            	break;
            }

            case ONRSPORDERINSERT:
            {
            	this->processRspOrderInsert(task);
            	break;
            }

            case ONRSPPARKEDORDERINSERT:
            {
            	this->processRspParkedOrderInsert(task);
            	break;
            }

            case ONRSPPARKEDORDERACTION:
            {
            	this->processRspParkedOrderAction(task);
            	break;
            }

            case ONRSPORDERACTION:
            {
            	this->processRspOrderAction(task);
            	break;
            }

            case ONRSPQUERYMAXORDERVOLUME:
            {
            	this->processRspQueryMaxOrderVolume(task);
            	break;
            }

            case ONRSPSETTLEMENTINFOCONFIRM:
            {
            	this->processRspSettlementInfoConfirm(task);
            	break;
            }

            case ONRSPREMOVEPARKEDORDER:
            {
            	this->processRspRemoveParkedOrder(task);
            	break;
            }

            case ONRSPREMOVEPARKEDORDERACTION:
            {
            	this->processRspRemoveParkedOrderAction(task);
            	break;
            }

            case ONRSPEXECORDERINSERT:
            {
            	this->processRspExecOrderInsert(task);
            	break;
            }

            case ONRSPEXECORDERACTION:
            {
            	this->processRspExecOrderAction(task);
            	break;
            }

            case ONRSPFORQUOTEINSERT:
            {
            	this->processRspForQuoteInsert(task);
            	break;
            }

            case ONRSPQUOTEINSERT:
            {
            	this->processRspQuoteInsert(task);
            	break;
            }

            case ONRSPQUOTEACTION:
            {
            	this->processRspQuoteAction(task);
            	break;
            }

            case ONRSPBATCHORDERACTION:
            {
            	this->processRspBatchOrderAction(task);
            	break;
            }

            case ONRSPOPTIONSELFCLOSEINSERT:
            {
            	this->processRspOptionSelfCloseInsert(task);
            	break;
            }

            case ONRSPOPTIONSELFCLOSEACTION:
            {
            	this->processRspOptionSelfCloseAction(task);
            	break;
            }

            case ONRSPCOMBACTIONINSERT:
            {
            	this->processRspCombActionInsert(task);
            	break;
            }

            case ONRSPQRYORDER:
            {
            	this->processRspQryOrder(task);
            	break;
            }

            case ONRSPQRYTRADE:
            {
            	this->processRspQryTrade(task);
            	break;
            }

            case ONRSPQRYINVESTORPOSITION:
            {
            	this->processRspQryInvestorPosition(task);
            	break;
            }

            case ONRSPQRYTRADINGACCOUNT:
            {
            	this->processRspQryTradingAccount(task);
            	break;
            }

            case ONRSPQRYINVESTOR:
            {
            	this->processRspQryInvestor(task);
            	break;
            }

            case ONRSPQRYTRADINGCODE:
            {
            	this->processRspQryTradingCode(task);
            	break;
            }

            case ONRSPQRYINSTRUMENTMARGINRATE:
            {
            	this->processRspQryInstrumentMarginRate(task);
            	break;
            }

            case ONRSPQRYINSTRUMENTCOMMISSIONRATE:
            {
            	this->processRspQryInstrumentCommissionRate(task);
            	break;
            }

            case ONRSPQRYEXCHANGE:
            {
            	this->processRspQryExchange(task);
            	break;
            }

            case ONRSPQRYPRODUCT:
            {
            	this->processRspQryProduct(task);
            	break;
            }

            case ONRSPQRYINSTRUMENT:
            {
            	this->processRspQryInstrument(task);
            	break;
            }

            case ONRSPQRYDEPTHMARKETDATA:
            {
            	this->processRspQryDepthMarketData(task);
            	break;
            }

            case ONRSPQRYSETTLEMENTINFO:
            {
            	this->processRspQrySettlementInfo(task);
            	break;
            }

            case ONRSPQRYTRANSFERBANK:
            {
            	this->processRspQryTransferBank(task);
            	break;
            }

            case ONRSPQRYINVESTORPOSITIONDETAIL:
            {
            	this->processRspQryInvestorPositionDetail(task);
            	break;
            }

            case ONRSPQRYNOTICE:
            {
            	this->processRspQryNotice(task);
            	break;
            }

            case ONRSPQRYSETTLEMENTINFOCONFIRM:
            {
            	this->processRspQrySettlementInfoConfirm(task);
            	break;
            }

            case ONRSPQRYINVESTORPOSITIONCOMBINEDETAIL:
            {
            	this->processRspQryInvestorPositionCombineDetail(task);
            	break;
            }

            case ONRSPQRYCFMMCTRADINGACCOUNTKEY:
            {
            	this->processRspQryCFMMCTradingAccountKey(task);
            	break;
            }

            case ONRSPQRYEWARRANTOFFSET:
            {
            	this->processRspQryEWarrantOffset(task);
            	break;
            }

            case ONRSPQRYINVESTORPRODUCTGROUPMARGIN:
            {
            	this->processRspQryInvestorProductGroupMargin(task);
            	break;
            }

            case ONRSPQRYEXCHANGEMARGINRATE:
            {
            	this->processRspQryExchangeMarginRate(task);
            	break;
            }

            case ONRSPQRYEXCHANGEMARGINRATEADJUST:
            {
            	this->processRspQryExchangeMarginRateAdjust(task);
            	break;
            }

            case ONRSPQRYEXCHANGERATE:
            {
            	this->processRspQryExchangeRate(task);
            	break;
            }

            case ONRSPQRYSECAGENTACIDMAP:
            {
            	this->processRspQrySecAgentACIDMap(task);
            	break;
            }

            case ONRSPQRYPRODUCTEXCHRATE:
            {
            	this->processRspQryProductExchRate(task);
            	break;
            }

            case ONRSPQRYPRODUCTGROUP:
            {
            	this->processRspQryProductGroup(task);
            	break;
            }

            case ONRSPQRYMMINSTRUMENTCOMMISSIONRATE:
            {
            	this->processRspQryMMInstrumentCommissionRate(task);
            	break;
            }

            case ONRSPQRYMMOPTIONINSTRCOMMRATE:
            {
            	this->processRspQryMMOptionInstrCommRate(task);
            	break;
            }

            case ONRSPQRYINSTRUMENTORDERCOMMRATE:
            {
            	this->processRspQryInstrumentOrderCommRate(task);
            	break;
            }

            case ONRSPQRYOPTIONINSTRTRADECOST:
            {
            	this->processRspQryOptionInstrTradeCost(task);
            	break;
            }

            case ONRSPQRYOPTIONINSTRCOMMRATE:
            {
            	this->processRspQryOptionInstrCommRate(task);
            	break;
            }

            case ONRSPQRYEXECORDER:
            {
            	this->processRspQryExecOrder(task);
            	break;
            }

            case ONRSPQRYFORQUOTE:
            {
            	this->processRspQryForQuote(task);
            	break;
            }

            case ONRSPQRYQUOTE:
            {
            	this->processRspQryQuote(task);
            	break;
            }

            case ONRSPQRYOPTIONSELFCLOSE:
            {
            	this->processRspQryOptionSelfClose(task);
            	break;
            }

            case ONRSPQRYINVESTUNIT:
            {
            	this->processRspQryInvestUnit(task);
            	break;
            }

            case ONRSPQRYCOMBINSTRUMENTGUARD:
            {
            	this->processRspQryCombInstrumentGuard(task);
            	break;
            }

            case ONRSPQRYCOMBACTION:
            {
            	this->processRspQryCombAction(task);
            	break;
            }

            case ONRSPQRYTRANSFERSERIAL:
            {
            	this->processRspQryTransferSerial(task);
            	break;
            }

            case ONRSPQRYACCOUNTREGISTER:
            {
            	this->processRspQryAccountregister(task);
            	break;
            }

            case ONRSPERROR:
            {
            	this->processRspError(task);
            	break;
            }

            case ONRTNORDER:
            {
            	this->processRtnOrder(task);
            	break;
            }

            case ONRTNTRADE:
            {
            	this->processRtnTrade(task);
            	break;
            }

            case ONERRRTNORDERINSERT:
            {
            	this->processErrRtnOrderInsert(task);
            	break;
            }

            case ONERRRTNORDERACTION:
            {
            	this->processErrRtnOrderAction(task);
            	break;
            }

            case ONRTNINSTRUMENTSTATUS:
            {
            	this->processRtnInstrumentStatus(task);
            	break;
            }

            case ONRTNBULLETIN:
            {
            	this->processRtnBulletin(task);
            	break;
            }

            case ONRTNTRADINGNOTICE:
            {
            	this->processRtnTradingNotice(task);
            	break;
            }

            case ONRTNERRORCONDITIONALORDER:
            {
            	this->processRtnErrorConditionalOrder(task);
            	break;
            }

            case ONRTNEXECORDER:
            {
            	this->processRtnExecOrder(task);
            	break;
            }

            case ONERRRTNEXECORDERINSERT:
            {
            	this->processErrRtnExecOrderInsert(task);
            	break;
            }

            case ONERRRTNEXECORDERACTION:
            {
            	this->processErrRtnExecOrderAction(task);
            	break;
            }

            case ONERRRTNFORQUOTEINSERT:
            {
            	this->processErrRtnForQuoteInsert(task);
            	break;
            }

            case ONRTNQUOTE:
            {
            	this->processRtnQuote(task);
            	break;
            }

            case ONERRRTNQUOTEINSERT:
            {
            	this->processErrRtnQuoteInsert(task);
            	break;
            }

            case ONERRRTNQUOTEACTION:
            {
            	this->processErrRtnQuoteAction(task);
            	break;
            }

            case ONRTNFORQUOTERSP:
            {
            	this->processRtnForQuoteRsp(task);
            	break;
            }

            case ONRTNCFMMCTRADINGACCOUNTTOKEN:
            {
            	this->processRtnCFMMCTradingAccountToken(task);
            	break;
            }

            case ONERRRTNBATCHORDERACTION:
            {
            	this->processErrRtnBatchOrderAction(task);
            	break;
            }

            case ONRTNOPTIONSELFCLOSE:
            {
            	this->processRtnOptionSelfClose(task);
            	break;
            }

            case ONERRRTNOPTIONSELFCLOSEINSERT:
            {
            	this->processErrRtnOptionSelfCloseInsert(task);
            	break;
            }

            case ONERRRTNOPTIONSELFCLOSEACTION:
            {
            	this->processErrRtnOptionSelfCloseAction(task);
            	break;
            }

            case ONRTNCOMBACTION:
            {
            	this->processRtnCombAction(task);
            	break;
            }

            case ONERRRTNCOMBACTIONINSERT:
            {
            	this->processErrRtnCombActionInsert(task);
            	break;
            }

            case ONRSPQRYCONTRACTBANK:
            {
            	this->processRspQryContractBank(task);
            	break;
            }

            case ONRSPQRYPARKEDORDER:
            {
            	this->processRspQryParkedOrder(task);
            	break;
            }

            case ONRSPQRYPARKEDORDERACTION:
            {
            	this->processRspQryParkedOrderAction(task);
            	break;
            }

            case ONRSPQRYTRADINGNOTICE:
            {
            	this->processRspQryTradingNotice(task);
            	break;
            }

            case ONRSPQRYBROKERTRADINGPARAMS:
            {
            	this->processRspQryBrokerTradingParams(task);
            	break;
            }

            case ONRSPQRYBROKERTRADINGALGOS:
            {
            	this->processRspQryBrokerTradingAlgos(task);
            	break;
            }

            case ONRSPQUERYCFMMCTRADINGACCOUNTTOKEN:
            {
            	this->processRspQueryCFMMCTradingAccountToken(task);
            	break;
            }

            case ONRTNFROMBANKTOFUTUREBYBANK:
            {
            	this->processRtnFromBankToFutureByBank(task);
            	break;
            }

            case ONRTNFROMFUTURETOBANKBYBANK:
            {
            	this->processRtnFromFutureToBankByBank(task);
            	break;
            }

            case ONRTNREPEALFROMBANKTOFUTUREBYBANK:
            {
            	this->processRtnRepealFromBankToFutureByBank(task);
            	break;
            }

            case ONRTNREPEALFROMFUTURETOBANKBYBANK:
            {
            	this->processRtnRepealFromFutureToBankByBank(task);
            	break;
            }

            case ONRTNFROMBANKTOFUTUREBYFUTURE:
            {
            	this->processRtnFromBankToFutureByFuture(task);
            	break;
            }

            case ONRTNFROMFUTURETOBANKBYFUTURE:
            {
            	this->processRtnFromFutureToBankByFuture(task);
            	break;
            }

            case ONRTNREPEALFROMBANKTOFUTUREBYFUTUREMANUAL:
            {
            	this->processRtnRepealFromBankToFutureByFutureManual(task);
            	break;
            }

            case ONRTNREPEALFROMFUTURETOBANKBYFUTUREMANUAL:
            {
            	this->processRtnRepealFromFutureToBankByFutureManual(task);
            	break;
            }

            case ONRTNQUERYBANKBALANCEBYFUTURE:
            {
            	this->processRtnQueryBankBalanceByFuture(task);
            	break;
            }

            case ONERRRTNBANKTOFUTUREBYFUTURE:
            {
            	this->processErrRtnBankToFutureByFuture(task);
            	break;
            }

            case ONERRRTNFUTURETOBANKBYFUTURE:
            {
            	this->processErrRtnFutureToBankByFuture(task);
            	break;
            }

            case ONERRRTNREPEALBANKTOFUTUREBYFUTUREMANUAL:
            {
            	this->processErrRtnRepealBankToFutureByFutureManual(task);
            	break;
            }

            case ONERRRTNREPEALFUTURETOBANKBYFUTUREMANUAL:
            {
            	this->processErrRtnRepealFutureToBankByFutureManual(task);
            	break;
            }

            case ONERRRTNQUERYBANKBALANCEBYFUTURE:
            {
            	this->processErrRtnQueryBankBalanceByFuture(task);
            	break;
            }

            case ONRTNREPEALFROMBANKTOFUTUREBYFUTURE:
            {
            	this->processRtnRepealFromBankToFutureByFuture(task);
            	break;
            }

            case ONRTNREPEALFROMFUTURETOBANKBYFUTURE:
            {
            	this->processRtnRepealFromFutureToBankByFuture(task);
            	break;
            }

            case ONRSPFROMBANKTOFUTUREBYFUTURE:
            {
            	this->processRspFromBankToFutureByFuture(task);
            	break;
            }

            case ONRSPFROMFUTURETOBANKBYFUTURE:
            {
            	this->processRspFromFutureToBankByFuture(task);
            	break;
            }

            case ONRSPQUERYBANKACCOUNTMONEYBYFUTURE:
            {
            	this->processRspQueryBankAccountMoneyByFuture(task);
            	break;
            }

            case ONRTNOPENACCOUNTBYBANK:
            {
            	this->processRtnOpenAccountByBank(task);
            	break;
            }

            case ONRTNCANCELACCOUNTBYBANK:
            {
            	this->processRtnCancelAccountByBank(task);
            	break;
            }

            case ONRTNCHANGEACCOUNTBYBANK:
            {
            	this->processRtnChangeAccountByBank(task);
            	break;
            }


			// render_data.ctp_td_switch end
		}
	}
}

// render_data.ctp_td_process start
void TdApi::processFrontConnected(Task task)
{
	PyLock lock;
	this->onFrontConnected();
};

void TdApi::processFrontDisconnected(Task task)
{
	PyLock lock;
	this->onFrontDisconnected(task.task_id);
};

void TdApi::processHeartBeatWarning(Task task)
{
	PyLock lock;
	this->onHeartBeatWarning(task.task_id);
};

void TdApi::processRspAuthenticate(Task task)
{
	PyLock lock;
	CThostFtdcRspAuthenticateField task_data = any_cast<CThostFtdcRspAuthenticateField>(task.task_data);
	dict data;
	data["UserProductInfo"] = boost::locale::conv::to_utf<char>(task_data.UserProductInfo, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspAuthenticate(data, error, task.task_id, task.task_last);
};

void TdApi::processRspUserLogin(Task task)
{
	PyLock lock;
	CThostFtdcRspUserLoginField task_data = any_cast<CThostFtdcRspUserLoginField>(task.task_data);
	dict data;
	data["FrontID"] = task_data.FrontID;
	data["SHFETime"] = boost::locale::conv::to_utf<char>(task_data.SHFETime, std::string("GB2312"));
	data["MaxOrderRef"] = boost::locale::conv::to_utf<char>(task_data.MaxOrderRef, std::string("GB2312"));
	data["SystemName"] = boost::locale::conv::to_utf<char>(task_data.SystemName, std::string("GB2312"));
	data["INETime"] = boost::locale::conv::to_utf<char>(task_data.INETime, std::string("GB2312"));
	data["DCETime"] = boost::locale::conv::to_utf<char>(task_data.DCETime, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["LoginTime"] = boost::locale::conv::to_utf<char>(task_data.LoginTime, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["FFEXTime"] = boost::locale::conv::to_utf<char>(task_data.FFEXTime, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["CZCETime"] = boost::locale::conv::to_utf<char>(task_data.CZCETime, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspUserLogin(data, error, task.task_id, task.task_last);
};

void TdApi::processRspUserLogout(Task task)
{
	PyLock lock;
	CThostFtdcUserLogoutField task_data = any_cast<CThostFtdcUserLogoutField>(task.task_data);
	dict data;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspUserLogout(data, error, task.task_id, task.task_last);
};

void TdApi::processRspUserPasswordUpdate(Task task)
{
	PyLock lock;
	CThostFtdcUserPasswordUpdateField task_data = any_cast<CThostFtdcUserPasswordUpdateField>(task.task_data);
	dict data;
	data["OldPassword"] = boost::locale::conv::to_utf<char>(task_data.OldPassword, std::string("GB2312"));
	data["NewPassword"] = boost::locale::conv::to_utf<char>(task_data.NewPassword, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspUserPasswordUpdate(data, error, task.task_id, task.task_last);
};

void TdApi::processRspTradingAccountPasswordUpdate(Task task)
{
	PyLock lock;
	CThostFtdcTradingAccountPasswordUpdateField task_data = any_cast<CThostFtdcTradingAccountPasswordUpdateField>(task.task_data);
	dict data;
	data["OldPassword"] = boost::locale::conv::to_utf<char>(task_data.OldPassword, std::string("GB2312"));
	data["NewPassword"] = boost::locale::conv::to_utf<char>(task_data.NewPassword, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspTradingAccountPasswordUpdate(data, error, task.task_id, task.task_last);
};

void TdApi::processRspOrderInsert(Task task)
{
	PyLock lock;
	CThostFtdcInputOrderField task_data = any_cast<CThostFtdcInputOrderField>(task.task_data);
	dict data;
	data["StopPrice"] = task_data.StopPrice;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["TimeCondition"] = task_data.TimeCondition;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["IsSwapOrder"] = task_data.IsSwapOrder;
	data["CombHedgeFlag"] = boost::locale::conv::to_utf<char>(task_data.CombHedgeFlag, std::string("GB2312"));
	data["ContingentCondition"] = task_data.ContingentCondition;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["GTDDate"] = boost::locale::conv::to_utf<char>(task_data.GTDDate, std::string("GB2312"));
	data["IsAutoSuspend"] = task_data.IsAutoSuspend;
	data["VolumeCondition"] = task_data.VolumeCondition;
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["LimitPrice"] = task_data.LimitPrice;
	data["Direction"] = task_data.Direction;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["CombOffsetFlag"] = boost::locale::conv::to_utf<char>(task_data.CombOffsetFlag, std::string("GB2312"));
	data["OrderRef"] = boost::locale::conv::to_utf<char>(task_data.OrderRef, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["VolumeTotalOriginal"] = task_data.VolumeTotalOriginal;
	data["MinVolume"] = task_data.MinVolume;
	data["OrderPriceType"] = task_data.OrderPriceType;
	data["UserForceClose"] = task_data.UserForceClose;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["ForceCloseReason"] = task_data.ForceCloseReason;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspOrderInsert(data, error, task.task_id, task.task_last);
};

void TdApi::processRspParkedOrderInsert(Task task)
{
	PyLock lock;
	CThostFtdcParkedOrderField task_data = any_cast<CThostFtdcParkedOrderField>(task.task_data);
	dict data;
	data["StopPrice"] = task_data.StopPrice;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["TimeCondition"] = task_data.TimeCondition;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["Status"] = task_data.Status;
	data["ParkedOrderID"] = boost::locale::conv::to_utf<char>(task_data.ParkedOrderID, std::string("GB2312"));
	data["CombHedgeFlag"] = boost::locale::conv::to_utf<char>(task_data.CombHedgeFlag, std::string("GB2312"));
	data["ContingentCondition"] = task_data.ContingentCondition;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["GTDDate"] = boost::locale::conv::to_utf<char>(task_data.GTDDate, std::string("GB2312"));
	data["IsAutoSuspend"] = task_data.IsAutoSuspend;
	data["VolumeCondition"] = task_data.VolumeCondition;
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["LimitPrice"] = task_data.LimitPrice;
	data["Direction"] = task_data.Direction;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["CombOffsetFlag"] = boost::locale::conv::to_utf<char>(task_data.CombOffsetFlag, std::string("GB2312"));
	data["IsSwapOrder"] = task_data.IsSwapOrder;
	data["OrderRef"] = boost::locale::conv::to_utf<char>(task_data.OrderRef, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["UserType"] = task_data.UserType;
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["VolumeTotalOriginal"] = task_data.VolumeTotalOriginal;
	data["MinVolume"] = task_data.MinVolume;
	data["OrderPriceType"] = task_data.OrderPriceType;
	data["UserForceClose"] = task_data.UserForceClose;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["ForceCloseReason"] = task_data.ForceCloseReason;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspParkedOrderInsert(data, error, task.task_id, task.task_last);
};

void TdApi::processRspParkedOrderAction(Task task)
{
	PyLock lock;
	CThostFtdcParkedOrderActionField task_data = any_cast<CThostFtdcParkedOrderActionField>(task.task_data);
	dict data;
	data["OrderSysID"] = boost::locale::conv::to_utf<char>(task_data.OrderSysID, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["Status"] = task_data.Status;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["ActionFlag"] = task_data.ActionFlag;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["UserType"] = task_data.UserType;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["ParkedOrderActionID"] = boost::locale::conv::to_utf<char>(task_data.ParkedOrderActionID, std::string("GB2312"));
	data["VolumeChange"] = task_data.VolumeChange;
	data["LimitPrice"] = task_data.LimitPrice;
	data["RequestID"] = task_data.RequestID;
	data["OrderRef"] = boost::locale::conv::to_utf<char>(task_data.OrderRef, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["OrderActionRef"] = task_data.OrderActionRef;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspParkedOrderAction(data, error, task.task_id, task.task_last);
};

void TdApi::processRspOrderAction(Task task)
{
	PyLock lock;
	CThostFtdcInputOrderActionField task_data = any_cast<CThostFtdcInputOrderActionField>(task.task_data);
	dict data;
	data["OrderSysID"] = boost::locale::conv::to_utf<char>(task_data.OrderSysID, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["LimitPrice"] = task_data.LimitPrice;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["VolumeChange"] = task_data.VolumeChange;
	data["RequestID"] = task_data.RequestID;
	data["OrderRef"] = boost::locale::conv::to_utf<char>(task_data.OrderRef, std::string("GB2312"));
	data["ActionFlag"] = task_data.ActionFlag;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["OrderActionRef"] = task_data.OrderActionRef;
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspOrderAction(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQueryMaxOrderVolume(Task task)
{
	PyLock lock;
	CThostFtdcQueryMaxOrderVolumeField task_data = any_cast<CThostFtdcQueryMaxOrderVolumeField>(task.task_data);
	dict data;
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["Direction"] = task_data.Direction;
	data["OffsetFlag"] = task_data.OffsetFlag;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["MaxVolume"] = task_data.MaxVolume;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQueryMaxOrderVolume(data, error, task.task_id, task.task_last);
};

void TdApi::processRspSettlementInfoConfirm(Task task)
{
	PyLock lock;
	CThostFtdcSettlementInfoConfirmField task_data = any_cast<CThostFtdcSettlementInfoConfirmField>(task.task_data);
	dict data;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["ConfirmTime"] = boost::locale::conv::to_utf<char>(task_data.ConfirmTime, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["ConfirmDate"] = boost::locale::conv::to_utf<char>(task_data.ConfirmDate, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["SettlementID"] = task_data.SettlementID;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspSettlementInfoConfirm(data, error, task.task_id, task.task_last);
};

void TdApi::processRspRemoveParkedOrder(Task task)
{
	PyLock lock;
	CThostFtdcRemoveParkedOrderField task_data = any_cast<CThostFtdcRemoveParkedOrderField>(task.task_data);
	dict data;
	data["ParkedOrderID"] = boost::locale::conv::to_utf<char>(task_data.ParkedOrderID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspRemoveParkedOrder(data, error, task.task_id, task.task_last);
};

void TdApi::processRspRemoveParkedOrderAction(Task task)
{
	PyLock lock;
	CThostFtdcRemoveParkedOrderActionField task_data = any_cast<CThostFtdcRemoveParkedOrderActionField>(task.task_data);
	dict data;
	data["ParkedOrderActionID"] = boost::locale::conv::to_utf<char>(task_data.ParkedOrderActionID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspRemoveParkedOrderAction(data, error, task.task_id, task.task_last);
};

void TdApi::processRspExecOrderInsert(Task task)
{
	PyLock lock;
	CThostFtdcInputExecOrderField task_data = any_cast<CThostFtdcInputExecOrderField>(task.task_data);
	dict data;
	data["ExecOrderRef"] = boost::locale::conv::to_utf<char>(task_data.ExecOrderRef, std::string("GB2312"));
	data["ReservePositionFlag"] = task_data.ReservePositionFlag;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["CloseFlag"] = task_data.CloseFlag;
	data["Volume"] = task_data.Volume;
	data["ActionType"] = task_data.ActionType;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["OffsetFlag"] = task_data.OffsetFlag;
	data["PosiDirection"] = task_data.PosiDirection;
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspExecOrderInsert(data, error, task.task_id, task.task_last);
};

void TdApi::processRspExecOrderAction(Task task)
{
	PyLock lock;
	CThostFtdcInputExecOrderActionField task_data = any_cast<CThostFtdcInputExecOrderActionField>(task.task_data);
	dict data;
	data["ExecOrderRef"] = boost::locale::conv::to_utf<char>(task_data.ExecOrderRef, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["ExecOrderSysID"] = boost::locale::conv::to_utf<char>(task_data.ExecOrderSysID, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["ExecOrderActionRef"] = task_data.ExecOrderActionRef;
	data["ActionFlag"] = task_data.ActionFlag;
	data["SessionID"] = task_data.SessionID;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspExecOrderAction(data, error, task.task_id, task.task_last);
};

void TdApi::processRspForQuoteInsert(Task task)
{
	PyLock lock;
	CThostFtdcInputForQuoteField task_data = any_cast<CThostFtdcInputForQuoteField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["ForQuoteRef"] = boost::locale::conv::to_utf<char>(task_data.ForQuoteRef, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspForQuoteInsert(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQuoteInsert(Task task)
{
	PyLock lock;
	CThostFtdcInputQuoteField task_data = any_cast<CThostFtdcInputQuoteField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["BidOrderRef"] = boost::locale::conv::to_utf<char>(task_data.BidOrderRef, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["AskPrice"] = task_data.AskPrice;
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["QuoteRef"] = boost::locale::conv::to_utf<char>(task_data.QuoteRef, std::string("GB2312"));
	data["AskOrderRef"] = boost::locale::conv::to_utf<char>(task_data.AskOrderRef, std::string("GB2312"));
	data["BidHedgeFlag"] = task_data.BidHedgeFlag;
	data["BidOffsetFlag"] = task_data.BidOffsetFlag;
	data["BidPrice"] = task_data.BidPrice;
	data["RequestID"] = task_data.RequestID;
	data["AskOffsetFlag"] = task_data.AskOffsetFlag;
	data["AskVolume"] = task_data.AskVolume;
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["BidVolume"] = task_data.BidVolume;
	data["ForQuoteSysID"] = boost::locale::conv::to_utf<char>(task_data.ForQuoteSysID, std::string("GB2312"));
	data["AskHedgeFlag"] = task_data.AskHedgeFlag;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQuoteInsert(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQuoteAction(Task task)
{
	PyLock lock;
	CThostFtdcInputQuoteActionField task_data = any_cast<CThostFtdcInputQuoteActionField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["QuoteRef"] = boost::locale::conv::to_utf<char>(task_data.QuoteRef, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["RequestID"] = task_data.RequestID;
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["QuoteSysID"] = boost::locale::conv::to_utf<char>(task_data.QuoteSysID, std::string("GB2312"));
	data["ActionFlag"] = task_data.ActionFlag;
	data["SessionID"] = task_data.SessionID;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["QuoteActionRef"] = task_data.QuoteActionRef;
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQuoteAction(data, error, task.task_id, task.task_last);
};

void TdApi::processRspBatchOrderAction(Task task)
{
	PyLock lock;
	CThostFtdcInputBatchOrderActionField task_data = any_cast<CThostFtdcInputBatchOrderActionField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["OrderActionRef"] = task_data.OrderActionRef;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["RequestID"] = task_data.RequestID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspBatchOrderAction(data, error, task.task_id, task.task_last);
};

void TdApi::processRspOptionSelfCloseInsert(Task task)
{
	PyLock lock;
	CThostFtdcInputOptionSelfCloseField task_data = any_cast<CThostFtdcInputOptionSelfCloseField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["OptionSelfCloseRef"] = boost::locale::conv::to_utf<char>(task_data.OptionSelfCloseRef, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["Volume"] = task_data.Volume;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["OptSelfCloseFlag"] = task_data.OptSelfCloseFlag;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspOptionSelfCloseInsert(data, error, task.task_id, task.task_last);
};

void TdApi::processRspOptionSelfCloseAction(Task task)
{
	PyLock lock;
	CThostFtdcInputOptionSelfCloseActionField task_data = any_cast<CThostFtdcInputOptionSelfCloseActionField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["OptionSelfCloseActionRef"] = task_data.OptionSelfCloseActionRef;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["OptionSelfCloseSysID"] = boost::locale::conv::to_utf<char>(task_data.OptionSelfCloseSysID, std::string("GB2312"));
	data["OptionSelfCloseRef"] = boost::locale::conv::to_utf<char>(task_data.OptionSelfCloseRef, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["RequestID"] = task_data.RequestID;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["ActionFlag"] = task_data.ActionFlag;
	data["SessionID"] = task_data.SessionID;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspOptionSelfCloseAction(data, error, task.task_id, task.task_last);
};

void TdApi::processRspCombActionInsert(Task task)
{
	PyLock lock;
	CThostFtdcInputCombActionField task_data = any_cast<CThostFtdcInputCombActionField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["Direction"] = task_data.Direction;
	data["Volume"] = task_data.Volume;
	data["CombActionRef"] = boost::locale::conv::to_utf<char>(task_data.CombActionRef, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["CombDirection"] = task_data.CombDirection;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspCombActionInsert(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryOrder(Task task)
{
	PyLock lock;
	CThostFtdcOrderField task_data = any_cast<CThostFtdcOrderField>(task.task_data);
	dict data;
	data["StopPrice"] = task_data.StopPrice;
	data["OrderStatus"] = task_data.OrderStatus;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["SuspendTime"] = boost::locale::conv::to_utf<char>(task_data.SuspendTime, std::string("GB2312"));
	data["OrderType"] = task_data.OrderType;
	data["FrontID"] = task_data.FrontID;
	data["VolumeTotal"] = task_data.VolumeTotal;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["ClearingPartID"] = boost::locale::conv::to_utf<char>(task_data.ClearingPartID, std::string("GB2312"));
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["VolumeTraded"] = task_data.VolumeTraded;
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["IsSwapOrder"] = task_data.IsSwapOrder;
	data["VolumeCondition"] = task_data.VolumeCondition;
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["InsertDate"] = boost::locale::conv::to_utf<char>(task_data.InsertDate, std::string("GB2312"));
	data["ActiveTime"] = boost::locale::conv::to_utf<char>(task_data.ActiveTime, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["RequestID"] = task_data.RequestID;
	data["CombOffsetFlag"] = boost::locale::conv::to_utf<char>(task_data.CombOffsetFlag, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["ActiveUserID"] = boost::locale::conv::to_utf<char>(task_data.ActiveUserID, std::string("GB2312"));
	data["SequenceNo"] = task_data.SequenceNo;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["UpdateTime"] = boost::locale::conv::to_utf<char>(task_data.UpdateTime, std::string("GB2312"));
	data["OrderPriceType"] = task_data.OrderPriceType;
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["OrderSysID"] = boost::locale::conv::to_utf<char>(task_data.OrderSysID, std::string("GB2312"));
	data["TimeCondition"] = task_data.TimeCondition;
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["OrderSubmitStatus"] = task_data.OrderSubmitStatus;
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["ZCETotalTradedVolume"] = task_data.ZCETotalTradedVolume;
	data["BrokerOrderSeq"] = task_data.BrokerOrderSeq;
	data["CombHedgeFlag"] = boost::locale::conv::to_utf<char>(task_data.CombHedgeFlag, std::string("GB2312"));
	data["SettlementID"] = task_data.SettlementID;
	data["OrderRef"] = boost::locale::conv::to_utf<char>(task_data.OrderRef, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["NotifySequence"] = task_data.NotifySequence;
	data["GTDDate"] = boost::locale::conv::to_utf<char>(task_data.GTDDate, std::string("GB2312"));
	data["OrderSource"] = task_data.OrderSource;
	data["IsAutoSuspend"] = task_data.IsAutoSuspend;
	data["UserProductInfo"] = boost::locale::conv::to_utf<char>(task_data.UserProductInfo, std::string("GB2312"));
	data["ContingentCondition"] = task_data.ContingentCondition;
	data["RelativeOrderSysID"] = boost::locale::conv::to_utf<char>(task_data.RelativeOrderSysID, std::string("GB2312"));
	data["CancelTime"] = boost::locale::conv::to_utf<char>(task_data.CancelTime, std::string("GB2312"));
	data["Direction"] = task_data.Direction;
	data["OrderLocalID"] = boost::locale::conv::to_utf<char>(task_data.OrderLocalID, std::string("GB2312"));
	data["LimitPrice"] = task_data.LimitPrice;
	data["ForceCloseReason"] = task_data.ForceCloseReason;
	data["InsertTime"] = boost::locale::conv::to_utf<char>(task_data.InsertTime, std::string("GB2312"));
	data["MinVolume"] = task_data.MinVolume;
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["VolumeTotalOriginal"] = task_data.VolumeTotalOriginal;
	data["ActiveTraderID"] = boost::locale::conv::to_utf<char>(task_data.ActiveTraderID, std::string("GB2312"));
	data["UserForceClose"] = task_data.UserForceClose;
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryOrder(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryTrade(Task task)
{
	PyLock lock;
	CThostFtdcTradeField task_data = any_cast<CThostFtdcTradeField>(task.task_data);
	dict data;
	data["OrderSysID"] = boost::locale::conv::to_utf<char>(task_data.OrderSysID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["SequenceNo"] = task_data.SequenceNo;
	data["BrokerOrderSeq"] = task_data.BrokerOrderSeq;
	data["Volume"] = task_data.Volume;
	data["PriceSource"] = task_data.PriceSource;
	data["TradeID"] = boost::locale::conv::to_utf<char>(task_data.TradeID, std::string("GB2312"));
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["OrderRef"] = boost::locale::conv::to_utf<char>(task_data.OrderRef, std::string("GB2312"));
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["TradingRole"] = task_data.TradingRole;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["TradeType"] = task_data.TradeType;
	data["TradeSource"] = task_data.TradeSource;
	data["Direction"] = task_data.Direction;
	data["OrderLocalID"] = boost::locale::conv::to_utf<char>(task_data.OrderLocalID, std::string("GB2312"));
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["SettlementID"] = task_data.SettlementID;
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["Price"] = task_data.Price;
	data["OffsetFlag"] = task_data.OffsetFlag;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["ClearingPartID"] = boost::locale::conv::to_utf<char>(task_data.ClearingPartID, std::string("GB2312"));
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryTrade(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryInvestorPosition(Task task)
{
	PyLock lock;
	CThostFtdcInvestorPositionField task_data = any_cast<CThostFtdcInvestorPositionField>(task.task_data);
	dict data;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["UseMargin"] = task_data.UseMargin;
	data["CashIn"] = task_data.CashIn;
	data["Commission"] = task_data.Commission;
	data["PositionDate"] = task_data.PositionDate;
	data["MarginRateByVolume"] = task_data.MarginRateByVolume;
	data["CloseVolume"] = task_data.CloseVolume;
	data["OpenCost"] = task_data.OpenCost;
	data["CombShortFrozen"] = task_data.CombShortFrozen;
	data["PositionCost"] = task_data.PositionCost;
	data["CloseAmount"] = task_data.CloseAmount;
	data["TodayPosition"] = task_data.TodayPosition;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["FrozenCash"] = task_data.FrozenCash;
	data["MarginRateByMoney"] = task_data.MarginRateByMoney;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["PosiDirection"] = task_data.PosiDirection;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["StrikeFrozen"] = task_data.StrikeFrozen;
	data["CombPosition"] = task_data.CombPosition;
	data["PreSettlementPrice"] = task_data.PreSettlementPrice;
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["CombLongFrozen"] = task_data.CombLongFrozen;
	data["Position"] = task_data.Position;
	data["CloseProfit"] = task_data.CloseProfit;
	data["ExchangeMargin"] = task_data.ExchangeMargin;
	data["CloseProfitByDate"] = task_data.CloseProfitByDate;
	data["PositionProfit"] = task_data.PositionProfit;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["ShortFrozenAmount"] = task_data.ShortFrozenAmount;
	data["AbandonFrozen"] = task_data.AbandonFrozen;
	data["YdStrikeFrozen"] = task_data.YdStrikeFrozen;
	data["LongFrozenAmount"] = task_data.LongFrozenAmount;
	data["OpenAmount"] = task_data.OpenAmount;
	data["FrozenCommission"] = task_data.FrozenCommission;
	data["StrikeFrozenAmount"] = task_data.StrikeFrozenAmount;
	data["PreMargin"] = task_data.PreMargin;
	data["LongFrozen"] = task_data.LongFrozen;
	data["ShortFrozen"] = task_data.ShortFrozen;
	data["SettlementPrice"] = task_data.SettlementPrice;
	data["SettlementID"] = task_data.SettlementID;
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["FrozenMargin"] = task_data.FrozenMargin;
	data["YdPosition"] = task_data.YdPosition;
	data["OpenVolume"] = task_data.OpenVolume;
	data["CloseProfitByTrade"] = task_data.CloseProfitByTrade;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryInvestorPosition(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryTradingAccount(Task task)
{
	PyLock lock;
	CThostFtdcTradingAccountField task_data = any_cast<CThostFtdcTradingAccountField>(task.task_data);
	dict data;
	data["PreDeposit"] = task_data.PreDeposit;
	data["InterestBase"] = task_data.InterestBase;
	data["ReserveBalance"] = task_data.ReserveBalance;
	data["CashIn"] = task_data.CashIn;
	data["PreFundMortgageIn"] = task_data.PreFundMortgageIn;
	data["PreCredit"] = task_data.PreCredit;
	data["SpecProductCloseProfit"] = task_data.SpecProductCloseProfit;
	data["Commission"] = task_data.Commission;
	data["SpecProductExchangeMargin"] = task_data.SpecProductExchangeMargin;
	data["ExchangeDeliveryMargin"] = task_data.ExchangeDeliveryMargin;
	data["Reserve"] = task_data.Reserve;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["SpecProductFrozenCommission"] = task_data.SpecProductFrozenCommission;
	data["PreFundMortgageOut"] = task_data.PreFundMortgageOut;
	data["SpecProductPositionProfitByAlg"] = task_data.SpecProductPositionProfitByAlg;
	data["Interest"] = task_data.Interest;
	data["PreMortgage"] = task_data.PreMortgage;
	data["DeliveryMargin"] = task_data.DeliveryMargin;
	data["Balance"] = task_data.Balance;
	data["FrozenCash"] = task_data.FrozenCash;
	data["FundMortgageOut"] = task_data.FundMortgageOut;
	data["SpecProductCommission"] = task_data.SpecProductCommission;
	data["Deposit"] = task_data.Deposit;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["Credit"] = task_data.Credit;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["Withdraw"] = task_data.Withdraw;
	data["RemainSwap"] = task_data.RemainSwap;
	data["SpecProductMargin"] = task_data.SpecProductMargin;
	data["CloseProfit"] = task_data.CloseProfit;
	data["WithdrawQuota"] = task_data.WithdrawQuota;
	data["PreBalance"] = task_data.PreBalance;
	data["SpecProductFrozenMargin"] = task_data.SpecProductFrozenMargin;
	data["MortgageableFund"] = task_data.MortgageableFund;
	data["SettlementID"] = task_data.SettlementID;
	data["Available"] = task_data.Available;
	data["SpecProductPositionProfit"] = task_data.SpecProductPositionProfit;
	data["CurrMargin"] = task_data.CurrMargin;
	data["FundMortgageIn"] = task_data.FundMortgageIn;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["FrozenCommission"] = task_data.FrozenCommission;
	data["PreMargin"] = task_data.PreMargin;
	data["FrozenMargin"] = task_data.FrozenMargin;
	data["PositionProfit"] = task_data.PositionProfit;
	data["Mortgage"] = task_data.Mortgage;
	data["BizType"] = task_data.BizType;
	data["ExchangeMargin"] = task_data.ExchangeMargin;
	data["FundMortgageAvailable"] = task_data.FundMortgageAvailable;
	data["FrozenSwap"] = task_data.FrozenSwap;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryTradingAccount(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryInvestor(Task task)
{
	PyLock lock;
	CThostFtdcInvestorField task_data = any_cast<CThostFtdcInvestorField>(task.task_data);
	dict data;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["IdentifiedCardType"] = task_data.IdentifiedCardType;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["Telephone"] = boost::locale::conv::to_utf<char>(task_data.Telephone, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["InvestorGroupID"] = boost::locale::conv::to_utf<char>(task_data.InvestorGroupID, std::string("GB2312"));
	data["Address"] = boost::locale::conv::to_utf<char>(task_data.Address, std::string("GB2312"));
	data["CommModelID"] = boost::locale::conv::to_utf<char>(task_data.CommModelID, std::string("GB2312"));
	data["MarginModelID"] = boost::locale::conv::to_utf<char>(task_data.MarginModelID, std::string("GB2312"));
	data["IsActive"] = task_data.IsActive;
	data["InvestorName"] = boost::locale::conv::to_utf<char>(task_data.InvestorName, std::string("GB2312"));
	data["Mobile"] = boost::locale::conv::to_utf<char>(task_data.Mobile, std::string("GB2312"));
	data["OpenDate"] = boost::locale::conv::to_utf<char>(task_data.OpenDate, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryInvestor(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryTradingCode(Task task)
{
	PyLock lock;
	CThostFtdcTradingCodeField task_data = any_cast<CThostFtdcTradingCodeField>(task.task_data);
	dict data;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["ClientIDType"] = task_data.ClientIDType;
	data["IsActive"] = task_data.IsActive;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["BizType"] = task_data.BizType;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryTradingCode(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryInstrumentMarginRate(Task task)
{
	PyLock lock;
	CThostFtdcInstrumentMarginRateField task_data = any_cast<CThostFtdcInstrumentMarginRateField>(task.task_data);
	dict data;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["LongMarginRatioByMoney"] = task_data.LongMarginRatioByMoney;
	data["ShortMarginRatioByVolume"] = task_data.ShortMarginRatioByVolume;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["IsRelative"] = task_data.IsRelative;
	data["LongMarginRatioByVolume"] = task_data.LongMarginRatioByVolume;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["InvestorRange"] = task_data.InvestorRange;
	data["ShortMarginRatioByMoney"] = task_data.ShortMarginRatioByMoney;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryInstrumentMarginRate(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryInstrumentCommissionRate(Task task)
{
	PyLock lock;
	CThostFtdcInstrumentCommissionRateField task_data = any_cast<CThostFtdcInstrumentCommissionRateField>(task.task_data);
	dict data;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["CloseRatioByMoney"] = task_data.CloseRatioByMoney;
	data["OpenRatioByVolume"] = task_data.OpenRatioByVolume;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["CloseRatioByVolume"] = task_data.CloseRatioByVolume;
	data["BizType"] = task_data.BizType;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["CloseTodayRatioByVolume"] = task_data.CloseTodayRatioByVolume;
	data["CloseTodayRatioByMoney"] = task_data.CloseTodayRatioByMoney;
	data["OpenRatioByMoney"] = task_data.OpenRatioByMoney;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["InvestorRange"] = task_data.InvestorRange;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryInstrumentCommissionRate(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryExchange(Task task)
{
	PyLock lock;
	CThostFtdcExchangeField task_data = any_cast<CThostFtdcExchangeField>(task.task_data);
	dict data;
	data["ExchangeName"] = boost::locale::conv::to_utf<char>(task_data.ExchangeName, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["ExchangeProperty"] = task_data.ExchangeProperty;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryExchange(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryProduct(Task task)
{
	PyLock lock;
	CThostFtdcProductField task_data = any_cast<CThostFtdcProductField>(task.task_data);
	dict data;
	data["CloseDealType"] = task_data.CloseDealType;
	data["PriceTick"] = task_data.PriceTick;
	data["PositionType"] = task_data.PositionType;
	data["MaxMarketOrderVolume"] = task_data.MaxMarketOrderVolume;
	data["MaxLimitOrderVolume"] = task_data.MaxLimitOrderVolume;
	data["MortgageFundUseRange"] = task_data.MortgageFundUseRange;
	data["PositionDateType"] = task_data.PositionDateType;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["MinLimitOrderVolume"] = task_data.MinLimitOrderVolume;
	data["TradeCurrencyID"] = boost::locale::conv::to_utf<char>(task_data.TradeCurrencyID, std::string("GB2312"));
	data["ProductName"] = boost::locale::conv::to_utf<char>(task_data.ProductName, std::string("GB2312"));
	data["UnderlyingMultiple"] = task_data.UnderlyingMultiple;
	data["ProductID"] = boost::locale::conv::to_utf<char>(task_data.ProductID, std::string("GB2312"));
	data["ExchangeProductID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeProductID, std::string("GB2312"));
	data["ProductClass"] = task_data.ProductClass;
	data["VolumeMultiple"] = task_data.VolumeMultiple;
	data["MinMarketOrderVolume"] = task_data.MinMarketOrderVolume;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryProduct(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryInstrument(Task task)
{
	PyLock lock;
	CThostFtdcInstrumentField task_data = any_cast<CThostFtdcInstrumentField>(task.task_data);
	dict data;
	data["StrikePrice"] = task_data.StrikePrice;
	data["MaxMarketOrderVolume"] = task_data.MaxMarketOrderVolume;
	data["MaxLimitOrderVolume"] = task_data.MaxLimitOrderVolume;
	data["ShortMarginRatio"] = task_data.ShortMarginRatio;
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["UnderlyingMultiple"] = task_data.UnderlyingMultiple;
	data["DeliveryYear"] = task_data.DeliveryYear;
	data["OptionsType"] = task_data.OptionsType;
	data["ProductClass"] = task_data.ProductClass;
	data["IsTrading"] = task_data.IsTrading;
	data["VolumeMultiple"] = task_data.VolumeMultiple;
	data["PriceTick"] = task_data.PriceTick;
	data["ExpireDate"] = boost::locale::conv::to_utf<char>(task_data.ExpireDate, std::string("GB2312"));
	data["StartDelivDate"] = boost::locale::conv::to_utf<char>(task_data.StartDelivDate, std::string("GB2312"));
	data["InstrumentName"] = boost::locale::conv::to_utf<char>(task_data.InstrumentName, std::string("GB2312"));
	data["MinMarketOrderVolume"] = task_data.MinMarketOrderVolume;
	data["PositionType"] = task_data.PositionType;
	data["PositionDateType"] = task_data.PositionDateType;
	data["CombinationType"] = task_data.CombinationType;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["MinLimitOrderVolume"] = task_data.MinLimitOrderVolume;
	data["EndDelivDate"] = boost::locale::conv::to_utf<char>(task_data.EndDelivDate, std::string("GB2312"));
	data["CreateDate"] = boost::locale::conv::to_utf<char>(task_data.CreateDate, std::string("GB2312"));
	data["ProductID"] = boost::locale::conv::to_utf<char>(task_data.ProductID, std::string("GB2312"));
	data["DeliveryMonth"] = task_data.DeliveryMonth;
	data["InstLifePhase"] = task_data.InstLifePhase;
	data["MaxMarginSideAlgorithm"] = task_data.MaxMarginSideAlgorithm;
	data["UnderlyingInstrID"] = boost::locale::conv::to_utf<char>(task_data.UnderlyingInstrID, std::string("GB2312"));
	data["LongMarginRatio"] = task_data.LongMarginRatio;
	data["OpenDate"] = boost::locale::conv::to_utf<char>(task_data.OpenDate, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryInstrument(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryDepthMarketData(Task task)
{
	PyLock lock;
	CThostFtdcDepthMarketDataField task_data = any_cast<CThostFtdcDepthMarketDataField>(task.task_data);
	dict data;
	data["BidVolume3"] = task_data.BidVolume3;
	data["OpenInterest"] = task_data.OpenInterest;
	data["PreDelta"] = task_data.PreDelta;
	data["BidPrice2"] = task_data.BidPrice2;
	data["Volume"] = task_data.Volume;
	data["AskPrice4"] = task_data.AskPrice4;
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["AveragePrice"] = task_data.AveragePrice;
	data["AskPrice1"] = task_data.AskPrice1;
	data["Turnover"] = task_data.Turnover;
	data["AskPrice5"] = task_data.AskPrice5;
	data["AskPrice3"] = task_data.AskPrice3;
	data["PreOpenInterest"] = task_data.PreOpenInterest;
	data["BidVolume5"] = task_data.BidVolume5;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["BidPrice4"] = task_data.BidPrice4;
	data["LowerLimitPrice"] = task_data.LowerLimitPrice;
	data["ClosePrice"] = task_data.ClosePrice;
	data["BidVolume1"] = task_data.BidVolume1;
	data["BidVolume4"] = task_data.BidVolume4;
	data["UpperLimitPrice"] = task_data.UpperLimitPrice;
	data["UpdateTime"] = boost::locale::conv::to_utf<char>(task_data.UpdateTime, std::string("GB2312"));
	data["AskPrice2"] = task_data.AskPrice2;
	data["PreSettlementPrice"] = task_data.PreSettlementPrice;
	data["PreClosePrice"] = task_data.PreClosePrice;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["BidPrice5"] = task_data.BidPrice5;
	data["HighestPrice"] = task_data.HighestPrice;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["BidVolume2"] = task_data.BidVolume2;
	data["ActionDay"] = boost::locale::conv::to_utf<char>(task_data.ActionDay, std::string("GB2312"));
	data["AskVolume5"] = task_data.AskVolume5;
	data["AskVolume4"] = task_data.AskVolume4;
	data["AskVolume2"] = task_data.AskVolume2;
	data["BidPrice1"] = task_data.BidPrice1;
	data["OpenPrice"] = task_data.OpenPrice;
	data["CurrDelta"] = task_data.CurrDelta;
	data["SettlementPrice"] = task_data.SettlementPrice;
	data["AskVolume1"] = task_data.AskVolume1;
	data["BidPrice3"] = task_data.BidPrice3;
	data["LastPrice"] = task_data.LastPrice;
	data["AskVolume3"] = task_data.AskVolume3;
	data["UpdateMillisec"] = task_data.UpdateMillisec;
	data["LowestPrice"] = task_data.LowestPrice;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryDepthMarketData(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQrySettlementInfo(Task task)
{
	PyLock lock;
	CThostFtdcSettlementInfoField task_data = any_cast<CThostFtdcSettlementInfoField>(task.task_data);
	dict data;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["Content"] = boost::locale::conv::to_utf<char>(task_data.Content, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["SequenceNo"] = task_data.SequenceNo;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["SettlementID"] = task_data.SettlementID;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQrySettlementInfo(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryTransferBank(Task task)
{
	PyLock lock;
	CThostFtdcTransferBankField task_data = any_cast<CThostFtdcTransferBankField>(task.task_data);
	dict data;
	data["IsActive"] = task_data.IsActive;
	data["BankBrchID"] = boost::locale::conv::to_utf<char>(task_data.BankBrchID, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["BankName"] = boost::locale::conv::to_utf<char>(task_data.BankName, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryTransferBank(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryInvestorPositionDetail(Task task)
{
	PyLock lock;
	CThostFtdcInvestorPositionDetailField task_data = any_cast<CThostFtdcInvestorPositionDetailField>(task.task_data);
	dict data;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["PositionProfitByDate"] = task_data.PositionProfitByDate;
	data["CombInstrumentID"] = boost::locale::conv::to_utf<char>(task_data.CombInstrumentID, std::string("GB2312"));
	data["CloseProfitByDate"] = task_data.CloseProfitByDate;
	data["Volume"] = task_data.Volume;
	data["LastSettlementPrice"] = task_data.LastSettlementPrice;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["MarginRateByVolume"] = task_data.MarginRateByVolume;
	data["TradeType"] = task_data.TradeType;
	data["CloseVolume"] = task_data.CloseVolume;
	data["Direction"] = task_data.Direction;
	data["SettlementPrice"] = task_data.SettlementPrice;
	data["ExchMargin"] = task_data.ExchMargin;
	data["CloseAmount"] = task_data.CloseAmount;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["SettlementID"] = task_data.SettlementID;
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["MarginRateByMoney"] = task_data.MarginRateByMoney;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["TradeID"] = boost::locale::conv::to_utf<char>(task_data.TradeID, std::string("GB2312"));
	data["Margin"] = task_data.Margin;
	data["OpenPrice"] = task_data.OpenPrice;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["PositionProfitByTrade"] = task_data.PositionProfitByTrade;
	data["CloseProfitByTrade"] = task_data.CloseProfitByTrade;
	data["OpenDate"] = boost::locale::conv::to_utf<char>(task_data.OpenDate, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryInvestorPositionDetail(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryNotice(Task task)
{
	PyLock lock;
	CThostFtdcNoticeField task_data = any_cast<CThostFtdcNoticeField>(task.task_data);
	dict data;
	data["SequenceLabel"] = boost::locale::conv::to_utf<char>(task_data.SequenceLabel, std::string("GB2312"));
	data["Content"] = boost::locale::conv::to_utf<char>(task_data.Content, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryNotice(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQrySettlementInfoConfirm(Task task)
{
	PyLock lock;
	CThostFtdcSettlementInfoConfirmField task_data = any_cast<CThostFtdcSettlementInfoConfirmField>(task.task_data);
	dict data;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["ConfirmTime"] = boost::locale::conv::to_utf<char>(task_data.ConfirmTime, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["ConfirmDate"] = boost::locale::conv::to_utf<char>(task_data.ConfirmDate, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["SettlementID"] = task_data.SettlementID;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQrySettlementInfoConfirm(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryInvestorPositionCombineDetail(Task task)
{
	PyLock lock;
	CThostFtdcInvestorPositionCombineDetailField task_data = any_cast<CThostFtdcInvestorPositionCombineDetailField>(task.task_data);
	dict data;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["Direction"] = task_data.Direction;
	data["ExchMargin"] = task_data.ExchMargin;
	data["CombInstrumentID"] = boost::locale::conv::to_utf<char>(task_data.CombInstrumentID, std::string("GB2312"));
	data["MarginRateByVolume"] = task_data.MarginRateByVolume;
	data["TotalAmt"] = task_data.TotalAmt;
	data["LegID"] = task_data.LegID;
	data["TradeID"] = boost::locale::conv::to_utf<char>(task_data.TradeID, std::string("GB2312"));
	data["SettlementID"] = task_data.SettlementID;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["TradeGroupID"] = task_data.TradeGroupID;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["ComTradeID"] = boost::locale::conv::to_utf<char>(task_data.ComTradeID, std::string("GB2312"));
	data["MarginRateByMoney"] = task_data.MarginRateByMoney;
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["Margin"] = task_data.Margin;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["LegMultiple"] = task_data.LegMultiple;
	data["OpenDate"] = boost::locale::conv::to_utf<char>(task_data.OpenDate, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryInvestorPositionCombineDetail(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryCFMMCTradingAccountKey(Task task)
{
	PyLock lock;
	CThostFtdcCFMMCTradingAccountKeyField task_data = any_cast<CThostFtdcCFMMCTradingAccountKeyField>(task.task_data);
	dict data;
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["CurrentKey"] = boost::locale::conv::to_utf<char>(task_data.CurrentKey, std::string("GB2312"));
	data["KeyID"] = task_data.KeyID;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryCFMMCTradingAccountKey(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryEWarrantOffset(Task task)
{
	PyLock lock;
	CThostFtdcEWarrantOffsetField task_data = any_cast<CThostFtdcEWarrantOffsetField>(task.task_data);
	dict data;
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["Direction"] = task_data.Direction;
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["Volume"] = task_data.Volume;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryEWarrantOffset(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryInvestorProductGroupMargin(Task task)
{
	PyLock lock;
	CThostFtdcInvestorProductGroupMarginField task_data = any_cast<CThostFtdcInvestorProductGroupMarginField>(task.task_data);
	dict data;
	data["LongExchMargin"] = task_data.LongExchMargin;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["UseMargin"] = task_data.UseMargin;
	data["CashIn"] = task_data.CashIn;
	data["SettlementID"] = task_data.SettlementID;
	data["Commission"] = task_data.Commission;
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["LongExchOffsetAmount"] = task_data.LongExchOffsetAmount;
	data["ExchOffsetAmount"] = task_data.ExchOffsetAmount;
	data["PositionProfit"] = task_data.PositionProfit;
	data["LongFrozenMargin"] = task_data.LongFrozenMargin;
	data["ShortOffsetAmount"] = task_data.ShortOffsetAmount;
	data["ShortFrozenMargin"] = task_data.ShortFrozenMargin;
	data["ProductGroupID"] = boost::locale::conv::to_utf<char>(task_data.ProductGroupID, std::string("GB2312"));
	data["FrozenCommission"] = task_data.FrozenCommission;
	data["CloseProfit"] = task_data.CloseProfit;
	data["ShortUseMargin"] = task_data.ShortUseMargin;
	data["FrozenMargin"] = task_data.FrozenMargin;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["ShortExchMargin"] = task_data.ShortExchMargin;
	data["LongOffsetAmount"] = task_data.LongOffsetAmount;
	data["ExchMargin"] = task_data.ExchMargin;
	data["FrozenCash"] = task_data.FrozenCash;
	data["ShortExchOffsetAmount"] = task_data.ShortExchOffsetAmount;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["OffsetAmount"] = task_data.OffsetAmount;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["LongUseMargin"] = task_data.LongUseMargin;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryInvestorProductGroupMargin(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryExchangeMarginRate(Task task)
{
	PyLock lock;
	CThostFtdcExchangeMarginRateField task_data = any_cast<CThostFtdcExchangeMarginRateField>(task.task_data);
	dict data;
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["LongMarginRatioByMoney"] = task_data.LongMarginRatioByMoney;
	data["ShortMarginRatioByVolume"] = task_data.ShortMarginRatioByVolume;
	data["ShortMarginRatioByMoney"] = task_data.ShortMarginRatioByMoney;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["LongMarginRatioByVolume"] = task_data.LongMarginRatioByVolume;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryExchangeMarginRate(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryExchangeMarginRateAdjust(Task task)
{
	PyLock lock;
	CThostFtdcExchangeMarginRateAdjustField task_data = any_cast<CThostFtdcExchangeMarginRateAdjustField>(task.task_data);
	dict data;
	data["ExchShortMarginRatioByVolume"] = task_data.ExchShortMarginRatioByVolume;
	data["NoShortMarginRatioByMoney"] = task_data.NoShortMarginRatioByMoney;
	data["LongMarginRatioByMoney"] = task_data.LongMarginRatioByMoney;
	data["ExchShortMarginRatioByMoney"] = task_data.ExchShortMarginRatioByMoney;
	data["ShortMarginRatioByVolume"] = task_data.ShortMarginRatioByVolume;
	data["NoLongMarginRatioByVolume"] = task_data.NoLongMarginRatioByVolume;
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["NoShortMarginRatioByVolume"] = task_data.NoShortMarginRatioByVolume;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["ExchLongMarginRatioByVolume"] = task_data.ExchLongMarginRatioByVolume;
	data["ExchLongMarginRatioByMoney"] = task_data.ExchLongMarginRatioByMoney;
	data["LongMarginRatioByVolume"] = task_data.LongMarginRatioByVolume;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["NoLongMarginRatioByMoney"] = task_data.NoLongMarginRatioByMoney;
	data["ShortMarginRatioByMoney"] = task_data.ShortMarginRatioByMoney;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryExchangeMarginRateAdjust(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryExchangeRate(Task task)
{
	PyLock lock;
	CThostFtdcExchangeRateField task_data = any_cast<CThostFtdcExchangeRateField>(task.task_data);
	dict data;
	data["ExchangeRate"] = task_data.ExchangeRate;
	data["FromCurrencyUnit"] = task_data.FromCurrencyUnit;
	data["FromCurrencyID"] = boost::locale::conv::to_utf<char>(task_data.FromCurrencyID, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ToCurrencyID"] = boost::locale::conv::to_utf<char>(task_data.ToCurrencyID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryExchangeRate(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQrySecAgentACIDMap(Task task)
{
	PyLock lock;
	CThostFtdcSecAgentACIDMapField task_data = any_cast<CThostFtdcSecAgentACIDMapField>(task.task_data);
	dict data;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BrokerSecAgentID"] = boost::locale::conv::to_utf<char>(task_data.BrokerSecAgentID, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQrySecAgentACIDMap(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryProductExchRate(Task task)
{
	PyLock lock;
	CThostFtdcProductExchRateField task_data = any_cast<CThostFtdcProductExchRateField>(task.task_data);
	dict data;
	data["QuoteCurrencyID"] = boost::locale::conv::to_utf<char>(task_data.QuoteCurrencyID, std::string("GB2312"));
	data["ExchangeRate"] = task_data.ExchangeRate;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["ProductID"] = boost::locale::conv::to_utf<char>(task_data.ProductID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryProductExchRate(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryProductGroup(Task task)
{
	PyLock lock;
	CThostFtdcProductGroupField task_data = any_cast<CThostFtdcProductGroupField>(task.task_data);
	dict data;
	data["ProductGroupID"] = boost::locale::conv::to_utf<char>(task_data.ProductGroupID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["ProductID"] = boost::locale::conv::to_utf<char>(task_data.ProductID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryProductGroup(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryMMInstrumentCommissionRate(Task task)
{
	PyLock lock;
	CThostFtdcMMInstrumentCommissionRateField task_data = any_cast<CThostFtdcMMInstrumentCommissionRateField>(task.task_data);
	dict data;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["CloseTodayRatioByVolume"] = task_data.CloseTodayRatioByVolume;
	data["CloseTodayRatioByMoney"] = task_data.CloseTodayRatioByMoney;
	data["OpenRatioByVolume"] = task_data.OpenRatioByVolume;
	data["CloseRatioByMoney"] = task_data.CloseRatioByMoney;
	data["OpenRatioByMoney"] = task_data.OpenRatioByMoney;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["InvestorRange"] = task_data.InvestorRange;
	data["CloseRatioByVolume"] = task_data.CloseRatioByVolume;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryMMInstrumentCommissionRate(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryMMOptionInstrCommRate(Task task)
{
	PyLock lock;
	CThostFtdcMMOptionInstrCommRateField task_data = any_cast<CThostFtdcMMOptionInstrCommRateField>(task.task_data);
	dict data;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["CloseRatioByMoney"] = task_data.CloseRatioByMoney;
	data["StrikeRatioByVolume"] = task_data.StrikeRatioByVolume;
	data["StrikeRatioByMoney"] = task_data.StrikeRatioByMoney;
	data["OpenRatioByVolume"] = task_data.OpenRatioByVolume;
	data["CloseTodayRatioByVolume"] = task_data.CloseTodayRatioByVolume;
	data["CloseRatioByVolume"] = task_data.CloseRatioByVolume;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["CloseTodayRatioByMoney"] = task_data.CloseTodayRatioByMoney;
	data["OpenRatioByMoney"] = task_data.OpenRatioByMoney;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["InvestorRange"] = task_data.InvestorRange;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryMMOptionInstrCommRate(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryInstrumentOrderCommRate(Task task)
{
	PyLock lock;
	CThostFtdcInstrumentOrderCommRateField task_data = any_cast<CThostFtdcInstrumentOrderCommRateField>(task.task_data);
	dict data;
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["OrderCommByVolume"] = task_data.OrderCommByVolume;
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["InvestorRange"] = task_data.InvestorRange;
	data["OrderActionCommByVolume"] = task_data.OrderActionCommByVolume;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryInstrumentOrderCommRate(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryOptionInstrTradeCost(Task task)
{
	PyLock lock;
	CThostFtdcOptionInstrTradeCostField task_data = any_cast<CThostFtdcOptionInstrTradeCostField>(task.task_data);
	dict data;
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["ExchFixedMargin"] = task_data.ExchFixedMargin;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["Royalty"] = task_data.Royalty;
	data["ExchMiniMargin"] = task_data.ExchMiniMargin;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["FixedMargin"] = task_data.FixedMargin;
	data["MiniMargin"] = task_data.MiniMargin;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryOptionInstrTradeCost(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryOptionInstrCommRate(Task task)
{
	PyLock lock;
	CThostFtdcOptionInstrCommRateField task_data = any_cast<CThostFtdcOptionInstrCommRateField>(task.task_data);
	dict data;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["CloseRatioByMoney"] = task_data.CloseRatioByMoney;
	data["StrikeRatioByVolume"] = task_data.StrikeRatioByVolume;
	data["StrikeRatioByMoney"] = task_data.StrikeRatioByMoney;
	data["OpenRatioByVolume"] = task_data.OpenRatioByVolume;
	data["CloseTodayRatioByVolume"] = task_data.CloseTodayRatioByVolume;
	data["CloseRatioByVolume"] = task_data.CloseRatioByVolume;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["CloseTodayRatioByMoney"] = task_data.CloseTodayRatioByMoney;
	data["OpenRatioByMoney"] = task_data.OpenRatioByMoney;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["InvestorRange"] = task_data.InvestorRange;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryOptionInstrCommRate(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryExecOrder(Task task)
{
	PyLock lock;
	CThostFtdcExecOrderField task_data = any_cast<CThostFtdcExecOrderField>(task.task_data);
	dict data;
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["ExecResult"] = task_data.ExecResult;
	data["SequenceNo"] = task_data.SequenceNo;
	data["CloseFlag"] = task_data.CloseFlag;
	data["Volume"] = task_data.Volume;
	data["ActionType"] = task_data.ActionType;
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["ReservePositionFlag"] = task_data.ReservePositionFlag;
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["BrokerExecOrderSeq"] = task_data.BrokerExecOrderSeq;
	data["ExecOrderRef"] = boost::locale::conv::to_utf<char>(task_data.ExecOrderRef, std::string("GB2312"));
	data["InsertDate"] = boost::locale::conv::to_utf<char>(task_data.InsertDate, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["ActiveUserID"] = boost::locale::conv::to_utf<char>(task_data.ActiveUserID, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["OffsetFlag"] = task_data.OffsetFlag;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["ClearingPartID"] = boost::locale::conv::to_utf<char>(task_data.ClearingPartID, std::string("GB2312"));
	data["PosiDirection"] = task_data.PosiDirection;
	data["InstallID"] = task_data.InstallID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["OrderSubmitStatus"] = task_data.OrderSubmitStatus;
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["ExecOrderSysID"] = boost::locale::conv::to_utf<char>(task_data.ExecOrderSysID, std::string("GB2312"));
	data["SettlementID"] = task_data.SettlementID;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["NotifySequence"] = task_data.NotifySequence;
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["UserProductInfo"] = boost::locale::conv::to_utf<char>(task_data.UserProductInfo, std::string("GB2312"));
	data["CancelTime"] = boost::locale::conv::to_utf<char>(task_data.CancelTime, std::string("GB2312"));
	data["InsertTime"] = boost::locale::conv::to_utf<char>(task_data.InsertTime, std::string("GB2312"));
	data["ExecOrderLocalID"] = boost::locale::conv::to_utf<char>(task_data.ExecOrderLocalID, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryExecOrder(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryForQuote(Task task)
{
	PyLock lock;
	CThostFtdcForQuoteField task_data = any_cast<CThostFtdcForQuoteField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["ForQuoteStatus"] = task_data.ForQuoteStatus;
	data["ForQuoteLocalID"] = boost::locale::conv::to_utf<char>(task_data.ForQuoteLocalID, std::string("GB2312"));
	data["ForQuoteRef"] = boost::locale::conv::to_utf<char>(task_data.ForQuoteRef, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["InsertDate"] = boost::locale::conv::to_utf<char>(task_data.InsertDate, std::string("GB2312"));
	data["BrokerForQutoSeq"] = task_data.BrokerForQutoSeq;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["InsertTime"] = boost::locale::conv::to_utf<char>(task_data.InsertTime, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["ActiveUserID"] = boost::locale::conv::to_utf<char>(task_data.ActiveUserID, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["InstallID"] = task_data.InstallID;
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryForQuote(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryQuote(Task task)
{
	PyLock lock;
	CThostFtdcQuoteField task_data = any_cast<CThostFtdcQuoteField>(task.task_data);
	dict data;
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["SequenceNo"] = task_data.SequenceNo;
	data["QuoteStatus"] = task_data.QuoteStatus;
	data["QuoteSysID"] = boost::locale::conv::to_utf<char>(task_data.QuoteSysID, std::string("GB2312"));
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["AskOrderSysID"] = boost::locale::conv::to_utf<char>(task_data.AskOrderSysID, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["QuoteRef"] = boost::locale::conv::to_utf<char>(task_data.QuoteRef, std::string("GB2312"));
	data["AskOrderRef"] = boost::locale::conv::to_utf<char>(task_data.AskOrderRef, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["AskOffsetFlag"] = task_data.AskOffsetFlag;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["ActiveUserID"] = boost::locale::conv::to_utf<char>(task_data.ActiveUserID, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["ClearingPartID"] = boost::locale::conv::to_utf<char>(task_data.ClearingPartID, std::string("GB2312"));
	data["AskHedgeFlag"] = task_data.AskHedgeFlag;
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["BidOrderRef"] = boost::locale::conv::to_utf<char>(task_data.BidOrderRef, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["OrderSubmitStatus"] = task_data.OrderSubmitStatus;
	data["AskVolume"] = task_data.AskVolume;
	data["QuoteLocalID"] = boost::locale::conv::to_utf<char>(task_data.QuoteLocalID, std::string("GB2312"));
	data["SettlementID"] = task_data.SettlementID;
	data["ForQuoteSysID"] = boost::locale::conv::to_utf<char>(task_data.ForQuoteSysID, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["AskPrice"] = task_data.AskPrice;
	data["NotifySequence"] = task_data.NotifySequence;
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["BidOrderSysID"] = boost::locale::conv::to_utf<char>(task_data.BidOrderSysID, std::string("GB2312"));
	data["UserProductInfo"] = boost::locale::conv::to_utf<char>(task_data.UserProductInfo, std::string("GB2312"));
	data["CancelTime"] = boost::locale::conv::to_utf<char>(task_data.CancelTime, std::string("GB2312"));
	data["BidHedgeFlag"] = task_data.BidHedgeFlag;
	data["BidOffsetFlag"] = task_data.BidOffsetFlag;
	data["InsertTime"] = boost::locale::conv::to_utf<char>(task_data.InsertTime, std::string("GB2312"));
	data["BidPrice"] = task_data.BidPrice;
	data["BidVolume"] = task_data.BidVolume;
	data["BrokerQuoteSeq"] = task_data.BrokerQuoteSeq;
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["InsertDate"] = boost::locale::conv::to_utf<char>(task_data.InsertDate, std::string("GB2312"));
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryQuote(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryOptionSelfClose(Task task)
{
	PyLock lock;
	CThostFtdcOptionSelfCloseField task_data = any_cast<CThostFtdcOptionSelfCloseField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["OrderSubmitStatus"] = task_data.OrderSubmitStatus;
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["ExecResult"] = task_data.ExecResult;
	data["SequenceNo"] = task_data.SequenceNo;
	data["SettlementID"] = task_data.SettlementID;
	data["Volume"] = task_data.Volume;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["NotifySequence"] = task_data.NotifySequence;
	data["OptionSelfCloseLocalID"] = boost::locale::conv::to_utf<char>(task_data.OptionSelfCloseLocalID, std::string("GB2312"));
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["UserProductInfo"] = boost::locale::conv::to_utf<char>(task_data.UserProductInfo, std::string("GB2312"));
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["OptSelfCloseFlag"] = task_data.OptSelfCloseFlag;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["ActiveUserID"] = boost::locale::conv::to_utf<char>(task_data.ActiveUserID, std::string("GB2312"));
	data["InsertDate"] = boost::locale::conv::to_utf<char>(task_data.InsertDate, std::string("GB2312"));
	data["CancelTime"] = boost::locale::conv::to_utf<char>(task_data.CancelTime, std::string("GB2312"));
	data["OptionSelfCloseSysID"] = boost::locale::conv::to_utf<char>(task_data.OptionSelfCloseSysID, std::string("GB2312"));
	data["OptionSelfCloseRef"] = boost::locale::conv::to_utf<char>(task_data.OptionSelfCloseRef, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["RequestID"] = task_data.RequestID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["InsertTime"] = boost::locale::conv::to_utf<char>(task_data.InsertTime, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["BrokerOptionSelfCloseSeq"] = task_data.BrokerOptionSelfCloseSeq;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["ClearingPartID"] = boost::locale::conv::to_utf<char>(task_data.ClearingPartID, std::string("GB2312"));
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryOptionSelfClose(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryInvestUnit(Task task)
{
	PyLock lock;
	CThostFtdcInvestUnitField task_data = any_cast<CThostFtdcInvestUnitField>(task.task_data);
	dict data;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["InvestorGroupID"] = boost::locale::conv::to_utf<char>(task_data.InvestorGroupID, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["CommModelID"] = boost::locale::conv::to_utf<char>(task_data.CommModelID, std::string("GB2312"));
	data["MarginModelID"] = boost::locale::conv::to_utf<char>(task_data.MarginModelID, std::string("GB2312"));
	data["InvestorUnitName"] = boost::locale::conv::to_utf<char>(task_data.InvestorUnitName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryInvestUnit(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryCombInstrumentGuard(Task task)
{
	PyLock lock;
	CThostFtdcCombInstrumentGuardField task_data = any_cast<CThostFtdcCombInstrumentGuardField>(task.task_data);
	dict data;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["GuarantRatio"] = task_data.GuarantRatio;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryCombInstrumentGuard(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryCombAction(Task task)
{
	PyLock lock;
	CThostFtdcCombActionField task_data = any_cast<CThostFtdcCombActionField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["SequenceNo"] = task_data.SequenceNo;
	data["SettlementID"] = task_data.SettlementID;
	data["CombActionRef"] = boost::locale::conv::to_utf<char>(task_data.CombActionRef, std::string("GB2312"));
	data["Volume"] = task_data.Volume;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["NotifySequence"] = task_data.NotifySequence;
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["UserProductInfo"] = boost::locale::conv::to_utf<char>(task_data.UserProductInfo, std::string("GB2312"));
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["ComTradeID"] = boost::locale::conv::to_utf<char>(task_data.ComTradeID, std::string("GB2312"));
	data["Direction"] = task_data.Direction;
	data["SessionID"] = task_data.SessionID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["ActionStatus"] = task_data.ActionStatus;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["CombDirection"] = task_data.CombDirection;
	data["ActionLocalID"] = boost::locale::conv::to_utf<char>(task_data.ActionLocalID, std::string("GB2312"));
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryCombAction(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryTransferSerial(Task task)
{
	PyLock lock;
	CThostFtdcTransferSerialField task_data = any_cast<CThostFtdcTransferSerialField>(task.task_data);
	dict data;
	data["AvailabilityFlag"] = task_data.AvailabilityFlag;
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["OperatorCode"] = boost::locale::conv::to_utf<char>(task_data.OperatorCode, std::string("GB2312"));
	data["BankNewAccount"] = boost::locale::conv::to_utf<char>(task_data.BankNewAccount, std::string("GB2312"));
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["BrokerFee"] = task_data.BrokerFee;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["FutureAccType"] = task_data.FutureAccType;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["PlateSerial"] = task_data.PlateSerial;
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryTransferSerial(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryAccountregister(Task task)
{
	PyLock lock;
	CThostFtdcAccountregisterField task_data = any_cast<CThostFtdcAccountregisterField>(task.task_data);
	dict data;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["TradeDay"] = boost::locale::conv::to_utf<char>(task_data.TradeDay, std::string("GB2312"));
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["RegDate"] = boost::locale::conv::to_utf<char>(task_data.RegDate, std::string("GB2312"));
	data["CustType"] = task_data.CustType;
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["BankAccType"] = task_data.BankAccType;
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["TID"] = task_data.TID;
	data["OpenOrDestroy"] = task_data.OpenOrDestroy;
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["IdCardType"] = task_data.IdCardType;
	data["OutDate"] = boost::locale::conv::to_utf<char>(task_data.OutDate, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryAccountregister(data, error, task.task_id, task.task_last);
};

void TdApi::processRspError(Task task)
{
	PyLock lock;
	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspError(error, task.task_id, task.task_last);
};

void TdApi::processRtnOrder(Task task)
{
	PyLock lock;
	CThostFtdcOrderField task_data = any_cast<CThostFtdcOrderField>(task.task_data);
	dict data;
	data["StopPrice"] = task_data.StopPrice;
	data["OrderStatus"] = task_data.OrderStatus;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["SuspendTime"] = boost::locale::conv::to_utf<char>(task_data.SuspendTime, std::string("GB2312"));
	data["OrderType"] = task_data.OrderType;
	data["FrontID"] = task_data.FrontID;
	data["VolumeTotal"] = task_data.VolumeTotal;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["ClearingPartID"] = boost::locale::conv::to_utf<char>(task_data.ClearingPartID, std::string("GB2312"));
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["VolumeTraded"] = task_data.VolumeTraded;
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["IsSwapOrder"] = task_data.IsSwapOrder;
	data["VolumeCondition"] = task_data.VolumeCondition;
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["InsertDate"] = boost::locale::conv::to_utf<char>(task_data.InsertDate, std::string("GB2312"));
	data["ActiveTime"] = boost::locale::conv::to_utf<char>(task_data.ActiveTime, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["RequestID"] = task_data.RequestID;
	data["CombOffsetFlag"] = boost::locale::conv::to_utf<char>(task_data.CombOffsetFlag, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["ActiveUserID"] = boost::locale::conv::to_utf<char>(task_data.ActiveUserID, std::string("GB2312"));
	data["SequenceNo"] = task_data.SequenceNo;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["UpdateTime"] = boost::locale::conv::to_utf<char>(task_data.UpdateTime, std::string("GB2312"));
	data["OrderPriceType"] = task_data.OrderPriceType;
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["OrderSysID"] = boost::locale::conv::to_utf<char>(task_data.OrderSysID, std::string("GB2312"));
	data["TimeCondition"] = task_data.TimeCondition;
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["OrderSubmitStatus"] = task_data.OrderSubmitStatus;
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["ZCETotalTradedVolume"] = task_data.ZCETotalTradedVolume;
	data["BrokerOrderSeq"] = task_data.BrokerOrderSeq;
	data["CombHedgeFlag"] = boost::locale::conv::to_utf<char>(task_data.CombHedgeFlag, std::string("GB2312"));
	data["SettlementID"] = task_data.SettlementID;
	data["OrderRef"] = boost::locale::conv::to_utf<char>(task_data.OrderRef, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["NotifySequence"] = task_data.NotifySequence;
	data["GTDDate"] = boost::locale::conv::to_utf<char>(task_data.GTDDate, std::string("GB2312"));
	data["OrderSource"] = task_data.OrderSource;
	data["IsAutoSuspend"] = task_data.IsAutoSuspend;
	data["UserProductInfo"] = boost::locale::conv::to_utf<char>(task_data.UserProductInfo, std::string("GB2312"));
	data["ContingentCondition"] = task_data.ContingentCondition;
	data["RelativeOrderSysID"] = boost::locale::conv::to_utf<char>(task_data.RelativeOrderSysID, std::string("GB2312"));
	data["CancelTime"] = boost::locale::conv::to_utf<char>(task_data.CancelTime, std::string("GB2312"));
	data["Direction"] = task_data.Direction;
	data["OrderLocalID"] = boost::locale::conv::to_utf<char>(task_data.OrderLocalID, std::string("GB2312"));
	data["LimitPrice"] = task_data.LimitPrice;
	data["ForceCloseReason"] = task_data.ForceCloseReason;
	data["InsertTime"] = boost::locale::conv::to_utf<char>(task_data.InsertTime, std::string("GB2312"));
	data["MinVolume"] = task_data.MinVolume;
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["VolumeTotalOriginal"] = task_data.VolumeTotalOriginal;
	data["ActiveTraderID"] = boost::locale::conv::to_utf<char>(task_data.ActiveTraderID, std::string("GB2312"));
	data["UserForceClose"] = task_data.UserForceClose;
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));

	this->onRtnOrder(data);
};

void TdApi::processRtnTrade(Task task)
{
	PyLock lock;
	CThostFtdcTradeField task_data = any_cast<CThostFtdcTradeField>(task.task_data);
	dict data;
	data["OrderSysID"] = boost::locale::conv::to_utf<char>(task_data.OrderSysID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["SequenceNo"] = task_data.SequenceNo;
	data["BrokerOrderSeq"] = task_data.BrokerOrderSeq;
	data["Volume"] = task_data.Volume;
	data["PriceSource"] = task_data.PriceSource;
	data["TradeID"] = boost::locale::conv::to_utf<char>(task_data.TradeID, std::string("GB2312"));
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["OrderRef"] = boost::locale::conv::to_utf<char>(task_data.OrderRef, std::string("GB2312"));
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["TradingRole"] = task_data.TradingRole;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["TradeType"] = task_data.TradeType;
	data["TradeSource"] = task_data.TradeSource;
	data["Direction"] = task_data.Direction;
	data["OrderLocalID"] = boost::locale::conv::to_utf<char>(task_data.OrderLocalID, std::string("GB2312"));
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["SettlementID"] = task_data.SettlementID;
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["Price"] = task_data.Price;
	data["OffsetFlag"] = task_data.OffsetFlag;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["ClearingPartID"] = boost::locale::conv::to_utf<char>(task_data.ClearingPartID, std::string("GB2312"));
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));

	this->onRtnTrade(data);
};

void TdApi::processErrRtnOrderInsert(Task task)
{
	PyLock lock;
	CThostFtdcInputOrderField task_data = any_cast<CThostFtdcInputOrderField>(task.task_data);
	dict data;
	data["StopPrice"] = task_data.StopPrice;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["TimeCondition"] = task_data.TimeCondition;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["IsSwapOrder"] = task_data.IsSwapOrder;
	data["CombHedgeFlag"] = boost::locale::conv::to_utf<char>(task_data.CombHedgeFlag, std::string("GB2312"));
	data["ContingentCondition"] = task_data.ContingentCondition;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["GTDDate"] = boost::locale::conv::to_utf<char>(task_data.GTDDate, std::string("GB2312"));
	data["IsAutoSuspend"] = task_data.IsAutoSuspend;
	data["VolumeCondition"] = task_data.VolumeCondition;
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["LimitPrice"] = task_data.LimitPrice;
	data["Direction"] = task_data.Direction;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["CombOffsetFlag"] = boost::locale::conv::to_utf<char>(task_data.CombOffsetFlag, std::string("GB2312"));
	data["OrderRef"] = boost::locale::conv::to_utf<char>(task_data.OrderRef, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["VolumeTotalOriginal"] = task_data.VolumeTotalOriginal;
	data["MinVolume"] = task_data.MinVolume;
	data["OrderPriceType"] = task_data.OrderPriceType;
	data["UserForceClose"] = task_data.UserForceClose;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["ForceCloseReason"] = task_data.ForceCloseReason;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnOrderInsert(data, error);
};

void TdApi::processErrRtnOrderAction(Task task)
{
	PyLock lock;
	CThostFtdcOrderActionField task_data = any_cast<CThostFtdcOrderActionField>(task.task_data);
	dict data;
	data["OrderSysID"] = boost::locale::conv::to_utf<char>(task_data.OrderSysID, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["ActionFlag"] = task_data.ActionFlag;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["VolumeChange"] = task_data.VolumeChange;
	data["LimitPrice"] = task_data.LimitPrice;
	data["ActionDate"] = boost::locale::conv::to_utf<char>(task_data.ActionDate, std::string("GB2312"));
	data["OrderLocalID"] = boost::locale::conv::to_utf<char>(task_data.OrderLocalID, std::string("GB2312"));
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["OrderRef"] = boost::locale::conv::to_utf<char>(task_data.OrderRef, std::string("GB2312"));
	data["OrderActionStatus"] = task_data.OrderActionStatus;
	data["ActionTime"] = boost::locale::conv::to_utf<char>(task_data.ActionTime, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["OrderActionRef"] = task_data.OrderActionRef;
	data["ActionLocalID"] = boost::locale::conv::to_utf<char>(task_data.ActionLocalID, std::string("GB2312"));
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnOrderAction(data, error);
};

void TdApi::processRtnInstrumentStatus(Task task)
{
	PyLock lock;
	CThostFtdcInstrumentStatusField task_data = any_cast<CThostFtdcInstrumentStatusField>(task.task_data);
	dict data;
	data["TradingSegmentSN"] = task_data.TradingSegmentSN;
	data["EnterTime"] = boost::locale::conv::to_utf<char>(task_data.EnterTime, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InstrumentStatus"] = task_data.InstrumentStatus;
	data["EnterReason"] = task_data.EnterReason;
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["SettlementGroupID"] = boost::locale::conv::to_utf<char>(task_data.SettlementGroupID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));

	this->onRtnInstrumentStatus(data);
};

void TdApi::processRtnBulletin(Task task)
{
	PyLock lock;
	CThostFtdcBulletinField task_data = any_cast<CThostFtdcBulletinField>(task.task_data);
	dict data;
	data["SendTime"] = boost::locale::conv::to_utf<char>(task_data.SendTime, std::string("GB2312"));
	data["Content"] = boost::locale::conv::to_utf<char>(task_data.Content, std::string("GB2312"));
	data["MarketID"] = boost::locale::conv::to_utf<char>(task_data.MarketID, std::string("GB2312"));
	data["SequenceNo"] = task_data.SequenceNo;
	data["ComeFrom"] = boost::locale::conv::to_utf<char>(task_data.ComeFrom, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["NewsUrgency"] = task_data.NewsUrgency;
	data["NewsType"] = boost::locale::conv::to_utf<char>(task_data.NewsType, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["BulletinID"] = task_data.BulletinID;
	data["Abstract"] = boost::locale::conv::to_utf<char>(task_data.Abstract, std::string("GB2312"));
	data["URLLink"] = boost::locale::conv::to_utf<char>(task_data.URLLink, std::string("GB2312"));

	this->onRtnBulletin(data);
};

void TdApi::processRtnTradingNotice(Task task)
{
	PyLock lock;
	CThostFtdcTradingNoticeInfoField task_data = any_cast<CThostFtdcTradingNoticeInfoField>(task.task_data);
	dict data;
	data["FieldContent"] = boost::locale::conv::to_utf<char>(task_data.FieldContent, std::string("GB2312"));
	data["SendTime"] = boost::locale::conv::to_utf<char>(task_data.SendTime, std::string("GB2312"));
	data["SequenceSeries"] = task_data.SequenceSeries;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["SequenceNo"] = task_data.SequenceNo;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));

	this->onRtnTradingNotice(data);
};

void TdApi::processRtnErrorConditionalOrder(Task task)
{
	PyLock lock;
	CThostFtdcErrorConditionalOrderField task_data = any_cast<CThostFtdcErrorConditionalOrderField>(task.task_data);
	dict data;
	data["StopPrice"] = task_data.StopPrice;
	data["OrderStatus"] = task_data.OrderStatus;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["SuspendTime"] = boost::locale::conv::to_utf<char>(task_data.SuspendTime, std::string("GB2312"));
	data["OrderType"] = task_data.OrderType;
	data["FrontID"] = task_data.FrontID;
	data["VolumeTotal"] = task_data.VolumeTotal;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["ClearingPartID"] = boost::locale::conv::to_utf<char>(task_data.ClearingPartID, std::string("GB2312"));
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["VolumeTraded"] = task_data.VolumeTraded;
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["VolumeCondition"] = task_data.VolumeCondition;
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["InsertDate"] = boost::locale::conv::to_utf<char>(task_data.InsertDate, std::string("GB2312"));
	data["ActiveTime"] = boost::locale::conv::to_utf<char>(task_data.ActiveTime, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["RequestID"] = task_data.RequestID;
	data["CombOffsetFlag"] = boost::locale::conv::to_utf<char>(task_data.CombOffsetFlag, std::string("GB2312"));
	data["IsSwapOrder"] = task_data.IsSwapOrder;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["ActiveUserID"] = boost::locale::conv::to_utf<char>(task_data.ActiveUserID, std::string("GB2312"));
	data["SequenceNo"] = task_data.SequenceNo;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["UpdateTime"] = boost::locale::conv::to_utf<char>(task_data.UpdateTime, std::string("GB2312"));
	data["OrderPriceType"] = task_data.OrderPriceType;
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["OrderSysID"] = boost::locale::conv::to_utf<char>(task_data.OrderSysID, std::string("GB2312"));
	data["TimeCondition"] = task_data.TimeCondition;
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["OrderSubmitStatus"] = task_data.OrderSubmitStatus;
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["ZCETotalTradedVolume"] = task_data.ZCETotalTradedVolume;
	data["BrokerOrderSeq"] = task_data.BrokerOrderSeq;
	data["CombHedgeFlag"] = boost::locale::conv::to_utf<char>(task_data.CombHedgeFlag, std::string("GB2312"));
	data["SettlementID"] = task_data.SettlementID;
	data["OrderRef"] = boost::locale::conv::to_utf<char>(task_data.OrderRef, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["NotifySequence"] = task_data.NotifySequence;
	data["GTDDate"] = boost::locale::conv::to_utf<char>(task_data.GTDDate, std::string("GB2312"));
	data["OrderSource"] = task_data.OrderSource;
	data["IsAutoSuspend"] = task_data.IsAutoSuspend;
	data["UserProductInfo"] = boost::locale::conv::to_utf<char>(task_data.UserProductInfo, std::string("GB2312"));
	data["ContingentCondition"] = task_data.ContingentCondition;
	data["RelativeOrderSysID"] = boost::locale::conv::to_utf<char>(task_data.RelativeOrderSysID, std::string("GB2312"));
	data["CancelTime"] = boost::locale::conv::to_utf<char>(task_data.CancelTime, std::string("GB2312"));
	data["Direction"] = task_data.Direction;
	data["OrderLocalID"] = boost::locale::conv::to_utf<char>(task_data.OrderLocalID, std::string("GB2312"));
	data["LimitPrice"] = task_data.LimitPrice;
	data["ForceCloseReason"] = task_data.ForceCloseReason;
	data["InsertTime"] = boost::locale::conv::to_utf<char>(task_data.InsertTime, std::string("GB2312"));
	data["MinVolume"] = task_data.MinVolume;
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["VolumeTotalOriginal"] = task_data.VolumeTotalOriginal;
	data["ActiveTraderID"] = boost::locale::conv::to_utf<char>(task_data.ActiveTraderID, std::string("GB2312"));
	data["UserForceClose"] = task_data.UserForceClose;
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));

	this->onRtnErrorConditionalOrder(data);
};

void TdApi::processRtnExecOrder(Task task)
{
	PyLock lock;
	CThostFtdcExecOrderField task_data = any_cast<CThostFtdcExecOrderField>(task.task_data);
	dict data;
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["ExecResult"] = task_data.ExecResult;
	data["SequenceNo"] = task_data.SequenceNo;
	data["CloseFlag"] = task_data.CloseFlag;
	data["Volume"] = task_data.Volume;
	data["ActionType"] = task_data.ActionType;
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["ReservePositionFlag"] = task_data.ReservePositionFlag;
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["BrokerExecOrderSeq"] = task_data.BrokerExecOrderSeq;
	data["ExecOrderRef"] = boost::locale::conv::to_utf<char>(task_data.ExecOrderRef, std::string("GB2312"));
	data["InsertDate"] = boost::locale::conv::to_utf<char>(task_data.InsertDate, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["ActiveUserID"] = boost::locale::conv::to_utf<char>(task_data.ActiveUserID, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["OffsetFlag"] = task_data.OffsetFlag;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["ClearingPartID"] = boost::locale::conv::to_utf<char>(task_data.ClearingPartID, std::string("GB2312"));
	data["PosiDirection"] = task_data.PosiDirection;
	data["InstallID"] = task_data.InstallID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["OrderSubmitStatus"] = task_data.OrderSubmitStatus;
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["ExecOrderSysID"] = boost::locale::conv::to_utf<char>(task_data.ExecOrderSysID, std::string("GB2312"));
	data["SettlementID"] = task_data.SettlementID;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["NotifySequence"] = task_data.NotifySequence;
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["UserProductInfo"] = boost::locale::conv::to_utf<char>(task_data.UserProductInfo, std::string("GB2312"));
	data["CancelTime"] = boost::locale::conv::to_utf<char>(task_data.CancelTime, std::string("GB2312"));
	data["InsertTime"] = boost::locale::conv::to_utf<char>(task_data.InsertTime, std::string("GB2312"));
	data["ExecOrderLocalID"] = boost::locale::conv::to_utf<char>(task_data.ExecOrderLocalID, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));

	this->onRtnExecOrder(data);
};

void TdApi::processErrRtnExecOrderInsert(Task task)
{
	PyLock lock;
	CThostFtdcInputExecOrderField task_data = any_cast<CThostFtdcInputExecOrderField>(task.task_data);
	dict data;
	data["ExecOrderRef"] = boost::locale::conv::to_utf<char>(task_data.ExecOrderRef, std::string("GB2312"));
	data["ReservePositionFlag"] = task_data.ReservePositionFlag;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["CloseFlag"] = task_data.CloseFlag;
	data["Volume"] = task_data.Volume;
	data["ActionType"] = task_data.ActionType;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["OffsetFlag"] = task_data.OffsetFlag;
	data["PosiDirection"] = task_data.PosiDirection;
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnExecOrderInsert(data, error);
};

void TdApi::processErrRtnExecOrderAction(Task task)
{
	PyLock lock;
	CThostFtdcExecOrderActionField task_data = any_cast<CThostFtdcExecOrderActionField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["ExecOrderSysID"] = boost::locale::conv::to_utf<char>(task_data.ExecOrderSysID, std::string("GB2312"));
	data["ActionType"] = task_data.ActionType;
	data["ActionFlag"] = task_data.ActionFlag;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["OrderActionStatus"] = task_data.OrderActionStatus;
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["ExecOrderRef"] = boost::locale::conv::to_utf<char>(task_data.ExecOrderRef, std::string("GB2312"));
	data["ActionDate"] = boost::locale::conv::to_utf<char>(task_data.ActionDate, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["RequestID"] = task_data.RequestID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ExecOrderActionRef"] = task_data.ExecOrderActionRef;
	data["ExecOrderLocalID"] = boost::locale::conv::to_utf<char>(task_data.ExecOrderLocalID, std::string("GB2312"));
	data["ActionTime"] = boost::locale::conv::to_utf<char>(task_data.ActionTime, std::string("GB2312"));
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["ActionLocalID"] = boost::locale::conv::to_utf<char>(task_data.ActionLocalID, std::string("GB2312"));
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnExecOrderAction(data, error);
};

void TdApi::processErrRtnForQuoteInsert(Task task)
{
	PyLock lock;
	CThostFtdcInputForQuoteField task_data = any_cast<CThostFtdcInputForQuoteField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["ForQuoteRef"] = boost::locale::conv::to_utf<char>(task_data.ForQuoteRef, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnForQuoteInsert(data, error);
};

void TdApi::processRtnQuote(Task task)
{
	PyLock lock;
	CThostFtdcQuoteField task_data = any_cast<CThostFtdcQuoteField>(task.task_data);
	dict data;
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["SequenceNo"] = task_data.SequenceNo;
	data["QuoteStatus"] = task_data.QuoteStatus;
	data["QuoteSysID"] = boost::locale::conv::to_utf<char>(task_data.QuoteSysID, std::string("GB2312"));
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["AskOrderSysID"] = boost::locale::conv::to_utf<char>(task_data.AskOrderSysID, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["QuoteRef"] = boost::locale::conv::to_utf<char>(task_data.QuoteRef, std::string("GB2312"));
	data["AskOrderRef"] = boost::locale::conv::to_utf<char>(task_data.AskOrderRef, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["AskOffsetFlag"] = task_data.AskOffsetFlag;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["ActiveUserID"] = boost::locale::conv::to_utf<char>(task_data.ActiveUserID, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["ClearingPartID"] = boost::locale::conv::to_utf<char>(task_data.ClearingPartID, std::string("GB2312"));
	data["AskHedgeFlag"] = task_data.AskHedgeFlag;
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["BidOrderRef"] = boost::locale::conv::to_utf<char>(task_data.BidOrderRef, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["OrderSubmitStatus"] = task_data.OrderSubmitStatus;
	data["AskVolume"] = task_data.AskVolume;
	data["QuoteLocalID"] = boost::locale::conv::to_utf<char>(task_data.QuoteLocalID, std::string("GB2312"));
	data["SettlementID"] = task_data.SettlementID;
	data["ForQuoteSysID"] = boost::locale::conv::to_utf<char>(task_data.ForQuoteSysID, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["AskPrice"] = task_data.AskPrice;
	data["NotifySequence"] = task_data.NotifySequence;
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["BidOrderSysID"] = boost::locale::conv::to_utf<char>(task_data.BidOrderSysID, std::string("GB2312"));
	data["UserProductInfo"] = boost::locale::conv::to_utf<char>(task_data.UserProductInfo, std::string("GB2312"));
	data["CancelTime"] = boost::locale::conv::to_utf<char>(task_data.CancelTime, std::string("GB2312"));
	data["BidHedgeFlag"] = task_data.BidHedgeFlag;
	data["BidOffsetFlag"] = task_data.BidOffsetFlag;
	data["InsertTime"] = boost::locale::conv::to_utf<char>(task_data.InsertTime, std::string("GB2312"));
	data["BidPrice"] = task_data.BidPrice;
	data["BidVolume"] = task_data.BidVolume;
	data["BrokerQuoteSeq"] = task_data.BrokerQuoteSeq;
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["InsertDate"] = boost::locale::conv::to_utf<char>(task_data.InsertDate, std::string("GB2312"));
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));

	this->onRtnQuote(data);
};

void TdApi::processErrRtnQuoteInsert(Task task)
{
	PyLock lock;
	CThostFtdcInputQuoteField task_data = any_cast<CThostFtdcInputQuoteField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["BidOrderRef"] = boost::locale::conv::to_utf<char>(task_data.BidOrderRef, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["AskPrice"] = task_data.AskPrice;
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["QuoteRef"] = boost::locale::conv::to_utf<char>(task_data.QuoteRef, std::string("GB2312"));
	data["AskOrderRef"] = boost::locale::conv::to_utf<char>(task_data.AskOrderRef, std::string("GB2312"));
	data["BidHedgeFlag"] = task_data.BidHedgeFlag;
	data["BidOffsetFlag"] = task_data.BidOffsetFlag;
	data["BidPrice"] = task_data.BidPrice;
	data["RequestID"] = task_data.RequestID;
	data["AskOffsetFlag"] = task_data.AskOffsetFlag;
	data["AskVolume"] = task_data.AskVolume;
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["BidVolume"] = task_data.BidVolume;
	data["ForQuoteSysID"] = boost::locale::conv::to_utf<char>(task_data.ForQuoteSysID, std::string("GB2312"));
	data["AskHedgeFlag"] = task_data.AskHedgeFlag;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnQuoteInsert(data, error);
};

void TdApi::processErrRtnQuoteAction(Task task)
{
	PyLock lock;
	CThostFtdcQuoteActionField task_data = any_cast<CThostFtdcQuoteActionField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["QuoteLocalID"] = boost::locale::conv::to_utf<char>(task_data.QuoteLocalID, std::string("GB2312"));
	data["QuoteSysID"] = boost::locale::conv::to_utf<char>(task_data.QuoteSysID, std::string("GB2312"));
	data["ActionFlag"] = task_data.ActionFlag;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["QuoteActionRef"] = task_data.QuoteActionRef;
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["QuoteRef"] = boost::locale::conv::to_utf<char>(task_data.QuoteRef, std::string("GB2312"));
	data["ActionDate"] = boost::locale::conv::to_utf<char>(task_data.ActionDate, std::string("GB2312"));
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["OrderActionStatus"] = task_data.OrderActionStatus;
	data["ActionTime"] = boost::locale::conv::to_utf<char>(task_data.ActionTime, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["ActionLocalID"] = boost::locale::conv::to_utf<char>(task_data.ActionLocalID, std::string("GB2312"));
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnQuoteAction(data, error);
};

void TdApi::processRtnForQuoteRsp(Task task)
{
	PyLock lock;
	CThostFtdcForQuoteRspField task_data = any_cast<CThostFtdcForQuoteRspField>(task.task_data);
	dict data;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["ActionDay"] = boost::locale::conv::to_utf<char>(task_data.ActionDay, std::string("GB2312"));
	data["ForQuoteTime"] = boost::locale::conv::to_utf<char>(task_data.ForQuoteTime, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["ForQuoteSysID"] = boost::locale::conv::to_utf<char>(task_data.ForQuoteSysID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));

	this->onRtnForQuoteRsp(data);
};

void TdApi::processRtnCFMMCTradingAccountToken(Task task)
{
	PyLock lock;
	CThostFtdcCFMMCTradingAccountTokenField task_data = any_cast<CThostFtdcCFMMCTradingAccountTokenField>(task.task_data);
	dict data;
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));
	data["Token"] = boost::locale::conv::to_utf<char>(task_data.Token, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["KeyID"] = task_data.KeyID;

	this->onRtnCFMMCTradingAccountToken(data);
};

void TdApi::processErrRtnBatchOrderAction(Task task)
{
	PyLock lock;
	CThostFtdcBatchOrderActionField task_data = any_cast<CThostFtdcBatchOrderActionField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["ActionDate"] = boost::locale::conv::to_utf<char>(task_data.ActionDate, std::string("GB2312"));
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["ActionTime"] = boost::locale::conv::to_utf<char>(task_data.ActionTime, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["OrderActionRef"] = task_data.OrderActionRef;
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["OrderActionStatus"] = task_data.OrderActionStatus;
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["ActionLocalID"] = boost::locale::conv::to_utf<char>(task_data.ActionLocalID, std::string("GB2312"));
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnBatchOrderAction(data, error);
};

void TdApi::processRtnOptionSelfClose(Task task)
{
	PyLock lock;
	CThostFtdcOptionSelfCloseField task_data = any_cast<CThostFtdcOptionSelfCloseField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["OrderSubmitStatus"] = task_data.OrderSubmitStatus;
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["ExecResult"] = task_data.ExecResult;
	data["SequenceNo"] = task_data.SequenceNo;
	data["SettlementID"] = task_data.SettlementID;
	data["Volume"] = task_data.Volume;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["NotifySequence"] = task_data.NotifySequence;
	data["OptionSelfCloseLocalID"] = boost::locale::conv::to_utf<char>(task_data.OptionSelfCloseLocalID, std::string("GB2312"));
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["UserProductInfo"] = boost::locale::conv::to_utf<char>(task_data.UserProductInfo, std::string("GB2312"));
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["OptSelfCloseFlag"] = task_data.OptSelfCloseFlag;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["ActiveUserID"] = boost::locale::conv::to_utf<char>(task_data.ActiveUserID, std::string("GB2312"));
	data["InsertDate"] = boost::locale::conv::to_utf<char>(task_data.InsertDate, std::string("GB2312"));
	data["CancelTime"] = boost::locale::conv::to_utf<char>(task_data.CancelTime, std::string("GB2312"));
	data["OptionSelfCloseSysID"] = boost::locale::conv::to_utf<char>(task_data.OptionSelfCloseSysID, std::string("GB2312"));
	data["OptionSelfCloseRef"] = boost::locale::conv::to_utf<char>(task_data.OptionSelfCloseRef, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["RequestID"] = task_data.RequestID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["InsertTime"] = boost::locale::conv::to_utf<char>(task_data.InsertTime, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["BrokerOptionSelfCloseSeq"] = task_data.BrokerOptionSelfCloseSeq;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["ClearingPartID"] = boost::locale::conv::to_utf<char>(task_data.ClearingPartID, std::string("GB2312"));
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));

	this->onRtnOptionSelfClose(data);
};

void TdApi::processErrRtnOptionSelfCloseInsert(Task task)
{
	PyLock lock;
	CThostFtdcInputOptionSelfCloseField task_data = any_cast<CThostFtdcInputOptionSelfCloseField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["OptionSelfCloseRef"] = boost::locale::conv::to_utf<char>(task_data.OptionSelfCloseRef, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["Volume"] = task_data.Volume;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["OptSelfCloseFlag"] = task_data.OptSelfCloseFlag;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnOptionSelfCloseInsert(data, error);
};

void TdApi::processErrRtnOptionSelfCloseAction(Task task)
{
	PyLock lock;
	CThostFtdcOptionSelfCloseActionField task_data = any_cast<CThostFtdcOptionSelfCloseActionField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["OptionSelfCloseActionRef"] = task_data.OptionSelfCloseActionRef;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["ActionFlag"] = task_data.ActionFlag;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["OptionSelfCloseLocalID"] = boost::locale::conv::to_utf<char>(task_data.OptionSelfCloseLocalID, std::string("GB2312"));
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["OptionSelfCloseSysID"] = boost::locale::conv::to_utf<char>(task_data.OptionSelfCloseSysID, std::string("GB2312"));
	data["OptionSelfCloseRef"] = boost::locale::conv::to_utf<char>(task_data.OptionSelfCloseRef, std::string("GB2312"));
	data["ActionDate"] = boost::locale::conv::to_utf<char>(task_data.ActionDate, std::string("GB2312"));
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["OrderActionStatus"] = task_data.OrderActionStatus;
	data["ActionTime"] = boost::locale::conv::to_utf<char>(task_data.ActionTime, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["ActionLocalID"] = boost::locale::conv::to_utf<char>(task_data.ActionLocalID, std::string("GB2312"));
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnOptionSelfCloseAction(data, error);
};

void TdApi::processRtnCombAction(Task task)
{
	PyLock lock;
	CThostFtdcCombActionField task_data = any_cast<CThostFtdcCombActionField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["BranchID"] = boost::locale::conv::to_utf<char>(task_data.BranchID, std::string("GB2312"));
	data["SequenceNo"] = task_data.SequenceNo;
	data["SettlementID"] = task_data.SettlementID;
	data["CombActionRef"] = boost::locale::conv::to_utf<char>(task_data.CombActionRef, std::string("GB2312"));
	data["Volume"] = task_data.Volume;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["NotifySequence"] = task_data.NotifySequence;
	data["TraderID"] = boost::locale::conv::to_utf<char>(task_data.TraderID, std::string("GB2312"));
	data["UserProductInfo"] = boost::locale::conv::to_utf<char>(task_data.UserProductInfo, std::string("GB2312"));
	data["ExchangeInstID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeInstID, std::string("GB2312"));
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["ComTradeID"] = boost::locale::conv::to_utf<char>(task_data.ComTradeID, std::string("GB2312"));
	data["Direction"] = task_data.Direction;
	data["SessionID"] = task_data.SessionID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["ActionStatus"] = task_data.ActionStatus;
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["CombDirection"] = task_data.CombDirection;
	data["ActionLocalID"] = boost::locale::conv::to_utf<char>(task_data.ActionLocalID, std::string("GB2312"));
	data["StatusMsg"] = boost::locale::conv::to_utf<char>(task_data.StatusMsg, std::string("GB2312"));
	data["ParticipantID"] = boost::locale::conv::to_utf<char>(task_data.ParticipantID, std::string("GB2312"));

	this->onRtnCombAction(data);
};

void TdApi::processErrRtnCombActionInsert(Task task)
{
	PyLock lock;
	CThostFtdcInputCombActionField task_data = any_cast<CThostFtdcInputCombActionField>(task.task_data);
	dict data;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["Direction"] = task_data.Direction;
	data["Volume"] = task_data.Volume;
	data["CombActionRef"] = boost::locale::conv::to_utf<char>(task_data.CombActionRef, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["HedgeFlag"] = task_data.HedgeFlag;
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["CombDirection"] = task_data.CombDirection;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnCombActionInsert(data, error);
};

void TdApi::processRspQryContractBank(Task task)
{
	PyLock lock;
	CThostFtdcContractBankField task_data = any_cast<CThostFtdcContractBankField>(task.task_data);
	dict data;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["BankBrchID"] = boost::locale::conv::to_utf<char>(task_data.BankBrchID, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["BankName"] = boost::locale::conv::to_utf<char>(task_data.BankName, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryContractBank(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryParkedOrder(Task task)
{
	PyLock lock;
	CThostFtdcParkedOrderField task_data = any_cast<CThostFtdcParkedOrderField>(task.task_data);
	dict data;
	data["StopPrice"] = task_data.StopPrice;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["TimeCondition"] = task_data.TimeCondition;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["Status"] = task_data.Status;
	data["ParkedOrderID"] = boost::locale::conv::to_utf<char>(task_data.ParkedOrderID, std::string("GB2312"));
	data["CombHedgeFlag"] = boost::locale::conv::to_utf<char>(task_data.CombHedgeFlag, std::string("GB2312"));
	data["ContingentCondition"] = task_data.ContingentCondition;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["GTDDate"] = boost::locale::conv::to_utf<char>(task_data.GTDDate, std::string("GB2312"));
	data["IsAutoSuspend"] = task_data.IsAutoSuspend;
	data["VolumeCondition"] = task_data.VolumeCondition;
	data["ClientID"] = boost::locale::conv::to_utf<char>(task_data.ClientID, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["LimitPrice"] = task_data.LimitPrice;
	data["Direction"] = task_data.Direction;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["CombOffsetFlag"] = boost::locale::conv::to_utf<char>(task_data.CombOffsetFlag, std::string("GB2312"));
	data["IsSwapOrder"] = task_data.IsSwapOrder;
	data["OrderRef"] = boost::locale::conv::to_utf<char>(task_data.OrderRef, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["UserType"] = task_data.UserType;
	data["BusinessUnit"] = boost::locale::conv::to_utf<char>(task_data.BusinessUnit, std::string("GB2312"));
	data["VolumeTotalOriginal"] = task_data.VolumeTotalOriginal;
	data["MinVolume"] = task_data.MinVolume;
	data["OrderPriceType"] = task_data.OrderPriceType;
	data["UserForceClose"] = task_data.UserForceClose;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["ForceCloseReason"] = task_data.ForceCloseReason;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryParkedOrder(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryParkedOrderAction(Task task)
{
	PyLock lock;
	CThostFtdcParkedOrderActionField task_data = any_cast<CThostFtdcParkedOrderActionField>(task.task_data);
	dict data;
	data["OrderSysID"] = boost::locale::conv::to_utf<char>(task_data.OrderSysID, std::string("GB2312"));
	data["FrontID"] = task_data.FrontID;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["Status"] = task_data.Status;
	data["MacAddress"] = boost::locale::conv::to_utf<char>(task_data.MacAddress, std::string("GB2312"));
	data["ActionFlag"] = task_data.ActionFlag;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["UserType"] = task_data.UserType;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));
	data["ParkedOrderActionID"] = boost::locale::conv::to_utf<char>(task_data.ParkedOrderActionID, std::string("GB2312"));
	data["VolumeChange"] = task_data.VolumeChange;
	data["LimitPrice"] = task_data.LimitPrice;
	data["RequestID"] = task_data.RequestID;
	data["OrderRef"] = boost::locale::conv::to_utf<char>(task_data.OrderRef, std::string("GB2312"));
	data["IPAddress"] = boost::locale::conv::to_utf<char>(task_data.IPAddress, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["OrderActionRef"] = task_data.OrderActionRef;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryParkedOrderAction(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryTradingNotice(Task task)
{
	PyLock lock;
	CThostFtdcTradingNoticeField task_data = any_cast<CThostFtdcTradingNoticeField>(task.task_data);
	dict data;
	data["FieldContent"] = boost::locale::conv::to_utf<char>(task_data.FieldContent, std::string("GB2312"));
	data["SendTime"] = boost::locale::conv::to_utf<char>(task_data.SendTime, std::string("GB2312"));
	data["SequenceSeries"] = task_data.SequenceSeries;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));
	data["SequenceNo"] = task_data.SequenceNo;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["InvestorRange"] = task_data.InvestorRange;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryTradingNotice(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryBrokerTradingParams(Task task)
{
	PyLock lock;
	CThostFtdcBrokerTradingParamsField task_data = any_cast<CThostFtdcBrokerTradingParamsField>(task.task_data);
	dict data;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["OptionRoyaltyPriceType"] = task_data.OptionRoyaltyPriceType;
	data["AvailIncludeCloseProfit"] = task_data.AvailIncludeCloseProfit;
	data["Algorithm"] = task_data.Algorithm;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["MarginPriceType"] = task_data.MarginPriceType;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryBrokerTradingParams(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQryBrokerTradingAlgos(Task task)
{
	PyLock lock;
	CThostFtdcBrokerTradingAlgosField task_data = any_cast<CThostFtdcBrokerTradingAlgosField>(task.task_data);
	dict data;
	data["HandlePositionAlgoID"] = task_data.HandlePositionAlgoID;
	data["InstrumentID"] = boost::locale::conv::to_utf<char>(task_data.InstrumentID, std::string("GB2312"));
	data["FindMarginRateAlgoID"] = task_data.FindMarginRateAlgoID;
	data["HandleTradingAccountAlgoID"] = task_data.HandleTradingAccountAlgoID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ExchangeID"] = boost::locale::conv::to_utf<char>(task_data.ExchangeID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQryBrokerTradingAlgos(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQueryCFMMCTradingAccountToken(Task task)
{
	PyLock lock;
	CThostFtdcQueryCFMMCTradingAccountTokenField task_data = any_cast<CThostFtdcQueryCFMMCTradingAccountTokenField>(task.task_data);
	dict data;
	data["InvestorID"] = boost::locale::conv::to_utf<char>(task_data.InvestorID, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["InvestUnitID"] = boost::locale::conv::to_utf<char>(task_data.InvestUnitID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQueryCFMMCTradingAccountToken(data, error, task.task_id, task.task_last);
};

void TdApi::processRtnFromBankToFutureByBank(Task task)
{
	PyLock lock;
	CThostFtdcRspTransferField task_data = any_cast<CThostFtdcRspTransferField>(task.task_data);
	dict data;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["PlateSerial"] = task_data.PlateSerial;
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["CustType"] = task_data.CustType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));

	this->onRtnFromBankToFutureByBank(data);
};

void TdApi::processRtnFromFutureToBankByBank(Task task)
{
	PyLock lock;
	CThostFtdcRspTransferField task_data = any_cast<CThostFtdcRspTransferField>(task.task_data);
	dict data;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["PlateSerial"] = task_data.PlateSerial;
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["CustType"] = task_data.CustType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));

	this->onRtnFromFutureToBankByBank(data);
};

void TdApi::processRtnRepealFromBankToFutureByBank(Task task)
{
	PyLock lock;
	CThostFtdcRspRepealField task_data = any_cast<CThostFtdcRspRepealField>(task.task_data);
	dict data;
	data["PlateRepealSerial"] = task_data.PlateRepealSerial;
	data["BankRepealFlag"] = task_data.BankRepealFlag;
	data["RepealTimeInterval"] = task_data.RepealTimeInterval;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["CustType"] = task_data.CustType;
	data["PlateSerial"] = task_data.PlateSerial;
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["FutureRepealSerial"] = task_data.FutureRepealSerial;
	data["RequestID"] = task_data.RequestID;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["BrokerRepealFlag"] = task_data.BrokerRepealFlag;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["RepealedTimes"] = task_data.RepealedTimes;
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankRepealSerial"] = boost::locale::conv::to_utf<char>(task_data.BankRepealSerial, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;

	this->onRtnRepealFromBankToFutureByBank(data);
};

void TdApi::processRtnRepealFromFutureToBankByBank(Task task)
{
	PyLock lock;
	CThostFtdcRspRepealField task_data = any_cast<CThostFtdcRspRepealField>(task.task_data);
	dict data;
	data["PlateRepealSerial"] = task_data.PlateRepealSerial;
	data["BankRepealFlag"] = task_data.BankRepealFlag;
	data["RepealTimeInterval"] = task_data.RepealTimeInterval;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["CustType"] = task_data.CustType;
	data["PlateSerial"] = task_data.PlateSerial;
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["FutureRepealSerial"] = task_data.FutureRepealSerial;
	data["RequestID"] = task_data.RequestID;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["BrokerRepealFlag"] = task_data.BrokerRepealFlag;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["RepealedTimes"] = task_data.RepealedTimes;
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankRepealSerial"] = boost::locale::conv::to_utf<char>(task_data.BankRepealSerial, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;

	this->onRtnRepealFromFutureToBankByBank(data);
};

void TdApi::processRtnFromBankToFutureByFuture(Task task)
{
	PyLock lock;
	CThostFtdcRspTransferField task_data = any_cast<CThostFtdcRspTransferField>(task.task_data);
	dict data;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["PlateSerial"] = task_data.PlateSerial;
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["CustType"] = task_data.CustType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));

	this->onRtnFromBankToFutureByFuture(data);
};

void TdApi::processRtnFromFutureToBankByFuture(Task task)
{
	PyLock lock;
	CThostFtdcRspTransferField task_data = any_cast<CThostFtdcRspTransferField>(task.task_data);
	dict data;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["PlateSerial"] = task_data.PlateSerial;
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["CustType"] = task_data.CustType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));

	this->onRtnFromFutureToBankByFuture(data);
};

void TdApi::processRtnRepealFromBankToFutureByFutureManual(Task task)
{
	PyLock lock;
	CThostFtdcRspRepealField task_data = any_cast<CThostFtdcRspRepealField>(task.task_data);
	dict data;
	data["PlateRepealSerial"] = task_data.PlateRepealSerial;
	data["BankRepealFlag"] = task_data.BankRepealFlag;
	data["RepealTimeInterval"] = task_data.RepealTimeInterval;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["CustType"] = task_data.CustType;
	data["PlateSerial"] = task_data.PlateSerial;
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["FutureRepealSerial"] = task_data.FutureRepealSerial;
	data["RequestID"] = task_data.RequestID;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["BrokerRepealFlag"] = task_data.BrokerRepealFlag;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["RepealedTimes"] = task_data.RepealedTimes;
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankRepealSerial"] = boost::locale::conv::to_utf<char>(task_data.BankRepealSerial, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;

	this->onRtnRepealFromBankToFutureByFutureManual(data);
};

void TdApi::processRtnRepealFromFutureToBankByFutureManual(Task task)
{
	PyLock lock;
	CThostFtdcRspRepealField task_data = any_cast<CThostFtdcRspRepealField>(task.task_data);
	dict data;
	data["PlateRepealSerial"] = task_data.PlateRepealSerial;
	data["BankRepealFlag"] = task_data.BankRepealFlag;
	data["RepealTimeInterval"] = task_data.RepealTimeInterval;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["CustType"] = task_data.CustType;
	data["PlateSerial"] = task_data.PlateSerial;
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["FutureRepealSerial"] = task_data.FutureRepealSerial;
	data["RequestID"] = task_data.RequestID;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["BrokerRepealFlag"] = task_data.BrokerRepealFlag;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["RepealedTimes"] = task_data.RepealedTimes;
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankRepealSerial"] = boost::locale::conv::to_utf<char>(task_data.BankRepealSerial, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;

	this->onRtnRepealFromFutureToBankByFutureManual(data);
};

void TdApi::processRtnQueryBankBalanceByFuture(Task task)
{
	PyLock lock;
	CThostFtdcNotifyQueryAccountField task_data = any_cast<CThostFtdcNotifyQueryAccountField>(task.task_data);
	dict data;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["LastFragment"] = task_data.LastFragment;
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["BankFetchAmount"] = task_data.BankFetchAmount;
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["CustType"] = task_data.CustType;
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["PlateSerial"] = task_data.PlateSerial;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["TID"] = task_data.TID;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["SessionID"] = task_data.SessionID;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["BankUseAmount"] = task_data.BankUseAmount;
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["IdCardType"] = task_data.IdCardType;
	data["InstallID"] = task_data.InstallID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;

	this->onRtnQueryBankBalanceByFuture(data);
};

void TdApi::processErrRtnBankToFutureByFuture(Task task)
{
	PyLock lock;
	CThostFtdcReqTransferField task_data = any_cast<CThostFtdcReqTransferField>(task.task_data);
	dict data;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["PlateSerial"] = task_data.PlateSerial;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["CustType"] = task_data.CustType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnBankToFutureByFuture(data, error);
};

void TdApi::processErrRtnFutureToBankByFuture(Task task)
{
	PyLock lock;
	CThostFtdcReqTransferField task_data = any_cast<CThostFtdcReqTransferField>(task.task_data);
	dict data;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["PlateSerial"] = task_data.PlateSerial;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["CustType"] = task_data.CustType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnFutureToBankByFuture(data, error);
};

void TdApi::processErrRtnRepealBankToFutureByFutureManual(Task task)
{
	PyLock lock;
	CThostFtdcReqRepealField task_data = any_cast<CThostFtdcReqRepealField>(task.task_data);
	dict data;
	data["PlateRepealSerial"] = task_data.PlateRepealSerial;
	data["BankRepealFlag"] = task_data.BankRepealFlag;
	data["RepealTimeInterval"] = task_data.RepealTimeInterval;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["CustType"] = task_data.CustType;
	data["PlateSerial"] = task_data.PlateSerial;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["FutureRepealSerial"] = task_data.FutureRepealSerial;
	data["RequestID"] = task_data.RequestID;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["BrokerRepealFlag"] = task_data.BrokerRepealFlag;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["RepealedTimes"] = task_data.RepealedTimes;
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankRepealSerial"] = boost::locale::conv::to_utf<char>(task_data.BankRepealSerial, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnRepealBankToFutureByFutureManual(data, error);
};

void TdApi::processErrRtnRepealFutureToBankByFutureManual(Task task)
{
	PyLock lock;
	CThostFtdcReqRepealField task_data = any_cast<CThostFtdcReqRepealField>(task.task_data);
	dict data;
	data["PlateRepealSerial"] = task_data.PlateRepealSerial;
	data["BankRepealFlag"] = task_data.BankRepealFlag;
	data["RepealTimeInterval"] = task_data.RepealTimeInterval;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["CustType"] = task_data.CustType;
	data["PlateSerial"] = task_data.PlateSerial;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["FutureRepealSerial"] = task_data.FutureRepealSerial;
	data["RequestID"] = task_data.RequestID;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["BrokerRepealFlag"] = task_data.BrokerRepealFlag;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["RepealedTimes"] = task_data.RepealedTimes;
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankRepealSerial"] = boost::locale::conv::to_utf<char>(task_data.BankRepealSerial, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnRepealFutureToBankByFutureManual(data, error);
};

void TdApi::processErrRtnQueryBankBalanceByFuture(Task task)
{
	PyLock lock;
	CThostFtdcReqQueryAccountField task_data = any_cast<CThostFtdcReqQueryAccountField>(task.task_data);
	dict data;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["LastFragment"] = task_data.LastFragment;
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["CustType"] = task_data.CustType;
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["PlateSerial"] = task_data.PlateSerial;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["TID"] = task_data.TID;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["SessionID"] = task_data.SessionID;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["IdCardType"] = task_data.IdCardType;
	data["InstallID"] = task_data.InstallID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onErrRtnQueryBankBalanceByFuture(data, error);
};

void TdApi::processRtnRepealFromBankToFutureByFuture(Task task)
{
	PyLock lock;
	CThostFtdcRspRepealField task_data = any_cast<CThostFtdcRspRepealField>(task.task_data);
	dict data;
	data["PlateRepealSerial"] = task_data.PlateRepealSerial;
	data["BankRepealFlag"] = task_data.BankRepealFlag;
	data["RepealTimeInterval"] = task_data.RepealTimeInterval;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["CustType"] = task_data.CustType;
	data["PlateSerial"] = task_data.PlateSerial;
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["FutureRepealSerial"] = task_data.FutureRepealSerial;
	data["RequestID"] = task_data.RequestID;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["BrokerRepealFlag"] = task_data.BrokerRepealFlag;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["RepealedTimes"] = task_data.RepealedTimes;
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankRepealSerial"] = boost::locale::conv::to_utf<char>(task_data.BankRepealSerial, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;

	this->onRtnRepealFromBankToFutureByFuture(data);
};

void TdApi::processRtnRepealFromFutureToBankByFuture(Task task)
{
	PyLock lock;
	CThostFtdcRspRepealField task_data = any_cast<CThostFtdcRspRepealField>(task.task_data);
	dict data;
	data["PlateRepealSerial"] = task_data.PlateRepealSerial;
	data["BankRepealFlag"] = task_data.BankRepealFlag;
	data["RepealTimeInterval"] = task_data.RepealTimeInterval;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["CustType"] = task_data.CustType;
	data["PlateSerial"] = task_data.PlateSerial;
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["FutureRepealSerial"] = task_data.FutureRepealSerial;
	data["RequestID"] = task_data.RequestID;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["BrokerRepealFlag"] = task_data.BrokerRepealFlag;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["RepealedTimes"] = task_data.RepealedTimes;
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankRepealSerial"] = boost::locale::conv::to_utf<char>(task_data.BankRepealSerial, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;

	this->onRtnRepealFromFutureToBankByFuture(data);
};

void TdApi::processRspFromBankToFutureByFuture(Task task)
{
	PyLock lock;
	CThostFtdcReqTransferField task_data = any_cast<CThostFtdcReqTransferField>(task.task_data);
	dict data;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["PlateSerial"] = task_data.PlateSerial;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["CustType"] = task_data.CustType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspFromBankToFutureByFuture(data, error, task.task_id, task.task_last);
};

void TdApi::processRspFromFutureToBankByFuture(Task task)
{
	PyLock lock;
	CThostFtdcReqTransferField task_data = any_cast<CThostFtdcReqTransferField>(task.task_data);
	dict data;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["PlateSerial"] = task_data.PlateSerial;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["FeePayFlag"] = task_data.FeePayFlag;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["LastFragment"] = task_data.LastFragment;
	data["FutureFetchAmount"] = task_data.FutureFetchAmount;
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["CustFee"] = task_data.CustFee;
	data["TID"] = task_data.TID;
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["TradeAmount"] = task_data.TradeAmount;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["Message"] = boost::locale::conv::to_utf<char>(task_data.Message, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["TransferStatus"] = task_data.TransferStatus;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["CustType"] = task_data.CustType;
	data["BrokerFee"] = task_data.BrokerFee;
	data["SessionID"] = task_data.SessionID;
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspFromFutureToBankByFuture(data, error, task.task_id, task.task_last);
};

void TdApi::processRspQueryBankAccountMoneyByFuture(Task task)
{
	PyLock lock;
	CThostFtdcReqQueryAccountField task_data = any_cast<CThostFtdcReqQueryAccountField>(task.task_data);
	dict data;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["LastFragment"] = task_data.LastFragment;
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["CustType"] = task_data.CustType;
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["PlateSerial"] = task_data.PlateSerial;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["TID"] = task_data.TID;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["FutureSerial"] = task_data.FutureSerial;
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["RequestID"] = task_data.RequestID;
	data["SessionID"] = task_data.SessionID;
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["IdCardType"] = task_data.IdCardType;
	data["InstallID"] = task_data.InstallID;
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));

	CThostFtdcRspInfoField task_error = any_cast<CThostFtdcRspInfoField>(task.task_error);
	dict error;
	error["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_error.ErrorMsg, std::string("GB2312"));
	error["ErrorID"] = task_error.ErrorID;

	this->onRspQueryBankAccountMoneyByFuture(data, error, task.task_id, task.task_last);
};

void TdApi::processRtnOpenAccountByBank(Task task)
{
	PyLock lock;
	CThostFtdcOpenAccountField task_data = any_cast<CThostFtdcOpenAccountField>(task.task_data);
	dict data;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["PlateSerial"] = task_data.PlateSerial;
	data["CountryCode"] = boost::locale::conv::to_utf<char>(task_data.CountryCode, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["ZipCode"] = boost::locale::conv::to_utf<char>(task_data.ZipCode, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["Address"] = boost::locale::conv::to_utf<char>(task_data.Address, std::string("GB2312"));
	data["Gender"] = task_data.Gender;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["LastFragment"] = task_data.LastFragment;
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["TID"] = task_data.TID;
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["Fax"] = boost::locale::conv::to_utf<char>(task_data.Fax, std::string("GB2312"));
	data["Telephone"] = boost::locale::conv::to_utf<char>(task_data.Telephone, std::string("GB2312"));
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["EMail"] = boost::locale::conv::to_utf<char>(task_data.EMail, std::string("GB2312"));
	data["CashExchangeCode"] = task_data.CashExchangeCode;
	data["CustType"] = task_data.CustType;
	data["MoneyAccountStatus"] = task_data.MoneyAccountStatus;
	data["MobilePhone"] = boost::locale::conv::to_utf<char>(task_data.MobilePhone, std::string("GB2312"));

	this->onRtnOpenAccountByBank(data);
};

void TdApi::processRtnCancelAccountByBank(Task task)
{
	PyLock lock;
	CThostFtdcCancelAccountField task_data = any_cast<CThostFtdcCancelAccountField>(task.task_data);
	dict data;
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["PlateSerial"] = task_data.PlateSerial;
	data["CountryCode"] = boost::locale::conv::to_utf<char>(task_data.CountryCode, std::string("GB2312"));
	data["UserID"] = boost::locale::conv::to_utf<char>(task_data.UserID, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["BankSecuAcc"] = boost::locale::conv::to_utf<char>(task_data.BankSecuAcc, std::string("GB2312"));
	data["OperNo"] = boost::locale::conv::to_utf<char>(task_data.OperNo, std::string("GB2312"));
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["DeviceID"] = boost::locale::conv::to_utf<char>(task_data.DeviceID, std::string("GB2312"));
	data["ZipCode"] = boost::locale::conv::to_utf<char>(task_data.ZipCode, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["Address"] = boost::locale::conv::to_utf<char>(task_data.Address, std::string("GB2312"));
	data["Gender"] = task_data.Gender;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["LastFragment"] = task_data.LastFragment;
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["SessionID"] = task_data.SessionID;
	data["TID"] = task_data.TID;
	data["IdCardType"] = task_data.IdCardType;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["Fax"] = boost::locale::conv::to_utf<char>(task_data.Fax, std::string("GB2312"));
	data["Telephone"] = boost::locale::conv::to_utf<char>(task_data.Telephone, std::string("GB2312"));
	data["BankSecuAccType"] = task_data.BankSecuAccType;
	data["EMail"] = boost::locale::conv::to_utf<char>(task_data.EMail, std::string("GB2312"));
	data["CashExchangeCode"] = task_data.CashExchangeCode;
	data["CustType"] = task_data.CustType;
	data["MoneyAccountStatus"] = task_data.MoneyAccountStatus;
	data["MobilePhone"] = boost::locale::conv::to_utf<char>(task_data.MobilePhone, std::string("GB2312"));

	this->onRtnCancelAccountByBank(data);
};

void TdApi::processRtnChangeAccountByBank(Task task)
{
	PyLock lock;
	CThostFtdcChangeAccountField task_data = any_cast<CThostFtdcChangeAccountField>(task.task_data);
	dict data;
	data["Gender"] = task_data.Gender;
	data["Digest"] = boost::locale::conv::to_utf<char>(task_data.Digest, std::string("GB2312"));
	data["LastFragment"] = task_data.LastFragment;
	data["ErrorMsg"] = boost::locale::conv::to_utf<char>(task_data.ErrorMsg, std::string("GB2312"));
	data["LongCustomerName"] = boost::locale::conv::to_utf<char>(task_data.LongCustomerName, std::string("GB2312"));
	data["SecuPwdFlag"] = task_data.SecuPwdFlag;
	data["Password"] = boost::locale::conv::to_utf<char>(task_data.Password, std::string("GB2312"));
	data["EMail"] = boost::locale::conv::to_utf<char>(task_data.EMail, std::string("GB2312"));
	data["BrokerBranchID"] = boost::locale::conv::to_utf<char>(task_data.BrokerBranchID, std::string("GB2312"));
	data["BankAccType"] = task_data.BankAccType;
	data["TradeTime"] = boost::locale::conv::to_utf<char>(task_data.TradeTime, std::string("GB2312"));
	data["IdentifiedCardNo"] = boost::locale::conv::to_utf<char>(task_data.IdentifiedCardNo, std::string("GB2312"));
	data["CustType"] = task_data.CustType;
	data["PlateSerial"] = task_data.PlateSerial;
	data["BrokerIDByBank"] = boost::locale::conv::to_utf<char>(task_data.BrokerIDByBank, std::string("GB2312"));
	data["BankPwdFlag"] = task_data.BankPwdFlag;
	data["CountryCode"] = boost::locale::conv::to_utf<char>(task_data.CountryCode, std::string("GB2312"));
	data["VerifyCertNoFlag"] = task_data.VerifyCertNoFlag;
	data["BankPassWord"] = boost::locale::conv::to_utf<char>(task_data.BankPassWord, std::string("GB2312"));
	data["CustomerName"] = boost::locale::conv::to_utf<char>(task_data.CustomerName, std::string("GB2312"));
	data["CurrencyID"] = boost::locale::conv::to_utf<char>(task_data.CurrencyID, std::string("GB2312"));
	data["BankAccount"] = boost::locale::conv::to_utf<char>(task_data.BankAccount, std::string("GB2312"));
	data["InstallID"] = task_data.InstallID;
	data["BankBranchID"] = boost::locale::conv::to_utf<char>(task_data.BankBranchID, std::string("GB2312"));
	data["BankID"] = boost::locale::conv::to_utf<char>(task_data.BankID, std::string("GB2312"));
	data["TradeCode"] = boost::locale::conv::to_utf<char>(task_data.TradeCode, std::string("GB2312"));
	data["BankSerial"] = boost::locale::conv::to_utf<char>(task_data.BankSerial, std::string("GB2312"));
	data["Fax"] = boost::locale::conv::to_utf<char>(task_data.Fax, std::string("GB2312"));
	data["NewBankAccount"] = boost::locale::conv::to_utf<char>(task_data.NewBankAccount, std::string("GB2312"));
	data["TradeDate"] = boost::locale::conv::to_utf<char>(task_data.TradeDate, std::string("GB2312"));
	data["MobilePhone"] = boost::locale::conv::to_utf<char>(task_data.MobilePhone, std::string("GB2312"));
	data["TID"] = task_data.TID;
	data["SessionID"] = task_data.SessionID;
	data["ZipCode"] = boost::locale::conv::to_utf<char>(task_data.ZipCode, std::string("GB2312"));
	data["AccountID"] = boost::locale::conv::to_utf<char>(task_data.AccountID, std::string("GB2312"));
	data["Address"] = boost::locale::conv::to_utf<char>(task_data.Address, std::string("GB2312"));
	data["TradingDay"] = boost::locale::conv::to_utf<char>(task_data.TradingDay, std::string("GB2312"));
	data["IdCardType"] = task_data.IdCardType;
	data["NewBankPassWord"] = boost::locale::conv::to_utf<char>(task_data.NewBankPassWord, std::string("GB2312"));
	data["BrokerID"] = boost::locale::conv::to_utf<char>(task_data.BrokerID, std::string("GB2312"));
	data["Telephone"] = boost::locale::conv::to_utf<char>(task_data.Telephone, std::string("GB2312"));
	data["ErrorID"] = task_data.ErrorID;
	data["MoneyAccountStatus"] = task_data.MoneyAccountStatus;

	this->onRtnChangeAccountByBank(data);
};


// render_data.ctp_td_process end


///-------------------------------------------------------------------------------------
///主动函数
///-------------------------------------------------------------------------------------

void TdApi::createFtdcTraderApi(string pszFlowPath)
{
	this->api = CThostFtdcTraderApi::CreateFtdcTraderApi(pszFlowPath.c_str());
	this->api->RegisterSpi(this);
};

void TdApi::release()
{
	this->api->Release();
};

void TdApi::init()
{
	this->api->Init();
};

int TdApi::join()
{
	int i = this->api->Join();
	return i;
};

int TdApi::exit()
{
	//该函数在原生API里没有，用于安全退出API用，原生的join似乎不太稳定
	this->api->RegisterSpi(NULL);
	this->api->Release();
	this->api = NULL;
	return 1;
};

string TdApi::getTradingDay()
{
	string day = this->api->GetTradingDay();
	return day;
};

void TdApi::registerFront(string pszFrontAddress)
{
	this->api->RegisterFront((char*)pszFrontAddress.c_str());
};

void TdApi::subscribePrivateTopic(int nType)
{
	//该函数为手动编写
	THOST_TE_RESUME_TYPE type;

	switch (nType)
	{
	case 0:
	{
		type = THOST_TERT_RESTART;
		break;
	};

	case 1:
	{
		type = THOST_TERT_RESUME;
		break;
	};

	case 2:
	{
		type = THOST_TERT_QUICK;
		break;
	};
	}

	this->api->SubscribePrivateTopic(type);
};

void TdApi::subscribePublicTopic(int nType)
{
	//该函数为手动编写
	THOST_TE_RESUME_TYPE type;

	switch (nType)
	{
	case 0:
	{
		type = THOST_TERT_RESTART;
		break;
	};

	case 1:
	{
		type = THOST_TERT_RESUME;
		break;
	};

	case 2:
	{
		type = THOST_TERT_QUICK;
		break;
	};
	}

	this->api->SubscribePublicTopic(type);
};

// render_data.ctp_td_function start
int TdApi::reqAuthenticate(dict req, int nRequestID)
{
	CThostFtdcReqAuthenticateField myreq = CThostFtdcReqAuthenticateField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "UserProductInfo", myreq.UserProductInfo);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "AuthCode", myreq.AuthCode);
	getStr(req, "UserID", myreq.UserID);
	int i = this->api->ReqAuthenticate(&myreq, nRequestID);
	return i;
};

int TdApi::reqUserLogin(dict req, int nRequestID)
{
	CThostFtdcReqUserLoginField myreq = CThostFtdcReqUserLoginField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ProtocolInfo", myreq.ProtocolInfo);
	getStr(req, "MacAddress", myreq.MacAddress);
	getStr(req, "LoginRemark", myreq.LoginRemark);
	getStr(req, "Password", myreq.Password);
	getStr(req, "TradingDay", myreq.TradingDay);
	getStr(req, "ClientIPAddress", myreq.ClientIPAddress);
	getStr(req, "UserProductInfo", myreq.UserProductInfo);
	getStr(req, "OneTimePassword", myreq.OneTimePassword);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "InterfaceProductInfo", myreq.InterfaceProductInfo);
	getStr(req, "UserID", myreq.UserID);
	int i = this->api->ReqUserLogin(&myreq, nRequestID);
	return i;
};

int TdApi::reqUserLogout(dict req, int nRequestID)
{
	CThostFtdcUserLogoutField myreq = CThostFtdcUserLogoutField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "UserID", myreq.UserID);
	int i = this->api->ReqUserLogout(&myreq, nRequestID);
	return i;
};

int TdApi::reqUserPasswordUpdate(dict req, int nRequestID)
{
	CThostFtdcUserPasswordUpdateField myreq = CThostFtdcUserPasswordUpdateField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "OldPassword", myreq.OldPassword);
	getStr(req, "NewPassword", myreq.NewPassword);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "UserID", myreq.UserID);
	int i = this->api->ReqUserPasswordUpdate(&myreq, nRequestID);
	return i;
};

int TdApi::reqTradingAccountPasswordUpdate(dict req, int nRequestID)
{
	CThostFtdcTradingAccountPasswordUpdateField myreq = CThostFtdcTradingAccountPasswordUpdateField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "OldPassword", myreq.OldPassword);
	getStr(req, "NewPassword", myreq.NewPassword);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "AccountID", myreq.AccountID);
	getStr(req, "CurrencyID", myreq.CurrencyID);
	int i = this->api->ReqTradingAccountPasswordUpdate(&myreq, nRequestID);
	return i;
};

int TdApi::reqUserLogin2(dict req, int nRequestID)
{
	CThostFtdcReqUserLoginField myreq = CThostFtdcReqUserLoginField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ProtocolInfo", myreq.ProtocolInfo);
	getStr(req, "MacAddress", myreq.MacAddress);
	getStr(req, "LoginRemark", myreq.LoginRemark);
	getStr(req, "Password", myreq.Password);
	getStr(req, "TradingDay", myreq.TradingDay);
	getStr(req, "ClientIPAddress", myreq.ClientIPAddress);
	getStr(req, "UserProductInfo", myreq.UserProductInfo);
	getStr(req, "OneTimePassword", myreq.OneTimePassword);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "InterfaceProductInfo", myreq.InterfaceProductInfo);
	getStr(req, "UserID", myreq.UserID);
	int i = this->api->ReqUserLogin2(&myreq, nRequestID);
	return i;
};

int TdApi::reqUserPasswordUpdate2(dict req, int nRequestID)
{
	CThostFtdcUserPasswordUpdateField myreq = CThostFtdcUserPasswordUpdateField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "OldPassword", myreq.OldPassword);
	getStr(req, "NewPassword", myreq.NewPassword);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "UserID", myreq.UserID);
	int i = this->api->ReqUserPasswordUpdate2(&myreq, nRequestID);
	return i;
};

int TdApi::reqOrderInsert(dict req, int nRequestID)
{
	CThostFtdcInputOrderField myreq = CThostFtdcInputOrderField();
	memset(&myreq, 0, sizeof(myreq));
	getDouble(req, "StopPrice", &myreq.StopPrice);
	getStr(req, "MacAddress", myreq.MacAddress);
	getChar(req, "TimeCondition", &myreq.TimeCondition);
	getStr(req, "InvestorID", myreq.InvestorID);
	getInt(req, "IsSwapOrder", &myreq.IsSwapOrder);
	getStr(req, "CombHedgeFlag", myreq.CombHedgeFlag);
	getChar(req, "ContingentCondition", &myreq.ContingentCondition);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "GTDDate", myreq.GTDDate);
	getInt(req, "IsAutoSuspend", &myreq.IsAutoSuspend);
	getChar(req, "VolumeCondition", &myreq.VolumeCondition);
	getStr(req, "ClientID", myreq.ClientID);
	getStr(req, "UserID", myreq.UserID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getDouble(req, "LimitPrice", &myreq.LimitPrice);
	getChar(req, "Direction", &myreq.Direction);
	getStr(req, "AccountID", myreq.AccountID);
	getInt(req, "RequestID", &myreq.RequestID);
	getStr(req, "CombOffsetFlag", myreq.CombOffsetFlag);
	getStr(req, "OrderRef", myreq.OrderRef);
	getStr(req, "IPAddress", myreq.IPAddress);
	getStr(req, "BusinessUnit", myreq.BusinessUnit);
	getInt(req, "VolumeTotalOriginal", &myreq.VolumeTotalOriginal);
	getInt(req, "MinVolume", &myreq.MinVolume);
	getChar(req, "OrderPriceType", &myreq.OrderPriceType);
	getInt(req, "UserForceClose", &myreq.UserForceClose);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getChar(req, "ForceCloseReason", &myreq.ForceCloseReason);
	int i = this->api->ReqOrderInsert(&myreq, nRequestID);
	return i;
};

int TdApi::reqParkedOrderInsert(dict req, int nRequestID)
{
	CThostFtdcParkedOrderField myreq = CThostFtdcParkedOrderField();
	memset(&myreq, 0, sizeof(myreq));
	getDouble(req, "StopPrice", &myreq.StopPrice);
	getStr(req, "MacAddress", myreq.MacAddress);
	getChar(req, "TimeCondition", &myreq.TimeCondition);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "ErrorMsg", myreq.ErrorMsg);
	getChar(req, "Status", &myreq.Status);
	getStr(req, "ParkedOrderID", myreq.ParkedOrderID);
	getStr(req, "CombHedgeFlag", myreq.CombHedgeFlag);
	getChar(req, "ContingentCondition", &myreq.ContingentCondition);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "GTDDate", myreq.GTDDate);
	getInt(req, "IsAutoSuspend", &myreq.IsAutoSuspend);
	getChar(req, "VolumeCondition", &myreq.VolumeCondition);
	getStr(req, "ClientID", myreq.ClientID);
	getStr(req, "UserID", myreq.UserID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getDouble(req, "LimitPrice", &myreq.LimitPrice);
	getChar(req, "Direction", &myreq.Direction);
	getStr(req, "AccountID", myreq.AccountID);
	getInt(req, "RequestID", &myreq.RequestID);
	getStr(req, "CombOffsetFlag", myreq.CombOffsetFlag);
	getInt(req, "IsSwapOrder", &myreq.IsSwapOrder);
	getStr(req, "OrderRef", myreq.OrderRef);
	getStr(req, "IPAddress", myreq.IPAddress);
	getChar(req, "UserType", &myreq.UserType);
	getStr(req, "BusinessUnit", myreq.BusinessUnit);
	getInt(req, "VolumeTotalOriginal", &myreq.VolumeTotalOriginal);
	getInt(req, "MinVolume", &myreq.MinVolume);
	getChar(req, "OrderPriceType", &myreq.OrderPriceType);
	getInt(req, "UserForceClose", &myreq.UserForceClose);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getInt(req, "ErrorID", &myreq.ErrorID);
	getChar(req, "ForceCloseReason", &myreq.ForceCloseReason);
	int i = this->api->ReqParkedOrderInsert(&myreq, nRequestID);
	return i;
};

int TdApi::reqParkedOrderAction(dict req, int nRequestID)
{
	CThostFtdcParkedOrderActionField myreq = CThostFtdcParkedOrderActionField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "OrderSysID", myreq.OrderSysID);
	getInt(req, "FrontID", &myreq.FrontID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "ErrorMsg", myreq.ErrorMsg);
	getChar(req, "Status", &myreq.Status);
	getStr(req, "MacAddress", myreq.MacAddress);
	getChar(req, "ActionFlag", &myreq.ActionFlag);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getChar(req, "UserType", &myreq.UserType);
	getStr(req, "UserID", myreq.UserID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "ParkedOrderActionID", myreq.ParkedOrderActionID);
	getInt(req, "VolumeChange", &myreq.VolumeChange);
	getDouble(req, "LimitPrice", &myreq.LimitPrice);
	getInt(req, "RequestID", &myreq.RequestID);
	getStr(req, "OrderRef", myreq.OrderRef);
	getStr(req, "IPAddress", myreq.IPAddress);
	getInt(req, "SessionID", &myreq.SessionID);
	getInt(req, "OrderActionRef", &myreq.OrderActionRef);
	getStr(req, "BrokerID", myreq.BrokerID);
	getInt(req, "ErrorID", &myreq.ErrorID);
	int i = this->api->ReqParkedOrderAction(&myreq, nRequestID);
	return i;
};

int TdApi::reqOrderAction(dict req, int nRequestID)
{
	CThostFtdcInputOrderActionField myreq = CThostFtdcInputOrderActionField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "OrderSysID", myreq.OrderSysID);
	getInt(req, "FrontID", &myreq.FrontID);
	getDouble(req, "LimitPrice", &myreq.LimitPrice);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "MacAddress", myreq.MacAddress);
	getInt(req, "VolumeChange", &myreq.VolumeChange);
	getInt(req, "RequestID", &myreq.RequestID);
	getStr(req, "OrderRef", myreq.OrderRef);
	getChar(req, "ActionFlag", &myreq.ActionFlag);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getInt(req, "SessionID", &myreq.SessionID);
	getInt(req, "OrderActionRef", &myreq.OrderActionRef);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "IPAddress", myreq.IPAddress);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "UserID", myreq.UserID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	int i = this->api->ReqOrderAction(&myreq, nRequestID);
	return i;
};

int TdApi::reqQueryMaxOrderVolume(dict req, int nRequestID)
{
	CThostFtdcQueryMaxOrderVolumeField myreq = CThostFtdcQueryMaxOrderVolumeField();
	memset(&myreq, 0, sizeof(myreq));
	getChar(req, "HedgeFlag", &myreq.HedgeFlag);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getChar(req, "Direction", &myreq.Direction);
	getChar(req, "OffsetFlag", &myreq.OffsetFlag);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getInt(req, "MaxVolume", &myreq.MaxVolume);
	int i = this->api->ReqQueryMaxOrderVolume(&myreq, nRequestID);
	return i;
};

int TdApi::reqSettlementInfoConfirm(dict req, int nRequestID)
{
	CThostFtdcSettlementInfoConfirmField myreq = CThostFtdcSettlementInfoConfirmField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getStr(req, "ConfirmTime", myreq.ConfirmTime);
	getStr(req, "AccountID", myreq.AccountID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "ConfirmDate", myreq.ConfirmDate);
	getStr(req, "BrokerID", myreq.BrokerID);
	getInt(req, "SettlementID", &myreq.SettlementID);
	int i = this->api->ReqSettlementInfoConfirm(&myreq, nRequestID);
	return i;
};

int TdApi::reqRemoveParkedOrder(dict req, int nRequestID)
{
	CThostFtdcRemoveParkedOrderField myreq = CThostFtdcRemoveParkedOrderField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ParkedOrderID", myreq.ParkedOrderID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	int i = this->api->ReqRemoveParkedOrder(&myreq, nRequestID);
	return i;
};

int TdApi::reqRemoveParkedOrderAction(dict req, int nRequestID)
{
	CThostFtdcRemoveParkedOrderActionField myreq = CThostFtdcRemoveParkedOrderActionField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ParkedOrderActionID", myreq.ParkedOrderActionID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	int i = this->api->ReqRemoveParkedOrderAction(&myreq, nRequestID);
	return i;
};

int TdApi::reqExecOrderInsert(dict req, int nRequestID)
{
	CThostFtdcInputExecOrderField myreq = CThostFtdcInputExecOrderField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ExecOrderRef", myreq.ExecOrderRef);
	getChar(req, "ReservePositionFlag", &myreq.ReservePositionFlag);
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "MacAddress", myreq.MacAddress);
	getStr(req, "AccountID", myreq.AccountID);
	getInt(req, "RequestID", &myreq.RequestID);
	getStr(req, "ClientID", myreq.ClientID);
	getChar(req, "CloseFlag", &myreq.CloseFlag);
	getInt(req, "Volume", &myreq.Volume);
	getChar(req, "ActionType", &myreq.ActionType);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getChar(req, "HedgeFlag", &myreq.HedgeFlag);
	getStr(req, "BusinessUnit", myreq.BusinessUnit);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getChar(req, "OffsetFlag", &myreq.OffsetFlag);
	getChar(req, "PosiDirection", &myreq.PosiDirection);
	getStr(req, "IPAddress", myreq.IPAddress);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "UserID", myreq.UserID);
	int i = this->api->ReqExecOrderInsert(&myreq, nRequestID);
	return i;
};

int TdApi::reqExecOrderAction(dict req, int nRequestID)
{
	CThostFtdcInputExecOrderActionField myreq = CThostFtdcInputExecOrderActionField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ExecOrderRef", myreq.ExecOrderRef);
	getInt(req, "FrontID", &myreq.FrontID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "MacAddress", myreq.MacAddress);
	getStr(req, "ExecOrderSysID", myreq.ExecOrderSysID);
	getInt(req, "RequestID", &myreq.RequestID);
	getInt(req, "ExecOrderActionRef", &myreq.ExecOrderActionRef);
	getChar(req, "ActionFlag", &myreq.ActionFlag);
	getInt(req, "SessionID", &myreq.SessionID);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "IPAddress", myreq.IPAddress);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "UserID", myreq.UserID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	int i = this->api->ReqExecOrderAction(&myreq, nRequestID);
	return i;
};

int TdApi::reqForQuoteInsert(dict req, int nRequestID)
{
	CThostFtdcInputForQuoteField myreq = CThostFtdcInputForQuoteField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "MacAddress", myreq.MacAddress);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "ForQuoteRef", myreq.ForQuoteRef);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "UserID", myreq.UserID);
	getStr(req, "IPAddress", myreq.IPAddress);
	int i = this->api->ReqForQuoteInsert(&myreq, nRequestID);
	return i;
};

int TdApi::reqQuoteInsert(dict req, int nRequestID)
{
	CThostFtdcInputQuoteField myreq = CThostFtdcInputQuoteField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "MacAddress", myreq.MacAddress);
	getStr(req, "BidOrderRef", myreq.BidOrderRef);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getDouble(req, "AskPrice", &myreq.AskPrice);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "ClientID", myreq.ClientID);
	getStr(req, "UserID", myreq.UserID);
	getStr(req, "QuoteRef", myreq.QuoteRef);
	getStr(req, "AskOrderRef", myreq.AskOrderRef);
	getChar(req, "BidHedgeFlag", &myreq.BidHedgeFlag);
	getChar(req, "BidOffsetFlag", &myreq.BidOffsetFlag);
	getDouble(req, "BidPrice", &myreq.BidPrice);
	getInt(req, "RequestID", &myreq.RequestID);
	getChar(req, "AskOffsetFlag", &myreq.AskOffsetFlag);
	getInt(req, "AskVolume", &myreq.AskVolume);
	getStr(req, "IPAddress", myreq.IPAddress);
	getStr(req, "BusinessUnit", myreq.BusinessUnit);
	getInt(req, "BidVolume", &myreq.BidVolume);
	getStr(req, "ForQuoteSysID", myreq.ForQuoteSysID);
	getChar(req, "AskHedgeFlag", &myreq.AskHedgeFlag);
	getStr(req, "BrokerID", myreq.BrokerID);
	int i = this->api->ReqQuoteInsert(&myreq, nRequestID);
	return i;
};

int TdApi::reqQuoteAction(dict req, int nRequestID)
{
	CThostFtdcInputQuoteActionField myreq = CThostFtdcInputQuoteActionField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "MacAddress", myreq.MacAddress);
	getStr(req, "QuoteRef", myreq.QuoteRef);
	getStr(req, "InvestorID", myreq.InvestorID);
	getInt(req, "FrontID", &myreq.FrontID);
	getInt(req, "RequestID", &myreq.RequestID);
	getStr(req, "ClientID", myreq.ClientID);
	getStr(req, "QuoteSysID", myreq.QuoteSysID);
	getChar(req, "ActionFlag", &myreq.ActionFlag);
	getInt(req, "SessionID", &myreq.SessionID);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getInt(req, "QuoteActionRef", &myreq.QuoteActionRef);
	getStr(req, "IPAddress", myreq.IPAddress);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "UserID", myreq.UserID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	int i = this->api->ReqQuoteAction(&myreq, nRequestID);
	return i;
};

int TdApi::reqBatchOrderAction(dict req, int nRequestID)
{
	CThostFtdcInputBatchOrderActionField myreq = CThostFtdcInputBatchOrderActionField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "MacAddress", myreq.MacAddress);
	getInt(req, "FrontID", &myreq.FrontID);
	getInt(req, "OrderActionRef", &myreq.OrderActionRef);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getInt(req, "SessionID", &myreq.SessionID);
	getInt(req, "RequestID", &myreq.RequestID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "UserID", myreq.UserID);
	getStr(req, "IPAddress", myreq.IPAddress);
	int i = this->api->ReqBatchOrderAction(&myreq, nRequestID);
	return i;
};

int TdApi::reqOptionSelfCloseInsert(dict req, int nRequestID)
{
	CThostFtdcInputOptionSelfCloseField myreq = CThostFtdcInputOptionSelfCloseField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "MacAddress", myreq.MacAddress);
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "OptionSelfCloseRef", myreq.OptionSelfCloseRef);
	getInt(req, "RequestID", &myreq.RequestID);
	getStr(req, "ClientID", myreq.ClientID);
	getInt(req, "Volume", &myreq.Volume);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getChar(req, "HedgeFlag", &myreq.HedgeFlag);
	getStr(req, "BusinessUnit", myreq.BusinessUnit);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "IPAddress", myreq.IPAddress);
	getStr(req, "BrokerID", myreq.BrokerID);
	getChar(req, "OptSelfCloseFlag", &myreq.OptSelfCloseFlag);
	getStr(req, "UserID", myreq.UserID);
	getStr(req, "AccountID", myreq.AccountID);
	int i = this->api->ReqOptionSelfCloseInsert(&myreq, nRequestID);
	return i;
};

int TdApi::reqOptionSelfCloseAction(dict req, int nRequestID)
{
	CThostFtdcInputOptionSelfCloseActionField myreq = CThostFtdcInputOptionSelfCloseActionField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "MacAddress", myreq.MacAddress);
	getInt(req, "OptionSelfCloseActionRef", &myreq.OptionSelfCloseActionRef);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "OptionSelfCloseSysID", myreq.OptionSelfCloseSysID);
	getStr(req, "OptionSelfCloseRef", myreq.OptionSelfCloseRef);
	getInt(req, "FrontID", &myreq.FrontID);
	getInt(req, "RequestID", &myreq.RequestID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getChar(req, "ActionFlag", &myreq.ActionFlag);
	getInt(req, "SessionID", &myreq.SessionID);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "IPAddress", myreq.IPAddress);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "UserID", myreq.UserID);
	int i = this->api->ReqOptionSelfCloseAction(&myreq, nRequestID);
	return i;
};

int TdApi::reqCombActionInsert(dict req, int nRequestID)
{
	CThostFtdcInputCombActionField myreq = CThostFtdcInputCombActionField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "MacAddress", myreq.MacAddress);
	getStr(req, "InvestorID", myreq.InvestorID);
	getChar(req, "Direction", &myreq.Direction);
	getInt(req, "Volume", &myreq.Volume);
	getStr(req, "CombActionRef", myreq.CombActionRef);
	getStr(req, "IPAddress", myreq.IPAddress);
	getChar(req, "HedgeFlag", &myreq.HedgeFlag);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getChar(req, "CombDirection", &myreq.CombDirection);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "UserID", myreq.UserID);
	int i = this->api->ReqCombActionInsert(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryOrder(dict req, int nRequestID)
{
	CThostFtdcQryOrderField myreq = CThostFtdcQryOrderField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "OrderSysID", myreq.OrderSysID);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "InsertTimeEnd", myreq.InsertTimeEnd);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InsertTimeStart", myreq.InsertTimeStart);
	int i = this->api->ReqQryOrder(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryTrade(dict req, int nRequestID)
{
	CThostFtdcQryTradeField myreq = CThostFtdcQryTradeField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "TradeTimeStart", myreq.TradeTimeStart);
	getStr(req, "TradeTimeEnd", myreq.TradeTimeEnd);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "TradeID", myreq.TradeID);
	int i = this->api->ReqQryTrade(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryInvestorPosition(dict req, int nRequestID)
{
	CThostFtdcQryInvestorPositionField myreq = CThostFtdcQryInvestorPositionField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	int i = this->api->ReqQryInvestorPosition(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryTradingAccount(dict req, int nRequestID)
{
	CThostFtdcQryTradingAccountField myreq = CThostFtdcQryTradingAccountField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getChar(req, "BizType", &myreq.BizType);
	getStr(req, "AccountID", myreq.AccountID);
	int i = this->api->ReqQryTradingAccount(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryInvestor(dict req, int nRequestID)
{
	CThostFtdcQryInvestorField myreq = CThostFtdcQryInvestorField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	int i = this->api->ReqQryInvestor(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryTradingCode(dict req, int nRequestID)
{
	CThostFtdcQryTradingCodeField myreq = CThostFtdcQryTradingCodeField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getChar(req, "ClientIDType", &myreq.ClientIDType);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ClientID", myreq.ClientID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	int i = this->api->ReqQryTradingCode(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryInstrumentMarginRate(dict req, int nRequestID)
{
	CThostFtdcQryInstrumentMarginRateField myreq = CThostFtdcQryInstrumentMarginRateField();
	memset(&myreq, 0, sizeof(myreq));
	getChar(req, "HedgeFlag", &myreq.HedgeFlag);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	int i = this->api->ReqQryInstrumentMarginRate(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryInstrumentCommissionRate(dict req, int nRequestID)
{
	CThostFtdcQryInstrumentCommissionRateField myreq = CThostFtdcQryInstrumentCommissionRateField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	int i = this->api->ReqQryInstrumentCommissionRate(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryExchange(dict req, int nRequestID)
{
	CThostFtdcQryExchangeField myreq = CThostFtdcQryExchangeField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ExchangeID", myreq.ExchangeID);
	int i = this->api->ReqQryExchange(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryProduct(dict req, int nRequestID)
{
	CThostFtdcQryProductField myreq = CThostFtdcQryProductField();
	memset(&myreq, 0, sizeof(myreq));
	getChar(req, "ProductClass", &myreq.ProductClass);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "ProductID", myreq.ProductID);
	int i = this->api->ReqQryProduct(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryInstrument(dict req, int nRequestID)
{
	CThostFtdcQryInstrumentField myreq = CThostFtdcQryInstrumentField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "ProductID", myreq.ProductID);
	getStr(req, "ExchangeInstID", myreq.ExchangeInstID);
	int i = this->api->ReqQryInstrument(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryDepthMarketData(dict req, int nRequestID)
{
	CThostFtdcQryDepthMarketDataField myreq = CThostFtdcQryDepthMarketDataField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	int i = this->api->ReqQryDepthMarketData(&myreq, nRequestID);
	return i;
};

int TdApi::reqQrySettlementInfo(dict req, int nRequestID)
{
	CThostFtdcQrySettlementInfoField myreq = CThostFtdcQrySettlementInfoField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "AccountID", myreq.AccountID);
	getStr(req, "TradingDay", myreq.TradingDay);
	int i = this->api->ReqQrySettlementInfo(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryTransferBank(dict req, int nRequestID)
{
	CThostFtdcQryTransferBankField myreq = CThostFtdcQryTransferBankField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "BankBrchID", myreq.BankBrchID);
	getStr(req, "BankID", myreq.BankID);
	int i = this->api->ReqQryTransferBank(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryInvestorPositionDetail(dict req, int nRequestID)
{
	CThostFtdcQryInvestorPositionDetailField myreq = CThostFtdcQryInvestorPositionDetailField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	int i = this->api->ReqQryInvestorPositionDetail(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryNotice(dict req, int nRequestID)
{
	CThostFtdcQryNoticeField myreq = CThostFtdcQryNoticeField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "BrokerID", myreq.BrokerID);
	int i = this->api->ReqQryNotice(&myreq, nRequestID);
	return i;
};

int TdApi::reqQrySettlementInfoConfirm(dict req, int nRequestID)
{
	CThostFtdcQrySettlementInfoConfirmField myreq = CThostFtdcQrySettlementInfoConfirmField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "AccountID", myreq.AccountID);
	int i = this->api->ReqQrySettlementInfoConfirm(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryInvestorPositionCombineDetail(dict req, int nRequestID)
{
	CThostFtdcQryInvestorPositionCombineDetailField myreq = CThostFtdcQryInvestorPositionCombineDetailField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "CombInstrumentID", myreq.CombInstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	int i = this->api->ReqQryInvestorPositionCombineDetail(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryCFMMCTradingAccountKey(dict req, int nRequestID)
{
	CThostFtdcQryCFMMCTradingAccountKeyField myreq = CThostFtdcQryCFMMCTradingAccountKeyField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	int i = this->api->ReqQryCFMMCTradingAccountKey(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryEWarrantOffset(dict req, int nRequestID)
{
	CThostFtdcQryEWarrantOffsetField myreq = CThostFtdcQryEWarrantOffsetField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	int i = this->api->ReqQryEWarrantOffset(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryInvestorProductGroupMargin(dict req, int nRequestID)
{
	CThostFtdcQryInvestorProductGroupMarginField myreq = CThostFtdcQryInvestorProductGroupMarginField();
	memset(&myreq, 0, sizeof(myreq));
	getChar(req, "HedgeFlag", &myreq.HedgeFlag);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "ProductGroupID", myreq.ProductGroupID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	int i = this->api->ReqQryInvestorProductGroupMargin(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryExchangeMarginRate(dict req, int nRequestID)
{
	CThostFtdcQryExchangeMarginRateField myreq = CThostFtdcQryExchangeMarginRateField();
	memset(&myreq, 0, sizeof(myreq));
	getChar(req, "HedgeFlag", &myreq.HedgeFlag);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	int i = this->api->ReqQryExchangeMarginRate(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryExchangeMarginRateAdjust(dict req, int nRequestID)
{
	CThostFtdcQryExchangeMarginRateAdjustField myreq = CThostFtdcQryExchangeMarginRateAdjustField();
	memset(&myreq, 0, sizeof(myreq));
	getChar(req, "HedgeFlag", &myreq.HedgeFlag);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "BrokerID", myreq.BrokerID);
	int i = this->api->ReqQryExchangeMarginRateAdjust(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryExchangeRate(dict req, int nRequestID)
{
	CThostFtdcQryExchangeRateField myreq = CThostFtdcQryExchangeRateField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ToCurrencyID", myreq.ToCurrencyID);
	getStr(req, "FromCurrencyID", myreq.FromCurrencyID);
	getStr(req, "BrokerID", myreq.BrokerID);
	int i = this->api->ReqQryExchangeRate(&myreq, nRequestID);
	return i;
};

int TdApi::reqQrySecAgentACIDMap(dict req, int nRequestID)
{
	CThostFtdcQrySecAgentACIDMapField myreq = CThostFtdcQrySecAgentACIDMapField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "AccountID", myreq.AccountID);
	getStr(req, "UserID", myreq.UserID);
	int i = this->api->ReqQrySecAgentACIDMap(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryProductExchRate(dict req, int nRequestID)
{
	CThostFtdcQryProductExchRateField myreq = CThostFtdcQryProductExchRateField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "ProductID", myreq.ProductID);
	int i = this->api->ReqQryProductExchRate(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryProductGroup(dict req, int nRequestID)
{
	CThostFtdcQryProductGroupField myreq = CThostFtdcQryProductGroupField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "ProductID", myreq.ProductID);
	int i = this->api->ReqQryProductGroup(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryMMInstrumentCommissionRate(dict req, int nRequestID)
{
	CThostFtdcQryMMInstrumentCommissionRateField myreq = CThostFtdcQryMMInstrumentCommissionRateField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	int i = this->api->ReqQryMMInstrumentCommissionRate(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryMMOptionInstrCommRate(dict req, int nRequestID)
{
	CThostFtdcQryMMOptionInstrCommRateField myreq = CThostFtdcQryMMOptionInstrCommRateField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	int i = this->api->ReqQryMMOptionInstrCommRate(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryInstrumentOrderCommRate(dict req, int nRequestID)
{
	CThostFtdcQryInstrumentOrderCommRateField myreq = CThostFtdcQryInstrumentOrderCommRateField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	int i = this->api->ReqQryInstrumentOrderCommRate(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryOptionInstrTradeCost(dict req, int nRequestID)
{
	CThostFtdcQryOptionInstrTradeCostField myreq = CThostFtdcQryOptionInstrTradeCostField();
	memset(&myreq, 0, sizeof(myreq));
	getChar(req, "HedgeFlag", &myreq.HedgeFlag);
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getDouble(req, "InputPrice", &myreq.InputPrice);
	getStr(req, "BrokerID", myreq.BrokerID);
	getDouble(req, "UnderlyingPrice", &myreq.UnderlyingPrice);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	int i = this->api->ReqQryOptionInstrTradeCost(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryOptionInstrCommRate(dict req, int nRequestID)
{
	CThostFtdcQryOptionInstrCommRateField myreq = CThostFtdcQryOptionInstrCommRateField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	int i = this->api->ReqQryOptionInstrCommRate(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryExecOrder(dict req, int nRequestID)
{
	CThostFtdcQryExecOrderField myreq = CThostFtdcQryExecOrderField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "ExecOrderSysID", myreq.ExecOrderSysID);
	getStr(req, "InsertTimeEnd", myreq.InsertTimeEnd);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InsertTimeStart", myreq.InsertTimeStart);
	int i = this->api->ReqQryExecOrder(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryForQuote(dict req, int nRequestID)
{
	CThostFtdcQryForQuoteField myreq = CThostFtdcQryForQuoteField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "InsertTimeEnd", myreq.InsertTimeEnd);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InsertTimeStart", myreq.InsertTimeStart);
	int i = this->api->ReqQryForQuote(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryQuote(dict req, int nRequestID)
{
	CThostFtdcQryQuoteField myreq = CThostFtdcQryQuoteField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	getStr(req, "InsertTimeEnd", myreq.InsertTimeEnd);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InsertTimeStart", myreq.InsertTimeStart);
	getStr(req, "QuoteSysID", myreq.QuoteSysID);
	int i = this->api->ReqQryQuote(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryOptionSelfClose(dict req, int nRequestID)
{
	CThostFtdcQryOptionSelfCloseField myreq = CThostFtdcQryOptionSelfCloseField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "OptionSelfCloseSysID", myreq.OptionSelfCloseSysID);
	getStr(req, "InsertTimeEnd", myreq.InsertTimeEnd);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InsertTimeStart", myreq.InsertTimeStart);
	int i = this->api->ReqQryOptionSelfClose(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryInvestUnit(dict req, int nRequestID)
{
	CThostFtdcQryInvestUnitField myreq = CThostFtdcQryInvestUnitField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	int i = this->api->ReqQryInvestUnit(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryCombInstrumentGuard(dict req, int nRequestID)
{
	CThostFtdcQryCombInstrumentGuardField myreq = CThostFtdcQryCombInstrumentGuardField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	int i = this->api->ReqQryCombInstrumentGuard(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryCombAction(dict req, int nRequestID)
{
	CThostFtdcQryCombActionField myreq = CThostFtdcQryCombActionField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	int i = this->api->ReqQryCombAction(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryTransferSerial(dict req, int nRequestID)
{
	CThostFtdcQryTransferSerialField myreq = CThostFtdcQryTransferSerialField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "AccountID", myreq.AccountID);
	getStr(req, "BankID", myreq.BankID);
	int i = this->api->ReqQryTransferSerial(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryAccountregister(dict req, int nRequestID)
{
	CThostFtdcQryAccountregisterField myreq = CThostFtdcQryAccountregisterField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "AccountID", myreq.AccountID);
	getStr(req, "BankID", myreq.BankID);
	getStr(req, "BankBranchID", myreq.BankBranchID);
	int i = this->api->ReqQryAccountregister(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryContractBank(dict req, int nRequestID)
{
	CThostFtdcQryContractBankField myreq = CThostFtdcQryContractBankField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "BankBrchID", myreq.BankBrchID);
	getStr(req, "BankID", myreq.BankID);
	int i = this->api->ReqQryContractBank(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryParkedOrder(dict req, int nRequestID)
{
	CThostFtdcQryParkedOrderField myreq = CThostFtdcQryParkedOrderField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	int i = this->api->ReqQryParkedOrder(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryParkedOrderAction(dict req, int nRequestID)
{
	CThostFtdcQryParkedOrderActionField myreq = CThostFtdcQryParkedOrderActionField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	int i = this->api->ReqQryParkedOrderAction(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryTradingNotice(dict req, int nRequestID)
{
	CThostFtdcQryTradingNoticeField myreq = CThostFtdcQryTradingNoticeField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	int i = this->api->ReqQryTradingNotice(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryBrokerTradingParams(dict req, int nRequestID)
{
	CThostFtdcQryBrokerTradingParamsField myreq = CThostFtdcQryBrokerTradingParamsField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "AccountID", myreq.AccountID);
	int i = this->api->ReqQryBrokerTradingParams(&myreq, nRequestID);
	return i;
};

int TdApi::reqQryBrokerTradingAlgos(dict req, int nRequestID)
{
	CThostFtdcQryBrokerTradingAlgosField myreq = CThostFtdcQryBrokerTradingAlgosField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InstrumentID", myreq.InstrumentID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "ExchangeID", myreq.ExchangeID);
	int i = this->api->ReqQryBrokerTradingAlgos(&myreq, nRequestID);
	return i;
};

int TdApi::reqQueryCFMMCTradingAccountToken(dict req, int nRequestID)
{
	CThostFtdcQueryCFMMCTradingAccountTokenField myreq = CThostFtdcQueryCFMMCTradingAccountTokenField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "InvestorID", myreq.InvestorID);
	getStr(req, "BrokerID", myreq.BrokerID);
	getStr(req, "InvestUnitID", myreq.InvestUnitID);
	int i = this->api->ReqQueryCFMMCTradingAccountToken(&myreq, nRequestID);
	return i;
};

int TdApi::reqFromBankToFutureByFuture(dict req, int nRequestID)
{
	CThostFtdcReqTransferField myreq = CThostFtdcReqTransferField();
	memset(&myreq, 0, sizeof(myreq));
	getChar(req, "BankAccType", &myreq.BankAccType);
	getStr(req, "TradeTime", myreq.TradeTime);
	getStr(req, "IdentifiedCardNo", myreq.IdentifiedCardNo);
	getChar(req, "BankPwdFlag", &myreq.BankPwdFlag);
	getInt(req, "PlateSerial", &myreq.PlateSerial);
	getStr(req, "UserID", myreq.UserID);
	getInt(req, "InstallID", &myreq.InstallID);
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getStr(req, "BankAccount", myreq.BankAccount);
	getStr(req, "BankPassWord", myreq.BankPassWord);
	getStr(req, "BankSecuAcc", myreq.BankSecuAcc);
	getInt(req, "RequestID", &myreq.RequestID);
	getStr(req, "OperNo", myreq.OperNo);
	getInt(req, "FutureSerial", &myreq.FutureSerial);
	getStr(req, "TradeDate", myreq.TradeDate);
	getStr(req, "DeviceID", myreq.DeviceID);
	getStr(req, "Password", myreq.Password);
	getStr(req, "TradingDay", myreq.TradingDay);
	getStr(req, "LongCustomerName", myreq.LongCustomerName);
	getStr(req, "BrokerID", myreq.BrokerID);
	getChar(req, "FeePayFlag", &myreq.FeePayFlag);
	getStr(req, "Digest", myreq.Digest);
	getChar(req, "LastFragment", &myreq.LastFragment);
	getDouble(req, "FutureFetchAmount", &myreq.FutureFetchAmount);
	getChar(req, "SecuPwdFlag", &myreq.SecuPwdFlag);
	getStr(req, "BrokerBranchID", myreq.BrokerBranchID);
	getDouble(req, "CustFee", &myreq.CustFee);
	getInt(req, "TID", &myreq.TID);
	getChar(req, "IdCardType", &myreq.IdCardType);
	getStr(req, "BrokerIDByBank", myreq.BrokerIDByBank);
	getDouble(req, "TradeAmount", &myreq.TradeAmount);
	getStr(req, "BankBranchID", myreq.BankBranchID);
	getChar(req, "VerifyCertNoFlag", &myreq.VerifyCertNoFlag);
	getStr(req, "AccountID", myreq.AccountID);
	getStr(req, "Message", myreq.Message);
	getStr(req, "BankID", myreq.BankID);
	getStr(req, "TradeCode", myreq.TradeCode);
	getStr(req, "BankSerial", myreq.BankSerial);
	getChar(req, "TransferStatus", &myreq.TransferStatus);
	getChar(req, "BankSecuAccType", &myreq.BankSecuAccType);
	getChar(req, "CustType", &myreq.CustType);
	getDouble(req, "BrokerFee", &myreq.BrokerFee);
	getInt(req, "SessionID", &myreq.SessionID);
	getStr(req, "CustomerName", myreq.CustomerName);
	int i = this->api->ReqFromBankToFutureByFuture(&myreq, nRequestID);
	return i;
};

int TdApi::reqFromFutureToBankByFuture(dict req, int nRequestID)
{
	CThostFtdcReqTransferField myreq = CThostFtdcReqTransferField();
	memset(&myreq, 0, sizeof(myreq));
	getChar(req, "BankAccType", &myreq.BankAccType);
	getStr(req, "TradeTime", myreq.TradeTime);
	getStr(req, "IdentifiedCardNo", myreq.IdentifiedCardNo);
	getChar(req, "BankPwdFlag", &myreq.BankPwdFlag);
	getInt(req, "PlateSerial", &myreq.PlateSerial);
	getStr(req, "UserID", myreq.UserID);
	getInt(req, "InstallID", &myreq.InstallID);
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getStr(req, "BankAccount", myreq.BankAccount);
	getStr(req, "BankPassWord", myreq.BankPassWord);
	getStr(req, "BankSecuAcc", myreq.BankSecuAcc);
	getInt(req, "RequestID", &myreq.RequestID);
	getStr(req, "OperNo", myreq.OperNo);
	getInt(req, "FutureSerial", &myreq.FutureSerial);
	getStr(req, "TradeDate", myreq.TradeDate);
	getStr(req, "DeviceID", myreq.DeviceID);
	getStr(req, "Password", myreq.Password);
	getStr(req, "TradingDay", myreq.TradingDay);
	getStr(req, "LongCustomerName", myreq.LongCustomerName);
	getStr(req, "BrokerID", myreq.BrokerID);
	getChar(req, "FeePayFlag", &myreq.FeePayFlag);
	getStr(req, "Digest", myreq.Digest);
	getChar(req, "LastFragment", &myreq.LastFragment);
	getDouble(req, "FutureFetchAmount", &myreq.FutureFetchAmount);
	getChar(req, "SecuPwdFlag", &myreq.SecuPwdFlag);
	getStr(req, "BrokerBranchID", myreq.BrokerBranchID);
	getDouble(req, "CustFee", &myreq.CustFee);
	getInt(req, "TID", &myreq.TID);
	getChar(req, "IdCardType", &myreq.IdCardType);
	getStr(req, "BrokerIDByBank", myreq.BrokerIDByBank);
	getDouble(req, "TradeAmount", &myreq.TradeAmount);
	getStr(req, "BankBranchID", myreq.BankBranchID);
	getChar(req, "VerifyCertNoFlag", &myreq.VerifyCertNoFlag);
	getStr(req, "AccountID", myreq.AccountID);
	getStr(req, "Message", myreq.Message);
	getStr(req, "BankID", myreq.BankID);
	getStr(req, "TradeCode", myreq.TradeCode);
	getStr(req, "BankSerial", myreq.BankSerial);
	getChar(req, "TransferStatus", &myreq.TransferStatus);
	getChar(req, "BankSecuAccType", &myreq.BankSecuAccType);
	getChar(req, "CustType", &myreq.CustType);
	getDouble(req, "BrokerFee", &myreq.BrokerFee);
	getInt(req, "SessionID", &myreq.SessionID);
	getStr(req, "CustomerName", myreq.CustomerName);
	int i = this->api->ReqFromFutureToBankByFuture(&myreq, nRequestID);
	return i;
};

int TdApi::reqQueryBankAccountMoneyByFuture(dict req, int nRequestID)
{
	CThostFtdcReqQueryAccountField myreq = CThostFtdcReqQueryAccountField();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "Digest", myreq.Digest);
	getChar(req, "LastFragment", &myreq.LastFragment);
	getChar(req, "VerifyCertNoFlag", &myreq.VerifyCertNoFlag);
	getStr(req, "LongCustomerName", myreq.LongCustomerName);
	getChar(req, "SecuPwdFlag", &myreq.SecuPwdFlag);
	getStr(req, "OperNo", myreq.OperNo);
	getStr(req, "BrokerBranchID", myreq.BrokerBranchID);
	getChar(req, "BankAccType", &myreq.BankAccType);
	getStr(req, "TradeTime", myreq.TradeTime);
	getStr(req, "IdentifiedCardNo", myreq.IdentifiedCardNo);
	getChar(req, "CustType", &myreq.CustType);
	getStr(req, "BankSecuAcc", myreq.BankSecuAcc);
	getStr(req, "BankPassWord", myreq.BankPassWord);
	getInt(req, "PlateSerial", &myreq.PlateSerial);
	getStr(req, "BrokerIDByBank", myreq.BrokerIDByBank);
	getChar(req, "BankPwdFlag", &myreq.BankPwdFlag);
	getStr(req, "UserID", myreq.UserID);
	getStr(req, "AccountID", myreq.AccountID);
	getStr(req, "CustomerName", myreq.CustomerName);
	getStr(req, "CurrencyID", myreq.CurrencyID);
	getStr(req, "BankAccount", myreq.BankAccount);
	getInt(req, "TID", &myreq.TID);
	getStr(req, "BankBranchID", myreq.BankBranchID);
	getStr(req, "BankID", myreq.BankID);
	getStr(req, "DeviceID", myreq.DeviceID);
	getStr(req, "TradeCode", myreq.TradeCode);
	getStr(req, "BankSerial", myreq.BankSerial);
	getInt(req, "FutureSerial", &myreq.FutureSerial);
	getStr(req, "TradeDate", myreq.TradeDate);
	getInt(req, "RequestID", &myreq.RequestID);
	getInt(req, "SessionID", &myreq.SessionID);
	getChar(req, "BankSecuAccType", &myreq.BankSecuAccType);
	getStr(req, "Password", myreq.Password);
	getStr(req, "TradingDay", myreq.TradingDay);
	getChar(req, "IdCardType", &myreq.IdCardType);
	getInt(req, "InstallID", &myreq.InstallID);
	getStr(req, "BrokerID", myreq.BrokerID);
	int i = this->api->ReqQueryBankAccountMoneyByFuture(&myreq, nRequestID);
	return i;
};


// render_data.ctp_td_function end


///-------------------------------------------------------------------------------------
///Boost.Python封装
///-------------------------------------------------------------------------------------

struct TdApiWrap : TdApi, wrapper < TdApi >
{
	virtual void onFrontConnected()
	{
		//在向python环境中调用回调函数推送数据前，需要先获取全局锁GIL，防止解释器崩溃
		PyLock lock;

		//以下的try...catch...可以实现捕捉python环境中错误的功能，防止C++直接出现原因未知的崩溃
		try
		{
			this->get_override("onFrontConnected")();
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onFrontDisconnected(int i)
	{
		PyLock lock;

		try
		{
			this->get_override("onFrontDisconnected")(i);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onHeartBeatWarning(int i)
	{
		PyLock lock;

		try
		{
			this->get_override("onHeartBeatWarning")(i);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	// render_data.ctp_td_wrap start
	virtual void onRspAuthenticate(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspAuthenticate")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspUserLogin(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspUserLogin")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspUserLogout(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspUserLogout")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspUserPasswordUpdate(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspUserPasswordUpdate")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspTradingAccountPasswordUpdate(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspTradingAccountPasswordUpdate")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspOrderInsert(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspOrderInsert")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspParkedOrderInsert(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspParkedOrderInsert")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspParkedOrderAction(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspParkedOrderAction")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspOrderAction(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspOrderAction")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQueryMaxOrderVolume(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQueryMaxOrderVolume")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspSettlementInfoConfirm(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspSettlementInfoConfirm")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspRemoveParkedOrder(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspRemoveParkedOrder")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspRemoveParkedOrderAction(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspRemoveParkedOrderAction")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspExecOrderInsert(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspExecOrderInsert")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspExecOrderAction(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspExecOrderAction")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspForQuoteInsert(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspForQuoteInsert")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQuoteInsert(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQuoteInsert")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQuoteAction(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQuoteAction")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspBatchOrderAction(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspBatchOrderAction")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspOptionSelfCloseInsert(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspOptionSelfCloseInsert")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspOptionSelfCloseAction(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspOptionSelfCloseAction")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspCombActionInsert(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspCombActionInsert")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryOrder(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryOrder")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryTrade(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryTrade")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryInvestorPosition(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryInvestorPosition")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryTradingAccount(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryTradingAccount")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryInvestor(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryInvestor")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryTradingCode(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryTradingCode")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryInstrumentMarginRate(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryInstrumentMarginRate")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryInstrumentCommissionRate(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryInstrumentCommissionRate")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryExchange(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryExchange")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryProduct(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryProduct")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryInstrument(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryInstrument")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryDepthMarketData(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryDepthMarketData")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQrySettlementInfo(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQrySettlementInfo")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryTransferBank(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryTransferBank")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryInvestorPositionDetail(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryInvestorPositionDetail")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryNotice(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryNotice")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQrySettlementInfoConfirm(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQrySettlementInfoConfirm")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryInvestorPositionCombineDetail(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryInvestorPositionCombineDetail")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryCFMMCTradingAccountKey(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryCFMMCTradingAccountKey")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryEWarrantOffset(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryEWarrantOffset")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryInvestorProductGroupMargin(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryInvestorProductGroupMargin")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryExchangeMarginRate(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryExchangeMarginRate")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryExchangeMarginRateAdjust(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryExchangeMarginRateAdjust")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryExchangeRate(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryExchangeRate")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQrySecAgentACIDMap(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQrySecAgentACIDMap")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryProductExchRate(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryProductExchRate")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryProductGroup(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryProductGroup")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryMMInstrumentCommissionRate(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryMMInstrumentCommissionRate")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryMMOptionInstrCommRate(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryMMOptionInstrCommRate")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryInstrumentOrderCommRate(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryInstrumentOrderCommRate")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryOptionInstrTradeCost(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryOptionInstrTradeCost")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryOptionInstrCommRate(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryOptionInstrCommRate")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryExecOrder(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryExecOrder")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryForQuote(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryForQuote")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryQuote(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryQuote")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryOptionSelfClose(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryOptionSelfClose")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryInvestUnit(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryInvestUnit")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryCombInstrumentGuard(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryCombInstrumentGuard")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryCombAction(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryCombAction")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryTransferSerial(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryTransferSerial")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryAccountregister(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryAccountregister")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspError(dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspError")(error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnOrder(dict data)
    {
    	try
    	{
    		this->get_override("onRtnOrder")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnTrade(dict data)
    {
    	try
    	{
    		this->get_override("onRtnTrade")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnOrderInsert(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnOrderInsert")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnOrderAction(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnOrderAction")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnInstrumentStatus(dict data)
    {
    	try
    	{
    		this->get_override("onRtnInstrumentStatus")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnBulletin(dict data)
    {
    	try
    	{
    		this->get_override("onRtnBulletin")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnTradingNotice(dict data)
    {
    	try
    	{
    		this->get_override("onRtnTradingNotice")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnErrorConditionalOrder(dict data)
    {
    	try
    	{
    		this->get_override("onRtnErrorConditionalOrder")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnExecOrder(dict data)
    {
    	try
    	{
    		this->get_override("onRtnExecOrder")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnExecOrderInsert(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnExecOrderInsert")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnExecOrderAction(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnExecOrderAction")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnForQuoteInsert(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnForQuoteInsert")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnQuote(dict data)
    {
    	try
    	{
    		this->get_override("onRtnQuote")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnQuoteInsert(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnQuoteInsert")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnQuoteAction(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnQuoteAction")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnForQuoteRsp(dict data)
    {
    	try
    	{
    		this->get_override("onRtnForQuoteRsp")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnCFMMCTradingAccountToken(dict data)
    {
    	try
    	{
    		this->get_override("onRtnCFMMCTradingAccountToken")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnBatchOrderAction(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnBatchOrderAction")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnOptionSelfClose(dict data)
    {
    	try
    	{
    		this->get_override("onRtnOptionSelfClose")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnOptionSelfCloseInsert(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnOptionSelfCloseInsert")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnOptionSelfCloseAction(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnOptionSelfCloseAction")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnCombAction(dict data)
    {
    	try
    	{
    		this->get_override("onRtnCombAction")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnCombActionInsert(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnCombActionInsert")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryContractBank(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryContractBank")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryParkedOrder(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryParkedOrder")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryParkedOrderAction(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryParkedOrderAction")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryTradingNotice(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryTradingNotice")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryBrokerTradingParams(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryBrokerTradingParams")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQryBrokerTradingAlgos(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQryBrokerTradingAlgos")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQueryCFMMCTradingAccountToken(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQueryCFMMCTradingAccountToken")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnFromBankToFutureByBank(dict data)
    {
    	try
    	{
    		this->get_override("onRtnFromBankToFutureByBank")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnFromFutureToBankByBank(dict data)
    {
    	try
    	{
    		this->get_override("onRtnFromFutureToBankByBank")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnRepealFromBankToFutureByBank(dict data)
    {
    	try
    	{
    		this->get_override("onRtnRepealFromBankToFutureByBank")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnRepealFromFutureToBankByBank(dict data)
    {
    	try
    	{
    		this->get_override("onRtnRepealFromFutureToBankByBank")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnFromBankToFutureByFuture(dict data)
    {
    	try
    	{
    		this->get_override("onRtnFromBankToFutureByFuture")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnFromFutureToBankByFuture(dict data)
    {
    	try
    	{
    		this->get_override("onRtnFromFutureToBankByFuture")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnRepealFromBankToFutureByFutureManual(dict data)
    {
    	try
    	{
    		this->get_override("onRtnRepealFromBankToFutureByFutureManual")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnRepealFromFutureToBankByFutureManual(dict data)
    {
    	try
    	{
    		this->get_override("onRtnRepealFromFutureToBankByFutureManual")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnQueryBankBalanceByFuture(dict data)
    {
    	try
    	{
    		this->get_override("onRtnQueryBankBalanceByFuture")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnBankToFutureByFuture(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnBankToFutureByFuture")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnFutureToBankByFuture(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnFutureToBankByFuture")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnRepealBankToFutureByFutureManual(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnRepealBankToFutureByFutureManual")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnRepealFutureToBankByFutureManual(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnRepealFutureToBankByFutureManual")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onErrRtnQueryBankBalanceByFuture(dict data, dict error)
    {
    	try
    	{
    		this->get_override("onErrRtnQueryBankBalanceByFuture")(data, error);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnRepealFromBankToFutureByFuture(dict data)
    {
    	try
    	{
    		this->get_override("onRtnRepealFromBankToFutureByFuture")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnRepealFromFutureToBankByFuture(dict data)
    {
    	try
    	{
    		this->get_override("onRtnRepealFromFutureToBankByFuture")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspFromBankToFutureByFuture(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspFromBankToFutureByFuture")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspFromFutureToBankByFuture(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspFromFutureToBankByFuture")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRspQueryBankAccountMoneyByFuture(dict data, dict error, int id, bool last)
    {
    	try
    	{
    		this->get_override("onRspQueryBankAccountMoneyByFuture")(data, error, id, last);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnOpenAccountByBank(dict data)
    {
    	try
    	{
    		this->get_override("onRtnOpenAccountByBank")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnCancelAccountByBank(dict data)
    {
    	try
    	{
    		this->get_override("onRtnCancelAccountByBank")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };

    virtual void onRtnChangeAccountByBank(dict data)
    {
    	try
    	{
    		this->get_override("onRtnChangeAccountByBank")(data);
    	}
    	catch (error_already_set const &)
    	{
    		PyErr_Print();
    	}
    };


	// render_data.ctp_td_wrap end

};

BOOST_PYTHON_MODULE(vnctptd)
{
	PyEval_InitThreads();	//导入时运行，保证先创建GIL

	class_<TdApiWrap, boost::noncopyable>("TdApi")
		// write manually
		.def("createFtdcTraderApi", &TdApiWrap::createFtdcTraderApi)
		.def("release", &TdApiWrap::release)
		.def("init", &TdApiWrap::init)
		.def("join", &TdApiWrap::join)
		.def("exit", &TdApiWrap::exit)
		.def("getTradingDay", &TdApiWrap::getTradingDay)
		.def("registerFront", &TdApiWrap::registerFront)
		.def("subscribePrivateTopic", &TdApiWrap::subscribePrivateTopic)
		.def("subscribePublicTopic", &TdApiWrap::subscribePublicTopic)

		.def("onFrontConnected", pure_virtual(&TdApiWrap::onFrontConnected))
		.def("onFrontDisconnected", pure_virtual(&TdApiWrap::onFrontDisconnected))
		.def("onHeartBeatWarning", pure_virtual(&TdApiWrap::onHeartBeatWarning))

		// render_data.ctp_td_python_module start
		.def("reqAuthenticate", &TdApiWrap::reqAuthenticate)
        .def("reqUserLogin", &TdApiWrap::reqUserLogin)
        .def("reqUserLogout", &TdApiWrap::reqUserLogout)
        .def("reqUserPasswordUpdate", &TdApiWrap::reqUserPasswordUpdate)
        .def("reqTradingAccountPasswordUpdate", &TdApiWrap::reqTradingAccountPasswordUpdate)
        .def("reqUserLogin2", &TdApiWrap::reqUserLogin2)
        .def("reqUserPasswordUpdate2", &TdApiWrap::reqUserPasswordUpdate2)
        .def("reqOrderInsert", &TdApiWrap::reqOrderInsert)
        .def("reqParkedOrderInsert", &TdApiWrap::reqParkedOrderInsert)
        .def("reqParkedOrderAction", &TdApiWrap::reqParkedOrderAction)
        .def("reqOrderAction", &TdApiWrap::reqOrderAction)
        .def("reqQueryMaxOrderVolume", &TdApiWrap::reqQueryMaxOrderVolume)
        .def("reqSettlementInfoConfirm", &TdApiWrap::reqSettlementInfoConfirm)
        .def("reqRemoveParkedOrder", &TdApiWrap::reqRemoveParkedOrder)
        .def("reqRemoveParkedOrderAction", &TdApiWrap::reqRemoveParkedOrderAction)
        .def("reqExecOrderInsert", &TdApiWrap::reqExecOrderInsert)
        .def("reqExecOrderAction", &TdApiWrap::reqExecOrderAction)
        .def("reqForQuoteInsert", &TdApiWrap::reqForQuoteInsert)
        .def("reqQuoteInsert", &TdApiWrap::reqQuoteInsert)
        .def("reqQuoteAction", &TdApiWrap::reqQuoteAction)
        .def("reqBatchOrderAction", &TdApiWrap::reqBatchOrderAction)
        .def("reqOptionSelfCloseInsert", &TdApiWrap::reqOptionSelfCloseInsert)
        .def("reqOptionSelfCloseAction", &TdApiWrap::reqOptionSelfCloseAction)
        .def("reqCombActionInsert", &TdApiWrap::reqCombActionInsert)
        .def("reqQryOrder", &TdApiWrap::reqQryOrder)
        .def("reqQryTrade", &TdApiWrap::reqQryTrade)
        .def("reqQryInvestorPosition", &TdApiWrap::reqQryInvestorPosition)
        .def("reqQryTradingAccount", &TdApiWrap::reqQryTradingAccount)
        .def("reqQryInvestor", &TdApiWrap::reqQryInvestor)
        .def("reqQryTradingCode", &TdApiWrap::reqQryTradingCode)
        .def("reqQryInstrumentMarginRate", &TdApiWrap::reqQryInstrumentMarginRate)
        .def("reqQryInstrumentCommissionRate", &TdApiWrap::reqQryInstrumentCommissionRate)
        .def("reqQryExchange", &TdApiWrap::reqQryExchange)
        .def("reqQryProduct", &TdApiWrap::reqQryProduct)
        .def("reqQryInstrument", &TdApiWrap::reqQryInstrument)
        .def("reqQryDepthMarketData", &TdApiWrap::reqQryDepthMarketData)
        .def("reqQrySettlementInfo", &TdApiWrap::reqQrySettlementInfo)
        .def("reqQryTransferBank", &TdApiWrap::reqQryTransferBank)
        .def("reqQryInvestorPositionDetail", &TdApiWrap::reqQryInvestorPositionDetail)
        .def("reqQryNotice", &TdApiWrap::reqQryNotice)
        .def("reqQrySettlementInfoConfirm", &TdApiWrap::reqQrySettlementInfoConfirm)
        .def("reqQryInvestorPositionCombineDetail", &TdApiWrap::reqQryInvestorPositionCombineDetail)
        .def("reqQryCFMMCTradingAccountKey", &TdApiWrap::reqQryCFMMCTradingAccountKey)
        .def("reqQryEWarrantOffset", &TdApiWrap::reqQryEWarrantOffset)
        .def("reqQryInvestorProductGroupMargin", &TdApiWrap::reqQryInvestorProductGroupMargin)
        .def("reqQryExchangeMarginRate", &TdApiWrap::reqQryExchangeMarginRate)
        .def("reqQryExchangeMarginRateAdjust", &TdApiWrap::reqQryExchangeMarginRateAdjust)
        .def("reqQryExchangeRate", &TdApiWrap::reqQryExchangeRate)
        .def("reqQrySecAgentACIDMap", &TdApiWrap::reqQrySecAgentACIDMap)
        .def("reqQryProductExchRate", &TdApiWrap::reqQryProductExchRate)
        .def("reqQryProductGroup", &TdApiWrap::reqQryProductGroup)
        .def("reqQryMMInstrumentCommissionRate", &TdApiWrap::reqQryMMInstrumentCommissionRate)
        .def("reqQryMMOptionInstrCommRate", &TdApiWrap::reqQryMMOptionInstrCommRate)
        .def("reqQryInstrumentOrderCommRate", &TdApiWrap::reqQryInstrumentOrderCommRate)
        .def("reqQryOptionInstrTradeCost", &TdApiWrap::reqQryOptionInstrTradeCost)
        .def("reqQryOptionInstrCommRate", &TdApiWrap::reqQryOptionInstrCommRate)
        .def("reqQryExecOrder", &TdApiWrap::reqQryExecOrder)
        .def("reqQryForQuote", &TdApiWrap::reqQryForQuote)
        .def("reqQryQuote", &TdApiWrap::reqQryQuote)
        .def("reqQryOptionSelfClose", &TdApiWrap::reqQryOptionSelfClose)
        .def("reqQryInvestUnit", &TdApiWrap::reqQryInvestUnit)
        .def("reqQryCombInstrumentGuard", &TdApiWrap::reqQryCombInstrumentGuard)
        .def("reqQryCombAction", &TdApiWrap::reqQryCombAction)
        .def("reqQryTransferSerial", &TdApiWrap::reqQryTransferSerial)
        .def("reqQryAccountregister", &TdApiWrap::reqQryAccountregister)
        .def("reqQryContractBank", &TdApiWrap::reqQryContractBank)
        .def("reqQryParkedOrder", &TdApiWrap::reqQryParkedOrder)
        .def("reqQryParkedOrderAction", &TdApiWrap::reqQryParkedOrderAction)
        .def("reqQryTradingNotice", &TdApiWrap::reqQryTradingNotice)
        .def("reqQryBrokerTradingParams", &TdApiWrap::reqQryBrokerTradingParams)
        .def("reqQryBrokerTradingAlgos", &TdApiWrap::reqQryBrokerTradingAlgos)
        .def("reqQueryCFMMCTradingAccountToken", &TdApiWrap::reqQueryCFMMCTradingAccountToken)
        .def("reqFromBankToFutureByFuture", &TdApiWrap::reqFromBankToFutureByFuture)
        .def("reqFromFutureToBankByFuture", &TdApiWrap::reqFromFutureToBankByFuture)
        .def("reqQueryBankAccountMoneyByFuture", &TdApiWrap::reqQueryBankAccountMoneyByFuture)

        .def("onRspAuthenticate", pure_virtual(&TdApiWrap::onRspAuthenticate))
        .def("onRspUserLogin", pure_virtual(&TdApiWrap::onRspUserLogin))
        .def("onRspUserLogout", pure_virtual(&TdApiWrap::onRspUserLogout))
        .def("onRspUserPasswordUpdate", pure_virtual(&TdApiWrap::onRspUserPasswordUpdate))
        .def("onRspTradingAccountPasswordUpdate", pure_virtual(&TdApiWrap::onRspTradingAccountPasswordUpdate))
        .def("onRspOrderInsert", pure_virtual(&TdApiWrap::onRspOrderInsert))
        .def("onRspParkedOrderInsert", pure_virtual(&TdApiWrap::onRspParkedOrderInsert))
        .def("onRspParkedOrderAction", pure_virtual(&TdApiWrap::onRspParkedOrderAction))
        .def("onRspOrderAction", pure_virtual(&TdApiWrap::onRspOrderAction))
        .def("onRspQueryMaxOrderVolume", pure_virtual(&TdApiWrap::onRspQueryMaxOrderVolume))
        .def("onRspSettlementInfoConfirm", pure_virtual(&TdApiWrap::onRspSettlementInfoConfirm))
        .def("onRspRemoveParkedOrder", pure_virtual(&TdApiWrap::onRspRemoveParkedOrder))
        .def("onRspRemoveParkedOrderAction", pure_virtual(&TdApiWrap::onRspRemoveParkedOrderAction))
        .def("onRspExecOrderInsert", pure_virtual(&TdApiWrap::onRspExecOrderInsert))
        .def("onRspExecOrderAction", pure_virtual(&TdApiWrap::onRspExecOrderAction))
        .def("onRspForQuoteInsert", pure_virtual(&TdApiWrap::onRspForQuoteInsert))
        .def("onRspQuoteInsert", pure_virtual(&TdApiWrap::onRspQuoteInsert))
        .def("onRspQuoteAction", pure_virtual(&TdApiWrap::onRspQuoteAction))
        .def("onRspBatchOrderAction", pure_virtual(&TdApiWrap::onRspBatchOrderAction))
        .def("onRspOptionSelfCloseInsert", pure_virtual(&TdApiWrap::onRspOptionSelfCloseInsert))
        .def("onRspOptionSelfCloseAction", pure_virtual(&TdApiWrap::onRspOptionSelfCloseAction))
        .def("onRspCombActionInsert", pure_virtual(&TdApiWrap::onRspCombActionInsert))
        .def("onRspQryOrder", pure_virtual(&TdApiWrap::onRspQryOrder))
        .def("onRspQryTrade", pure_virtual(&TdApiWrap::onRspQryTrade))
        .def("onRspQryInvestorPosition", pure_virtual(&TdApiWrap::onRspQryInvestorPosition))
        .def("onRspQryTradingAccount", pure_virtual(&TdApiWrap::onRspQryTradingAccount))
        .def("onRspQryInvestor", pure_virtual(&TdApiWrap::onRspQryInvestor))
        .def("onRspQryTradingCode", pure_virtual(&TdApiWrap::onRspQryTradingCode))
        .def("onRspQryInstrumentMarginRate", pure_virtual(&TdApiWrap::onRspQryInstrumentMarginRate))
        .def("onRspQryInstrumentCommissionRate", pure_virtual(&TdApiWrap::onRspQryInstrumentCommissionRate))
        .def("onRspQryExchange", pure_virtual(&TdApiWrap::onRspQryExchange))
        .def("onRspQryProduct", pure_virtual(&TdApiWrap::onRspQryProduct))
        .def("onRspQryInstrument", pure_virtual(&TdApiWrap::onRspQryInstrument))
        .def("onRspQryDepthMarketData", pure_virtual(&TdApiWrap::onRspQryDepthMarketData))
        .def("onRspQrySettlementInfo", pure_virtual(&TdApiWrap::onRspQrySettlementInfo))
        .def("onRspQryTransferBank", pure_virtual(&TdApiWrap::onRspQryTransferBank))
        .def("onRspQryInvestorPositionDetail", pure_virtual(&TdApiWrap::onRspQryInvestorPositionDetail))
        .def("onRspQryNotice", pure_virtual(&TdApiWrap::onRspQryNotice))
        .def("onRspQrySettlementInfoConfirm", pure_virtual(&TdApiWrap::onRspQrySettlementInfoConfirm))
        .def("onRspQryInvestorPositionCombineDetail", pure_virtual(&TdApiWrap::onRspQryInvestorPositionCombineDetail))
        .def("onRspQryCFMMCTradingAccountKey", pure_virtual(&TdApiWrap::onRspQryCFMMCTradingAccountKey))
        .def("onRspQryEWarrantOffset", pure_virtual(&TdApiWrap::onRspQryEWarrantOffset))
        .def("onRspQryInvestorProductGroupMargin", pure_virtual(&TdApiWrap::onRspQryInvestorProductGroupMargin))
        .def("onRspQryExchangeMarginRate", pure_virtual(&TdApiWrap::onRspQryExchangeMarginRate))
        .def("onRspQryExchangeMarginRateAdjust", pure_virtual(&TdApiWrap::onRspQryExchangeMarginRateAdjust))
        .def("onRspQryExchangeRate", pure_virtual(&TdApiWrap::onRspQryExchangeRate))
        .def("onRspQrySecAgentACIDMap", pure_virtual(&TdApiWrap::onRspQrySecAgentACIDMap))
        .def("onRspQryProductExchRate", pure_virtual(&TdApiWrap::onRspQryProductExchRate))
        .def("onRspQryProductGroup", pure_virtual(&TdApiWrap::onRspQryProductGroup))
        .def("onRspQryMMInstrumentCommissionRate", pure_virtual(&TdApiWrap::onRspQryMMInstrumentCommissionRate))
        .def("onRspQryMMOptionInstrCommRate", pure_virtual(&TdApiWrap::onRspQryMMOptionInstrCommRate))
        .def("onRspQryInstrumentOrderCommRate", pure_virtual(&TdApiWrap::onRspQryInstrumentOrderCommRate))
        .def("onRspQryOptionInstrTradeCost", pure_virtual(&TdApiWrap::onRspQryOptionInstrTradeCost))
        .def("onRspQryOptionInstrCommRate", pure_virtual(&TdApiWrap::onRspQryOptionInstrCommRate))
        .def("onRspQryExecOrder", pure_virtual(&TdApiWrap::onRspQryExecOrder))
        .def("onRspQryForQuote", pure_virtual(&TdApiWrap::onRspQryForQuote))
        .def("onRspQryQuote", pure_virtual(&TdApiWrap::onRspQryQuote))
        .def("onRspQryOptionSelfClose", pure_virtual(&TdApiWrap::onRspQryOptionSelfClose))
        .def("onRspQryInvestUnit", pure_virtual(&TdApiWrap::onRspQryInvestUnit))
        .def("onRspQryCombInstrumentGuard", pure_virtual(&TdApiWrap::onRspQryCombInstrumentGuard))
        .def("onRspQryCombAction", pure_virtual(&TdApiWrap::onRspQryCombAction))
        .def("onRspQryTransferSerial", pure_virtual(&TdApiWrap::onRspQryTransferSerial))
        .def("onRspQryAccountregister", pure_virtual(&TdApiWrap::onRspQryAccountregister))
        .def("onRspError", pure_virtual(&TdApiWrap::onRspError))
        .def("onRtnOrder", pure_virtual(&TdApiWrap::onRtnOrder))
        .def("onRtnTrade", pure_virtual(&TdApiWrap::onRtnTrade))
        .def("onErrRtnOrderInsert", pure_virtual(&TdApiWrap::onErrRtnOrderInsert))
        .def("onErrRtnOrderAction", pure_virtual(&TdApiWrap::onErrRtnOrderAction))
        .def("onRtnInstrumentStatus", pure_virtual(&TdApiWrap::onRtnInstrumentStatus))
        .def("onRtnBulletin", pure_virtual(&TdApiWrap::onRtnBulletin))
        .def("onRtnTradingNotice", pure_virtual(&TdApiWrap::onRtnTradingNotice))
        .def("onRtnErrorConditionalOrder", pure_virtual(&TdApiWrap::onRtnErrorConditionalOrder))
        .def("onRtnExecOrder", pure_virtual(&TdApiWrap::onRtnExecOrder))
        .def("onErrRtnExecOrderInsert", pure_virtual(&TdApiWrap::onErrRtnExecOrderInsert))
        .def("onErrRtnExecOrderAction", pure_virtual(&TdApiWrap::onErrRtnExecOrderAction))
        .def("onErrRtnForQuoteInsert", pure_virtual(&TdApiWrap::onErrRtnForQuoteInsert))
        .def("onRtnQuote", pure_virtual(&TdApiWrap::onRtnQuote))
        .def("onErrRtnQuoteInsert", pure_virtual(&TdApiWrap::onErrRtnQuoteInsert))
        .def("onErrRtnQuoteAction", pure_virtual(&TdApiWrap::onErrRtnQuoteAction))
        .def("onRtnForQuoteRsp", pure_virtual(&TdApiWrap::onRtnForQuoteRsp))
        .def("onRtnCFMMCTradingAccountToken", pure_virtual(&TdApiWrap::onRtnCFMMCTradingAccountToken))
        .def("onErrRtnBatchOrderAction", pure_virtual(&TdApiWrap::onErrRtnBatchOrderAction))
        .def("onRtnOptionSelfClose", pure_virtual(&TdApiWrap::onRtnOptionSelfClose))
        .def("onErrRtnOptionSelfCloseInsert", pure_virtual(&TdApiWrap::onErrRtnOptionSelfCloseInsert))
        .def("onErrRtnOptionSelfCloseAction", pure_virtual(&TdApiWrap::onErrRtnOptionSelfCloseAction))
        .def("onRtnCombAction", pure_virtual(&TdApiWrap::onRtnCombAction))
        .def("onErrRtnCombActionInsert", pure_virtual(&TdApiWrap::onErrRtnCombActionInsert))
        .def("onRspQryContractBank", pure_virtual(&TdApiWrap::onRspQryContractBank))
        .def("onRspQryParkedOrder", pure_virtual(&TdApiWrap::onRspQryParkedOrder))
        .def("onRspQryParkedOrderAction", pure_virtual(&TdApiWrap::onRspQryParkedOrderAction))
        .def("onRspQryTradingNotice", pure_virtual(&TdApiWrap::onRspQryTradingNotice))
        .def("onRspQryBrokerTradingParams", pure_virtual(&TdApiWrap::onRspQryBrokerTradingParams))
        .def("onRspQryBrokerTradingAlgos", pure_virtual(&TdApiWrap::onRspQryBrokerTradingAlgos))
        .def("onRspQueryCFMMCTradingAccountToken", pure_virtual(&TdApiWrap::onRspQueryCFMMCTradingAccountToken))
        .def("onRtnFromBankToFutureByBank", pure_virtual(&TdApiWrap::onRtnFromBankToFutureByBank))
        .def("onRtnFromFutureToBankByBank", pure_virtual(&TdApiWrap::onRtnFromFutureToBankByBank))
        .def("onRtnRepealFromBankToFutureByBank", pure_virtual(&TdApiWrap::onRtnRepealFromBankToFutureByBank))
        .def("onRtnRepealFromFutureToBankByBank", pure_virtual(&TdApiWrap::onRtnRepealFromFutureToBankByBank))
        .def("onRtnFromBankToFutureByFuture", pure_virtual(&TdApiWrap::onRtnFromBankToFutureByFuture))
        .def("onRtnFromFutureToBankByFuture", pure_virtual(&TdApiWrap::onRtnFromFutureToBankByFuture))
        .def("onRtnRepealFromBankToFutureByFutureManual", pure_virtual(&TdApiWrap::onRtnRepealFromBankToFutureByFutureManual))
        .def("onRtnRepealFromFutureToBankByFutureManual", pure_virtual(&TdApiWrap::onRtnRepealFromFutureToBankByFutureManual))
        .def("onRtnQueryBankBalanceByFuture", pure_virtual(&TdApiWrap::onRtnQueryBankBalanceByFuture))
        .def("onErrRtnBankToFutureByFuture", pure_virtual(&TdApiWrap::onErrRtnBankToFutureByFuture))
        .def("onErrRtnFutureToBankByFuture", pure_virtual(&TdApiWrap::onErrRtnFutureToBankByFuture))
        .def("onErrRtnRepealBankToFutureByFutureManual", pure_virtual(&TdApiWrap::onErrRtnRepealBankToFutureByFutureManual))
        .def("onErrRtnRepealFutureToBankByFutureManual", pure_virtual(&TdApiWrap::onErrRtnRepealFutureToBankByFutureManual))
        .def("onErrRtnQueryBankBalanceByFuture", pure_virtual(&TdApiWrap::onErrRtnQueryBankBalanceByFuture))
        .def("onRtnRepealFromBankToFutureByFuture", pure_virtual(&TdApiWrap::onRtnRepealFromBankToFutureByFuture))
        .def("onRtnRepealFromFutureToBankByFuture", pure_virtual(&TdApiWrap::onRtnRepealFromFutureToBankByFuture))
        .def("onRspFromBankToFutureByFuture", pure_virtual(&TdApiWrap::onRspFromBankToFutureByFuture))
        .def("onRspFromFutureToBankByFuture", pure_virtual(&TdApiWrap::onRspFromFutureToBankByFuture))
        .def("onRspQueryBankAccountMoneyByFuture", pure_virtual(&TdApiWrap::onRspQueryBankAccountMoneyByFuture))
        .def("onRtnOpenAccountByBank", pure_virtual(&TdApiWrap::onRtnOpenAccountByBank))
        .def("onRtnCancelAccountByBank", pure_virtual(&TdApiWrap::onRtnCancelAccountByBank))
        .def("onRtnChangeAccountByBank", pure_virtual(&TdApiWrap::onRtnChangeAccountByBank))

		// render_data.ctp_td_python_module end
		;
}