/**
 *  pyhashxx - Fast Hash Algorithm
 *  Copyright 2013, Ewen Cheslack-Postava
 *  BSD 2-Clause License -- See LICENSE file for details.
 */

#include <Python.h>
#include "pycompat.h"
#include "xxhash.h"

typedef struct {
    PyObject_HEAD
    void* xxhash_state;
} HashxxObject;

static void
Hashxx_dealloc(HashxxObject* self)
{
    XXH32_destroy(self->xxhash_state);
    Py_TYPE(self)->tp_free((PyObject*)self);
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

#if PY_MAJOR_VERSION >= 3
    if (PyBytes_Check(arg_obj)) {
        XXH32_update(hash_state, PyBytes_AsString(arg_obj), PyBytes_Size(arg_obj));
    }
#else
    if (PyString_Check(arg_obj)) {
        XXH32_update(hash_state, PyString_AsString(arg_obj), PyString_Size(arg_obj));
    }
#endif
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
    else if (PyUnicode_Check(arg_obj)) {
        PyErr_BadArgument();
        return NULL;
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

    if (arg_length == 0) {
        PyErr_BadArgument();
        return NULL;
    }

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
    PyVarObject_HEAD_INIT(NULL, 0)
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



static PyObject *
pyhashxx_hashxx(PyObject* self, PyObject *args, PyObject *kwds)
{
    unsigned int seed = 0;

    if (kwds != NULL) {
        Py_ssize_t kwds_size = PyDict_Size(kwds);
        PyObject* seed_obj = PyDict_GetItemString(kwds, "seed");

        if (kwds_size > 1)
            goto badarg;

        if (kwds_size == 1) {
            if (seed_obj == NULL)
                goto badarg;
#if PY_MAJOR_VERSION < 3
            if (PyInt_Check(seed_obj))
                seed = PyInt_AsLong(seed_obj);
            else
#endif
                if (PyLong_Check(seed_obj))
                seed = PyLong_AsLong(seed_obj);
            else
                goto badarg;
        }
    }
    if (PyTuple_GET_SIZE(args) == 0)
        goto badarg;

    void* state = XXH32_init(seed);
    if (_update_hash(state, args) == NULL) {
        XXH32_destroy(state);
        return NULL;
    }

    unsigned int digest = XXH32_digest(state);
    XXH32_destroy(state);

    return Py_BuildValue("I", digest);

badarg:
    PyErr_BadArgument();
    return NULL;
}

static PyMethodDef pyhashxx_methods[] = {
    {"hashxx", (PyCFunction)pyhashxx_hashxx, METH_VARARGS | METH_KEYWORDS,
     "Compute the xxHash value for the given value, optionally providing a seed."
    },
    {NULL}  /* Sentinel */
};



MOD_INIT(pyhashxx) {
    PyObject* m;

    if (PyType_Ready(&pyhashxx_HashxxType) < 0)
        RETURN_MOD_INIT_ERROR;

    MOD_DEF(m, "pyhashxx",
        "Python wrapper of the xxHash fast hash algorithm.",
        pyhashxx_methods);
    if (m == NULL)
        RETURN_MOD_INIT_ERROR;

    Py_INCREF(&pyhashxx_HashxxType);
    PyModule_AddObject(m, "Hashxx", (PyObject *)&pyhashxx_HashxxType);

    RETURN_MOD_INIT_SUCCESS(m);
}
