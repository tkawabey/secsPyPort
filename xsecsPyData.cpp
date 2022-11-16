#include "xsecsPyPort.h"
#include "structmember.h"

PyMODINIT_FUNC
XSECSData_dealloc(XSECSData* self)
{
    XSSPY_XDECREF(self->first);
    XSSPY_XDECREF(self->last);
    self->ob_type->tp_free((PyObject*)self);
}


PYTHON_DLL_EXTERN PyObject *
XSECSData_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    XSECSData *self;

    self = (XSECSData *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->first = PyString_FromString("");
        if (self->first == NULL)
          {
            Py_DECREF(self);
            return NULL;
          }
        
        self->last = PyString_FromString("");
        if (self->last == NULL)
          {
            Py_DECREF(self);
            return NULL;
          }

        self->number = 0;
    }

    return (PyObject *)self;
}

PYTHON_DLL_EXTERN int XSECSData_init(XSECSData *self, PyObject *args, PyObject *kwds)
{
    PyObject *first=NULL, *last=NULL, *tmp;

    static char *kwlist[] = {"first", "last", "number", NULL};

    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|OOi", kwlist, 
                                      &first, &last, 
                                      &self->number)) {
        return -1; 
	}

    if (first) {
		tmp = self->first;
        Py_INCREF(first);
        self->first = first;
        XSSPY_XDECREF(tmp);
    }

    if (last) {
        tmp = self->last;
        Py_INCREF(last);
        self->last = last;
        XSSPY_XDECREF(tmp);
    }

    return 0;
}

PYTHON_DLL_EXTERN PyObject *
XSECSData_name(XSECSData* self)
{
    static PyObject *format = NULL;
    PyObject *args, *result;

    if (format == NULL) {
        format = PyString_FromString("%s %s");
        if (format == NULL)
            return NULL;
    }

    if (self->first == NULL) {
        PyErr_SetString(PyExc_AttributeError, "first");
        return NULL;
    }

    if (self->last == NULL) {
        PyErr_SetString(PyExc_AttributeError, "last");
        return NULL;
    }

    args = Py_BuildValue("OO", self->first, self->last);
    if (args == NULL)
        return NULL;

    result = PyString_Format(format, args);
    Py_DECREF(args);
    
    return result;
}




