#ifndef __XSECS_PYTHON_GEM_API__
#define __XSECS_PYTHON_GEM_API__

#pragma warning( disable : 4786 ) 
#include "xsecsPyPort.h"
#include "xlib/semi/gem/xsemi_gem.h"
#include "xlib/semi/gem/xsemi_gem_api.h"
#include <map>


/**	@brief XSEMI_GemAPI���b�v */
typedef struct {
	PyObject_HEAD
// inner member	
	PyObject	*m_pReciveEventCallBack;
	/**	@brief	GEM_API�̃A�C�e�� */
	long		m_gemAPI;
} PyXSECSGemAPI;




/**	@brief	PyXSECSGemAPI��Python�I�u�W�F�N�g�Ƃ��āA�g�p�ł���悤�ɓo�^���܂��B
 *
 *	@param	pMOdule	Py_InitModule�̃C���X�^���X
 *
 *	@retval	true	����
 *	@retval	false	���s
 */
bool PyXSECSGemAPI_RegistModule(PyObject *pMOdule);

/**	@brief	�V�K�I�u�W�F�N�g���쐬����B
 *
 *	@param	pGemAPI	PyXSECSGemAPI*�̃C���X�^���X
 *
 *	@return	�V�K�I�u�W�F�N�g
 */
PyXSECSGemAPI* PyXSECSGemAPI_GCNew(long pGemAPI);

/**	@brief	�w�肵���I�u�W�F�N�g���APyXSECSGem�I�u�W�F�N�g�ł��邩�m�F�B
 *
 *	@param	data	��������I�u�W�F�N�g
 *
 *	@retval	true	PyXSECSGemAPI�I�u�W�F�N�g�ł���B
 *	@retval	false	PyXSECSGemAPI�I�u�W�F�N�g�łȂ��B
 */
bool PyXSECSGemAPI_CheckType(PyObject *data);

#endif /*__XSECS_PYTHON_GEM_API__*/

