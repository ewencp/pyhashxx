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
#define MOD_DECL(ob, name, doc, methods) \
    static struct PyModuleDef ob##_moduledef = { \
        PyModuleDef_HEAD_INIT, name, doc, -1, methods, };
#define MOD_DEF(ob) \
    ob = PyModule_Create(&ob##_moduledef);
#else
#define MOD_DECL(ob, name, doc, methods) \
    char* ob##_name = name; \
    char* ob##_doc = doc; \
    PyMethodDef* ob##_methods = methods;
#define MOD_DEF(ob) \
    ob = Py_InitModule3(ob##_name, ob##_methods, ob##_doc);
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
