#ifdef _MSC_VER
#	pragma warning( disable : 4786 ) 
#endif
#include "xsecsPyPort.h"
#include "xsecsPyGEM.h"
#include "xsecsPySECSData.h"
#include "xsecsPySECSItem.h"
#include "wxssp/xsspExternalMdl.h"
#include "structmember.h"

// http://pukiwiki.tuntunkun.com/index.php?%B8%C0%B8%EC%2FC%2FEmbedded%20Javascript

#include "xlib/xeception.h"
#include "xlib/xbuff.h"
#include "xlib/semi/secs/xsecs.h"
#include "xlib/semi/gem/xsemi_gem.h"


extern PyObject *g_pError;

PYTHON_DLL_EXTERN 
PyObject *PyXSECSGem_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

PyMODINIT_FUNC
PyXSECSGem_dealloc(PyXSECSGem* self);

PYTHON_DLL_EXTERN 
int PyXSECSGem_init(PyXSECSGem *self, PyObject *args, PyObject *kwds);


PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_sendRequest(PyXSECSGem *self, PyObject *args);

PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_sendResponse(PyXSECSGem *self, PyObject *args);

PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_gemProcess(PyXSECSGem *self, PyObject *args);

PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_registSxFxCallBack(PyXSECSGem *self, PyObject *args);

PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_registGetVariableFnc(PyXSECSGem *self, PyObject *args);

PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_registPutVariableFnc(PyXSECSGem *self, PyObject *args);

PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_registUserCommandFnc(PyXSECSGem *self, PyObject *args);

PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_registReleaseMdlFun(PyXSECSGem *self, PyObject *args);

PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_sleep(PyXSECSGem *self, PyObject *args);

PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_logMessage(PyXSECSGem *self, PyObject *args);


//
//	PyXSECSGemクラス：メンバー変数の定義
//
static PyMemberDef PyXSECSGem_members[] = {
    {NULL}  /* Sentinel */
};

//
//	PyXSECSGemクラス：メンバー関数の定義
//
static PyMethodDef PyXSECSGem_methods[] = {
	{"sendRequest",           (PyCFunction)PyXSECSGem_sendRequest, METH_VARARGS,  "sendRequest."},
	{"sendResponse",          (PyCFunction)PyXSECSGem_sendResponse, METH_VARARGS,  "sendResponse."},
	{"gemProcess",            (PyCFunction)PyXSECSGem_sendResponse, METH_VARARGS,  "gemProcess."},
	{"registSxFxCallBack",    (PyCFunction)PyXSECSGem_registSxFxCallBack, METH_VARARGS,  "function to SxFx CallBack register.."},
	{"registGetVariableFnc",  (PyCFunction)PyXSECSGem_registGetVariableFnc, METH_VARARGS,  "function to acquire value of GetVarible register."},
	{"registPutVariableFnc",  (PyCFunction)PyXSECSGem_registPutVariableFnc, METH_VARARGS,  "function to acquire value of PutVarible register."},
	{"registUserCommandFnc",  (PyCFunction)PyXSECSGem_registUserCommandFnc, METH_VARARGS,  "function to UserCommand register."},
	{"registReleseModuleFnc", (PyCFunction)PyXSECSGem_registReleaseMdlFun, METH_VARARGS,  "function to ReleseModule register."},
	{"sleep",  (PyCFunction)PyXSECSGem_sleep, METH_VARARGS,  "sleep."},
	{"logMessage",  (PyCFunction)PyXSECSGem_logMessage, METH_VARARGS,  "logMessage."},
    {NULL}  /* Sentinel */
};


/**	@brief タイプオブジェクト  */
static PyTypeObject g_XSECSGemType = {
    PyObject_HEAD_INIT(NULL)
    0,								/*ob_size*/
    "xsecsPyPort.XSECSGem",		/*tp_name*/
    sizeof(PyXSECSGem),				/*tp_basicsize*/
    0,								/*tp_itemsize*/
    (destructor)PyXSECSGem_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "PyXSECSGem objects",			/* tp_doc */
    0,								/* tp_traverse */
    0,								/* tp_clear */
    0,								/* tp_richcompare */
    0,								/* tp_weaklistoffset */
    0,								/* tp_iter */
    0,								/* tp_iternext */
    PyXSECSGem_methods,				/* tp_methods */
    PyXSECSGem_members,				/* tp_members */
    //PyXSECSGem_getseters,			/* tp_getset */
	NULL, 
    0,								/* tp_base */
    0,								/* tp_dict */
    0,								/* tp_descr_get */
    0,								/* tp_descr_set */
    0,								/* tp_dictoffset */
    (initproc)PyXSECSGem_init,      /* tp_init */
    0,                         /* tp_alloc */
    PyXSECSGem_new,                 /* tp_new */
};



PYTHON_DLL_EXTERN 
PyObject *PyXSECSGem_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PyXSECSGem *self;

	self = (PyXSECSGem *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->m_gem = 0;
		self->m_userWndThread = 0;
		self->m_pDictionary = NULL;
		self->m_pGetEx = NULL;
		self->m_pPutEx = NULL;
		self->m_pUserCommand = NULL;
		self->m_pReleaseModule = NULL;
		self->m_pSxFx = new std::map<long, PyObject*>();
	}
	return (PyObject *)self;
}

PyMODINIT_FUNC
PyXSECSGem_dealloc(PyXSECSGem* self)
{
	if( self->m_pGetEx != NULL ) {
		XSSPY_XDECREF(self->m_pGetEx); 
		self->m_pGetEx = NULL;
	}
	if( self->m_pPutEx != NULL ) {
		XSSPY_XDECREF(self->m_pPutEx); 
		self->m_pPutEx = NULL;
	}
	if( self->m_pUserCommand != NULL ) {
		XSSPY_XDECREF(self->m_pUserCommand); 
		self->m_pUserCommand = NULL;
	}
	if( self->m_pReleaseModule != NULL ) {
		XSSPY_XDECREF(self->m_pReleaseModule); 
		self->m_pReleaseModule = NULL;
	}

	if( self->m_pDictionary != NULL ) {
		XSSPY_XDECREF(self->m_pDictionary); 
		self->m_pDictionary = NULL;
		

		std::map<long, PyObject*>::iterator ite = self->m_pSxFx->begin();
		for( ; ite != self->m_pSxFx->end(); ite++) {
			XSSPY_XDECREF( (*ite).second ); 
		}
		self->m_pSxFx->clear();
		delete self->m_pSxFx;
		self->m_pSxFx = NULL;
	}
	if( self->m_pModule != NULL ) {
		XSSPY_XDECREF(self->m_pModule); 
		self->m_pModule = NULL;
	}
    self->ob_type->tp_free((PyObject*)self);
}


PYTHON_DLL_EXTERN 
int PyXSECSGem_init(PyXSECSGem *self, PyObject *args, PyObject *kwds)
{
	self->m_gem = 0;
	self->m_userWndThread = 0;
	self->m_pDictionary = NULL;
	self->m_pGetEx = NULL;
	self->m_pPutEx = NULL;
	self->m_pSxFx = NULL;
	self->m_pModule = NULL;
	self->m_pUserCommand = NULL;
	self->m_pReleaseModule = NULL;
    return 0;
}


bool PyXSECSGem_RegistModule(PyObject *pMOdule)
{
    if (PyType_Ready(&g_XSECSGemType) < 0)
        return false;
    g_XSECSGemType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&g_XSECSGemType) < 0)
        return false;

    Py_INCREF(&g_XSECSGemType);
    PyModule_AddObject(pMOdule, "PyXSECSGem", (PyObject *)&g_XSECSGemType);
	return true;
}

PyXSECSGem* PyXSECSGem_GCNew(long pGem)
{
	PyXSECSGem* op = (PyXSECSGem*)PyXSECSGem_new(&g_XSECSGemType, 0, 0);
	op->m_gem = pGem;

	return op;
}

bool PyXSECSGem_CheckType(PyObject *data)
{
	return PyObject_TypeCheck(data, &g_XSECSGemType);
}




#undef  FUNC 
#define FUNC "PyXSECSGem_sendRequest"
PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_sendRequest(PyXSECSGem *self, PyObject *args)
{
	bool bretVal = true;
	try {
		XSECSData*		pSECSData = NULL;
		XSECSMSG		rspmsg;
		XSECS_SEND_STS	sendSts;
		long			lWait = 1;

		// Gemオブジェクトが初期化されていない場合はエラー
		if( self->m_gem == 0 ) {
			PyErr_SetString(g_pError, "not set GEM object .");
			return NULL;
		}
		// 引数を解析する。
		if( !PyArg_ParseTuple(args, "O|l", (XSECSData*)&pSECSData, &lWait) ) {
			PyErr_SetString(g_pError, "arguments error. [sendRequest(XSECSData[,wait time])]");
			return NULL;
		}
		
		// 第一引数に渡された、XSECSDataのデータタイプをチェックする。
		if( ! XSECSData_CheckType((PyObject*)pSECSData) ) {
			PyErr_SetString(g_pError, "arguments error not XSECSData.");
			return NULL;
		}


		//
		//	SECSメッセージの組み立て
		//
		XSECSHeader			head;		
		XInflateStreamBuff	buff(256);
		std::iostream		stream(&buff);
		XDataWriter			writer(&stream);
		
		::memset(&head, 0, sizeof(head));
		XSecsData::setSxFx(head,
			pSECSData->m_stream, 
			pSECSData->m_function,
			pSECSData->m_wait, 
			pSECSData->m_ticket);
/*
		head.byte2 = (unsigned char)pSECSData->m_stream;
		if( pSECSData->m_wait == true ) {
			head.byte2 |= 0x80;
		}
		head.status = (unsigned char)pSECSData->m_function;
		::memcpy(head.systembyte, &pSECSData->m_ticket, sizeof(long));
*/
		if( XSECSItem_storeStream(writer, (XSECSItem*)pSECSData->m_data) == false ) {
			XTRACE(_T("ERR 2\n"));
			PyErr_SetString(g_pError, "fail to build secs message.");
			return NULL;
		}

		// 1次メッセージの送信
		sendSts = ((XSEMI_Gem*)self->m_gem)->sendRequest(
			&head, buff.getBuff(), buff.size(), &rspmsg,
			lWait == 1 ? true : false);
		if( sendSts == XSECS_SEND_STS_S_AND_HAS_REP ) {
			// 2次メッセージがある。
			PyObjectHelper<XSECSData> pData   = XSECSData_GCNew();
			if(pData == NULL ) {
				PyErr_SetString(g_pError, "fail to create XSECSData object.");
				return NULL;
			}
			pData.increment_reference();
		
			pData->m_stream   = (int)XSecsData::get_stream(rspmsg.header);//(rspmsg.header.byte2 & ~0x80);
			pData->m_function = (int)XSecsData::get_function(rspmsg.header);//(rspmsg.header.status);
			pData->m_wait     = XSecsData::get_wait(rspmsg.header);//( rspmsg.header.byte2 & 0x80 ) ? true : false;
			pData->m_ticket   = XSecsData::get_ticket(rspmsg.header);
			//::memcpy(&pData->m_ticket, rspmsg.header.systembyte, sizeof(pData->m_ticket));


			XDataReader reader(rspmsg.pData,  rspmsg.ulDataSize );
			XData       data;
			while( reader.getValue(&data) ) {
				XSECSItem_loadStream((XSECSItem*)pData->m_data, reader, data);
			}
			pData.increment_reference();

			// チケットの解放
			if( ((XSEMI_Gem*)self->m_gem)->dropTicket( ::ntohl(XSecsData::get_ticket(head)) ) == false ) {
				XTRACE(_T("ERR [%s]Line:%d  fail to dropTicket(%08X)\n"), FUNC, __LINE__, XSecsData::get_ticket(head));
			}

			return (PyObject*)pData.m_pT;
		} else if( sendSts ==  XSECS_SEND_STS_S) {
			// 2次メッセージが無い。
			PyObjectHelper<XSECSData> pData   = XSECSData_GCNew();
			if(pData == NULL ) {
				PyErr_SetString(g_pError, "fail to create XSECSData object.");
				return NULL;
			}
			pData.increment_reference();
		
			return (PyObject*)pData.m_pT;
		} else {
			PyErr_SetString(g_pError, "fail to sendRequest.");
		}
	} catch (...) {
		PyErr_SetString(g_pError, "PyXSECSGem_sendRequest.");
	}
	return NULL;
}

#undef  FUNC 
#define FUNC "PyXSECSGem_gemProcess"
PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_gemProcess(PyXSECSGem *self, PyObject *args)
{
	bool bretVal = true;
	try {
		XSECSData *pSECSData = NULL;
		int gemProcRes = 0;

		// 引数を解析する。
		if( !PyArg_ParseTuple(args, "O", (XSECSData*)&pSECSData) ) {
			PyErr_SetString(g_pError, "arguments error. [sendRequest(XSECSData)]");
			return NULL;
		}
		

		if( ! XSECSData_CheckType((PyObject*)pSECSData) ) {
			PyErr_SetString(g_pError, "arguments error not XSECSData.");
			return NULL;
		}
		if( self->m_gem == 0 ) {
			PyErr_SetString(g_pError, "not set GEM object .");
			return NULL;
		}


		// 入力データのバイナリー化
		XSecsData			secsData;
		XInflateStreamBuff	buff(256);
		std::iostream		stream(&buff);
		XInflateStreamBuff	oBuff(256);
		std::ostream		oStream(&oBuff);
		XDataWriter			writer(&stream);

		// ヘッダーデータを作成
		secsData.put_stream(pSECSData->m_stream);
		secsData.put_function(pSECSData->m_function);
		secsData.put_wait(pSECSData->m_wait);


		// Py->Binary
		if( XSECSItem_storeStream(writer, (XSECSItem*)pSECSData->m_data) == false ) {
			XTRACE(_T("ERR 2\n"));
			PyErr_SetString(g_pError, "fail to build secs message.");
			return NULL;
		}
		// Binary->XSecsData
		if( secsData.setFromBinary(buff.getBuff(), buff.size(), XSecsData::HEAD_AND_BODY) == false ) {
			PyErr_SetString(g_pError, "fail to build secs message.");
			return NULL;
		}
		gemProcRes = ((XSEMI_Gem*)self->m_gem)->processGemMessage(
			secsData, oStream);
		if( gemProcRes == XSEMI_GEM_ERR_NON_TRANSACT_DEF ||
			gemProcRes == XSEMI_GEM_ERR_NON_TRANSACT_OV ) {

			PyObjectHelper<XSECSData> pData   = XSECSData_GCNew();
			if(pData == NULL ) {
				PyErr_SetString(g_pError, "fail to create XSECSData object.");
				return NULL;
			}
			pData.increment_reference();
		
			pData->m_stream   = pSECSData->m_stream;
			pData->m_function = pSECSData->m_function+1;
			pData->m_wait     =  false;


			XDataReader reader(buff.getBuff(), buff.size() );
			XData       data;
			while( reader.getValue(&data) ) {
				XSECSItem_loadStream((XSECSItem*)pData->m_data, reader, data);
			}
			pData.increment_reference();

			return (PyObject*)pData.m_pT;
		} else {
			PyErr_SetString(g_pError, "fail to gemProcess.");
			return NULL;
		}
	} catch (...) {
		PyErr_SetString(g_pError, "PyXSECSGem_sendRequest.");
	}
	return NULL;
}

PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_sendResponse(PyXSECSGem *self, PyObject *args)
{
	bool bretVal = true;
	XSECS_SEND_STS	sendSts;
	try {
		XSECSData *pSECSData = NULL;

		// 引数を解析する。
		if( !PyArg_ParseTuple(args, "O", (XSECSData*)&pSECSData) ) {
			PyErr_SetString(g_pError, "arguments error. [sendResponse(XSECSData )]");
			return NULL;
		}
		if( ! XSECSData_CheckType((PyObject*)pSECSData) ) {
			PyErr_SetString(g_pError, "arguments error not XSECSData.");
			return NULL;
		}

		if( self->m_gem == 0 ) {
			PyErr_SetString(g_pError, "not set GEM object .");
			return NULL;
		}	


		XSECSHeader			head;		
		XInflateStreamBuff	buff(256);
		std::iostream		stream(&buff);
		XDataWriter			writer(&stream);
		
		::memset(&head, 0, sizeof(head));
		head.byte2 = (unsigned char)pSECSData->m_stream;
		head.status = (unsigned char)pSECSData->m_function;
		::memcpy(head.tr_id, &pSECSData->m_ticket, sizeof(long));

		if( ((XSECSItem*)pSECSData->m_data)->m_type != 0 ) {
			// ボディーデータがある
			if( XSECSItem_storeStream(writer, (XSECSItem*)pSECSData->m_data)== false ) {
				XTRACE(_T("ERR 1\n"));
			}
		}

		sendSts = ((XSEMI_Gem*)self->m_gem)->sendResponse(&head, buff.getBuff(), buff.size());
	} catch (...) {
		PyErr_SetString(g_pError, "PyXSECSGem_sendResponse.");
	}
	return Py_BuildValue("i", sendSts == XSECS_SEND_STS_S ? 1 : -1);
}


PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_registSxFxCallBack(PyXSECSGem *self, PyObject *args)
{
	PyObject*		result = NULL;
	bool bretVal = true;
	try {
		XStrBuff			strMsg;
		unsigned short		stream, function;
		long				sxfx;
		PyObject			*p_Func;

		if( self->m_gem == 0 ) {
			PyErr_SetString(g_pError, "not set GEM object .");
			return NULL;
		}
		// 引数を解析する。
		if( !PyArg_ParseTuple(args, "HHO", &stream, &function, &p_Func ) ) {
			PyErr_SetString(g_pError, "arguments error. [setSxFxCallBack(stream, function, function)]");
			return NULL;
		}


		if( PyCallable_Check(p_Func) == 0 ) {
			PyErr_SetString(PyExc_TypeError, "function must be callable");
			return NULL;
		}
		Py_INCREF( p_Func );

		// 配列に追加する
		sxfx = (stream << 16 ) | function;
		self->m_pSxFx->insert( std::pair<long, PyObject*>(sxfx, p_Func) );

		strMsg.Format(_T("  regist Function. S%dF%d."), stream, function);
		((XSEMI_Gem*)self->m_gem)->writeLogCallBack(XLIB_LOG_LEVEL_DEBUG, strMsg);

		/* "None" を返す際の定型句 */
		Py_INCREF(Py_None);
		result = Py_None;
	} catch(...) {
		XStrBuff strErrMsg;
		strErrMsg.Format(_T("[%s] invalid exception occur in xsecsPyPort init.\n")
						 _T("\tfile:%s(%d)"), XSECS_PY_PORT, __FILE__, __LINE__);
		PyErr_SetString(g_pError, strErrMsg);
		return false;
	}
	return result;
}

PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_registGetVariableFnc(PyXSECSGem *self, PyObject *args)
{
	PyObject*		result = NULL;
	PyObject*		temp;

	try {
		if (PyArg_ParseTuple(args, "O:registGetVariableFnc", &temp)) {
			if (!PyCallable_Check(temp)) {
				PyErr_SetString(PyExc_TypeError, "parameter must be callable");
				return NULL;
			}
			Py_XINCREF(temp);         /* 新たなコールバックへの参照を追加 */
			XSSPY_XDECREF(self->m_pGetEx);  /* 以前のコールバックを捨てる */
			self->m_pGetEx = temp;       /* 新たなコールバックを記憶 */
			/* "None" を返す際の定型句 */
			Py_INCREF(Py_None);
			result = Py_None;
		}
	} catch (...) {
		PyErr_SetString(g_pError, "PyXSECSGem_registGetVariableFnc.");
		return NULL;
	}
	return result;
}


PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_registPutVariableFnc(PyXSECSGem *self, PyObject *args)
{
	PyObject*		result = NULL;
	PyObject*		temp;

	try {
		if (PyArg_ParseTuple(args, "O:registPutVariableFnc", &temp)) {
			if (!PyCallable_Check(temp)) {
				PyErr_SetString(PyExc_TypeError, "parameter must be callable");
				return NULL;
			}
			Py_XINCREF(temp);         /* 新たなコールバックへの参照を追加 */
			XSSPY_XDECREF(self->m_pPutEx);  /* 以前のコールバックを捨てる */
			self->m_pPutEx = temp;       /* 新たなコールバックを記憶 */
			/* "None" を返す際の定型句 */
			Py_INCREF(Py_None);
			result = Py_None;
		}
	} catch (...) {
		PyErr_SetString(g_pError, "PyXSECSGem_registPutVariableFnc.");
		return NULL;
	}
	return result;
}



PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_registUserCommandFnc(PyXSECSGem *self, PyObject *args)
{
	PyObject*		result = NULL;
	PyObject*		temp;

	try {
		if (PyArg_ParseTuple(args, "O:registUserCommandFnc", &temp)) {
			if (!PyCallable_Check(temp)) {
				PyErr_SetString(PyExc_TypeError, "parameter must be callable");
				return NULL;
			}
			Py_XINCREF(temp);         /* 新たなコールバックへの参照を追加 */
			XSSPY_XDECREF(self->m_pUserCommand);  /* 以前のコールバックを捨てる */
			self->m_pUserCommand = temp;       /* 新たなコールバックを記憶 */
			/* "None" を返す際の定型句 */
			Py_INCREF(Py_None);
			result = Py_None;
		}
	} catch (...) {
		PyErr_SetString(g_pError, "PyXSECSGem_registUserCommandFnc.");
		return NULL;
	}
	return result;
}


PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_registReleaseMdlFun(PyXSECSGem *self, PyObject *args)
{
	PyObject*		result = NULL;
	PyObject*		temp;

	try {
		if (PyArg_ParseTuple(args, "O:registRelese", &temp)) {
			if (!PyCallable_Check(temp)) {
				PyErr_SetString(PyExc_TypeError, "parameter must be callable");
				return NULL;
			}
			Py_XINCREF(temp);         /* 新たなコールバックへの参照を追加 */
			XSSPY_XDECREF(self->m_pReleaseModule);  /* 以前のコールバックを捨てる */
			self->m_pReleaseModule = temp;       /* 新たなコールバックを記憶 */
			/* "None" を返す際の定型句 */
			Py_INCREF(Py_None);
			result = Py_None;
		}
	} catch (...) {
		PyErr_SetString(g_pError, "PyXSECSGem_registUserCommandFnc.");
		return NULL;
	}
	return result;
}


PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_sleep(PyXSECSGem *self, PyObject *args)
{
	PyObject*		result = NULL;
	try {
		long				sec, msec = 0;


		// 引数を解析する。
		if( !PyArg_ParseTuple(args, "l|l", &sec, &msec ) ) {
			PyErr_SetString(g_pError, "arguments error. [logMessage(level, msg)]");
			return NULL;
		}
		if( self->m_gem == 0 ) {
			PyErr_SetString(g_pError, "not set GEM object .");
			return NULL;
		}

		XThread::Sleep(sec, msec);
		Py_INCREF(Py_None);
		result = Py_None;
	} catch (...) {
		PyErr_SetString(g_pError, "PyXSECSGem_sleep.");
	}
	return result;
}

PYTHON_DLL_EXTERN
PyObject *PyXSECSGem_logMessage(PyXSECSGem *self, PyObject *args)
{
	bool bretVal = true;
	try {
		char				*p_msg;
		long				level;


		// 引数を解析する。
		if( !PyArg_ParseTuple(args, "ls", &level, &p_msg ) ) {
			PyErr_SetString(g_pError, "arguments error. [logMessage(level, msg)]");
			return NULL;
		}
		if( self->m_gem == 0 ) {
			PyErr_SetString(g_pError, "not set GEM object .");
			return NULL;
		}
#ifdef _UNICODE
		((XSEMI_Gem*)self->m_gem)->writeLogCallBack(level, XStrBuff(p_msg));
#else
		((XSEMI_Gem*)self->m_gem)->writeLogCallBack(level, p_msg);
#endif
	} catch (...) {
		PyErr_SetString(g_pError, "PyXSECSGem_logMessage.");
	}
	return Py_BuildValue("i", 1);
}