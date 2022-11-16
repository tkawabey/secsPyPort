#ifndef __XSECS_PYTHON_SECS_ITEM__
#define __XSECS_PYTHON_SECS_ITEM__

#include "xsecsPyPort.h"
#include "xlib/xdata.h"
#include "xlib/xbuff.h"

/**	@brief SECSデータアイテム */
typedef struct {
	PyObject_HEAD

	/**	@brief	データタイプ */
	unsigned int		m_type;
	/**	@brief	レングズバイト */
	XD_INT1				m_iLengthByte;
	/**	@brief	データ */
	PyObject			*m_values;
	/**	@brief	親 */
	PyObject			*m_pParent;
} XSECSItem;

XSECSItem* XSECSItem_GCNew();


bool           XSECSItem_RegistMudule(PyObject *pMOdule);

PYTHON_DLL_EXTERN 
PyObject*      XSECSItem_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

PyMODINIT_FUNC XSECSItem_dealloc(XSECSItem* self);

PYTHON_DLL_EXTERN 
int            XSECSItem_init(XSECSItem *self, PyObject *args, PyObject *kwds);

bool           XSECSItem_CheckType(PyObject *data);

PYTHON_DLL_EXTERN 
PyObject*      XSECSData_getTypeString(XSECSItem* self);

PYTHON_DLL_EXTERN 
PyObject*      XSECSData_getValueString(XSECSItem* self);

PYTHON_DLL_EXTERN 
PyObject*      XSECSData_setValueFromString(XSECSItem* self, PyObject *args);

PYTHON_DLL_EXTERN 
PyObject *XSECSItem_clone(XSECSItem* self, PyObject *args);

bool XSECSItem_storeStream(XDataWriter &writer,  XSECSItem* pItem, bool bStream = false);

bool XSECSItem_loadStream(XSECSItem* pItem, XDataReader &reader, XData &data);

void XSECSItem_ZeroInit(XSECSItem* pItem);


#endif /* __XSECS_PYTHON_SECS_ITEM__ */
