#ifdef _MSC_VER
#	pragma warning( disable : 4786 ) 
#endif

#include "xsecsPyPort.h"
#include "xsecsPySECSData.h"
#include "xsecsPySECSItem.h"
#include "structmember.h"


static PyMemberDef XSECSData_members[] = {
	// 時間
	{"status", T_LONG, offsetof(XSECSData, m_lStatus), 0, "Status"},
	// 時間
	{"timeSec", T_LONG, offsetof(XSECSData, m_timeSec), 0, "Date Time(sec)"},
	// ミリ秒
	{"timeUsec", T_UINT, offsetof(XSECSData, m_timeUsec), 0, "Date Time(msec)"},
	// 送信元IP
	{"ipSrc", T_UINT, offsetof(XSECSData, m_ipSrc), 0, "Src IP Address"},
	//送信先IP
	{"ipDst", T_UINT, offsetof(XSECSData, m_ipDst), 0, "Dst IP Address"},
	// 送信元ポート
	{"portSrc", T_USHORT, offsetof(XSECSData, m_portSrc), 0, "Src Port Number"},
	// 送信先ポート
	{"portDst", T_USHORT, offsetof(XSECSData, m_portDst), 0, "Dst Port Number"},
	// ストリーム
	{"stream", T_UINT, offsetof(XSECSData, m_stream), 0, "stream"},
	// ファンクション
	{"function", T_UINT, offsetof(XSECSData, m_function), 0, "function"},
	// WAIT
	{"wait", T_USHORT, offsetof(XSECSData, m_wait), 0, "wait"},
	// デバイスID
	{"deviceID", T_USHORT, offsetof(XSECSData, m_deviceID), 0, "deviceID"},
	// チケット
	{"ticket", T_UINT, offsetof(XSECSData, m_ticket), 0, "ticket"},
	// データ
	{"data", T_OBJECT_EX, offsetof(XSECSData, m_data), 0, "Data Items(XSECSItem instance)"},
    {NULL}  /* Sentinel */
};



static PyMethodDef XSECSData_methods[] = {
	// 時間文字列を取得
    {"timeString", (PyCFunction)XSECSData_timeString, METH_NOARGS, "Return Time String" },
	// 送信元ホスト情報文字列を取得
    {"srcHostString", (PyCFunction)XSECSData_srcHostString, METH_NOARGS, "Return Src IPAddress:PortNumber" },
    // 送信先ホスト情報文字列を取得
	{"dstHostString", (PyCFunction)XSECSData_dstHostString, METH_NOARGS, "Return Dst IPAddress:PortNumber" },
    {NULL}  /* Sentinel */
};



/**	@brief タイプオブジェクト 
 */
static PyTypeObject g_XSECSDataType = {
    PyObject_HEAD_INIT(NULL)
    0,								/*ob_size*/
    "xsecsPyPort.XSECSData",		/*tp_name*/
    sizeof(XSECSData),				/*tp_basicsize*/
    0,								/*tp_itemsize*/
    (destructor)XSECSData_dealloc, /*tp_dealloc*/
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
    "XSECSData objects",			/* tp_doc */
    0,								/* tp_traverse */
    0,								/* tp_clear */
    0,								/* tp_richcompare */
    0,								/* tp_weaklistoffset */
    0,								/* tp_iter */
    0,								/* tp_iternext */
    XSECSData_methods,				/* tp_methods */
    XSECSData_members,				/* tp_members */
//    XSECSData_getseters,			/* tp_getset */
	0,								/* tp_getset */
    0,								/* tp_base */
    0,								/* tp_dict */
    0,								/* tp_descr_get */
    0,								/* tp_descr_set */
    0,								/* tp_dictoffset */
    (initproc)XSECSData_init,		/* tp_init */
    0,								/* tp_alloc */
    XSECSData_new,                  /* tp_new */
};



bool XSECSData_RegistMudule(PyObject *pMOdule)
{
    if (PyType_Ready(&g_XSECSDataType) < 0)
        return false;
    g_XSECSDataType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&g_XSECSDataType) < 0)
        return false;

    Py_INCREF(&g_XSECSDataType);
    PyModule_AddObject(pMOdule, "XSECSData", (PyObject *)&g_XSECSDataType);
	return true;
}


XSECSData* XSECSData_GCNew()
{
#ifdef _DEBUG
//printf("XSECSData_GCNew\n");
#endif
	XSECSData* op = (XSECSData*)XSECSData_new(&g_XSECSDataType, 0, 0);//PyObject_New(XSECSData, &g_XSECSDataType);
	return op;
}

static PyObject *
XSECSData_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
#ifdef _DEBUG
//printf("XSECSData_new\n");
#endif
	
	XSECSData *self = NULL;
	try {
		self = (XSECSData *)type->tp_alloc(type, 0);
		if (self != NULL) {
			self->m_lStatus = 0;
			self->m_timeSec = 0;
			self->m_timeUsec = 0;
			self->m_ipSrc = 0;
			self->m_ipDst = 0;
			self->m_portSrc = 0;
			self->m_portDst = 0;
			self->m_stream = 0;
			self->m_function = 0;
			self->m_wait = false;
			self->m_data = (PyObject*)XSECSItem_GCNew();
		}
	} catch(...) {
		XTRACE(_T("Error !! XSECSData_new. unknown error occured !!"));
		return NULL;
	}
	return (PyObject *)self;
}

PyMODINIT_FUNC
XSECSData_dealloc(XSECSData* self)
{
#ifdef _DEBUG
//printf("XSECSData_dealloc\n");
#endif
	try {
		Py_XDECREF(self->m_data);
		self->ob_type->tp_free((PyObject*)self);
	} catch (...) {
		XTRACE(_T("Error !! XSECSData_dealloc. unknown error occured !!"));
	}
}

PYTHON_DLL_EXTERN int XSECSData_init(XSECSData *self, PyObject *args, PyObject *kwds)
{
#ifdef _DEBUG
//printf("XSECSData_init\n");
#endif
	try {
		static char *kwlist[] = {"stream", "function", "wait",  NULL};
		if (! PyArg_ParseTupleAndKeywords(args, kwds, "|iii", kwlist, 
										  &self->m_stream,
										  &self->m_function,
										  &self->m_wait)) {
			return -1; 
		}

		self->m_timeSec = 0;
		self->m_timeUsec = 0;
		self->m_ipSrc = 0;
		self->m_ipDst = 0;
		self->m_portSrc = 0;
		self->m_portDst = 0;

//		self->m_wait = false;
		if( self->m_data == NULL ) {
			self->m_data = (PyObject*)XSECSItem_GCNew();
		} else {
#ifdef _DEBUG
//printf("  NN\n");
#endif
		}
	} catch (...) {
		XTRACE(_T("Error !! XSECSData_init. unknown error occured !!"));
		return -1;
	}
    return 0;
}

bool XSECSData_CheckType(PyObject *data)
{
#ifdef _DEBUG
//printf("XSECSData_CheckType\n");
#endif
	return PyObject_TypeCheck(data, &g_XSECSDataType);
}

PYTHON_DLL_EXTERN 
PyObject *XSECSData_timeString(XSECSData* self)
{
	char szTime[64];
	time_t wkTime = self->m_timeSec;
	struct tm *pTm = localtime(&wkTime);

    if (pTm == NULL) {
        PyErr_SetString(PyExc_AttributeError, "timeSec");
        return NULL;
    }

	sprintf(szTime, "%04d/%02d/%02d %02d:%02d:%02d.%03u",
		pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday,
		pTm->tm_hour, pTm->tm_min, pTm->tm_sec, self->m_timeUsec / 1000);

     return Py_BuildValue("s", szTime);
}


PYTHON_DLL_EXTERN 
PyObject *XSECSData_srcHostString(XSECSData* self)
{
	char szString[64];
	unsigned char szIP[4];

	memcpy(szIP, &self->m_ipSrc, sizeof(self->m_ipSrc));
	sprintf(szString, "%u.%u.%u.%u:%u", (int)szIP[0], (int)szIP[1]
		, (int)szIP[2], (int)szIP[3], self->m_portSrc);

     return Py_BuildValue("s", szString);
}

PYTHON_DLL_EXTERN 
PyObject *XSECSData_dstHostString(XSECSData* self)
{
	char szString[64];
	unsigned char szIP[4];

	memcpy(szIP, &self->m_ipSrc, sizeof(self->m_ipDst));
	sprintf(szString, "%u.%u.%u.%u:%u", (int)szIP[0], (int)szIP[1]
		, (int)szIP[2], (int)szIP[3], self->m_portDst);

     return Py_BuildValue("s", szString);
}

