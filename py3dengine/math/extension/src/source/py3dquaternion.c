#include "py3dquaternion.h"

#include <structmember.h>

#include "py3dvector3.h"
#include "quaternion.h"

#define MAX_STRING_SIZE 64

static int Py3dQuaternion_Init(struct Py3dQuaternion *self, PyObject *args, PyObject *kwds);
static void Py3dQuaternion_Dealloc(struct Py3dQuaternion *self);

static PyGetSetDef Py3dQuaternion_GettersSetters[] = {
    {"x", (getter) Py3dQuaternion_GetX, (setter) NULL, "X Component of Quaternion", NULL},
    {"y", (getter) Py3dQuaternion_GetY, (setter) NULL, "Y Component of Quaternion", NULL},
    {"z", (getter) Py3dQuaternion_GetZ, (setter) NULL, "Z Component of Quaternion", NULL},
    {"w", (getter) Py3dQuaternion_GetW, (setter) NULL, "W Component of Quaternion", NULL},
    {NULL}
};

static PyMethodDef Py3dQuaternion_Methods[] = {
    {"FromAxisAndDegrees", (PyCFunction) Py3dQuaternion_FromAxisAndDegrees, METH_VARARGS | METH_STATIC, "Create a Quaternion from an axis and a angle in degrees"},
    {"normalize", (PyCFunction) Py3dQuaternion_Normalize, METH_NOARGS, "Return a normalized version of the quaterion"},
    {NULL}
};

static PyNumberMethods Py3dQuaternion_NumberMethods = {
    .nb_multiply = (binaryfunc) Py3dQuaternion_Mult,
};

PyTypeObject Py3dQuaternion_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "py3dmath.Quaternion",
    .tp_doc = "A quaternion",
    .tp_basicsize = sizeof(struct Py3dQuaternion),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_init = (initproc) Py3dQuaternion_Init,
    .tp_methods = Py3dQuaternion_Methods,
    .tp_dealloc = (destructor) Py3dQuaternion_Dealloc,
    .tp_new = PyType_GenericNew,
    .tp_repr = (reprfunc) Py3dQuaternion_Repr,
    .tp_str = (reprfunc) Py3dQuaternion_Repr,
    .tp_getset = Py3dQuaternion_GettersSetters
};

int PyInit_Py3dQuaternion(PyObject *module) {
    Py3dQuaternion_Type.tp_as_number = &Py3dQuaternion_NumberMethods;
    if (PyType_Ready(&Py3dQuaternion_Type) < 0) return 0;

    if (PyModule_AddObjectRef(module, "Quaternion", (PyObject *) &Py3dQuaternion_Type) < 0) return 0;

    return 1;
}

static PyObject *float_cast(PyObject *obj) {
    if (obj == NULL) return NULL;

    obj = PyNumber_Float(obj);
    return obj;
}

static int init_from_list(struct Py3dQuaternion *self, PyObject *list) {
    if (PyObject_Length(list) < 4) {
        PyErr_SetString(PyExc_ValueError, "Init Quaternion from list requires 4 list items");
        return -1;
    }

    float data[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    for (Py_ssize_t i = 0; i < 4; ++i) {
        PyObject *li = float_cast(PyList_GetItem(list, i));
        if (li == NULL) return -1;

        data[i] = (float) PyFloat_AsDouble(li);
        //float_cast returns new ref that must be decref'ed
        //PyList_GetItem does not
        Py_CLEAR(li);
    }

    QuaternionCopy(self->elements, data);

    return 0;
}

static int init_from_dict(struct Py3dQuaternion *self, PyObject *dict) {
    char key_names[8] = {'x', 0, 'y', 0, 'z', 0, 'w', 0};
    float data[4] = {0.0f, 0.0f, 0.0f, 1.0f};

    for (Py_ssize_t i = 0; i < 4; ++i) {
        PyObject *li = PyDict_GetItemString(dict, &(key_names[i*2]));
        if (li == NULL) {
            PyErr_SetString(PyExc_KeyError, "Init Quaternion from dictionary requires keys with names \"x, y, z, w\"");
        }

        li = float_cast(li);
        if (li == NULL) return -1;

        data[i] = (float) PyFloat_AsDouble(li);
        //float_cast returns new ref that must be decref'ed
        //PyDict_GetItemString does not
        Py_CLEAR(li);
    }

    QuaternionCopy(self->elements, data);

    return 0;
}

static int Py3dQuaternion_Init(struct Py3dQuaternion *self, PyObject *args, PyObject *Py_UNUSED(kwds)) {
    if (PyTuple_Size(args) == 0) {
        QuaternionIdentity(self->elements);

        return 0;
    }

    PyObject *singleObjArg = NULL;
    if (PyArg_ParseTuple(args, "O", &singleObjArg) == 1) {
        if (PyList_Check(singleObjArg)) {
            return init_from_list(self, singleObjArg);
        } else if (PyDict_Check(singleObjArg)) {
            return init_from_dict(self, singleObjArg);
        } else if (Py3dQuaternion_Check(singleObjArg)) {
            struct Py3dQuaternion *other = (struct Py3dQuaternion *) singleObjArg;
            QuaternionCopy(self->elements, other->elements);
            return 0;
        } else {
            PyErr_SetString(PyExc_TypeError, "Single object argument must be of type list, dictionary or Quaternion");
            return -1;
        }
    }
    PyErr_Clear();

    if (
        PyArg_ParseTuple(
            args, "ffff", &(self->elements[0]), &(self->elements[1]), &(self->elements[2]), &(self->elements[3])
        ) != 1
    ) return -1;

    return 0;
}

static void Py3dQuaternion_Dealloc(struct Py3dQuaternion *self) {
    Py_TYPE(self)->tp_free((PyObject *) self);
}

struct Py3dQuaternion *Py3dQuaternion_New(float x, float y, float z, float w) {
    PyObject *mathModule = PyImport_ImportModule("py3dengine.math");
    if (mathModule == NULL) return NULL;

    PyObject *quatctor = PyObject_GetAttrString(mathModule, "Quaternion");
    Py_CLEAR(mathModule);
    if (quatctor == NULL) return NULL;

    if (!PyCallable_Check(quatctor)) {
        PyErr_SetString(PyExc_TypeError, "Quaternion ctor must be callable");
        Py_CLEAR(quatctor);
        return NULL;
    }

    PyObject *args = Py_BuildValue("(ffff)", x, y, z, w);
    PyObject *ret = PyObject_Call(quatctor, args, NULL);
    Py_CLEAR(args);
    Py_CLEAR(quatctor);

    if (ret == NULL) return NULL;

    if (!Py3dQuaternion_Check(ret)) {
        // WTF???
        Py_CLEAR(ret);
        PyErr_SetString(PyExc_TypeError, "Quaternion ctor did not return a Quaternion");
        return NULL;
    }

    return (struct Py3dQuaternion *) ret;
}

int Py3dQuaternion_Check(PyObject *obj) {
    if (obj == NULL) return 0;

    int ret = PyObject_IsInstance(obj, (PyObject *) &Py3dQuaternion_Type);
    if (ret == -1) {
        PyErr_Clear();
        return 0;
    }

    return ret;
}

PyObject *Py3dQuaternion_GetX(struct Py3dQuaternion *self, void *Py_UNUSED(closure)) {
    return PyFloat_FromDouble(self->elements[0]);
}

PyObject *Py3dQuaternion_GetY(struct Py3dQuaternion *self, void *Py_UNUSED(closure)) {
    return PyFloat_FromDouble(self->elements[1]);
}

PyObject *Py3dQuaternion_GetZ(struct Py3dQuaternion *self, void *Py_UNUSED(closure)) {
    return PyFloat_FromDouble(self->elements[2]);
}

PyObject *Py3dQuaternion_GetW(struct Py3dQuaternion *self, void *Py_UNUSED(closure)) {
    return PyFloat_FromDouble(self->elements[3]);
}

PyObject *Py3dQuaternion_Repr(struct Py3dQuaternion *self) {
    char buffer[MAX_STRING_SIZE];
    QuaternionToCStr(buffer, self->elements, MAX_STRING_SIZE);

    return PyUnicode_FromString(buffer);
}

PyObject *Py3dQuaternion_Mult(struct Py3dQuaternion *self, PyObject *other) {
    if (!Py3dQuaternion_Check(other)) {
        PyErr_SetString(PyExc_TypeError, "Operand must be of type Quaternion");
        return NULL;
    }
    struct Py3dQuaternion *otherQuaternion = (struct Py3dQuaternion *) other;

    struct Py3dQuaternion *result = Py3dQuaternion_New(0.0f,0.0f,0.0f,1.0f);
    if (result == NULL) return NULL;

    QuaternionMult(result->elements, self->elements, otherQuaternion->elements);

    return (PyObject *) result;
}

PyObject *Py3dQuaternion_Normalize(struct Py3dQuaternion *self, PyObject *Py_UNUSED(args)) {
    struct Py3dQuaternion *result = Py3dQuaternion_New(0.0f,0.0f,0.0f,1.0f);
    if (result == NULL) return NULL;

    QuaternionNormalize(result->elements, self->elements);

    return (PyObject *) result;
}

PyObject *Py3dQuaternion_FromAxisAndDegrees(struct Py3dQuaternion *Py_UNUSED(self), PyObject *args, PyObject *Py_UNUSED(kwds)) {
    struct Py3dVector3 *axis = NULL;
    float angle = 0.0f;
    if (PyArg_ParseTuple(args, "O!f", &Py3dVector3_Type, &axis, &angle) != 1) return NULL;

    struct Py3dQuaternion *result = Py3dQuaternion_New(0.0f, 0.0f, 0.0f, 1.0f);
    if (result == NULL) return NULL;

    QuaternionFromAxisAngle(
        result->elements,
        axis->elements[0],
        axis->elements[1],
        axis->elements[2],
        angle
    );

    return (PyObject *) result;
}
