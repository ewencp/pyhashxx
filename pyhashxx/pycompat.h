// Python 2/3 Compatibility Helpers
// Assumes you've already included Python.h
//
// See http://python3porting.com/cextensions.html for the source of most of
// these.

#ifndef PyVarObject_HEAD_INIT
#define PyVarObject_HEAD_INIT(type, size) \
    PyObject_HEAD_INIT(type) size,
#endif

#ifndef Py_TYPE
#define Py_TYPE(ob) (((PyObject*)(ob))->ob_type)
#endif


#if PY_MAJOR_VERSION >= 3
#define MOD_DEF(ob, name, doc, methods) \
    static struct PyModuleDef moduledef = { \
        PyModuleDef_HEAD_INIT, name, doc, -1, methods, }; \
    ob = PyModule_Create(&moduledef);
#else
#define MOD_DEF(ob, name, doc, methods) \
    ob = Py_InitModule3(name, methods, doc);
#endif

#if PY_MAJOR_VERSION >= 3
#define MOD_INIT(name) PyMODINIT_FUNC PyInit_##name(void)
#define RETURN_MOD_INIT_ERROR return ((PyObject*)NULL)
#define RETURN_MOD_INIT_SUCCESS(modvar) return modvar
#else
#define MOD_INIT(name) PyMODINIT_FUNC init##name(void)
#define RETURN_MOD_INIT_ERROR return
#define RETURN_MOD_INIT_SUCCESS(modvar) return
#endif
