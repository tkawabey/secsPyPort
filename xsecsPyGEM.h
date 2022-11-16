#ifndef __XSECS_PYTHON_GEM__
#define __XSECS_PYTHON_GEM__


#pragma warning( disable : 4786 ) 
#include "xsecsPyPort.h"
#include "xlib/semi/gem/xsemi_gem.h"
#include <map>


/**	@brief GEM���b�v */
typedef struct {
	PyObject_HEAD
// inner member	
	/**	@brief	GEM�̃A�C�e�� */
	long		m_gem;
	/**	@brief	 */
	long		m_userWndThread;

	/**	@brief	�X�N���v�g���W���[�� */
	PyObject*	m_pModule;
	/**	@brief	�X�N���v�g�̃f�B�N�V���i���[ */
	PyObject*	m_pDictionary;
	/**	@brief	 */
	PyObject*	m_pGetEx;
	/**	@brief	 */
	PyObject*	m_pPutEx;
	/**	@brief	���[�U�[�R�}���h */
	PyObject*	m_pUserCommand;
	/**	@brief	���W���[���̌㏈�� */
	PyObject*	m_pReleaseModule;
	/**	@brief	 */
	std::map<long, PyObject*>* m_pSxFx;
} PyXSECSGem;



/**	@brief	PyXSECSGem��Python�I�u�W�F�N�g�Ƃ��āA�g�p�ł���悤�ɓo�^���܂��B
 *
 *	@param	pMOdule	Py_InitModule�̃C���X�^���X
 *
 *	@retval	true	����
 *	@retval	false	���s
 */
bool PyXSECSGem_RegistModule(PyObject *pMOdule);

/**	@brief	�V�K�I�u�W�F�N�g���쐬����B
 *
 *	@param	pGem	XSEMI_Gem*�̃C���X�^���X
 *
 *	@return	�V�K�I�u�W�F�N�g
 */
PyXSECSGem* PyXSECSGem_GCNew(long pGem);

/**	@brief	�w�肵���I�u�W�F�N�g���APyXSECSGem�I�u�W�F�N�g�ł��邩�m�F�B
 *
 *	@param	data	��������I�u�W�F�N�g
 *
 *	@retval	true	PyXSECSGem�I�u�W�F�N�g�ł���B
 *	@retval	false	PyXSECSGem�I�u�W�F�N�g�łȂ��B
 */
bool PyXSECSGem_CheckType(PyObject *data);

#endif /* __XSECS_PYTHON_GEM__ */