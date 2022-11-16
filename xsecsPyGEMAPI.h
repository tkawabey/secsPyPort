#ifndef __XSECS_PYTHON_GEM_API__
#define __XSECS_PYTHON_GEM_API__

#pragma warning( disable : 4786 ) 
#include "xsecsPyPort.h"
#include "xlib/semi/gem/xsemi_gem.h"
#include "xlib/semi/gem/xsemi_gem_api.h"
#include <map>


/**	@brief XSEMI_GemAPIラップ */
typedef struct {
	PyObject_HEAD
// inner member	
	PyObject	*m_pReciveEventCallBack;
	/**	@brief	GEM_APIのアイテム */
	long		m_gemAPI;
} PyXSECSGemAPI;




/**	@brief	PyXSECSGemAPIをPythonオブジェクトとして、使用できるように登録します。
 *
 *	@param	pMOdule	Py_InitModuleのインスタンス
 *
 *	@retval	true	成功
 *	@retval	false	失敗
 */
bool PyXSECSGemAPI_RegistModule(PyObject *pMOdule);

/**	@brief	新規オブジェクトを作成する。
 *
 *	@param	pGemAPI	PyXSECSGemAPI*のインスタンス
 *
 *	@return	新規オブジェクト
 */
PyXSECSGemAPI* PyXSECSGemAPI_GCNew(long pGemAPI);

/**	@brief	指定したオブジェクトが、PyXSECSGemオブジェクトであるか確認。
 *
 *	@param	data	検査するオブジェクト
 *
 *	@retval	true	PyXSECSGemAPIオブジェクトである。
 *	@retval	false	PyXSECSGemAPIオブジェクトでない。
 */
bool PyXSECSGemAPI_CheckType(PyObject *data);

#endif /*__XSECS_PYTHON_GEM_API__*/

