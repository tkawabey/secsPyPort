#ifndef __XSECS_PYTHON_GEM__
#define __XSECS_PYTHON_GEM__


#pragma warning( disable : 4786 ) 
#include "xsecsPyPort.h"
#include "xlib/semi/gem/xsemi_gem.h"
#include <map>


/**	@brief GEMラップ */
typedef struct {
	PyObject_HEAD
// inner member	
	/**	@brief	GEMのアイテム */
	long		m_gem;
	/**	@brief	 */
	long		m_userWndThread;

	/**	@brief	スクリプトモジュール */
	PyObject*	m_pModule;
	/**	@brief	スクリプトのディクショナリー */
	PyObject*	m_pDictionary;
	/**	@brief	 */
	PyObject*	m_pGetEx;
	/**	@brief	 */
	PyObject*	m_pPutEx;
	/**	@brief	ユーザーコマンド */
	PyObject*	m_pUserCommand;
	/**	@brief	モジュールの後処理 */
	PyObject*	m_pReleaseModule;
	/**	@brief	 */
	std::map<long, PyObject*>* m_pSxFx;
} PyXSECSGem;



/**	@brief	PyXSECSGemをPythonオブジェクトとして、使用できるように登録します。
 *
 *	@param	pMOdule	Py_InitModuleのインスタンス
 *
 *	@retval	true	成功
 *	@retval	false	失敗
 */
bool PyXSECSGem_RegistModule(PyObject *pMOdule);

/**	@brief	新規オブジェクトを作成する。
 *
 *	@param	pGem	XSEMI_Gem*のインスタンス
 *
 *	@return	新規オブジェクト
 */
PyXSECSGem* PyXSECSGem_GCNew(long pGem);

/**	@brief	指定したオブジェクトが、PyXSECSGemオブジェクトであるか確認。
 *
 *	@param	data	検査するオブジェクト
 *
 *	@retval	true	PyXSECSGemオブジェクトである。
 *	@retval	false	PyXSECSGemオブジェクトでない。
 */
bool PyXSECSGem_CheckType(PyObject *data);

#endif /* __XSECS_PYTHON_GEM__ */