#ifdef _MSC_VER
#	pragma warning( disable : 4786 ) 
#endif

#include "xsecsPyPort.h"

#include "structmember.h"

#include "xsecsPySECSItem.h"
#include "xlib/xdata.h"
#include "xlib/xbuff.h"
#include "xlib/semi/secs/xsecs_text_parser.h"
#include "xlib/xeception.h"
#include <iostream>

extern PyObject *g_pError;


static PyObject* XSECSItem_getvalues(XSECSItem *self, void *closure);
static int XSECSItem_setvalues(XSECSItem *self, PyObject *value, void *closure);


static PyMemberDef g_XSECSItem_members[] = {
    {"type", T_INT, offsetof(XSECSItem, m_type), 0, "item type"},
	{"lengthByte", T_INT, offsetof(XSECSItem, m_iLengthByte), 0, "item Length Byte"},
//    {"values", T_OBJECT_EX, offsetof(XSECSItem, m_values), 0, "item values"},
    {"parent", T_OBJECT_EX, offsetof(XSECSItem, m_pParent), 0, "get parent Item"},
    {NULL}  /* Sentinel */
};

static PyMethodDef g_XSECSItema_methods[] = {
    {"getTypeString",      (PyCFunction)XSECSData_getTypeString, METH_NOARGS, "Return Type String" },
    {"getValueString",     (PyCFunction)XSECSData_getValueString, METH_NOARGS, "Return Value String" },
    {"setValueFromString", (PyCFunction)XSECSData_setValueFromString, METH_VARARGS, "set Value from String" },
	{"clone",               (PyCFunction)XSECSItem_clone, METH_VARARGS, "Clone This Item"},
    {NULL}  /* Sentinel */
};

static PyGetSetDef g_XSECSItema_getseters[] = {
   
	{"values", (getter)XSECSItem_getvalues, (setter)XSECSItem_setvalues, "values", NULL},
    {NULL}  /* Sentinel */
};


/**	@brief タイプオブジェクト 
 */
static PyTypeObject g_XSECSItemType = {
    PyObject_HEAD_INIT(NULL)
    0,							/*ob_size*/
    "xsecsPyPort.XSECSItem",	/*tp_name*/
    sizeof(XSECSItem),			/*tp_basicsize*/
    0,							/*tp_itemsize*/
    (destructor)XSECSItem_dealloc, /*tp_dealloc*/
    0,							/*tp_print*/
    0,							/*tp_getattr*/
    0,							/*tp_setattr*/
    0,							/*tp_compare*/
    0,							/*tp_repr*/
    0,							/*tp_as_number*/
    0,							/*tp_as_sequence*/
    0,							/*tp_as_mapping*/
    0,							/*tp_hash */
    0,							/*tp_call*/
    0,							/*tp_str*/
    0,							/*tp_getattro*/
    0,							/*tp_setattro*/
    0,							/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "XSECSItem objects",		/* tp_doc */
    0,							/* tp_traverse */
    0,							/* tp_clear */
    0,							/* tp_richcompare */
    0,							/* tp_weaklistoffset */
    0,							/* tp_iter */
    0,							/* tp_iternext */
    g_XSECSItema_methods,		/* tp_methods */
    g_XSECSItem_members,		/* tp_members */
    g_XSECSItema_getseters,		/* tp_getset */
    0,							/* tp_base */
    0,							/* tp_dict */
    0,							/* tp_descr_get */
    0,							/* tp_descr_set */
    0,							/* tp_dictoffset */
    (initproc)XSECSItem_init,	/* tp_init */
    0,							/* tp_alloc */
    XSECSItem_new,				/* tp_new */
};

bool XSECSItem_RegistMudule(PyObject *pMOdule)
{
#ifdef _DEBUG
//printf("XSECSItem_RegistMudule\n");
#endif
	if (PyType_Ready(&g_XSECSItemType) < 0)
		return false;
	g_XSECSItemType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&g_XSECSItemType) < 0)
		return false;
	Py_INCREF(&g_XSECSItemType);
	PyModule_AddObject(pMOdule, "XSECSItem", (PyObject *)&g_XSECSItemType);
	return true;
}

XSECSItem* XSECSItem_GCNew()
{
#ifdef _DEBUG
//printf("XSECSItem_GCNew\n");
#endif
	try {
		XSECSItem* op = (XSECSItem*)XSECSItem_new(&g_XSECSItemType, 0, 0);//PyObject_New(XSECSItem, &g_XSECSItemType);
		return op;
	} catch (...) {
		PyErr_SetString(g_pError, "XSECSItem_GCNew");
		return NULL;
	}
}



PYTHON_DLL_EXTERN PyObject *XSECSItem_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
#ifdef _DEBUG
//printf("XSECSItem_new\n");
#endif
	try {
		XSECSItem *self;
		PyObject *values=NULL;

		self = (XSECSItem *)type->tp_alloc(type, 0);
		if (self != NULL) {
			self->m_values = PyList_New(0);
			if (self->m_values == NULL) {
				self->ob_type->tp_free((PyObject*)self);
				return NULL;
			}

			self->m_type = 0;
			self->m_iLengthByte = -1;
			self->m_pParent = NULL;
		}
		return (PyObject *)self;
	} catch (...) {
		PyErr_SetString(g_pError, "XSECSItem_new");
		return NULL;
	}
}


PyMODINIT_FUNC XSECSItem_dealloc(XSECSItem* self)
{
#ifdef _DEBUG
//printf("XSECSItem_dealloc\n");
#endif
	try {
		XSSPY_XDECREF(self->m_pParent);
	    XSSPY_XDECREF(self->m_values);
	    self->ob_type->tp_free((PyObject*)self);
	} catch (...) {
		PyErr_SetString(g_pError, "XSECSItem_dealloc");
	}
}

void XSECSItem_ZeroInit(XSECSItem* pItem)
{
	PyObject *tmp;


	pItem->m_iLengthByte = -1;
	tmp = pItem->m_values;
	switch( pItem->m_type ) {
	case XDATA_TYPE_A:
	case XDATA_TYPE_J:
	case XDATA_TYPE_UTF16:
		pItem->m_values = Py_BuildValue("s", "");
		break;
	default:
		pItem->m_values = (PyObject*)PyList_New(0);
		break;
	}
	XSSPY_XDECREF(tmp);
}

PYTHON_DLL_EXTERN int XSECSItem_init(XSECSItem *self, PyObject *args, PyObject *kwds)
{
#ifdef _DEBUG
//printf("XSECSItem_init\n");
#endif
	try {
		PyObject *values=NULL, *tmp;
		PyObject *value = NULL;
		int		iLengthByte = -1;
		static char *kwlist[] = {"type", "value", "lengthByte",  NULL};

		if (! PyArg_ParseTupleAndKeywords(args, kwds, "|iOi", kwlist, 
										  &self->m_type, &value, &iLengthByte)) {
			return -1; 
		}
		self->m_iLengthByte = iLengthByte;

		if (values) {
			tmp = self->m_values;
			Py_INCREF(values);
			self->m_values = values;
			XSSPY_XDECREF(tmp);
		}

		switch( self->m_type ) {
		case XDATA_TYPE_A:
		case XDATA_TYPE_J:
		case XDATA_TYPE_UTF16:
			tmp = self->m_values;
			self->m_values = Py_BuildValue("s", "");
			XSSPY_XDECREF(tmp);

			if( value != NULL ) {

				if( !PyString_Check(value) ) {
					return -1;
				}

				Py_INCREF(value);
				XSSPY_XDECREF(self->m_values);
				self->m_values = value;
			}



			break;
		default:
			tmp = self->m_values;
			self->m_values = (PyObject*)PyList_New(0);
			XSSPY_XDECREF(tmp);

			if( value != NULL ) {
				if( !PyList_Check(value) ) {
					return -1;
				}
				Py_INCREF(value);
				XSSPY_XDECREF(self->m_values);
				self->m_values = value;
			}
			break;
		}
		return 0;
	} catch (...) {
		PyErr_SetString(g_pError, "XSECSItem_init.");
		return NULL;
	}
}

bool XSECSItem_CheckType(PyObject *data)
{
#ifdef _DEBUG
//printf("XSECSItem_CheckType\n");
#endif
	return PyObject_TypeCheck(data, &g_XSECSItemType);
}



static PyObject *
XSECSItem_getvalues(XSECSItem *self, void *closure)
{
#ifdef _DEBUG
//printf("XSECSItem_getvalues\n");
#endif
    Py_INCREF(self->m_values);
    return self->m_values;
}

static int
XSECSItem_setvalues(XSECSItem *self, PyObject *value, void *closure)
{
#ifdef _DEBUG
//printf("XSECSItem_setvalues\n");
#endif
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the values attribute");
		return -1;
	}
	switch( self->m_type ) {
	case XDATA_TYPE_A:
	case XDATA_TYPE_J:
	case XDATA_TYPE_UTF16:
		if( !PyString_Check(value)) {
			PyErr_SetString(PyExc_TypeError, 
							"values attribute value must be a string");
			return -1;
		}
		break;
		//
	case XDATA_TYPE_B:
		if( PyString_Check(value)) {

			// 文字列で指定された場合は、配列に変換

			char* pString = NULL;
			pString = PyString_AsString(value);

			XSSPY_XDECREF(self->m_values);
			self->m_values = (PyObject*)PyList_New(0);
			while( *pString != 0 ) {
				unsigned char c = (unsigned char)*pString;
				PyObject *pObject = Py_BuildValue("B", c);
				PyList_Append(self->m_values, (PyObject*)pObject);
				pString++;
			}
			return 0;
		} else
		if( !PyList_Check(value)) {
			PyErr_SetString(PyExc_TypeError, 
							"values attribute value must be a list or string");
			return -1;
		}
		break;
	default:
		if( !PyList_Check(value)) {
			PyErr_SetString(PyExc_TypeError, 
							"values attribute value must be a list");
			return -1;
		}
		break;
	}
	Py_DECREF(self->m_values);
	Py_INCREF(value);
	self->m_values = value;    
	return 0;
}



PYTHON_DLL_EXTERN 
PyObject *XSECSData_getTypeString(XSECSItem* self)
{
	char sz[64] = "";

	if( self == NULL ) {
		return 0;
	}

	switch( (XDATA_TYPE)self->m_type ) {
	case XDATA_TYPE_L:		strcpy(sz, "L");		break;
	case XDATA_TYPE_B:		strcpy(sz, "B");		break;
	case XDATA_TYPE_BOOL:	strcpy(sz, "BOOL");		break;
	case XDATA_TYPE_A:		strcpy(sz, "A");		break;
	case XDATA_TYPE_J:		strcpy(sz, "J");		break;
	case XDATA_TYPE_UTF16:	strcpy(sz, "UTF16");	break;
	case XDATA_TYPE_U1:		strcpy(sz, "U1");		break;
	case XDATA_TYPE_U2:		strcpy(sz, "U2");		break;
	case XDATA_TYPE_U4:		strcpy(sz, "U4");		break;
	case XDATA_TYPE_U8:		strcpy(sz, "U8");		break;
	case XDATA_TYPE_I1:		strcpy(sz, "I1");		break;
	case XDATA_TYPE_I2:		strcpy(sz, "I2");		break;
	case XDATA_TYPE_I4:		strcpy(sz, "I4");		break;
	case XDATA_TYPE_I8:		strcpy(sz, "I8");		break;
	case XDATA_TYPE_F4:		strcpy(sz, "F4");		break;
	case XDATA_TYPE_F8:		strcpy(sz, "F8");		break;
	default:
		sprintf(sz, "invalid type;%02X", self->m_type);
		break;
	}

	return Py_BuildValue("s", sz);
}
PYTHON_DLL_EXTERN 
PyObject *XSECSData_getValueString(XSECSItem* self)
{
	if( self->m_values == NULL || self->m_values == (PyObject*)-1 ) {
		return NULL;
	}


	switch( (XDATA_TYPE)self->m_type ) {
	case XDATA_TYPE_L:
		break;
	case XDATA_TYPE_A:
	case XDATA_TYPE_J:
	case XDATA_TYPE_UTF16:
		{
			char* pString = NULL;
			if( PyString_Check(self->m_values) != 1 ) {
				return NULL;
			}
			pString = PyString_AsString(self->m_values);
			if( pString == NULL ) {
				return NULL;
			}
			return Py_BuildValue("s", pString);
		}
		break;
	default:
		{
			XInflateStreamBuff buff;
			std::ostream stream(&buff);

			char szBUFF[245];
			if( PyList_CheckExact(self->m_values) != 1 ) {
				return NULL;
			}
			int iSize = PyList_Size(self->m_values);
			for(int i = 0; i < iSize; i++) {
				PyObject *pItem = PyList_GetItem(self->m_values, i);
				if( pItem == NULL ) {
					return NULL;
				}
				if( i != 0 ) {
					stream << " ";
				}
				if( ((XDATA_TYPE)self->m_type) == XDATA_TYPE_B		||
					((XDATA_TYPE)self->m_type) == XDATA_TYPE_BOOL	||
					((XDATA_TYPE)self->m_type) == XDATA_TYPE_U1		||
					((XDATA_TYPE)self->m_type) == XDATA_TYPE_U2		||
					((XDATA_TYPE)self->m_type) == XDATA_TYPE_U4		||
					((XDATA_TYPE)self->m_type) == XDATA_TYPE_U8		||
					((XDATA_TYPE)self->m_type) == XDATA_TYPE_I1		||
					((XDATA_TYPE)self->m_type) == XDATA_TYPE_I2		||
					((XDATA_TYPE)self->m_type) == XDATA_TYPE_I4		||
					((XDATA_TYPE)self->m_type) == XDATA_TYPE_I8) {

					if( PyInt_Check(pItem) != 1 ) {
						return NULL;
					}

					if( ((XDATA_TYPE)self->m_type) == XDATA_TYPE_B ) {
						sprintf(szBUFF, "%02X", (unsigned char)PyLong_AsUnsignedLong(pItem));
					} else 
					if( ((XDATA_TYPE)self->m_type) == XDATA_TYPE_BOOL ) {
						if( PyInt_AsLong(pItem) == 0 ) {
							strcpy(szBUFF, "false");
						} else {
							strcpy(szBUFF, "true");
						}
					} else 
					if( ((XDATA_TYPE)self->m_type) == XDATA_TYPE_U1 ||
						((XDATA_TYPE)self->m_type) == XDATA_TYPE_U2 ||
						((XDATA_TYPE)self->m_type) == XDATA_TYPE_U4 ) {
						sprintf(szBUFF, "%u", PyLong_AsUnsignedLongMask(pItem));
					} else 
					if( ((XDATA_TYPE)self->m_type) == XDATA_TYPE_I1 ||
						((XDATA_TYPE)self->m_type) == XDATA_TYPE_I2 ||
						((XDATA_TYPE)self->m_type) == XDATA_TYPE_I4 ) {
						sprintf(szBUFF, "%d", PyLong_AsLong(pItem));
					}
					stream << szBUFF;
				} else 
				if( ((XDATA_TYPE)self->m_type) == XDATA_TYPE_U8		||
					((XDATA_TYPE)self->m_type) == XDATA_TYPE_I8) {

					if( PyLong_Check(pItem) != 1 ) {
						return NULL;
					}
					if( ((XDATA_TYPE)self->m_type) == XDATA_TYPE_U8 ) {
						sprintf(szBUFF, "%I64u", PyLong_AsUnsignedLongLong(pItem));
					} else 
					if( ((XDATA_TYPE)self->m_type) == XDATA_TYPE_I8 ) {
						sprintf(szBUFF, "%I64d", PyLong_AsLongLong(pItem));
					}
					stream << szBUFF;
				}else 
				if( ((XDATA_TYPE)self->m_type) == XDATA_TYPE_F4		||
					((XDATA_TYPE)self->m_type) == XDATA_TYPE_F8) {
					if( PyFloat_Check(pItem) != 1 ) {
						return NULL;
					}
					sprintf(szBUFF, "%e", PyFloat_AsDouble(pItem));
					stream << szBUFF;
				}
				pItem = NULL;
			}
			return Py_BuildValue("s", buff.toString());
		}

	}

	return Py_BuildValue("s", "!! ERROR !!");
}


#ifdef _UNICODE
#define XSECSDATA_SETVALSTE() 		strBuff = pString; TCHAR *pBuff = strBuff;\
			strBuff << _T(">");	\
			size_t len = strBuff.getLength();\
			XSecsTextParser parser;\
			XRichData data((XDATA_TYPE)self->m_type);\
			TCHAR *p = pBuff;\
			bool bret = parser.parseDigitFromString(&p, len, &data);\
			delete []pBuff;\
			if( bret == false ) {\
				PyErr_SetString(g_pError, XStrBuff(parser.getErrorString()));\
				return NULL;\
			}
#else 


#define XSECSDATA_SETVALSTE() 		char *pBuff = new char[strlen(pString) + 2];\
			if( pBuff == NULL ) {\
				PyErr_SetString(PyExc_MemoryError, "");\
				return NULL;\
			}\
			sprintf(pBuff, "%s>", pString);\
			size_t len = strlen(pBuff);\
			XSecsTextParser parser;\
			XRichData data((XDATA_TYPE)self->m_type);\
			char *p = pBuff;\
			bool bret = parser.parseDigitFromString(&p, len, &data);\
			delete []pBuff;\
			if( bret == false ) {\
				PyErr_SetString(g_pError, parser.getErrorString());\
				return NULL;\
			}
#endif



PYTHON_DLL_EXTERN 
PyObject *XSECSData_setValueFromString(XSECSItem* self, PyObject *args)
{
	try {
		const char *pString;
		XStrBuff	strBuff;

		if (!PyArg_ParseTuple(args, "s", &pString))
			return NULL;


		if( strBuff.Alloc( (strlen(pString)+255) * sizeof( TCHAR )  ) == NULL ) {
			PyErr_SetString(PyExc_MemoryError, "");
			return NULL;
		}

		switch( (XDATA_TYPE)self->m_type ) {
		case XDATA_TYPE_A:
		case XDATA_TYPE_J:
		case XDATA_TYPE_UTF16:
			Py_DECREF(self->m_values);
			self->m_values = Py_BuildValue("s", (const char*)pString);
			break;
		case XDATA_TYPE_BOOL:
			{
				XSECSDATA_SETVALSTE()

				Py_DECREF(self->m_values);
				self->m_values = (PyObject*)PyList_New(0);
				for(XD_INT4 i = 0 ; i < data.getDataCount(); i++ ) {
					PyObject *pObject = Py_BuildValue("B", data.getValueUInt1(i));
					PyList_Append(self->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_B:
			{
				strBuff = pString; TCHAR *pBuff = strBuff;
				strBuff << _T(">");	
				size_t len = strBuff.getLength();
				XSecsTextParser parser;
				XRichData data((XDATA_TYPE)self->m_type);
				TCHAR *p = pBuff;
				bool bret = parser.parseDigitFromString(&p, len, &data);
				delete []pBuff;
				if( bret == false ) {
					PyErr_SetString(g_pError, XStrBuff(parser.getErrorString()));
					return NULL;
				}

				Py_DECREF(self->m_values);
				self->m_values = (PyObject*)PyList_New(0);
				for(XD_INT4 i = 0 ; i < data.getDataCount(); i++ ) {
					PyObject *pObject = Py_BuildValue("B", data.getValueUInt1(i));
					PyList_Append(self->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_U1:
			{
				XSECSDATA_SETVALSTE()

				Py_DECREF(self->m_values);
				self->m_values = (PyObject*)PyList_New(0);
				for(XD_INT4 i = 0 ; i < data.getDataCount(); i++ ) {
					PyObject *pObject = Py_BuildValue("B", data.getValueUInt1(i));
					PyList_Append(self->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_U2:
			{
				XSECSDATA_SETVALSTE()

				Py_DECREF(self->m_values);
				self->m_values = (PyObject*)PyList_New(0);
				for(XD_INT4 i = 0 ; i < data.getDataCount(); i++ ) {
					PyObject *pObject = Py_BuildValue("H", data.getValueUInt2(i));
					PyList_Append(self->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_U4:
			{
				XSECSDATA_SETVALSTE()

				Py_DECREF(self->m_values);
				self->m_values = (PyObject*)PyList_New(0);
				for(XD_INT4 i = 0 ; i < data.getDataCount(); i++ ) {
					PyObject *pObject = Py_BuildValue("k", data.getValueUInt4(i));
					if( pObject == NULL ) {
						PyErr_SetString(g_pError, "Py_BuildValue('k') Error.");
						return NULL;
					}
					PyList_Append(self->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_U8:
			{
				XSECSDATA_SETVALSTE()

				Py_DECREF(self->m_values);
				self->m_values = (PyObject*)PyList_New(0);
				for(XD_INT4 i = 0 ; i < data.getDataCount(); i++ ) {
					PyObject *pObject = Py_BuildValue("K", data.getValueUInt8(i));
					PyList_Append(self->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_I1:
			{
				XSECSDATA_SETVALSTE()

				Py_DECREF(self->m_values);
				self->m_values = (PyObject*)PyList_New(0);
				for(XD_INT4 i = 0 ; i < data.getDataCount(); i++ ) {
					PyObject *pObject = Py_BuildValue("b", data.getValueInt1(i));
					PyList_Append(self->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_I2:
			{
				XSECSDATA_SETVALSTE()

				Py_DECREF(self->m_values);
				self->m_values = (PyObject*)PyList_New(0);
				for(XD_INT4 i = 0 ; i < data.getDataCount(); i++ ) {
					PyObject *pObject = Py_BuildValue("h", data.getValueInt2(i));
					PyList_Append(self->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_I4:
			{
				XSECSDATA_SETVALSTE()

				Py_DECREF(self->m_values);
				self->m_values = (PyObject*)PyList_New(0);
				for(XD_INT4 i = 0 ; i < data.getDataCount(); i++ ) {
					PyObject *pObject = Py_BuildValue("l", data.getValueInt4(i));
					PyList_Append(self->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_I8:
			{
				XSECSDATA_SETVALSTE()

				Py_DECREF(self->m_values);
				self->m_values = (PyObject*)PyList_New(0);
				for(XD_INT4 i = 0 ; i < data.getDataCount(); i++ ) {
					PyObject *pObject = Py_BuildValue("L", data.getValueInt8(i));
					PyList_Append(self->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_F4:
			{
				XSECSDATA_SETVALSTE()

				Py_DECREF(self->m_values);
				self->m_values = (PyObject*)PyList_New(0);
				for(XD_INT4 i = 0 ; i < data.getDataCount(); i++ ) {
					PyObject *pObject = Py_BuildValue("f", data.getValueFloat4(i));
					PyList_Append(self->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_F8:
			{
				XSECSDATA_SETVALSTE()

				Py_DECREF(self->m_values);
				self->m_values = (PyObject*)PyList_New(0);
				for(XD_INT4 i = 0 ; i < data.getDataCount(); i++ ) {
					PyObject *pObject = Py_BuildValue("f", data.getValueFloat8(i));
					PyList_Append(self->m_values, (PyObject*)pObject);
				}
			}
			break;
		default:
			{
				XStrBuff strErr;
				strErr.Format(_T("data type error . %08X in (XSECSData_setValueFromString). "), self->m_type);
				PyErr_SetString(g_pError, strErr);
			}
			return NULL;
		}
		return Py_BuildValue("s", pString);
	} catch (...) {
		PyErr_SetString(g_pError, "XSECSData_setValueFromString.");
		return NULL;
	}
}


#undef  FUNC 
#define FUNC "XSECSItem_clone"
PYTHON_DLL_EXTERN 
PyObject *XSECSItem_clone(XSECSItem* self, PyObject *args)
{
	try {
		//
		// 自身のクーロンを作成する。
		//
		Py_INCREF(self);
		return Py_BuildValue("O", self);
	} catch (...) {
		PyErr_SetString(g_pError, FUNC);
		return NULL;
	}
	return NULL;
}

#undef  FUNC 
#define FUNC "XSECSItem_storeStream"
bool XSECSItem_storeStream(XDataWriter &writer,  XSECSItem* pItem, bool bStream /*= false*/)
{
	try {
		if( pItem == NULL ) {
			return true;
		}
		if( pItem->m_values == NULL || pItem->m_values == (PyObject*)-1) {
			return true;
		}
		if( pItem->m_type == 0 ) {
			return true;
		}

		switch( (XDATA_TYPE)pItem->m_type ) {
		case XDATA_TYPE_L:
			{
				if( !PyList_Check(pItem->m_values) ) {
					XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
					return false;
				}

				int iSizeItem, iItem;
				iSizeItem = PyList_Size(pItem->m_values);

				writer.putValAsList(iSizeItem);
				
				for(iItem = 0; iItem < iSizeItem; iItem++) {
					PyObject* pObje = PyList_GetItem(pItem->m_values, iItem);
					if( pObje == NULL ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					if( !XSECSItem_CheckType(pObje) ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					if( XSECSItem_storeStream(writer, (XSECSItem*)pObje) == false ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
				}
			}				
			break;
		case XDATA_TYPE_A:
		case XDATA_TYPE_J:
		case XDATA_TYPE_UTF16:
			{
				if( !PyString_Check(pItem->m_values) ) {
					XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
					return false;
				}
				char* pString = PyString_AsString(pItem->m_values);

				if( (XDATA_TYPE)pItem->m_type == XDATA_TYPE_A ) {
					if( bStream == false ) {
						writer.putValAsStringA(pString);
					} else {
						*(writer.get_stream()) << pString;
					}
				} else
				if( (XDATA_TYPE)pItem->m_type == XDATA_TYPE_J ) {
					if( bStream == false ) {
						writer.putValAsStringA(pString);
					} else {
						*(writer.get_stream()) << pString;
					}
				} else {
					if( bStream == false ) {
						writer.putValAsStringW(XStrBuff(pString));
					} else {
						writer.get_stream()->write(
							(const char *)(const wchar_t*)XStrBuff(pString),
							(wcslen((const wchar_t*)XStrBuff(pString))+1)*sizeof(wchar_t)
							);
					}
				}
			}
			break;
		case XDATA_TYPE_B:
		case XDATA_TYPE_BOOL:
		case XDATA_TYPE_U1:
			{
				if( !PyList_Check(pItem->m_values) ) {
					XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
					return false;
				}
				unsigned char *pData = NULL;
				unsigned long iSizeItem, iItem;
				
				iSizeItem = (unsigned long)PyList_Size(pItem->m_values);
				pData = new unsigned char[iSizeItem+10];

				for(iItem = 0; iItem < iSizeItem; iItem++) {
					PyObject* pObje = PyList_GetItem(pItem->m_values, iItem);
					if( pObje == NULL ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					if( !PyInt_Check(pObje) ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					pData[iItem] = (unsigned char)PyInt_AsUnsignedLongMask(pObje);
				}
				if( bStream == false ) {
					writer.putValue((XDATA_TYPE)pItem->m_type, pData, iSizeItem, pItem->m_iLengthByte);
				} else {
					writer.get_stream()->write((const char *)pData, iSizeItem);
				}
				delete []pData;
			}
			break;
		case XDATA_TYPE_I1:
			{
				if( !PyList_Check(pItem->m_values) ) {
					XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
					return false;
				}
				char *pData = NULL;
				unsigned long iSizeItem, iItem;
				
				iSizeItem = (unsigned long)PyList_Size(pItem->m_values);
				pData = new char[iSizeItem+10];

				for(iItem = 0; iItem < iSizeItem; iItem++) {
					PyObject* pObje = PyList_GetItem(pItem->m_values, iItem);
					if( pObje == NULL ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					if( !PyInt_Check(pObje) ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					pData[iItem] = (char)PyInt_AsLong(pObje);
				}
				if( bStream == false ) {
					writer.putValue((XDATA_TYPE)pItem->m_type, pData, iSizeItem, pItem->m_iLengthByte);
				} else {
					writer.get_stream()->write((const char *)pData, iSizeItem);
				}
				delete []pData;
			}
			break;
		case XDATA_TYPE_U2:
			{
				if( !PyList_Check(pItem->m_values) ) {
					return false;
				}
				unsigned short *pData = NULL;
				unsigned long iSizeItem, iItem;
				
				iSizeItem = (unsigned long)PyList_Size(pItem->m_values);
				pData = new unsigned short[iSizeItem+10];

				for(iItem = 0; iItem < iSizeItem; iItem++) {
					PyObject* pObje = PyList_GetItem(pItem->m_values, iItem);
					if( pObje == NULL ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					if( !PyInt_Check(pObje) ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					pData[iItem] = (unsigned short)PyInt_AsUnsignedLongMask(pObje);
				}
				if( bStream == false ) {
					writer.putValue((XDATA_TYPE)pItem->m_type, pData, iSizeItem, pItem->m_iLengthByte);
				} else {
					writer.get_stream()->write((const char *)pData, iSizeItem*sizeof(unsigned short));
				}
				delete []pData;
			}
			break;
		case XDATA_TYPE_I2:
			{
				if( !PyList_Check(pItem->m_values) ) {
					XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
					return false;
				}
				short *pData = NULL;
				unsigned long iSizeItem, iItem;
				
				iSizeItem = (unsigned long)PyList_Size(pItem->m_values);
				pData = new short[iSizeItem+10];

				for(iItem = 0; iItem < iSizeItem; iItem++) {
					PyObject* pObje = PyList_GetItem(pItem->m_values, iItem);
					if( pObje == NULL ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					if( !PyInt_Check(pObje) ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					pData[iItem] = (short)PyInt_AsLong(pObje);
				}
				if( bStream == false ) {
					writer.putValue((XDATA_TYPE)pItem->m_type, pData, iSizeItem, pItem->m_iLengthByte);
				} else {
					writer.get_stream()->write((const char *)pData, iSizeItem*sizeof(short));
				}
				delete []pData;
			}
			break;
		case XDATA_TYPE_U4:
			{
				if( !PyList_Check(pItem->m_values) ) {
					XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
					return false;
				}
				unsigned long *pData = NULL;
				unsigned long iSizeItem, iItem;
				
				iSizeItem = (unsigned long)PyList_Size(pItem->m_values);
				pData = new unsigned long[iSizeItem+10];

				for(iItem = 0; iItem < iSizeItem; iItem++) {
					PyObject* pObje = PyList_GetItem(pItem->m_values, iItem);
					if( pObje == NULL ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					if( !PyInt_Check(pObje) ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					pData[iItem] = (unsigned long)PyInt_AsUnsignedLongMask(pObje);
				}
				if( bStream == false ) {
					writer.putValue((XDATA_TYPE)pItem->m_type, pData, iSizeItem, pItem->m_iLengthByte);
				} else {
					writer.get_stream()->write((const char *)pData, iSizeItem*sizeof(unsigned long));
				}
				delete []pData;
			}
			break;
		case XDATA_TYPE_I4:
			{
				if( !PyList_Check(pItem->m_values) ) {
					XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
					return false;
				}
				long *pData = NULL;
				unsigned long iSizeItem, iItem;
				
				iSizeItem = (unsigned long)PyList_Size(pItem->m_values);
				pData = new long[iSizeItem+10];

				for(iItem = 0; iItem < iSizeItem; iItem++) {
					PyObject* pObje = PyList_GetItem(pItem->m_values, iItem);
					if( pObje == NULL ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					if( !PyInt_Check(pObje) ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					pData[iItem] = (long)PyInt_AsLong(pObje);
				}
				if( bStream == false ) {
					writer.putValue((XDATA_TYPE)pItem->m_type, pData, iSizeItem, pItem->m_iLengthByte);
				} else {
					writer.get_stream()->write((const char *)pData, iSizeItem*sizeof(long));
				}
				delete []pData;
			}
			break;
		case XDATA_TYPE_U8:
			{
				if( !PyList_Check(pItem->m_values) ) {
					XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
					return false;
				}
				XD_UINT8 *pData = NULL;
				unsigned long iSizeItem, iItem;
				
				iSizeItem = (unsigned long)PyList_Size(pItem->m_values);
				pData = new XD_UINT8[iSizeItem+10];

				for(iItem = 0; iItem < iSizeItem; iItem++) {
					PyObject* pObje = PyList_GetItem(pItem->m_values, iItem);
					if( pObje == NULL ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					if( !PyInt_Check(pObje) ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					pData[iItem] = (XD_UINT8)PyInt_AsUnsignedLongLongMask(pObje);
				}
				if( bStream == false ) {
					writer.putValue((XDATA_TYPE)pItem->m_type, pData, iSizeItem, pItem->m_iLengthByte);
				} else {
					writer.get_stream()->write((const char *)pData, iSizeItem*sizeof(XD_UINT8));
				}
				delete []pData;
			}
			break;
		case XDATA_TYPE_I8:
			{
				if( !PyList_Check(pItem->m_values) ) {
					XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
					return false;
				}
				XD_INT8 *pData = NULL;
				unsigned long iSizeItem, iItem;
				
				iSizeItem = (unsigned long)PyList_Size(pItem->m_values);
				pData = new XD_INT8[iSizeItem+10];

				for(iItem = 0; iItem < iSizeItem; iItem++) {
					PyObject* pObje = PyList_GetItem(pItem->m_values, iItem);
					if( pObje == NULL ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					if( !PyInt_Check(pObje) ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					pData[iItem] = (XD_INT8)PyInt_AsUnsignedLongLongMask(pObje);
				}
				if( bStream == false ) {
					writer.putValue((XDATA_TYPE)pItem->m_type, pData, iSizeItem, pItem->m_iLengthByte);
				} else {
					writer.get_stream()->write((const char *)pData, iSizeItem*sizeof(XD_INT8));
				}
				delete []pData;
			}
			break;
		case XDATA_TYPE_F4:
			{
				if( !PyList_Check(pItem->m_values) ) {
					XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
					return false;
				}
				XD_FLOAT4 *pData = NULL;
				unsigned long iSizeItem, iItem;
				
				iSizeItem = (unsigned long)PyList_Size(pItem->m_values);
				pData = new XD_FLOAT4[iSizeItem+10];

				for(iItem = 0; iItem < iSizeItem; iItem++) {
					PyObject* pObje = PyList_GetItem(pItem->m_values, iItem);
					if( pObje == NULL ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					if( !PyFloat_Check(pObje) ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					pData[iItem] = (XD_FLOAT4)PyFloat_AsDouble(pObje);
				}
				if( bStream == false ) {
					writer.putValue((XDATA_TYPE)pItem->m_type, pData, iSizeItem, pItem->m_iLengthByte);
				} else {
					writer.get_stream()->write((const char *)pData, iSizeItem*sizeof(XD_FLOAT4));
				}
				delete []pData;
			}
			break;
		case XDATA_TYPE_F8:
			{
				if( !PyList_Check(pItem->m_values) ) {
					XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
					return false;
				}
				XD_FLOAT8 *pData = NULL;
				unsigned long iSizeItem, iItem;
				
				iSizeItem = (unsigned long)PyList_Size(pItem->m_values);
				pData = new XD_FLOAT8[iSizeItem+10];

				for(iItem = 0; iItem < iSizeItem; iItem++) {
					PyObject* pObje = PyList_GetItem(pItem->m_values, iItem);
					if( pObje == NULL ) {
						XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
						return false;
					}
					if( !PyFloat_Check(pObje) ) {
						return false;
					}
					pData[iItem] = (XD_FLOAT8)PyFloat_AsDouble(pObje);
				}
				if( bStream == false ) {
					writer.putValue((XDATA_TYPE)pItem->m_type, pData, iSizeItem, pItem->m_iLengthByte);
				} else {
					writer.get_stream()->write((const char *)pData, iSizeItem*sizeof(XD_FLOAT8));
				}
				delete []pData;
			}
			break;
		default:
			XTRACE(_T("ERR [%s]Line:%d\n"), FUNC, __LINE__);
			{
				XStrBuff strErr;
				strErr.Format(_T("data type error . %08X in (XSECSItem_storeStream). "), pItem->m_type);
				PyErr_SetString(g_pError, strErr);
				XTRACE(_T("%s\n"), (const TCHAR*)strErr);
			}
			return false;
		}
	} catch (...) {
		PyErr_SetString(g_pError, "XSECSData_setValueFromString.");
		return false;
	}
	return true;
}

#undef  FUNC
#define FUNC "XSECSItem_loadStream"
bool XSECSItem_loadStream(XSECSItem* pItem, XDataReader &reader, XData &data)
{
	try {
		pItem->m_type   = (unsigned int)data.getType();

		switch( data.getType() ) {
		case XDATA_TYPE_L:
			{
				XSSPY_XDECREF(pItem->m_values);
				pItem->m_values = (PyObject*)PyList_New(0);
				if( pItem->m_values == NULL ) {
					return false;
				}
				for(XD_UINT4 i = 0; i < data.getCount(); i++) {
					XData dataChiled;
					if( reader.getValue(&dataChiled) == false ) {
						return false;
					}
					XSECSItem *pItemChiled = XSECSItem_GCNew();
					if( pItemChiled == NULL ) {
						return false;
					}
					PyList_Append(pItem->m_values, (PyObject*)pItemChiled);
					if( XSECSItem_loadStream(pItemChiled, reader, dataChiled) == false ) {
						return false;
					}
				}
			}
			break;
		case XDATA_TYPE_A:
		case XDATA_TYPE_J:
			{
				XSSPY_XDECREF(pItem->m_values);
				XStrBuff str = data.getValAsStringA();
				pItem->m_values = Py_BuildValue("s", (const char*)str);
			}
			break;
		case XDATA_TYPE_UTF16:
			{
				XSSPY_XDECREF(pItem->m_values);
				XStrBuff str = data.getValAsStringUTF16();
				pItem->m_values = Py_BuildValue("s", (const char*)str);
			}
			break;
		case XDATA_TYPE_B:
		case XDATA_TYPE_BOOL:
		case XDATA_TYPE_U1:
			{
				XSSPY_XDECREF(pItem->m_values);
				pItem->m_values = (PyObject*)PyList_New(0);
				for(XD_UINT4 i = 0; i < data.getCount(); i++) {
					PyObject *pObject = Py_BuildValue("B", data.getValAsUInt1(i));
					PyList_Append(pItem->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_I1:
			{
				XSSPY_XDECREF(pItem->m_values);
				pItem->m_values = (PyObject*)PyList_New(0);
				for(XD_UINT4 i = 0; i < data.getCount(); i++) {
					PyObject *pObject = Py_BuildValue("b", data.getValAsInt1(i));
					PyList_Append(pItem->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_U2:
			{
				XSSPY_XDECREF(pItem->m_values);
				pItem->m_values = (PyObject*)PyList_New(0);
				for(XD_UINT4 i = 0; i < data.getCount(); i++) {
					PyObject *pObject = Py_BuildValue("H", data.getValAsUInt2(i));
					PyList_Append(pItem->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_I2:
			{
				XSSPY_XDECREF(pItem->m_values);
				pItem->m_values = (PyObject*)PyList_New(0);
				for(XD_UINT4 i = 0; i < data.getCount(); i++) {
					PyObject *pObject = Py_BuildValue("h", data.getValAsInt2(i));
					PyList_Append(pItem->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_U4:
			{
				XSSPY_XDECREF(pItem->m_values);
				pItem->m_values = (PyObject*)PyList_New(0);
				for(XD_UINT4 i = 0; i < data.getCount(); i++) {
					PyObject *pObject = Py_BuildValue("k", data.getValAsUInt4(i));
					PyList_Append(pItem->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_I4:
			{
				XSSPY_XDECREF(pItem->m_values);
				pItem->m_values = (PyObject*)PyList_New(0);
				for(XD_UINT4 i = 0; i < data.getCount(); i++) {
					PyObject *pObject = Py_BuildValue("l", data.getValAsInt4(i));
					PyList_Append(pItem->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_U8:
			{
				XSSPY_XDECREF(pItem->m_values);
				pItem->m_values = (PyObject*)PyList_New(0);
				for(XD_UINT4 i = 0; i < data.getCount(); i++) {
					PyObject *pObject = Py_BuildValue("K", data.getValAsUInt8(i));
					PyList_Append(pItem->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_I8:
			{
				XSSPY_XDECREF(pItem->m_values);
				pItem->m_values = (PyObject*)PyList_New(0);
				for(XD_UINT4 i = 0; i < data.getCount(); i++) {
					PyObject *pObject = Py_BuildValue("L", data.getValAsInt8(i));
					PyList_Append(pItem->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_F4:
			{
				XSSPY_XDECREF(pItem->m_values);
				pItem->m_values = (PyObject*)PyList_New(0);
				for(XD_UINT4 i = 0; i < data.getCount(); i++) {
					PyObject *pObject = Py_BuildValue("f", data.getValAsFloat4(i));
					PyList_Append(pItem->m_values, (PyObject*)pObject);
				}
			}
			break;
		case XDATA_TYPE_F8:
			{
				XSSPY_XDECREF(pItem->m_values);
				pItem->m_values = (PyObject*)PyList_New(0);
				for(XD_UINT4 i = 0; i < data.getCount(); i++) {
					PyObject *pObject = Py_BuildValue("d", data.getValAsFloat8(i));
					PyList_Append(pItem->m_values, (PyObject*)pObject);
				}
			}
			break;
		}
	} catch (XExcept &e ) {
		PyErr_SetString(g_pError, e.what());
		return false;
	} catch (...) {
		PyErr_SetString(g_pError, FUNC);
		return false;
	}

	//
	// LengthByte
	//
	if( data.isFitLengthByte() == true ) {
		pItem->m_iLengthByte = -1;
	} else {
		pItem->m_iLengthByte = data.getLengthByte();
	}

	return true;
}
