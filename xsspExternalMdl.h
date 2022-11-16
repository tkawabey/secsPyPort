#ifndef __XSSP_EXTERNAL_MODULE_H__
#define __XSSP_EXTERNAL_MODULE_H__

#include "xlib/semi/gem/xsemi_gem.h"


#define	XSECS_PY_PORT	"xsecsPyPort"

//
//	ä÷êîÇÃñºëO
//
#define PyXSEMI_INIT_EXTERNAL			_T("XSEMI_initExternalGemPort")
#define PyXSEMI_RELEASE_EXTERNAL		_T("XSEMI_releaseExternalGemPort")
#define PyXSEMI_RECEIVE_CALLBACK		_T("XSEMI_ReceiveDataCallBack")
#define PyXSEMI_USER_COMMAND			_T("XSEMI_UserCommand")
#define PyXSEMI_GET_VALUE				_T("XSEMI_GetValue")

//
//	ä÷êîÇÃå^íËã`
//
typedef bool (*PyXSEMI_initExternalGemPort)(XSEMI_Gem *,int, TCHAR**, TCHAR*);
typedef void (*PyXSEMI_releaseExternalGemPort)(XSEMI_Gem *);
typedef int  (*PyXSEMI_ReceiveDataCallBack)(XSEMI_Gem *, XSECSHeader& , XBIN* ,XD_UINT4 );
typedef int  (*PyXSEMI_UserCommand)(XSEMI_Gem *, int , TCHAR** );
typedef int  (*PyXSEMI_GetValue)(XSEMI_Gem *, const XD_UINT4, const XD_UINT4, const XDATA_TYPE, XBIN*,  const XD_UINT4 );

#endif /* __XSSP_EXTERNAL_MODULE_H__ */
