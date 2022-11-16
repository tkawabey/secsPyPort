#ifdef _MSC_VER
#	pragma warning( disable : 4786 ) 
#endif

#include "xsecsPyPort.h"
#include "structmember.h"
#include "xsecsPyGEM.h"
#include "xsecsPyGEMAPI.h"
#include "xsecsPySECSData.h"
#include "xsecsPySECSItem.h"
#include "wxssp/xsspExternalMdl.h"
#include "xlib/semi/secs/xsecs_pcap_parser.h"
#include "xlib/semi/gem/xsemi_gem.h"
#include "xlib/xeception.h"
#include "xlib/xdata.h"
#include "xlib/xthread.h"

#include <sys/stat.h>
#include <sys/types.h>


#include <map>
#include <strstream>

extern PyObject *g_pError;
xsspExtDllAPIs*	g_APIs = NULL;





#ifdef WIN32
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if( ul_reason_for_call == DLL_PROCESS_ATTACH ) {
//		Py_Initialize();
	} else 
	if( ul_reason_for_call == DLL_PROCESS_DETACH ) {
//		Py_Finalize();
	}
    return TRUE;
}
#endif /*WIN32*/











/**	@brief	The *openLogFromOffline function
 *
 *	@param	self	a parameter of type PyObject *
 *	@param	args	a parameter of type PyObject *
 *
 *	@return	PYTHON_DLL_EXTERN PyObject
 */
PYTHON_DLL_EXTERN PyObject *openLogFromOffline(PyObject *self, PyObject *args);
/**	@brief	The *openXSECSDataFromFile function
 *
 *	@param	self	a parameter of type PyObject *
 *	@param	args	a parameter of type PyObject *
 *
 *	@return	PYTHON_DLL_EXTERN PyObject
 *
 *	
 */
PYTHON_DLL_EXTERN PyObject *openXSECSDataFromFile(PyObject *self, PyObject *args);
/**	@brief	The *saveXSECSDataToFile function
 *
 *	@param	self	a parameter of type PyObject *
 *	@param	args	a parameter of type PyObject *
 *
 *	@return	PYTHON_DLL_EXTERN PyObject
 *
 *	
 */
PYTHON_DLL_EXTERN PyObject *saveXSECSDataToFile(PyObject *self, PyObject *args);

PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_L(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_A(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_J(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_UTF16(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_B(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_BOOL(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_U1(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_U2(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_U4(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_U8(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_I1(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_I2(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_I4(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_I8(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_F4(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_F8(PyObject *self, PyObject *args);


PYTHON_DLL_EXTERN PyObject *XSSP_LoadResource(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *XSSP_FreeResource(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *XSSP_LoadDialog(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *XSSP_DialogShow(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *XSSP_EventConnect(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *XSSP_CloseWnd(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *XSSP_SetWindowText(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *XSSP_GetWindowText(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *XSSP_SetRadiChkValue(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *XSSP_GetRadiChkValue(PyObject *self, PyObject *args);
PYTHON_DLL_EXTERN PyObject *XSSP_EndModal(PyObject *self, PyObject *args);


void EventCallBack(XSSP_HANDLE hWnd, int type, const TCHAR* p_id);


static PyMethodDef module_methods[] = {
	{"DATA_TYPE_L",		get_DATA_TYPE_L,		METH_NOARGS,  "DATA_TYPE_L."},
	{"DATA_TYPE_A",		get_DATA_TYPE_A,		METH_NOARGS,  "DATA_TYPE_A."},
	{"DATA_TYPE_J",		get_DATA_TYPE_J,		METH_NOARGS,  "DATA_TYPE_J."},
	{"DATA_TYPE_UTF16",	get_DATA_TYPE_UTF16,	METH_NOARGS,  "DATA_TYPE_UTF1."},
	{"DATA_TYPE_B",		get_DATA_TYPE_B,		METH_NOARGS,  "DATA_TYPE_B."},
	{"DATA_TYPE_BOOL",	get_DATA_TYPE_BOOL,		METH_NOARGS,  "DATA_TYPE_BOOL."},
	{"DATA_TYPE_U1",	get_DATA_TYPE_U1,		METH_NOARGS,  "DATA_TYPE_U1."},
	{"DATA_TYPE_U2",	get_DATA_TYPE_U2,		METH_NOARGS,  "DATA_TYPE_U2."},
	{"DATA_TYPE_U4",	get_DATA_TYPE_U4, 		METH_NOARGS,  "DATA_TYPE_U4."},
	{"DATA_TYPE_U8",	get_DATA_TYPE_U8, 		METH_NOARGS,  "DATA_TYPE_U8."},
	{"DATA_TYPE_I1",	get_DATA_TYPE_I1, 		METH_NOARGS,  "DATA_TYPE_I1."},
	{"DATA_TYPE_I2",	get_DATA_TYPE_I2, 		METH_NOARGS,  "DATA_TYPE_I2."},
	{"DATA_TYPE_I4",	get_DATA_TYPE_I4, 		METH_NOARGS,  "DATA_TYPE_I4."},
	{"DATA_TYPE_I8",	get_DATA_TYPE_I8, 		METH_NOARGS,  "DATA_TYPE_I8."},
	{"DATA_TYPE_F4",	get_DATA_TYPE_F4, 		METH_NOARGS,  "DATA_TYPE_F4."},
	{"DATA_TYPE_F8",	get_DATA_TYPE_F8, 		METH_NOARGS,  "DATA_TYPE_F8."},

	{"XSSP_LoadResource",	XSSP_LoadResource, 		METH_VARARGS,  "GUI Load Resource Template."},
	{"XSSP_FreeResource",	XSSP_FreeResource, 		METH_VARARGS,  "GUI Free Resource Template."},
	{"XSSP_LoadDialog",		XSSP_LoadDialog, 		METH_VARARGS,  "GUI Load Dialog from Resource."},
	{"XSSP_DialogShow",		XSSP_DialogShow, 		METH_VARARGS,  "GUI Show Dialog."},
	{"XSSP_EventConnect",	XSSP_EventConnect, 		METH_VARARGS,  "GUI EventConnect."},
	{"XSSP_CloseWnd",		XSSP_CloseWnd, 			METH_VARARGS,  "GUI Close Window."},
	{"XSSP_SetWindowText",	XSSP_SetWindowText, 	METH_VARARGS,  "GUI Set Window Text."},
	{"XSSP_GetWindowText",	XSSP_GetWindowText, 	METH_VARARGS,  "GUI Get Window Text."},
	{"XSSP_SetRadiChkValue",	XSSP_SetRadiChkValue, 	METH_VARARGS,  "GUI Set Value from radiobutton, radiobox, checkbotton."},
	{"XSSP_GetRadiChkValue",	XSSP_GetRadiChkValue, 	METH_VARARGS,  "GUI Get Value from radiobutton, radiobox, checkbotton."},
	{"XSSP_EndModal",		XSSP_EndModal, 			METH_VARARGS,  "GUI EndModal."},
//	{"openLogFromOffline",  openLogFromOffline, METH_VARARGS,  "open Log FromOffline."},
//	{"openXSECSDataFromFile",  openXSECSDataFromFile, METH_VARARGS,  "open xsecs data."},
//	{"saveXSECSDataToFile",  saveXSECSDataToFile, METH_VARARGS,  "save XSECS datas."},
	{NULL}  /* Sentinel */
};


PyObject							*g_pError;
std::map<XSEMI_Gem*, PyXSECSGem*>	g_gemInsts;

struct GuiEventMap
{
	GuiEventMap(long hWnd, const TCHAR* p_id, int type, PyObject* pFunc)
	{
		m_hWnd   = hWnd;
		m_strID  = p_id;
		m_type   = type;
		m_pFunc  = pFunc;
	}
	long				m_hWnd;
#ifdef _UNICODE
	std::wstring		m_strID;
#else
	std::string			m_strID;
#endif
	int					m_type;
	PyObject*			m_pFunc;
};
std::vector<GuiEventMap>			g_GuiEventMap;



#include <frameobject.h>
struct tracebackobject {
	PyObject_HEAD
	tracebackobject *tb_next;
	PyFrameObject *tb_frame;
	int tb_lasti;
	int tb_lineno;
};

/**	@brief	Pythonエラーが発生していた場合、エラーを報告する。
 *
 *	@param	pGem	ログ出力する、XSEMI_Gemインスタンス
 *
 *	@return	void
 */
void xsecsPyPortGemErrorOut(XSEMI_Gem *pGem)
{
	XStrBuff _M_msg;
	PyObject *type = NULL, *value = NULL, *traceback = NULL, *pyString = NULL;

	// Extra paranoia...
	if (!PyErr_Occurred()) {
		return ;
	}
	// PyErr_Print();

	PyErr_Fetch(&type, &value, &traceback);
	PyErr_Clear();

	_M_msg.Format(_T("[%s]"), XSECS_PY_PORT);
	if (type != NULL && (pyString=PyObject_Str(type))!=NULL &&  (PyString_Check(pyString)))
		_M_msg << PyString_AsString(pyString);
	else
		_M_msg << _T("<unknown exception type> ");
	XSSPY_XDECREF(pyString);

	if (value != NULL && (pyString=PyObject_Str(value))!=NULL &&  (PyString_Check(pyString))) {
		_M_msg << _T(": ");
		_M_msg << PyString_AsString(pyString);
	} else {
		_M_msg << _T("<unknown exception date> ");
	}
	XSSPY_XDECREF(pyString);


	if (traceback != NULL && PyTraceBack_Check(traceback)) {
		// what belongs here? TBD
		tracebackobject* tb = (tracebackobject*)traceback;

		char buff[2048];
		while (tb != NULL) {
		  sprintf(buff, "\r\n\tfile:%s(%d) in %s\n"
			, PyString_AsString(tb->tb_frame->f_code->co_filename)
			, tb->tb_lineno
			, PyString_AsString(tb->tb_frame->f_code->co_name)
		  );
		  _M_msg << buff;
		  tb = tb->tb_next;
		}

	}
	XSSPY_XDECREF(type);
	XSSPY_XDECREF(value);
	XSSPY_XDECREF(traceback);

	if( pGem != NULL )	{
		pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, _M_msg);
	}
}


extern "C" {
PyXSECSGem*	g_pGem = NULL;



PyMODINIT_FUNC 
/**	@brief	拡張オブジェクトの初期化 */
initxsecsPyPort(void) 
{
    PyObject* pModule;
	// xsecsPyPortモジュールを登録
    pModule = Py_InitModule3("xsecsPyPort", module_methods, "SEMI port Python.");

	// エラーオブジェクトを登録します。
	g_pError = PyErr_NewException("xsecsPyPort.error", NULL, NULL);
    Py_INCREF(g_pError);
	PyModule_AddObject(pModule, "error.xsecsPort", g_pError);


	// PyXSECSGemを登録します。
	if( PyXSECSGem_RegistModule(pModule) == false ) {
		return ;
	}
	// PyXSECSGemAPIを登録します。
	if( PyXSECSGemAPI_RegistModule(pModule) == false ) {
		return ;
	}
	// XSECSDataを登録します。
	if( XSECSData_RegistMudule(pModule) == false ) {
		return ;
	}
	// XSECSItem を登録します。
	if( XSECSItem_RegistMudule(pModule) == false ) {
		return ;
	}
}





PYTHON_DLL_EXTERN

/**	@brief	PyXSEMIGemインスタンスの初期化
 *
 *	@param	pGem	Gemインスタンス
 *	@param	pAPI	API
 *	@param	iParamNum	パラメータ数
 *	@param	pParameters	パラメータ
 *	@param	pErr	エラー発生時にエラー理由を受け取るバッファ
 *
 *	@retval	true	成功
 *	@retval	false	失敗
 */
bool XSEMI_initExternalGemPort(XSEMI_Gem *pGem, xsspExtDllAPIs* pAPI, int iParamNum, TCHAR** pParameters, TCHAR *pErr)
{
	bool	bRetVal = false;
	XStrBuff	strErrMsg;

	g_APIs = pAPI;
	try {
		PyObject*	pInitFunc = NULL;
		PyObject*	pModule = NULL;
		PyXSECSGem* pNewInstance = NULL;
		PyObjectHelper<PyObject> nameObject;


		if( iParamNum <= 1 ) {
			strErrMsg.Format(_T("[%s] parameter error!!. parameter must [Module] [Init Function name] [...]."), XSECS_PY_PORT);
			pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErrMsg);
			return bRetVal;
		}
		// Pythonの初期化
		Py_Initialize();

		// パイソンスクリプトをロード をロードする。
		pGem->writeLogCallBack(XLIB_LOG_LEVEL_DEBUG, _T("  Loading Script..."));
#ifdef _UNICODE
		XStrBuff strPyFileName(pParameters[0]);
		nameObject = (const char*)strPyFileName;
#else
		nameObject = (const char*)pParameters[0];
#endif
		if( nameObject == NULL ) {
			strErrMsg.Format(_T("[%s] python create object error."), XSECS_PY_PORT);
			pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErrMsg);
			return bRetVal;
		}
		pGem->writeLogCallBack(XLIB_LOG_LEVEL_DEBUG, _T("  Import Module..."));
		pModule = PyImport_Import(nameObject);
		if( pModule == NULL ) {
			xsecsPyPortGemErrorOut(pGem);
			strErrMsg.Format(_T("[%s] fail to load [%s.py]."), XSECS_PY_PORT, pParameters[0]);
			pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErrMsg);
			return bRetVal;
		}

		// Gemオブジェクト作成
		pGem->writeLogCallBack(XLIB_LOG_LEVEL_DEBUG, _T("  New Py_GemInstance..."));
		pNewInstance = PyXSECSGem_GCNew( (long)pGem );
		g_gemInsts.insert(std::pair<XSEMI_Gem*, PyXSECSGem*>(pGem, pNewInstance) );
		pNewInstance->m_pModule = pModule;


		// 関数receiveCallBackを探す。
		pGem->writeLogCallBack(XLIB_LOG_LEVEL_DEBUG, _T("  Getiing Dictionary..."));
		pNewInstance->m_pDictionary = PyModule_GetDict( pNewInstance->m_pModule );
		if( pNewInstance->m_pDictionary == NULL ) {
			strErrMsg.Format(_T("[%s] python get dictionary error. [%s.py] in xsecsPyPort init."), XSECS_PY_PORT, pParameters[0]);
			pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErrMsg);
			return bRetVal;
		}


		{
			// 初期化関数をコール
			PyObjectHelper<PyObject>	pArgs   = PyTuple_New(    2    );
			PyObjectHelper<PyObject>	pReturnValue;
			PyListObject*				pList	= NULL;

			pGem->writeLogCallBack(XLIB_LOG_LEVEL_DEBUG, _T("  Find Init Function..."));
#ifdef _UNICODE
			pInitFunc = PyDict_GetItemString(pNewInstance->m_pDictionary, XStrBuff(pParameters[1]));
#else
			pInitFunc = PyDict_GetItemString(pNewInstance->m_pDictionary, pParameters[1]);
#endif
			if( pInitFunc == NULL ) {
				strErrMsg.Format(_T("[%s] fail to load Python Function. ")
					_T("Function:[%s] Module:[%s.py]."), XSECS_PY_PORT, pParameters[1], pParameters[0]);
				pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErrMsg);
				return bRetVal;
			}
			if (!PyCallable_Check(pInitFunc)) {
				strErrMsg.Format(_T("[%s] . must be callable. Function:[%s] Module:[%s.py]."), XSECS_PY_PORT, pParameters[1], pParameters[0]);
				pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErrMsg);
				return bRetVal;
			}

			Py_INCREF(pNewInstance);


			pList	= (PyListObject*)PyList_New(0);
			PyTuple_SetItem(pArgs, 0, (PyObject*)pNewInstance);
			PyTuple_SetItem(pArgs, 1, (PyObject*)pList);
			for(int i = 0; i < iParamNum; i++) {
				PyObject* pStr = Py_BuildValue("s", (const char*)pParameters[i]);
				PyList_Append((PyObject*)pList, (PyObject*)pStr);
			}			
			
			pGem->writeLogCallBack(XLIB_LOG_LEVEL_DEBUG, _T("  Call Init Function..."));
			pReturnValue = PyObject_CallObject(pInitFunc, pArgs);
			if( pReturnValue == NULL ) {
				xsecsPyPortGemErrorOut(pGem);
			} else {
				if( PyInt_Check(pReturnValue) ) {
					long lVal = PyInt_AsLong(pReturnValue);
					if( lVal == 1 ) {
						bRetVal = true;
					}
				} else {
					strErrMsg.Format(_T("[%s] . invalie return value type. [%s] Module:[%s.py]."), XSECS_PY_PORT, pParameters[1], pParameters[0]);
					pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErrMsg);
				}
			}
		}
	} catch(...) {
		strErrMsg.Format(_T("[%s] invalid exception occur in xsecsPyPort init.\n")
						 _T("\tfile:%s(%d)"), XSECS_PY_PORT, (const TCHAR*)XStrBuff(__FILE__), __LINE__);
		pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErrMsg);
	}
	return bRetVal;
}




PYTHON_DLL_EXTERN
void XSEMI_releaseExternalGemPort(XSEMI_Gem *pGem)
{
	try {
		std::map<XSEMI_Gem*, PyXSECSGem*>::iterator iteGemObj;
		PyXSECSGem*	pPyGemObj = NULL;


		// PyXSECSGemオブジェクトを探す
		iteGemObj = g_gemInsts.find( pGem );
		if( g_gemInsts.end() != iteGemObj ) {
			pPyGemObj = (*iteGemObj).second;
		}
		if( pPyGemObj == NULL ) {
			xsecsPyPortGemErrorOut(pGem);
			return ;
		}

		do{
			// 開放関数をコール
			PyObjectHelper<PyObject>  pArgs   = PyTuple_New(    1    );
			PyObjectHelper<PyObject>  pValue;
			if( pArgs == NULL ) {
				xsecsPyPortGemErrorOut(pGem);
				break;
			}
			Py_INCREF(pPyGemObj);

			PyTuple_SetItem(pArgs, 0, (PyObject*)pPyGemObj);
			pValue = PyObject_CallObject(pPyGemObj->m_pReleaseModule, pArgs);



			XSSPY_XDECREF( pPyGemObj );
		}while(0);

		XSSPY_XDECREF(pPyGemObj);
		g_gemInsts.erase( iteGemObj );
	} catch (...) {
	}
	g_APIs = NULL;
//	Py_Finalize();
}


PYTHON_DLL_EXTERN
/**	@brief	The XSEMI_Gem_ReceiveDataCallBack function
 *
 *	@param	pGem	a parameter of type XSEMI_Gem *
 *	@param	header	a parameter of type XSECSHeader&
 *	@param	pMsgData	a parameter of type XBIN*
 *	@param	ulMsgSize	a parameter of type XD_UINT4
 *
 *	@return	bool
 */
int XSEMI_ReceiveDataCallBack(XSSP_HANDLE hParent, XSEMI_Gem *pGem, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize)
{
	try {
		std::map<XSEMI_Gem*, PyXSECSGem*>::iterator iteGemObj;
		std::map<long, PyObject*>::iterator ite;
		unsigned short stream   = (unsigned short)(header.byte2 & ~0x80);
		unsigned short function = (unsigned short)header.status;
		long sxfx = (stream << 16 ) | function;
		PyObject*	pFunc = NULL;
		PyXSECSGem*	pPyGemObj = NULL;

		// PyXSECSGemオブジェクトを探す
		iteGemObj = g_gemInsts.find( pGem );
		if( g_gemInsts.end() != iteGemObj ) {
			pPyGemObj = (*iteGemObj).second;
		}
		if( pPyGemObj == NULL ) {
			xsecsPyPortGemErrorOut(pGem);
			return -1;
		}


		// 指定されたSxFxのコールバックが登録されているか調べる
		ite = pPyGemObj->m_pSxFx->find(sxfx);
		if( pPyGemObj->m_pSxFx->end() == ite ) {
			return 0;
		}
		pFunc = (*ite).second;
		if( pFunc == NULL ) {
			xsecsPyPortGemErrorOut(pGem);
			return -1;
		}
		PyObjectHelper<XSECSData> pData   = XSECSData_GCNew();
		PyObjectHelper<PyObject>  pArgs   = PyTuple_New(    2    );
		PyObjectHelper<PyObject>  pValue;
		if(pData == NULL || pArgs == NULL ) {
			xsecsPyPortGemErrorOut(pGem);
			return -1;
		}
		Py_INCREF(pPyGemObj);
		pData.increment_reference();
		PyTuple_SetItem(pArgs, 0, (PyObject*)pPyGemObj);
		PyTuple_SetItem(pArgs, 1, (PyObject*)pData.m_pT);

		pData->m_stream   = (int)(header.byte2 & ~0x80);
		pData->m_function = (int)(header.status);
		pData->m_wait     = ( header.byte2 & 0x80 ) ? true : false;
		::memcpy(&pData->m_ticket, header.tr_id, sizeof(pData->m_ticket));

		XDataReader reader(pMsgData+10,  ulMsgSize-10 );
		XData       data;
		while( reader.getValue(&data) ) {
			XSECSItem_loadStream((XSECSItem*)pData->m_data, reader, data);
		}
		pValue = PyObject_CallObject(pFunc, pArgs);
		if( pValue == NULL ) {
			xsecsPyPortGemErrorOut(pGem);
			return -1;
		}
		int iRetVal = PyInt_AsLong(pValue);
		return iRetVal;
	} catch (XExcept &e) {
		XInflateStrstream errStrm;
		e.print( errStrm );
		pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, errStrm.str() );
	} catch (...) {
		XStrBuff strErr;
		strErr.Format(_T("<unkown exception> FILE:[%s(%d)]"), (const TCHAR*)XStrBuff(__FILE__), __LINE__);
		pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, (const TCHAR*)strErr );
	}
	return -1;
}




PYTHON_DLL_EXTERN
/**	@brief	The XSEMI_Gem_ReceiveDataCallBack function
 *
 *	@param	pGem	a parameter of type XSEMI_Gem *
 *	@param	header	a parameter of type XSECSHeader&
 *	@param	pMsgData	a parameter of type XBIN*
 *	@param	ulMsgSize	a parameter of type XD_UINT4
 *
 *	@return	bool
 */
int XSEMI_ReceiveResponseByNoWaitCallBack(XSSP_HANDLE hParent, XSEMI_Gem *pGem, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize)
{
/*
	try {
		std::map<XSEMI_Gem*, PyXSECSGem*>::iterator iteGemObj;
		std::map<long, PyObject*>::iterator ite;
		unsigned short stream   = (unsigned short)(header.byte2 & ~0x80);
		unsigned short function = (unsigned short)header.status;
		long sxfx = (stream << 16 ) | function;
		PyObject*	pFunc = NULL;
		PyXSECSGem*	pPyGemObj = NULL;

		// PyXSECSGemオブジェクトを探す
		iteGemObj = g_gemInsts.find( pGem );
		if( g_gemInsts.end() != iteGemObj ) {
			pPyGemObj = (*iteGemObj).second;
		}
		if( pPyGemObj == NULL ) {
			xsecsPyPortGemErrorOut(pGem);
			return -1;
		}


		// 指定されたSxFxのコールバックが登録されているか調べる
		ite = pPyGemObj->m_pSxFx->find(sxfx);
		if( pPyGemObj->m_pSxFx->end() == ite ) {
			return 0;
		}
		pFunc = (*ite).second;
		if( pFunc == NULL ) {
			xsecsPyPortGemErrorOut(pGem);
			return -1;
		}
		PyObjectHelper<XSECSData> pData   = XSECSData_GCNew();
		PyObjectHelper<PyObject>  pArgs   = PyTuple_New(    2    );
		PyObjectHelper<PyObject>  pValue;
		if(pData == NULL || pArgs == NULL ) {
			xsecsPyPortGemErrorOut(pGem);
			return -1;
		}
		Py_INCREF(pPyGemObj);
		pData.increment_reference();
		PyTuple_SetItem(pArgs, 0, (PyObject*)pPyGemObj);
		PyTuple_SetItem(pArgs, 1, (PyObject*)pData.m_pT);

		pData->m_stream   = (int)(header.byte2 & ~0x80);
		pData->m_function = (int)(header.status);
		pData->m_wait     = ( header.byte2 & 0x80 ) ? true : false;
		::memcpy(&pData->m_ticket, header.tr_id, sizeof(pData->m_ticket));

		XDataReader reader(pMsgData+10,  ulMsgSize-10 );
		XData       data;
		while( reader.getValue(&data) ) {
			XSECSItem_loadStream((XSECSItem*)pData->m_data, reader, data);
		}
		pValue = PyObject_CallObject(pFunc, pArgs);
		if( pValue == NULL ) {
			xsecsPyPortGemErrorOut(pGem);
			return -1;
		}
		int iRetVal = PyInt_AsLong(pValue);
		return iRetVal;
	} catch (XExcept &e) {
		XInflateStrstream errStrm;
		e.print( errStrm );
		pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, errStrm.str() );
	} catch (...) {
		XStrBuff strErr;
		strErr.Format(_T("<unkown exception> FILE:[%s(%d)]"), (const TCHAR*)XStrBuff(__FILE__), __LINE__);
		pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, (const TCHAR*)strErr );
	}
*/
	return -1;
}

PYTHON_DLL_EXTERN
/**	@brief	The XSEMI_UserCommand function
 *
 *	@param	pGem	a parameter of type XSEMI_Gem *
 *	@param	argc	a parameter of type int
 *	@param	argv	a parameter of type char**
 *
 *	@return	int
 *
 *	
 */
int XSEMI_UserCommand(XSSP_HANDLE hParent, XSEMI_Gem *pGem, int argc, TCHAR** argv)
{
	try {
		std::map<XSEMI_Gem*, PyXSECSGem*>::iterator iteGemObj;
		PyXSECSGem*	pPyGemObj = NULL;

		// PyXSECSGemオブジェクトを探す
		iteGemObj = g_gemInsts.find( pGem );
		if( g_gemInsts.end() != iteGemObj ) {
			pPyGemObj = (*iteGemObj).second;
		}
		if( pPyGemObj == NULL ) {
			xsecsPyPortGemErrorOut(pGem);
			return -1;
		}
		if( pPyGemObj->m_pUserCommand == NULL ) {
			// ユーザーコマンドが登録されていない
			xsecsPyPortGemErrorOut(pGem);
			return -1;
		}

	
		// 引数の配列オブジェクトを作成
		PyObjectHelper<PyObject>  pArgs   = PyTuple_New(    2    );
		PyObjectHelper<PyObject>  pValue;
		PyListObject *pList					= (PyListObject*)PyList_New(0);

		for(int i = 0; i < argc; i++) {
			PyObject* pStr = Py_BuildValue("s", (const char*)argv[i]);
			PyList_Append((PyObject*)pList, (PyObject*)pStr);
		}
		Py_INCREF(pPyGemObj);
		PyTuple_SetItem(pArgs, 0, (PyObject*)pPyGemObj);
		PyTuple_SetItem(pArgs, 1, (PyObject*)pList);



		pValue = PyObject_CallObject(pPyGemObj->m_pUserCommand, pArgs);
		XSSPY_XDECREF(pList);
		if( pValue == NULL ) {
			xsecsPyPortGemErrorOut(pGem);
			return -1;
		}

		int iRetVal = PyInt_AsLong(pValue);
		return iRetVal;
	} catch (XExcept &e) {
		XInflateStrstream errStrm;
		e.print( errStrm );
		pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, errStrm.str() );
	} catch (...) {
		XStrBuff strErr;
		strErr.Format(_T("<unkown exception> FILE:[%s(%d)]"), (const TCHAR*)XStrBuff(__FILE__), __LINE__);
		pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, (const TCHAR*)strErr );
	}
	return -1;
}

PYTHON_DLL_EXTERN
/**	@brief	Vの値を取得するコールバック関数
 *
 *	@param	pGem	XSEMI_Gemのインスタンス
 *	@param	ceid	CEID
 *	@param	vid	VID
 *	@param	type	データタイプ
 *	@param	pBuffer	バッファ
 *	@param	ulBufferSize	バッファサイズ
 *
 *	@retval	-1	アプリケーションエラーが発生
 *	@retval	-2	実装にないVIDが指定された
 *	@retval	-3	バッファが少ない
 *
 *	
 */
int XSEMI_GetValue(XSEMI_Gem *pGem, const XD_UINT4 ceid, const XD_UINT4 vid, const XDATA_TYPE type, 
				   XBIN* pBuffer,  const XD_UINT4 ulBufferSize )
{
	try {
		std::map<XSEMI_Gem*, PyXSECSGem*>::iterator iteGemObj;
		PyXSECSGem*	pPyGemObj = NULL;

		// PyXSECSGemオブジェクトを探す
		iteGemObj = g_gemInsts.find( pGem );
		if( g_gemInsts.end() != iteGemObj ) {
			pPyGemObj = (*iteGemObj).second;
		}
		if( pPyGemObj == NULL ) {
			xsecsPyPortGemErrorOut(pGem);
			return -1;
		}
		if( pPyGemObj->m_pGetEx == NULL ) {
			// ユーザーコマンドが登録されていない
			xsecsPyPortGemErrorOut(pGem);
			return -1;
		}


		// 引数の配列オブジェクトを作成
		PyObjectHelper<PyObject>  pArgs		= PyTuple_New(    4    );
		PyObjectHelper<PyObject>  pValue;
		PyObjectHelper<XSECSItem> pSecsItem;	

		pSecsItem				= XSECSItem_GCNew();
		pSecsItem->m_type = type;
		XSECSItem_ZeroInit(pSecsItem);


		Py_INCREF(pPyGemObj);
		PyTuple_SetItem(pArgs, 0, (PyObject*)pPyGemObj);
		PyTuple_SetItem(pArgs, 1, (PyObject*)PyInt_FromLong(ceid));
		PyTuple_SetItem(pArgs, 2, (PyObject*)PyInt_FromLong(vid));
		PyTuple_SetItem(pArgs, 3, (PyObject*)(XSECSItem*)pSecsItem);
		pValue = PyObject_CallObject(pPyGemObj->m_pGetEx, pArgs);
		if( pValue == NULL ) {
			xsecsPyPortGemErrorOut(pGem);
			return -1;
		}
		int iRetVal = PyInt_AsLong(pValue);

		std::ostrstream stream((char*)pBuffer, ulBufferSize);
		XDataWriter			dataWriter( &stream );
		if( XSECSItem_storeStream(dataWriter, pSecsItem, type == XDATA_TYPE_L ? false : true ) == false) {
			XTRACE(_T("ERR 3\n"));
		}
		iRetVal = stream.pcount();
		return iRetVal;
	} catch (XExcept &e) {
		XInflateStrstream errStrm;
		e.print( errStrm );
		pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, errStrm.str() );
	} catch (...) {
		XStrBuff strErr;
		strErr.Format(_T("<unkown exception> FILE:[%s(%d)]"), (const TCHAR*)XStrBuff(__FILE__), __LINE__);
		pGem->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, (const TCHAR*)strErr );
	}
	return -1;
}


/**	@brief	V値を設定するコールバック関数
 *
 *	@param	pGem	XSEMI_Gemのインスタンス
 *	@param	vid	VID
 *	@param	type	データタイプ
 *	@param	pBuffer	バッファ
 *	@param	ulBufferSize	バッファサイズ
 *
 *	@return	int
 */
int XSEMI_PutValue(
	   XSEMI_Gem *pGem, 
	   const XD_UINT4 vid, 
	   const XDATA_TYPE type, 
	   const XBIN* pBuffer,  
	   const XD_UINT4 ulBufferSize )
{

	return -1;
}

}



static void openLogFromOfflineCallback(XSecsPcapData* pPcapData,void* pUserDara)
{
	PyListObject *pList = (PyListObject*)pUserDara;

	if( pPcapData->m_type == XSECS_HSMS_TYPE_DATA ) {
		XSECSData *pData = XSECSData_GCNew();
		if(pData == NULL ) {
			return ;
		}
		pData->m_stream = pPcapData->m_secsHeader.byte2 & 0x80 ? 
			pPcapData->m_secsHeader.byte2 - 0x80 : pPcapData->m_secsHeader.byte2;
		pData->m_function = pPcapData->m_secsHeader.status;
		pData->m_wait = pPcapData->m_secsHeader.byte2 & 0x80 ? true : false;
		pData->m_timeSec  = ::mktime(&pPcapData->m_Time);
		pData->m_timeUsec = pPcapData->m_ulTimeUSec;

		pData->m_ipSrc = pPcapData->m_ip_src.S_un.S_addr;
		pData->m_ipDst = pPcapData->m_ip_dst.S_un.S_addr;
		pData->m_portSrc = pPcapData->m_port_src;
		pData->m_portDst = pPcapData->m_port_dst;
		::memcpy(&pData->m_deviceID, &pPcapData->m_secsHeader.sessionID, sizeof(pPcapData->m_secsHeader.sessionID));
		::memcpy(&pData->m_ticket, &pPcapData->m_secsHeader.tr_id, sizeof(pPcapData->m_secsHeader.tr_id));


		XDataReader reader(pPcapData->m_pData+14,  pPcapData->m_length-14 );
		XData       data;

		while( reader.getValue(&data) ) {
			XSECSItem_loadStream((XSECSItem*)pData->m_data, reader, data);
		}
		PyList_Append((PyObject*)pList, (PyObject*)pData);
	}
}

PYTHON_DLL_EXTERN PyObject *openLogFromOffline(PyObject *self, PyObject *args)
{
    const char *p_file_path;

    if (!PyArg_ParseTuple(args, "s", &p_file_path))
        return NULL;

	PyListObject *pList = (PyListObject*)PyList_New(0);

	try {
		XSecsPcapParser* parser = XSecsPcapParser::newInstance();
		parser->setCallBackFunction(openLogFromOfflineCallback, pList);
		parser->parse(p_file_path);

		XSecsPcapParser::deleteInstance(&parser);
	} catch(XExcept &ex ) {
		PyErr_SetString(g_pError, ex.what());
		return NULL;
	}

    return Py_BuildValue("O", pList);
}

PYTHON_DLL_EXTERN PyObject *openXSECSDataFromFile(PyObject *self, PyObject *args)
{
	const char *p_file_path;

	if (!PyArg_ParseTuple(args, "s", &p_file_path))
		return NULL;

	PyListObject *pList = (PyListObject*)PyList_New(0);

	try {
		XD_UINT4 ulFileSize;
		struct stat fileInfo;
		std::fstream fst;
		XBIN *pBuff;
		XBuff dataBuff;

		// ファイル情報を取得
		if( ::stat(p_file_path, &fileInfo) != 0 ) {
			PyErr_SetString(g_pError, "invalid file path.");
			return NULL;
		}
		ulFileSize = fileInfo.st_size;
		pBuff = (XBIN*)dataBuff.Alloc(ulFileSize+100);
		if( pBuff == NULL ) {
			PyErr_SetString(PyExc_MemoryError, "");
			return NULL;
		}
		dataBuff.MemSet();

		fst.open(p_file_path, std::ios::in 
#ifdef _WIN32
			| std::ios::binary
#endif /* _WIN32 */
			);
		if( !fst.is_open() ) {
			PyErr_SetString(g_pError, "fail to open file");
			return NULL;
		}
		fst.read((char*)pBuff, ulFileSize);
		if( fst.gcount() != ulFileSize) {
			PyErr_SetString(g_pError, "invalid data file.");
			return NULL;
		}
		fst.close();

		while( ulFileSize >= 34 ) { // time(4+4) + IP(4*2) + Port(2*2) datasize(4) + SECSheader(10)
			XD_UINT4 ulDataSize;
			XSECSData *pData = XSECSData_GCNew();

			// 時間の取得
			pData->m_timeSec = XDataCoder::_ntoh4( *((long*)pBuff) );
			pBuff += sizeof(XD_UINT4);
			pData->m_timeUsec = XDataCoder::_ntoh4( *((long*)pBuff) );
			pBuff += sizeof(XD_UINT4);
			// IP&Portの取得
			pData->m_ipSrc = XDataCoder::_ntoh4( *((long*)pBuff) );
			pBuff += sizeof(XD_UINT4);
			pData->m_portSrc = XDataCoder::_ntoh2( *((short*)pBuff) );
			pBuff += sizeof(XD_UINT2);
			pData->m_ipDst = XDataCoder::_ntoh4( *((long*)pBuff) );
			pBuff += sizeof(XD_UINT4);
			pData->m_portDst = XDataCoder::_ntoh2( *((short*)pBuff) );
			pBuff += sizeof(XD_UINT2);

			// データサイズ
			ulDataSize = XDataCoder::_ntoh4( *((long*)pBuff) );
			pBuff += sizeof(XD_UINT4);

			// SECS header
			XSECSHeader header;
			::memcpy(&header, pBuff, sizeof(XSECSHeader));
			pBuff += sizeof(XSECSHeader);


			pData->m_stream = (int)(header.byte2 & 0x80 ? header.byte2 - 0x80 : header.byte2);
			pData->m_function = (int)(header.status);
			pData->m_wait =(header.byte2 & 0x80 ? true : false);
			pData->m_deviceID = XDataCoder::_ntoh2( header.sessionID );
			pData->m_ticket = XDataCoder::_ntoh2( *((short*)header.tr_id) );


			ulFileSize -= ( sizeof(XD_UINT4) + sizeof(XD_UINT4) + sizeof(XD_UINT4) + sizeof(XD_UINT2) +
				sizeof(XD_UINT4) + sizeof(XD_UINT2) + sizeof(XD_UINT4) + sizeof(XSECSHeader));



			XDataReader reader(pBuff,  ulDataSize-10 );
			XData       data;

			while( reader.getValue(&data) ) {
				XSECSItem_loadStream((XSECSItem*)pData->m_data, reader, data);
			}
			pBuff += (ulDataSize-10);
			ulFileSize -= (ulDataSize-10);


			PyList_Append((PyObject*)pList, (PyObject*)pData);
		}

	} catch(XExcept &ex ) {
		PyErr_SetString(g_pError, ex.what());
		return NULL;
	}

	return Py_BuildValue("O", pList);
}




PYTHON_DLL_EXTERN PyObject *saveXSECSDataToFile(PyObject *self, PyObject *args)
{
    const char *p_file_path = NULL;
	PyObject*	p_datas = NULL;

    if (!PyArg_ParseTuple(args, "Os", &p_datas, &p_file_path))
        return NULL;

	try {
		int iSize, i;
		long  l4Data = 0;
		short l2Data = 0;

		if( !PyList_Check(p_datas) ) {
			return NULL;
		}
		iSize = PyList_Size(p_datas);

		std::fstream fst;
		fst.open(p_file_path, std::ios::out 
#ifdef _WIN32
			| std::ios::binary
#endif /* _WIN32 */
			);


		for(i = 0; i < iSize; i++) {
			PyObject* pObje = PyList_GetItem(p_datas, i);
			if( !XSECSData_CheckType(pObje) ) {
				return NULL;
			}
			XSECSData* pData = (XSECSData*)pObje;

			l4Data = XDataCoder::_hton4(pData->m_timeSec);
			fst.write((const char *)&l4Data, sizeof(l4Data));
			l4Data = XDataCoder::_hton4(pData->m_timeUsec);
			fst.write((const char *)&l4Data, sizeof(l4Data));
			l4Data = XDataCoder::_hton4(pData->m_ipSrc);
			fst.write((const char *)&l4Data, sizeof(l4Data));
			l2Data = XDataCoder::_hton2(pData->m_portSrc);
			fst.write((const char *)&l2Data, sizeof(l2Data));
			l4Data = XDataCoder::_hton4(pData->m_ipDst);
			fst.write((const char *)&l4Data, sizeof(l4Data));
			l2Data = XDataCoder::_hton2(pData->m_portDst);
			fst.write((const char *)&l2Data, sizeof(l2Data));


			// ヘッダーをバッファに書き込み
			XSECSHeader head;
			XInflateStreamBuff buff(255);
			std::ostream bstream(&buff);

			::memset(&head, 0, sizeof(head));
			head.sessionID = XDataCoder::_hton2(pData->m_deviceID);
			l2Data = XDataCoder::_hton2(pData->m_ticket);
			::memcpy(head.tr_id, &l2Data, sizeof(l2Data));
			head.byte2 = (XBIN)pData->m_stream;
			if( pData->m_wait == 1 ) {
				head.byte2 |= 0x80;
			}
			head.status = (XBIN)pData->m_function;


			bstream.write((const char *)&head, sizeof(head));


			if( !XSECSItem_CheckType(pData->m_data) ) {
				return NULL;
			}
	
			
			XDataWriter writer(&bstream);			
			if( XSECSItem_storeStream(writer, (XSECSItem*)pData->m_data) == false ) {
				XTRACE(_T("ERR 5\n"));
			}

			// バッファのサイズを取得
			l4Data = XDataCoder::_hton4( buff.size() );
			fst.write((const char *)&l4Data, sizeof(l4Data));
			if( buff.size() ) {
				fst.write((const char*)buff.getBuff(), buff.size());
			}
		}

		fst.close();
	} catch(XExcept &ex ) {
		PyErr_SetString(g_pError, ex.what());
		return NULL;
	}
    return Py_BuildValue("i", 0);
}


PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_L(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_L);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_A(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_A);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_J(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_J);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_UTF16(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_UTF16);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_B(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_B);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_BOOL(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_BOOL);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_U1(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_U1);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_U2(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_U2);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_U4(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_U4);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_U8(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_U8);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_I1(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_I1);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_I2(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_I2);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_I4(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_I4);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_I8(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_I8);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_F4(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_F4);}
PYTHON_DLL_EXTERN PyObject *get_DATA_TYPE_F8(PyObject *self, PyObject *args){    return Py_BuildValue("i", XD2_F8);}





PYTHON_DLL_EXTERN 

PyObject *XSSP_LoadResource(PyObject *self, PyObject *args)
{
	try {
		XStrBuff	strTmp;
		if( g_APIs == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}
		if( g_APIs->XSSP_LoadResource == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}

	   const char *p_file_path = NULL;
		PyObject*	p_datas = NULL;

		if (!PyArg_ParseTuple(args, "s", &p_file_path)) {
			return NULL;
		}
#ifdef _UNICODE
		long lHandle = (long)g_APIs->XSSP_LoadResource(XStrBuff(p_file_path));
#else
		long lHandle = (long)g_APIs->XSSP_LoadResource(p_file_path);
#endif
		if( lHandle == 0 ) {
			strTmp.Format(_T("fail to load resource.[%s]"), p_file_path);
			PyErr_SetString(g_pError, strTmp);
			return NULL;
		}

		return Py_BuildValue("i", lHandle);
	} catch(XExcept &ex ) {
		PyErr_SetString(g_pError, ex.what());
		return NULL;
	}
	return NULL;
}


PYTHON_DLL_EXTERN 

PyObject *XSSP_FreeResource(PyObject *self, PyObject *args)
{
	try {
		if( g_APIs == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}
		if( g_APIs->XSSP_FreeResource == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}

		int handle;

		if (!PyArg_ParseTuple(args, "i", &handle))
			return NULL;

		g_APIs->XSSP_FreeResource((XSSP_HANDLE)handle);
		return Py_None;
	} catch(XExcept &ex ) {
		PyErr_SetString(g_pError, ex.what());
		return NULL;
	}
	return NULL;
}


PYTHON_DLL_EXTERN 

PyObject *XSSP_LoadDialog(PyObject *self, PyObject *args)
{
	try {
		XStrBuff	strTmp;

		if( g_APIs == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}
		if( g_APIs->XSSP_LoadDialog == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}

		const char *p_res_name = NULL;
		int hRes;

		if (!PyArg_ParseTuple(args, "is", &hRes, &p_res_name))
			return NULL;
#ifdef _UNICODE
		long lHandle = (long)g_APIs->XSSP_LoadDialog(0, (XSSP_HANDLE)hRes, XStrBuff(p_res_name) );
#else
		long lHandle = (long)g_APIs->XSSP_LoadDialog(0, (XSSP_HANDLE)hRes, p_res_name);
#endif
		if( lHandle == 0 ) {
			strTmp.Format(_T("fail to load dialog.[%s]"), p_res_name);
			PyErr_SetString(g_pError, strTmp);
			return NULL;
		}

		return Py_BuildValue("i", lHandle);
	} catch(XExcept &ex ) {
		PyErr_SetString(g_pError, ex.what());
		return NULL;
	}
	return NULL;
}
PYTHON_DLL_EXTERN 

PyObject *XSSP_DialogShow(PyObject *self, PyObject *args)
{
	try {
		if( g_APIs == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}
		if( g_APIs->XSSP_DialogShow == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}

		int hRes;

		if (!PyArg_ParseTuple(args, "i", &hRes))
			return NULL;

		int retcode = g_APIs->XSSP_DialogShow((XSSP_HANDLE)hRes);
		return Py_BuildValue("i", retcode);
	} catch(XExcept &ex ) {
		PyErr_SetString(g_pError, ex.what());
		return NULL;
	}
	return NULL;
}




PYTHON_DLL_EXTERN 

PyObject *XSSP_EventConnect(PyObject *self, PyObject *args)
{
	try {
		if( g_APIs == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}
		if( g_APIs->XSSP_EventConnect == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}

		int					hWnd = 0;
		const char*			p_id = NULL;
		int					eventtype = 0;
		PyObject			*p_Func = NULL;

		if (!PyArg_ParseTuple(args, "isiO", &hWnd, &p_id, &eventtype, &p_Func))
			return NULL;
		if( PyCallable_Check(p_Func) == 0 ) {
			PyErr_SetString(PyExc_TypeError, "function must be callable");
			return NULL;
		}
		Py_INCREF( p_Func );

#ifdef _UNICODE
		g_APIs->XSSP_EventConnect((XSSP_HANDLE)hWnd, XStrBuff(p_id), eventtype, EventCallBack);
#else
		g_APIs->XSSP_EventConnect((XSSP_HANDLE)hWnd, p_id, eventtype, EventCallBack);
#endif

		g_GuiEventMap.push_back( GuiEventMap( hWnd, XStrBuff(p_id), eventtype, p_Func ) );


		return Py_None;
	} catch(XExcept &ex ) {
		PyErr_SetString(g_pError, ex.what());
		return NULL;
	}
	return NULL;
}




PYTHON_DLL_EXTERN 

PyObject *XSSP_CloseWnd(PyObject *self, PyObject *args)
{
	try {
		if( g_APIs == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}
		if( g_APIs->XSSP_CloseWnd == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}

		int hWnd;

		if (!PyArg_ParseTuple(args, "i", &hWnd))
			return NULL;



		// イベントマップのクリア
		std::vector<GuiEventMap>::iterator ite = g_GuiEventMap.begin();
		for(; ite != g_GuiEventMap.end(); ite++) {
			if( (*ite).m_hWnd == hWnd  ) {
				g_GuiEventMap.erase(ite);
				ite = g_GuiEventMap.begin();
			}
		}
		g_APIs->XSSP_CloseWnd((XSSP_HANDLE)hWnd);
		return Py_None;
	} catch(XExcept &ex ) {
		PyErr_SetString(g_pError, ex.what());
		return NULL;
	}
	return NULL;
}

PYTHON_DLL_EXTERN 
PyObject *XSSP_SetWindowText(PyObject *self, PyObject *args)
{
	try {
		if( g_APIs == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}
		if( g_APIs->XSSP_SetWindowText == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}

		int hWnd;
		const char* p_id;
		const char* p_val;

		if (!PyArg_ParseTuple(args, "iss", &hWnd, &p_id, &p_val))
			return NULL;
		
#ifdef _UNICODE
		g_APIs->XSSP_SetWindowText((XSSP_HANDLE)hWnd, XStrBuff(p_id), XStrBuff(p_val));
#else
		g_APIs->XSSP_SetWindowText((XSSP_HANDLE)hWnd, p_id, p_val);
#endif
		return Py_None;
	} catch(XExcept &ex ) {
		PyErr_SetString(g_pError, ex.what());
		return NULL;
	}
	return NULL;
}
PYTHON_DLL_EXTERN 
PyObject *XSSP_GetWindowText(PyObject *self, PyObject *args)
{
	try {
		if( g_APIs == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}
		if( g_APIs->XSSP_SetWindowText == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}

		int hWnd;
		const char* p_id;

		if (!PyArg_ParseTuple(args, "is", &hWnd, &p_id))
			return NULL;


		TCHAR szBuff[4029];
		memset(szBuff, 0, sizeof(szBuff));
#ifdef _UNICODE
		g_APIs->XSSP_GetWindowText((XSSP_HANDLE)hWnd, XStrBuff(p_id), szBuff, sizeof(szBuff)-1);
#else
		g_APIs->XSSP_GetWindowText((XSSP_HANDLE)hWnd, p_id, szBuff, sizeof(szBuff)-1);
#endif
		

		
#ifdef _UNICODE
		return PyString_FromString( (const char*)XStrBuff(szBuff) );
#else
		return PyString_FromString(szBuff);
#endif
	} catch(XExcept &ex ) {
		PyErr_SetString(g_pError, ex.what());
		return NULL;
	}
	return NULL;
}


PYTHON_DLL_EXTERN
PyObject *XSSP_SetRadiChkValue(PyObject *self, PyObject *args)
{
	try {
		if( g_APIs == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}
		if( g_APIs->XSSP_SetRadiChkValue == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}

		int hWnd = 0;
		const char* p_id = NULL;
		int value = 0;

		if (!PyArg_ParseTuple(args, "isi", &hWnd, &p_id, &value))
			return NULL;
#ifdef _UNICODE
		g_APIs->XSSP_SetRadiChkValue((XSSP_HANDLE)hWnd, XStrBuff(p_id), value);
#else
		g_APIs->XSSP_SetRadiChkValue((XSSP_HANDLE)hWnd, p_id, value);
#endif
		return Py_None;
	} catch(XExcept &ex ) {
		PyErr_SetString(g_pError, ex.what());
		return NULL;
	}
	return NULL;
}
PYTHON_DLL_EXTERN
PyObject *XSSP_GetRadiChkValue(PyObject *self, PyObject *args)
{
	try {
		if( g_APIs == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}
		if( g_APIs->XSSP_GetRadiChkValue == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}

		int hWnd = 0;
		const char* p_id = NULL;

		if (!PyArg_ParseTuple(args, "is", &hWnd, &p_id))
			return NULL;

#ifdef _UNICODE
		int retcode = g_APIs->XSSP_GetRadiChkValue((XSSP_HANDLE)hWnd, XStrBuff(p_id));
#else
		int retcode = g_APIs->XSSP_GetRadiChkValue((XSSP_HANDLE)hWnd, p_id);
#endif

		return Py_BuildValue("i", retcode);
	} catch(XExcept &ex ) {
		PyErr_SetString(g_pError, ex.what());
		return NULL;
	}
	return NULL;
}



PYTHON_DLL_EXTERN 

PyObject *XSSP_EndModal(PyObject *self, PyObject *args)
{
	try {
		if( g_APIs == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}
		if( g_APIs->XSSP_DialogShow == NULL ) {	PyErr_SetString(g_pError, "API is null.");	return NULL;		}

		int hRes, iExitCode;

		if (!PyArg_ParseTuple(args, "ii", &hRes, &iExitCode))
			return NULL;

		g_APIs->XSSP_EndModal((XSSP_HANDLE)hRes, iExitCode);
		return Py_None;
	} catch(XExcept &ex ) {
		PyErr_SetString(g_pError, ex.what());
		return NULL;
	}
	return NULL;
}



void EventCallBack(XSSP_HANDLE hWnd, int type, const TCHAR* p_id)
{
	std::vector<GuiEventMap>::iterator ite = g_GuiEventMap.begin();


	for(; ite != g_GuiEventMap.end(); ite++) {
		if( (*ite).m_hWnd == (long)hWnd && 
			(*ite).m_strID == p_id &&
			(*ite).m_type == type ) {
		
			PyObjectHelper<PyObject>  pValue;
			PyObjectHelper<PyObject>  pArgs   = PyTuple_New(    3    );
			PyTuple_SetItem(pArgs, 0, PyInt_FromLong((*ite).m_hWnd));
			PyTuple_SetItem(pArgs, 1, PyInt_FromLong(type));
#ifdef _UNICODE
			PyTuple_SetItem(pArgs, 2, PyString_FromString( XStrBuff(p_id) ));
#else
			PyTuple_SetItem(pArgs, 2, PyString_FromString(p_id));
#endif
			pValue = PyObject_CallObject((*ite).m_pFunc, pArgs);
			if( pValue == NULL ) {
				std::map<XSEMI_Gem*, PyXSECSGem*>::iterator ite;
				for( ite = g_gemInsts.begin(); ite != g_gemInsts.end(); ite++ ) {
					xsecsPyPortGemErrorOut( (*ite).first );
				}
			}
			break;
		}
	}
}