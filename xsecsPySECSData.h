#ifndef __XSECS_PYTHON_SECS_DATA__
#define __XSECS_PYTHON_SECS_DATA__

#include "xsecsPyPort.h"


/**	@brief	SECS�f�[�^
 */
typedef struct {
    PyObject_HEAD
//    PyObject				*first;
//    PyObject				*last;
	/**	@brief	�f�[�^ */
	PyObject				*m_data;
	/**	@brief	�X�e�[�^�X */
	long					m_lStatus;
	/**	@brief	���� */
	long					m_timeSec;
	/**	@brief	�~���b */
	unsigned int			m_timeUsec;
	/**	@brief	���M��IP */
	unsigned int			m_ipSrc;
	/**	@brief	���M��IP */
	unsigned int			m_ipDst;
	/**	@brief	���M���|�[�g */
	unsigned short			m_portSrc;
	/**	@brief	���M��|�[�g */
	unsigned short			m_portDst;
	/**	@brief	�X�g���[�� */
	unsigned int			m_stream;
	/**	@brief	�t�@���N�V���� */
	unsigned int			m_function;
	/**	@brief	WAIT */
	bool					m_wait;
	/**	@brief	�f�o�C�XID */
	unsigned short			m_deviceID;
	/**	@brief	�`�P�b�g */
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
/**	@brief	���ԕ�������擾
 *
 *	@param	self	Python���g�̃I�u�W�F�N�g
 *
 *	@return	���ԕ�����
 */
PYTHON_DLL_EXTERN PyObject *XSECSData_timeString(XSECSData* self);

/**	@brief	���M���z�X�g��񕶎�����擾
 *
 *	@param	self	Python���g�̃I�u�W�F�N�g
 *
 *	@return	���M���z�X�g��񕶎���
 */
PYTHON_DLL_EXTERN PyObject *XSECSData_srcHostString(XSECSData* self);

/**	@brief	���M��z�X�g��񕶎�����擾
 *
 *	@param	self	Python���g�̃I�u�W�F�N�g
 *
 *	@return	���M��z�X�g��񕶎���
 */
PYTHON_DLL_EXTERN PyObject *XSECSData_dstHostString(XSECSData* self);



#endif /* __XSECS_PYTHON_SECS_DATA__ */
