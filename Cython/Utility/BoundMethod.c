/////////////// BoundMethod.proto ///////////////

#if CYTHON_COMPILING_IN_LIMITED_API
#define __Pyx_BoundMethod_USED
static int __pyx_BoundMethod_init(void); /*proto*/
#endif

/////////////// BoundMethod ///////////////
//@requires: CommonStructures.c::FetchCommonType
//@requires: CommonStructures.c::PyMemberDef

#if CYTHON_COMPILING_IN_LIMITED_API
typedef struct {
    PyObject_HEAD
    PyObject *function;
    PyObject *self;
    PyObject *weakreflist;
} __pyx_BoundMethodObject;

static __pyx_BoundMethodObject *__pyx_cymethod_free_list;
static int __pyx_cymethod_numfree = 0;
#define CYTHON_CYMETHOD_MAXFREELIST 64

static PyObject *
__Pyx_BoundMethod_New(PyObject *function, PyObject *self)
{
    __pyx_BoundMethodObject *result = __pyx_cymethod_free_list;
    if (result != NULL) {
        __pyx_cymethod_free_list = (__pyx_BoundMethodObject *)(result->self);
        (void)PyObject_Init((PyObject *)result,
                            (PyTypeObject *)__pyx_BoundMethodType);
        __pyx_cymethod_numfree--;
    } else {
        result = PyObject_GC_New(__pyx_BoundMethodObject,
                                 (PyTypeObject *)__pyx_BoundMethodType);
        if (result == NULL)
            return NULL;
    }
    Py_INCREF(function);
    result->function = function;
    Py_INCREF(self);
    result->self = self;
    result->weakreflist = NULL;
    PyObject_GC_Track(result);
    return (PyObject *)result;
}

static void
__Pyx_BoundMethod_dealloc(__pyx_BoundMethodObject *self)
{
    PyObject_GC_UnTrack(self);
    if (self->weakreflist != NULL) {
        PyObject_ClearWeakRefs((PyObject *)self);
    }
    Py_DECREF(self->function);
    Py_DECREF(self->self);
    if (__pyx_cymethod_numfree < CYTHON_CYMETHOD_MAXFREELIST) {
        self->self = (PyObject *)__pyx_cymethod_free_list;
        __pyx_cymethod_free_list = self;
        __pyx_cymethod_numfree++;
    } else {
        PyObject_GC_Del(self);
    }
}

static int
__Pyx_BoundMethod_traverse(__pyx_BoundMethodObject *self, visitproc visit,
                           void *arg)
{
    Py_VISIT(self->function);
    Py_VISIT(self->self);
    return 0;
}

static PyObject *
__Pyx_BoundMethod_repr(__pyx_BoundMethodObject *self)
{
    PyObject *function = self->function;
    PyObject *name;
    PyObject *result;

    name = __Pyx_PyObject_GetAttrStr(function, PYIDENT("__qualname__"));
    if (name == NULL) {
        if (!PyErr_ExceptionMatches(PyExc_AttributeError)) {
            return NULL;
        }
        name = __Pyx_PyObject_GetAttrStr(function, PYIDENT("__name__"));
        if (name == NULL && !PyErr_ExceptionMatches(PyExc_AttributeError)) {
            return NULL;
        }
    }
    if (name != NULL && !PyUnicode_Check(name)) {
        Py_DECREF(name);
        name = NULL;
    }

    result =
        PyUnicode_FromFormat("<bound method %V of %R>", name, "?", self->self);
    Py_XDECREF(name);
    return result;
}

static PyObject *
__pyx_BoundMethod_get_doc(__pyx_BoundMethodObject *self, void *context)
{
    return __Pyx_PyObject_GetAttrStr(self->function, PYIDENT("__doc__"));
}

static PyObject *
__Pyx_BoundMethod_call(__pyx_BoundMethodObject *self, PyObject *args,
                       PyObject *kwargs)
{
    PyObject *result;
    Py_ssize_t argcount = PyTuple_Size(args);
    PyObject *self_with_args = PyTuple_New(argcount + 1);
    PyObject *self_self = self->self;
    Py_INCREF(self_self);
    PyTuple_SetItem(self_with_args, 0, self_self);
    for (Py_ssize_t i = 0; i < argcount; i++) {
        PyObject *arg = PyTuple_GetItem(args, i);
        Py_INCREF(arg);
        PyTuple_SetItem(self_with_args, i + 1, arg);
    }
    result = PyObject_Call(self->function, self_with_args, kwargs);
    Py_DECREF(self_with_args);
    return result;
}

static PyObject *
__Pyx_BoundMethod_descr_get(PyObject *self, CYTHON_UNUSED PyObject *object,
                            CYTHON_UNUSED PyObject *type)
{
    Py_INCREF(self);
    return self;
}

static Py_hash_t
__Pyx_BoundMethod_hash(__pyx_BoundMethodObject *self)
{
    Py_hash_t function_hash;
    Py_hash_t result;
    Py_hash_t self_hash = PyObject_Hash(self->self);
    if (self_hash == -1) {
        return -1;
    }
    function_hash = PyObject_Hash(self->function);
    if (function_hash == -1) {
        return -1;
    }
    result = self_hash ^ function_hash;
    if (result == -1) {
        result = -2;
    }
    return result;
}

// TODO(T67592555) Add tp_getattro, tp_richcompare, __reduce__ support.

static PyMemberDef __pyx_BoundMethod_members[] = {
    {(char *)"__func__", T_OBJECT, offsetof(__pyx_BoundMethodObject, function),
     READONLY,
     (char *)PyDoc_STR(
         "the function (or other callable) implementing a method")},
    {(char *)"__self__", T_OBJECT, offsetof(__pyx_BoundMethodObject, self),
     READONLY, (char *)PyDoc_STR("the instance to which a method is bound")},
    {(char *)"__weaklistoffset__", T_LONG,
     offsetof(__pyx_BoundMethodObject, weakreflist), 0},
    {0, 0, 0, 0, 0},
};

static PyGetSetDef __pyx_BoundMethod_getset[] = {
    {(char *)"__doc__", (getter)__pyx_BoundMethod_get_doc, 0, 0, 0},
    {0, 0, 0, 0, 0},
};

static PyType_Slot __pyx_BoundMethodType_slots[] = {
    {Py_tp_call, (void *)__Pyx_BoundMethod_call},
    {Py_tp_clear, (void *)__Pyx_BoundMethod_traverse},
    {Py_tp_dealloc, (void *)__Pyx_BoundMethod_dealloc},
    {Py_tp_descr_get, (void *)__Pyx_BoundMethod_descr_get},
    {Py_tp_getset, (void *)__pyx_BoundMethod_getset},
    {Py_tp_hash, (void *)__Pyx_BoundMethod_hash},
    {Py_tp_members, (void *)__pyx_BoundMethod_members},
    {Py_tp_repr, (void *)__Pyx_BoundMethod_repr},
    {Py_tp_traverse, (void *)__Pyx_BoundMethod_traverse},
    {0, 0},
};

static PyType_Spec __pyx_BoundMethodType_spec = {
    "bound_method",
    sizeof(__pyx_BoundMethodObject),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
    __pyx_BoundMethodType_slots,
};

static int
__pyx_BoundMethod_init(void)
{
    __pyx_BoundMethodType =
        __Pyx_FetchCommonTypeFromSpec(&__pyx_BoundMethodType_spec, NULL);
    if (unlikely(__pyx_BoundMethodType == NULL)) {
        return -1;
    }
    return 0;
}
#endif

/////////////// PyMethodNew.proto ///////////////

#if PY_MAJOR_VERSION >= 3
static PyObject *__Pyx_PyMethod_New(PyObject *func, PyObject *self,
                                    PyObject *typ);
#else
#define __Pyx_PyMethod_New PyMethod_New
#endif

/////////////// PyMethodNew ///////////////
//@requires: BoundMethod

#if PY_MAJOR_VERSION >= 3
static PyObject *
__Pyx_PyMethod_New(PyObject *func, PyObject *self, CYTHON_UNUSED PyObject *type)
{
    if (self == NULL) {
        Py_INCREF(func);
        return func;
    }
#if CYTHON_COMPILING_IN_LIMITED_API
    return __Pyx_BoundMethod_New(func, self);
#else
    return PyMethod_New(func, self);
#endif
}
#endif
