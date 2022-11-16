#ifndef __XSECS_PYTHON_SECS_DATA__
#define __XSECS_PYTHON_SECS_DATA__

#include "xsecsPyPort.h"


/**	@brief	SECSデータ
 */
typedef struct {
    PyObject_HEAD
//    PyObject				*first;
//    PyObject				*last;
	/**	@brief	データ */
	PyObject				*m_data;
	/**	@brief	ステータス */
	long					m_lStatus;
	/**	@brief	時間 */
	long					m_timeSec;
	/**	@brief	ミリ秒 */
	unsigned int			m_timeUsec;
	/**	@brief	送信元IP */
	unsigned int			m_ipSrc;
	/**	@brief	送信先IP */
	unsigned int			m_ipDst;
	/**	@brief	送信元ポート */
	unsigned short			m_portSrc;
	/**	@brief	送信先ポート */
	unsigned short			m_portDst;
	/**	@brief	ストリーム */
	unsigned int			m_stream;
	/**	@brief	ファンクション */
	unsigned int			m_function;
	/**	@brief	WAIT */
	bool					m_wait;
	/**	@brief	デバイスID */
	unsigned short			m_deviceID;
	/**	@brief	チケット */
	unsigned short			m_ticket;

} XSECSData;
PyAPI_DATA(XSECSData) XSECSData_Type;


bool XSECSData_RegistMudule(PyObject *pMOdule);

XSECSData* XSECSData_GCNew();

static PyObject *XSECSData_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

PyMODINIT_FUNC
XSECSData_dealloc(XSECSData* self);

PYTHON_DLL_EXTERN 
int XSECSData_init(XSECSData *self, PyObject *args, PyObject *kwds);

PYTHON_DLL_EXTERN PyObject *
XSECSData_getfirst(XSECSData *self, void *closure);


PYTHON_DLL_EXTERN int
XSECSData_setfirst(XSECSData *self, PyObject *value, void *closure);

bool XSECSData_CheckType(PyObject *data);
/**	@brief	時間文字列を取得
 *
 *	@param	self	Python自身のオブジェクト
 *
 *	@return	時間文字列
 */
PYTHON_DLL_EXTERN PyObject *XSECSData_timeString(XSECSData* self);

/**	@brief	送信元ホスト情報文字列を取得
 *
 *	@param	self	Python自身のオブジェクト
 *
 *	@return	送信元ホスト情報文字列
 */
PYTHON_DLL_EXTERN PyObject *XSECSData_srcHostString(XSECSData* self);

/**	@brief	送信先ホスト情報文字列を取得
 *
 *	@param	self	Python自身のオブジェクト
 *
 *	@return	送信先ホスト情報文字列
 */
PYTHON_DLL_EXTERN PyObject *XSECSData_dstHostString(XSECSData* self);



#endif /* __XSECS_PYTHON_SECS_DATA__ */
