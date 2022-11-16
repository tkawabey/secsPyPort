#ifndef __XSECS_PYTHON_PORT__
#define __XSECS_PYTHON_PORT__



#include <Python.h>
#ifdef WIN32
	#include <winsock2.h>
#endif
#include "xlib/semi/gem/xsemi_gem.h"



#define XSECE_PY_MODULE_NAME	"xsecsPyPort"



#define PYTHON_DLL_EXTERN __declspec(dllexport)

#ifdef _DEBUG
#define XSSPY_XDECREF(x)	if(x != NULL ) {XTRACE(_T("Python object decrement refcnt(%d) %s(%d)\n"), x->ob_refcnt, (const TCHAR*)XStrBuff(__FILE__), __LINE__);}	Py_XDECREF(x);
#else
#define XSSPY_XDECREF(x)	Py_XDECREF(x);
#endif

template <class T = PyObject>
class PyObjectHelper {
public:
	T* m_pT;
	PyObjectHelper(){m_pT = NULL;}
	PyObjectHelper(T* p){ m_pT = p;}
	~PyObjectHelper(){   XSSPY_XDECREF(m_pT); }
	int increment_reference() {
		if( m_pT == NULL ) {return -1;}
		Py_INCREF(m_pT);
		return m_pT->ob_refcnt;
	}
	void ZeroClear(){
		if( m_pT == NULL ) {return;}
		while( m_pT->ob_refcnt != 0 ) {
			XSSPY_XDECREF(m_pT);
		}
		m_pT = NULL;
	}
	operator T*(){ return m_pT;}
	T& operator*() const{return *m_pT;}
	T** operator&(){return &m_pT;}
	T* operator->() const{return m_pT;}
	bool operator!() const{	return (m_pT == NULL); }
	bool operator<(T* pT) const{ return m_pT < pT; }
	bool operator==(T* pT) const { return m_pT == pT; }
	bool operator!=(T* pT) const { return m_pT != pT; }
	T* operator=(const PyObjectHelper<T>& lp){m_pT = lp.m_pT;return m_pT;}
	const PyObjectHelper<T>& operator=(T* p){m_pT = p; return *this;}

	const PyObjectHelper<T>& operator=(const char* pValue){m_pT = PyString_FromString(pValue); return *this;}
};


void xsecsPyPortGemErrorOut(XSEMI_Gem *pGem);

#endif /* __XSECS_PYTHON_PORT__ */


