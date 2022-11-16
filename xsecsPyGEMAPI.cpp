#ifdef _MSC_VER
#	pragma warning( disable : 4786 ) 
#endif

#include "xsecsPyPort.h"
#include "xsecsPyGEMAPI.h"
#include "xsecsPySECSData.h"
#include "xsecsPySECSItem.h"
#include "structmember.h"



#include "xlib/xeception.h"
#include "xlib/xbuff.h"
#include "xlib/semi/secs/xsecs.h"
#include "xlib/semi/gem/xsemi_gem.h"


extern PyObject *g_pError;



/**	@brief	The XLIB_API class*/
class xsecsPyGemAPI : public XSEMI_GemAPI
{
public:
	xsecsPyGemAPI(PyXSECSGemAPI* pGemAPI)
//		:	m_pGemAPI(pGemAPI)
		:	m_pCallBack( NULL )
	{
	}
	virtual ~xsecsPyGemAPI()
	{
	}

	virtual int receiveMessage(XSECSHeader* pHeader, unsigned char *pData, unsigned long ulDataSize);

	PyObject*		m_pCallBack;
	PyXSECSGemAPI*	m_pGemAPI;
};

int xsecsPyGemAPI::receiveMessage(XSECSHeader* pHeader, 
								   unsigned char *pData, 
								   unsigned long ulDataSize)
{
/*
	// コールバックが登録されいる？
	if( m_pCallBack == NULL ) {
		return ;
	}

	XSECSHeader& head = *pHeader;
	PyObjectHelper<XSECSData> pXSECSData   = XSECSData_GCNew();
	if(pData == NULL ) {
		return ;
	}

	pXSECSData->m_stream   = (int)(head.byte2 & ~0x80);
	pXSECSData->m_function = (int)(head.status);
	pXSECSData->m_wait     = ( head.byte2 & 0x80 ) ? true : false;
	::memcpy(&pXSECSData->m_ticket, head.tr_id, sizeof(pXSECSData->m_ticket));


	XDataReader reader(pData+10,  ulDataSize-10 );
	XData       data;
	while( reader.getValue(&data) ) {
		XSECSItem_loadStream((XSECSItem*)pXSECSData->m_data, reader, data);
	}


// !<-- for dbg
	if( m_pCallBack != NULL ) {
		PyObjectHelper<PyObject>  pArgs   = PyTuple_New(    1    );
		PyObjectHelper<PyObject>  pValue;

		pXSECSData.increment_reference();
		PyTuple_SetItem(pArgs, 0, (PyObject*)pXSECSData.m_pT);
		pValue = PyEval_CallObject(m_pCallBack, pArgs);
	}
// -->

	XTRACE(_T("  PyXSECSGemAPI.receiveMessage time(2) itemRefeCnt:%d\n"), pXSECSData->ob_refcnt);
*/
	return XQueue_POP_CALLBCK_IGNORE;
}


PYTHON_DLL_EXTERN 
PyObject *PyXSECSGemAPI_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

PyMODINIT_FUNC
PyXSECSGemAPI_dealloc(PyXSECSGemAPI* self);

PYTHON_DLL_EXTERN 
int PyXSECSGemAPI_init(PyXSECSGemAPI *self, PyObject *args, PyObject *kwds);


PYTHON_DLL_EXTERN
PyObject *PyXSECSGemAPI_open(PyXSECSGemAPI *self, PyObject *args);

PYTHON_DLL_EXTERN
PyObject *PyXSECSGemAPI_sendRequest(PyXSECSGemAPI *self, PyObject *args);


PYTHON_DLL_EXTERN
PyObject *PyXSECSGemAPI_reciveMessageCallBack(PyXSECSGemAPI *self, PyObject *args);

PYTHON_DLL_EXTERN
PyObject *PyXSECSGemAPI_pop(PyXSECSGemAPI *self, PyObject *args, PyObject *kwds);


//
//	PyXSECSGemAPIクラス：メンバー変数の定義
//
static PyMemberDef PyXSECSGemAPI_members[] = {
    {NULL}  /* Sentinel */
};


//
//	PyXSECSGemクラス：メンバー関数の定義
//
static PyMethodDef PyXSECSGemAPI_methods[] = {
	{"open",					(PyCFunction)PyXSECSGemAPI_open,        METH_VARARGS,  "open."},
	{"sendRequest",				(PyCFunction)PyXSECSGemAPI_sendRequest, METH_VARARGS,  "sendRequest."},
//	{"registRcvMsgCallBack",	(PyCFunction)PyXSECSGemAPI_reciveMessageCallBack, METH_VARARGS,  "reciveMessageCallBack."},
	{"pop",	                    (PyCFunction)PyXSECSGemAPI_pop, METH_KEYWORDS,  "pop."},


    {NULL}  /* Sentinel */
};


/**	@brief タイプオブジェクト  */
static PyTypeObject g_XSECSGemAPIType = {
    PyObject_HEAD_INIT(NULL)
    0,								/*ob_size*/
    "xsecsPyPort.XSECSGemAPI",		/*tp_name*/
    sizeof(PyXSECSGemAPI),				/*tp_basicsize*/
    0,								/*tp_itemsize*/
    (destructor)PyXSECSGemAPI_dealloc, /*tp_dealloc*/
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
    "PyXSECSGemAPI objects",			/* tp_doc */
    0,								/* tp_traverse */
    0,								/* tp_clear */
    0,								/* tp_richcompare */
    0,								/* tp_weaklistoffset */
    0,								/* tp_iter */
    0,								/* tp_iternext */
    PyXSECSGemAPI_methods,				/* tp_methods */
    PyXSECSGemAPI_members,				/* tp_members */
    //PyXSECSGem_getseters,			/* tp_getset */
	NULL, 
    0,								/* tp_base */
    0,								/* tp_dict */
    0,								/* tp_descr_get */
    0,								/* tp_descr_set */
    0,								/* tp_dictoffset */
    (initproc)PyXSECSGemAPI_init,      /* tp_init */
    0,                         /* tp_alloc */
    PyXSECSGemAPI_new,                 /* tp_new */
};



PYTHON_DLL_EXTERN 
PyObject *PyXSECSGemAPI_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PyXSECSGemAPI *self;

	self = (PyXSECSGemAPI*)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->m_gemAPI = 0;
		self->m_pReciveEventCallBack = NULL;
	}
	return (PyObject *)self;
}


PyMODINIT_FUNC
PyXSECSGemAPI_dealloc(PyXSECSGemAPI* self)
{
	Py_XINCREF(self->m_pReciveEventCallBack);       
	if( self->m_gemAPI != 0 ) {
		xsecsPyGemAPI* pGemAPI = (xsecsPyGemAPI*)self->m_gemAPI;
		pGemAPI->close();
		delete pGemAPI;
		self->m_gemAPI = 0;
	}
    self->ob_type->tp_free((PyObject*)self);
}


PYTHON_DLL_EXTERN 
int PyXSECSGemAPI_init(PyXSECSGemAPI *self, PyObject *args, PyObject *kwds)
{
	self->m_gemAPI = 0;
	self->m_pReciveEventCallBack = NULL;
    return 0;
}




bool PyXSECSGemAPI_RegistModule(PyObject *pMOdule)
{
    if (PyType_Ready(&g_XSECSGemAPIType) < 0)
        return false;
    g_XSECSGemAPIType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&g_XSECSGemAPIType) < 0)
        return false;

    Py_INCREF(&g_XSECSGemAPIType);
    PyModule_AddObject(pMOdule, "PyXSECSGemAPI", (PyObject *)&g_XSECSGemAPIType);
	return true;
}

PyXSECSGemAPI*PyXSECSGemAPI_GCNew(long pGemAPI)
{
	PyXSECSGemAPI* op = (PyXSECSGemAPI*)PyXSECSGemAPI_new(&g_XSECSGemAPIType, 0, 0);
	return op;
}

bool PyXSECSGeAPI_m_CheckType(PyObject *data)
{
	return PyObject_TypeCheck(data, &g_XSECSGemAPIType);
}



#undef FUNC 
#define FUNC "PyXSECSGemAPI_open"
PYTHON_DLL_EXTERN
PyObject *PyXSECSGemAPI_open(PyXSECSGemAPI *self, PyObject *args)
{
	bool bretVal = true;
	std::string	strErrorMsg;
	try {
		const char *pName;
		xsecsPyGemAPI* pGemAPI;

		// 引数を解析する。
		// 名前は必須
		if (!PyArg_ParseTuple(args, "s", &pName)) {
			return NULL;
		}
		
		if( self->m_gemAPI != 0 ) {
			pGemAPI = (xsecsPyGemAPI*)self->m_gemAPI;
			delete pGemAPI;
			self->m_gemAPI = 0;
		}


		pGemAPI = new xsecsPyGemAPI( self );
		if( pGemAPI->open(pName) == false ) {
			delete pGemAPI;

			strErrorMsg = "fail to open GemAPI[";
			strErrorMsg += pName;
			strErrorMsg += "]";

			PyErr_SetString(g_pError, strErrorMsg.c_str());
			return NULL;
		} else {
			self->m_gemAPI = (long)pGemAPI;
		}
		return Py_BuildValue("i", self->m_gemAPI == 0 ? 0 : 1 );
	} catch (...) {
		PyErr_SetString(g_pError, "PyXSECSGem_sendRequest");
	}
	return NULL;
}




#undef FUNC 
#define FUNC "PyXSECSGemAPI_sendRequest"
PYTHON_DLL_EXTERN
PyObject *PyXSECSGemAPI_sendRequest(PyXSECSGemAPI *self, PyObject *args)
{
	bool bretVal = true;
	try {
		XSECSData *		pSECSData = NULL;
		unsigned char *	pRespData = NULL;
		XBuff			buffRsp;
		XD_UINT4		ulRespDataSize = 0;
		XSECS_SEND_STS	sendSts;
		xsecsPyGemAPI*	pGemAPI;

		// 引数を解析する。
		if( !PyArg_ParseTuple(args, "O", (XSECSData*)&pSECSData) ) {
			PyErr_SetString(g_pError, "arguments error. [sendRequest(XSECSData)]");
			return NULL;
		}
		if( ! XSECSData_CheckType((PyObject*)pSECSData) ) {
			PyErr_SetString(g_pError, "arguments error not XSECSData.");
			return NULL;
		}
		// APIの実態
		if( self->m_gemAPI == 0 ) {
			PyErr_SetString(g_pError, "not set GEM_API object .");
			return NULL;
		}
		pGemAPI = (xsecsPyGemAPI*)self->m_gemAPI;

		//
		//	送信バイナリーデータを作成
		//
		XSECSHeader			head;
		XInflateStreamBuff	buff(256);
		std::iostream		stream(&buff);
		XDataWriter			writer(&stream);
		
		::memset(&head, 0, sizeof(head));
		head.byte2 = (unsigned char)pSECSData->m_stream;
		if( pSECSData->m_wait == true ) {
			head.byte2 |= 0x80;
		}
		head.status = (unsigned char)pSECSData->m_function;
		::memcpy(head.tr_id, &pSECSData->m_ticket, sizeof(long));

		if( XSECSItem_storeStream(writer, (XSECSItem*)pSECSData->m_data) == false ) {
			PyErr_SetString(g_pError, "fail to build secs message.");
			return NULL;
		}
		
		// 戻り値のデータを作成
		PyObjectHelper<XSECSData> pData   = XSECSData_GCNew();
		if(pData == NULL ) {
			PyErr_SetString(g_pError, "fail to create XSECSData object.");
			return NULL;
		}

		// 送信。
		sendSts = pGemAPI->sendRequest(
			&head, buff.getBuff(), buff.size(), buffRsp, &ulRespDataSize);
		if( sendSts == XSECS_SEND_STS_S ) {
			pData.increment_reference();
			return (PyObject*)pData.m_pT;
		} else
		if( sendSts == XSECS_SEND_STS_S_SPOOL ) {
			pData.increment_reference();
			return (PyObject*)pData.m_pT;

		} else
		if( sendSts == XSECS_SEND_STS_S_AND_HAS_REP ) {
			pRespData = buffRsp;
			memcpy(&head, pRespData, sizeof(XSECSHeader));


		
			pData->m_stream   = (int)(head.byte2 & ~0x80);
			pData->m_function = (int)(head.status);
			pData->m_wait     = ( head.byte2 & 0x80 ) ? true : false;
			::memcpy(&pData->m_ticket, head.tr_id, sizeof(pData->m_ticket));


			XDataReader reader(pRespData+10,  ulRespDataSize-10 );
			XData       data;
			while( reader.getValue(&data) ) {
				XSECSItem_loadStream((XSECSItem*)pData->m_data, reader, data);
			}

			pData.increment_reference();
			return (PyObject*)pData.m_pT;
		} else
		if( sendSts == XSECS_SEND_STS_F_NOT_CONNECTED ) {
			PyErr_SetString(g_pError, "Not Connected.");
		} else
		if( sendSts == XSECS_SEND_STS_F_TIMEOUT ) {
			PyErr_SetString(g_pError, "TIME OUT Occured.");
		} else
		if( sendSts == XSECS_SEND_STS_F_DELIV ) {
			PyErr_SetString(g_pError, "Delivery failure.");
		} else
		if( sendSts == XSECS_SEND_STS_F_DATA ) {
			PyErr_SetString(g_pError, "Receive data broken.");
		} else
		if( sendSts == XSECS_SEND_STS_F_UNINIT ) {
			PyErr_SetString(g_pError, "not initilized.");
		} else {
			PyErr_SetString(g_pError, "fail to sendRequest.");
		}
	} catch (...) {
		PyErr_SetString(g_pError, "invalid throw PyXSECSGemAPI_sendRequest.");
		return NULL;
	}
	return NULL;
}


#undef FUNC 
#define FUNC "PyXSECSGemAPI_pop"
PYTHON_DLL_EXTERN
PyObject *PyXSECSGemAPI_pop(PyXSECSGemAPI *self, PyObject *args, PyObject *kwds)
{
	PyObject*		result = NULL;
	xsecsPyGemAPI*	pGemAPI;

	pGemAPI = (xsecsPyGemAPI*)self->m_gemAPI;
	if( pGemAPI == NULL ) {
		// まだＯＰＥＮしていない
		PyErr_SetString(g_pError, "not opened.");
		return NULL;
	}

	int iTimeOut = -1;
	int iMsgID   = -1;
	static char *kwlist[] = {"timeout", "msgid",  NULL};
	if (! PyArg_ParseTupleAndKeywords(args, kwds, "|ii", kwlist, 
									  &iTimeOut,
									  &iMsgID)) {
		return NULL; 
	}


	XBuff dataBuff;
	size_t ulDataSize;
	XMsgQueueERR sts = pGemAPI->pop(dataBuff, &ulDataSize, iMsgID, iTimeOut);

	// 戻り値のデータを作成
	PyObjectHelper<XSECSData> pData   = XSECSData_GCNew();
	if(pData == NULL ) {
		PyErr_SetString(g_pError, "fail to create XSECSData object.");
		return NULL;
	}
	if( sts == XMsgQueueERR_SUCCESS ) {

		pData->m_lStatus = 0;

		XSECSHeader			head;
		unsigned char* pRespData = dataBuff;

		memcpy(&head, pRespData, sizeof(XSECSHeader));
	
		pData->m_stream   = (int)(head.byte2 & ~0x80);
		pData->m_function = (int)(head.status);
		pData->m_wait     = ( head.byte2 & 0x80 ) ? true : false;
		::memcpy(&pData->m_ticket, head.tr_id, sizeof(pData->m_ticket));

XTRACE(_T("  PyXSECSGemAPI_pop SIZE:%d  <S%dF%d %s>\n"), ulDataSize, 
	   pData->m_stream, pData->m_function, pData->m_wait == true ? _T("W") : _T(""));

		XDataReader reader(pRespData+10,  ulDataSize-10 );
		XData       data;
		while( reader.getValue(&data) ) {
			XSECSItem_loadStream((XSECSItem*)pData->m_data, reader, data);
		}
	} else {
		pData->m_lStatus = -1;
	}
	pData.increment_reference();
	return (PyObject*)pData.m_pT;
}



#undef FUNC 
#define FUNC "PyXSECSGemAPI_reciveMessageCallBack"
PYTHON_DLL_EXTERN
PyObject *PyXSECSGemAPI_reciveMessageCallBack(PyXSECSGemAPI *self, PyObject *args)
{
	PyObject*		result = NULL;
	PyObject*		temp;
	xsecsPyGemAPI*	pGemAPI;

	pGemAPI = (xsecsPyGemAPI*)self->m_gemAPI;

	if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
		if (!PyCallable_Check(temp)) {
			PyErr_SetString(PyExc_TypeError, "parameter must be callable");
			return NULL;
		}
		Py_XINCREF(temp);         /* 新たなコールバックへの参照を追加 */
		XSSPY_XDECREF(self->m_pReciveEventCallBack);  /* 以前のコールバックを捨てる */
		self->m_pReciveEventCallBack = temp;       /* 新たなコールバックを記憶 */
		/* "None" を返す際の定型句 */
		Py_INCREF(Py_None);
		result = Py_None;

		pGemAPI->m_pCallBack = temp;
	}
	return result;
}
