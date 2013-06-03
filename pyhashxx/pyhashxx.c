/**
 *  pyhashxx - Fast Hash Algorithm
 *  Copyright 2013, Ewen Cheslack-Postava
 *  BSD 2-Clause License -- See LICENSE file for details.
 */

#include <Python.h>
#include "xxhash.h"

typedef struct {
    PyObject_HEAD
    void* xxhash_state;
} HashxxObject;

static void
Hashxx_dealloc(HashxxObject* self)
{
    XXH32_destroy(self->xxhash_state);
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
Hashxx_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    HashxxObject *self;

    self = (HashxxObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->xxhash_state = NULL;
    }

    return (PyObject *)self;
}

static int
Hashxx_init(HashxxObject *self, PyObject *args, PyObject *kwds)
{
    unsigned int seed = 0;
    static char *kwlist[] = {"seed", NULL};

    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|I", kwlist,
            &seed))
        return -1;

    self->xxhash_state = XXH32_init(seed);

    return 0;
}


static PyObject* _update_hash(void* hash_state, PyObject* arg_obj) {
    Py_ssize_t tuple_length;
    Py_ssize_t tuple_i;
    PyObject* tuple_obj, *partial_result;

    if (arg_obj == Py_None) Py_RETURN_NONE;

    if (PyString_Check(arg_obj)) {
        XXH32_update(hash_state, PyString_AsString(arg_obj), PyString_Size(arg_obj));
    }
    else if (PyUnicode_Check(arg_obj)) {
        XXH32_update(hash_state, PyUnicode_AS_DATA(arg_obj), PyUnicode_GET_DATA_SIZE(arg_obj));
    }
    else if (PyByteArray_Check(arg_obj)) {
        XXH32_update(hash_state, PyByteArray_AsString(arg_obj), PyByteArray_Size(arg_obj));
    }
    else if (PyTuple_Check(arg_obj)) {
        tuple_length = PyTuple_GET_SIZE(arg_obj);
        for(tuple_i = 0; tuple_i < tuple_length; tuple_i++) {
            tuple_obj = PyTuple_GetItem(arg_obj, tuple_i);
            partial_result = _update_hash(hash_state, tuple_obj);
            // Check exceptions
            if (partial_result == NULL) return NULL;
        }
    }
    else {
        PyErr_BadArgument();
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
Hashxx_update(HashxxObject* self, PyObject *args)
{
    Py_ssize_t arg_length = PyTuple_GET_SIZE(args);
    Py_ssize_t arg_i;
    PyObject* arg_obj, *partial_result;

    for(arg_i = 0; arg_i < arg_length; arg_i++) {
        arg_obj = PyTuple_GetItem(args, arg_i);
        partial_result = _update_hash(self->xxhash_state, arg_obj);
        // Check exceptions
        if (partial_result == NULL) return NULL;
    }

    Py_RETURN_NONE;
}


static PyObject *
Hashxx_digest(HashxxObject* self)
{
    unsigned int digest = XXH32_digest(self->xxhash_state);
    return Py_BuildValue("I", digest);
}

static PyMethodDef Hashxx_methods[] = {
    {"update", (PyCFunction)Hashxx_update, METH_VARARGS,
     "Update the digest with new data."
    },
    {"digest", (PyCFunction)Hashxx_digest, METH_NOARGS,
     "Return the current digest value of the data processed so far."
    },
    {NULL}  /* Sentinel */
};



static PyTypeObject pyhashxx_HashxxType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "pyhashxx.Hashxx",         /*tp_name*/
    sizeof(HashxxObject), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)Hashxx_dealloc,  /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "Hashxx objects",           /* tp_doc */
    0,               /* tp_traverse */
    0,               /* tp_clear */
    0,               /* tp_richcompare */
    0,               /* tp_weaklistoffset */
    0,               /* tp_iter */
    0,               /* tp_iternext */
    Hashxx_methods,             /* tp_methods */
    0,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Hashxx_init,      /* tp_init */
    0,                         /* tp_alloc */
    Hashxx_new,                 /* tp_new */
};

static PyMethodDef pyhashxx_methods[] = {
    {NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initpyhashxx(void)
{
    PyObject* m;

    if (PyType_Ready(&pyhashxx_HashxxType) < 0)
        return;

    m = Py_InitModule3("pyhashxx", pyhashxx_methods,
        "Python wrapper of the xxHash fast hash algorithm.");
    if (m == NULL)
        return;

    Py_INCREF(&pyhashxx_HashxxType);
    PyModule_AddObject(m, "Hashxx", (PyObject *)&pyhashxx_HashxxType);
}
