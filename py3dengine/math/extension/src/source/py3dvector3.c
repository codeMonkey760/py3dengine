#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

#include <vector.h>
#include <quaternion.h>
#include <matrix.h>

#include "py3dvector3.h"
#include "py3dquaternion.h"
#include "py3dmatrix4x4.h"

#define MAX_STRING_SIZE 64

static void Py3dVector3_Dealloc(struct Py3dVector3 *self);
static int Py3dVector3_Init(struct Py3dVector3 *self, PyObject *args, PyObject *kwds);

PyGetSetDef Py3dVector3_GettersSetters[] = {
    {"x", (getter) Py3dVector3_GetX, (setter) NULL, "X Component of Vector3", NULL},
    {"y", (getter) Py3dVector3_GetY, (setter) NULL, "Y Component of Vector3", NULL},
    {"z", (getter) Py3dVector3_GetZ, (setter) NULL, "Z Component of Vector3", NULL},
    {NULL}
};

PyMethodDef Py3dVector3_Methods[] = {
    {"dot", (PyCFunction) Py3dVector3_Dot, METH_VARARGS, "Return the dot product of two Vector3 instances"},
    {"length", (PyCFunction) Py3dVector3_Length, METH_NOARGS, "Return the length of a Vector3 instance"},
    {"normalize", (PyCFunction) Py3dVector3_Normalize, METH_NOARGS, "Return the normalized version of a Vector3 instance"},
    {"copy", (PyCFunction) Py3dVector3_Copy, METH_NOARGS, "Create a copy of the provided Vector3 instance"},
    {"Fill", (PyCFunction) Py3dVector3_Fill, METH_VARARGS | METH_STATIC, "Create a Vector3 filled with the provided number"},
    {NULL}
};

PyNumberMethods Py3dVector3_NumberMethods = {
    .nb_add = (binaryfunc) Py3dVector3_Add,
    .nb_subtract = (binaryfunc) Py3dVector3_Sub,
    .nb_multiply = (binaryfunc) Py3dVector3_Mult,
    .nb_true_divide = (binaryfunc) Py3dVector3_Div
};

PyTypeObject Py3dVector3_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "py3dmath.Vector3",
    .tp_doc = "A 3 dimensional vector",
    .tp_basicsize = sizeof(struct Py3dVector3),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_init = (initproc) Py3dVector3_Init,
    .tp_methods = Py3dVector3_Methods,
    .tp_dealloc = (destructor) Py3dVector3_Dealloc,
    .tp_new = PyType_GenericNew,
    .tp_repr = (reprfunc) Py3dVector3_Repr,
    .tp_str = (reprfunc) Py3dVector3_Repr,
    .tp_getset = Py3dVector3_GettersSetters
};

static PyObject *float_cast(PyObject *obj) {
    if (obj == NULL) return NULL;

    obj = PyNumber_Float(obj);
    return obj;
}

static int init_from_list(struct Py3dVector3 *self, PyObject *list) {
    if (PyObject_Length(list) < 3) {
        PyErr_SetString(PyExc_ValueError, "Init Vector3 from list requires 3 list items");
        return -1;
    }

    float data[3] = {0.0f, 0.0f, 0.0f};
    for (Py_ssize_t i = 0; i < 3; ++i) {
        PyObject *li = float_cast(PyList_GetItem(list, i));
        if (li == NULL) return -1;

        data[i] = (float) PyFloat_AsDouble(li);
        //float_cast returns new ref that must be decref'ed
        //PyList_GetItem does not
        Py_CLEAR(li);
    }

    Vec3Copy(self->elements, data);

    return 0;
}

static int init_from_dict(struct Py3dVector3 *self, PyObject *dict) {
    char key_names[6] = {'x', 0, 'y', 0, 'z', 0};
    float data[3] = {0.0f, 0.0f, 0.0f};

    for (Py_ssize_t i = 0; i < 3; ++i) {
        PyObject *li = PyDict_GetItemString(dict, &(key_names[i*2]));
        if (li == NULL) {
            PyErr_SetString(PyExc_KeyError, "Init Vector3 from dictionary requires keys with names \"x, y, z\"");
        }

        li = float_cast(li);
        if (li == NULL) return -1;

        data[i] = (float) PyFloat_AsDouble(li);
        //float_cast returns new ref that must be decref'ed
        //PyDict_GetItemString does not
        Py_CLEAR(li);
    }

    Vec3Copy(self->elements, data);

    return 0;
}

static int Py3dVector3_Init(struct Py3dVector3 *self, PyObject *args, PyObject *Py_UNUSED(kwds)) {
    if (PyTuple_Size(args) == 0) {
        Vec3Fill(self->elements, 0.0f);

        return 0;
    }

    PyObject *singleObjArg = NULL;
    if (PyArg_ParseTuple(args, "O", &singleObjArg) == 1) {
        if (PyList_Check(singleObjArg)) {
            return init_from_list(self, singleObjArg);
        } else if (PyDict_Check(singleObjArg)) {
            return init_from_dict(self, singleObjArg);
        } else if (Py3dVector3_Check(singleObjArg)) {
            struct Py3dVector3 *other = (struct Py3dVector3 *) singleObjArg;
            Vec3Copy(self->elements, other->elements);
            return 0;
        } else {
            PyErr_SetString(PyExc_TypeError, "Single object argument must be of type list, dictionary or Vector3");
            return -1;
        }
    }
    PyErr_Clear();

    if (
        PyArg_ParseTuple(
            args, "fff", &(self->elements[0]), &(self->elements[1]), &(self->elements[2])
        ) != 1
    ) return -1;

    return 0;
}

static void Py3dVector3_Dealloc(struct Py3dVector3 *self) {
    Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject *Py3dVector3_GetX(struct Py3dVector3 *self, void *Py_UNUSED(closure)) {
    return PyFloat_FromDouble(self->elements[0]);
}

PyObject *Py3dVector3_GetY(struct Py3dVector3 *self, void *Py_UNUSED(closure)) {
    return PyFloat_FromDouble(self->elements[1]);
}

PyObject *Py3dVector3_GetZ(struct Py3dVector3 *self, void *Py_UNUSED(closure)) {
    return PyFloat_FromDouble(self->elements[2]);
}

PyObject *Py3dVector3_Repr(struct Py3dVector3 *self) {
    char buffer[MAX_STRING_SIZE];
    Vec3ToCStr(buffer, self->elements, MAX_STRING_SIZE);

    return PyUnicode_FromString(buffer);
}

static struct Py3dVector3 *checkTypeOfOther(PyObject *other) {
    int isInstanceCheck = PyObject_IsInstance(other, (PyObject *) &Py3dVector3_Type);
    if (isInstanceCheck == -1) {
        return NULL;
    } else if (isInstanceCheck == 0) {
        PyErr_SetString(PyExc_TypeError, "Vector3 can only be added to other Vector3 instances");
        return NULL;
    }

    return (struct Py3dVector3 *) other;
}

PyObject *Py3dVector3_Add(struct Py3dVector3 *self, PyObject *other) {
    struct Py3dVector3 *otherAsVec3 = checkTypeOfOther(other);
    if (otherAsVec3 == NULL) return NULL;

    struct Py3dVector3 *result = Py3dVector3_New(0.0f, 0.0f, 0.0f);
    if (result == NULL) return NULL;

    Vec3Add(result->elements, self->elements, otherAsVec3->elements);

    return (PyObject *) result;
}

PyObject *Py3dVector3_Sub(struct Py3dVector3 *self, PyObject *other) {
    struct Py3dVector3 *otherAsVec3 = checkTypeOfOther(other);
    if (otherAsVec3 == NULL) return NULL;

    struct Py3dVector3 *result = Py3dVector3_New(0.0f, 0.0f, 0.0f);
    if (result == NULL) return NULL;

    Vec3Subtract(result->elements, self->elements, otherAsVec3->elements);

    return (PyObject *) result;
}

static PyObject *do_Vector3_Cross_Mult(struct Py3dVector3 *self, struct Py3dVector3 *other) {
    struct Py3dVector3 *result = Py3dVector3_New(0.0f, 0.0f, 0.0f);
    if (result == NULL) return NULL;

    Vec3Cross(result->elements, self->elements, other->elements);

    return (PyObject *) result;
}

static PyObject *do_Vector3_Quaternion_Mult(struct Py3dVector3 *self, struct Py3dQuaternion *other) {
    struct Py3dVector3 *result = Py3dVector3_New(0.0f, 0.0f, 0.0f);
    if (result == NULL) return NULL;

    QuaternionVec3Rotation(result->elements, self->elements, other->elements);

    return (PyObject *) result;
}

static PyObject *do_Vector3_Matrix4x4_Mult(struct Py3dVector3 *self, struct Py3dMatrix4x4 *other) {
    struct Py3dVector3 *result = Py3dVector3_New(0.0f, 0.0f, 0.0f);
    if (result == NULL) return NULL;

    float vec4[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float temp[] = {0.0f, 0.0f, 0.0f, 1.0f};
    Vec3Copy(vec4, self->elements);

    Mat4Vec4Mult(temp, other->elements, vec4);

    Vec3Copy(result->elements, temp);

    return (PyObject *) result;
}

static PyObject *do_Vector3_Scalar_Mult(struct Py3dVector3 *self, float scalar) {
    struct Py3dVector3 *result = Py3dVector3_New(0.0f, 0.0f, 0.0f);
    if (result == NULL) return NULL;

    Vec3Scalar(result->elements, self->elements, scalar);

    return (PyObject *) result;
}

PyObject *Py3dVector3_Mult(struct Py3dVector3 *self, PyObject *other) {
    if (Py3dVector3_Check(other)) {
        return do_Vector3_Cross_Mult(self, (struct Py3dVector3 *) other);
    }

    if (Py3dQuaternion_Check(other)) {
        return do_Vector3_Quaternion_Mult(self, (struct Py3dQuaternion *) other);
    }

    if (Py3dMatrix4x4_Check(other)) {
        return do_Vector3_Matrix4x4_Mult(self, (struct Py3dMatrix4x4 *) other);
    }

    PyObject *otherAsFlt = PyNumber_Float(other);
    if (otherAsFlt == NULL) {
        PyErr_Clear();
        PyErr_SetString(PyExc_TypeError, "Second operand must be cast-able to float for scalar multiplication");
        return NULL;
    }
    float scalar = (float) PyFloat_AsDouble(otherAsFlt);
    Py_CLEAR(otherAsFlt);

    return (PyObject *) do_Vector3_Scalar_Mult(self, scalar);
}

PyObject *Py3dVector3_Div(struct Py3dVector3 *self, PyObject *other) {
    PyObject *otherAsFlt = PyNumber_Float(other);
    if (otherAsFlt == NULL) {
        PyErr_SetString(PyExc_TypeError, "Vector3 can only be divided by numbers");
        return NULL;
    }

    float scalar = (float) PyFloat_AsDouble(otherAsFlt);
    Py_CLEAR(otherAsFlt);

    if (scalar == 0.0f) {
        PyErr_SetString(PyExc_ZeroDivisionError, "Vector3 cannot be divided by zero");
        return NULL;
    }

    struct Py3dVector3 *result = Py3dVector3_New(0.0f, 0.0f, 0.0f);
    if (result == NULL) return NULL;

    Vec3Divide(result->elements, self->elements, scalar);

    return (PyObject *) result;
}

PyObject *Py3dVector3_Dot(struct Py3dVector3 *self, PyObject *args, PyObject *Py_UNUSED(kwds)) {
    struct Py3dVector3 *other = NULL;
    if (PyArg_ParseTuple(args, "O!", &Py3dVector3_Type, &other) == 0) return NULL;

    float ret = 0.0f;
    Vec3Dot(&ret, self->elements, other->elements);

    return PyFloat_FromDouble(ret);
}

PyObject *Py3dVector3_Length(struct Py3dVector3 *self, PyObject *Py_UNUSED(args)) {
    float ret = 0.0f;
    Vec3Length(&ret, self->elements);

    return PyFloat_FromDouble(ret);
}

PyObject *Py3dVector3_Normalize(struct Py3dVector3 *self, PyObject *Py_UNUSED(args)) {
    struct Py3dVector3 *result = Py3dVector3_New(0.0f, 0.0f, 0.0f);
    if (result == NULL) return NULL;

    Vec3Normalize(result->elements, self->elements);

    return (PyObject *) result;
}

PyObject *Py3dVector3_Fill(struct Py3dVector3 *Py_UNUSED(self), PyObject *args, PyObject *Py_UNUSED(kwds)) {
    float value = 0.0f;
    if (PyArg_ParseTuple(args, "f", &value) != 1) return NULL;

    struct Py3dVector3 *result = Py3dVector3_New(0.0f, 0.0f, 0.0f);
    if (result == NULL) return NULL;

    Vec3Fill(result->elements, value);

    return (PyObject *) result;
}

// Is this even needed for immutable objects?
PyObject *Py3dVector3_Copy(struct Py3dVector3 *self, PyObject *Py_UNUSED(args)) {
    struct Py3dVector3 *result = Py3dVector3_New(0.0f, 0.0f, 0.0f);
    if (result == NULL) return NULL;

    Vec3Copy(result->elements, self->elements);

    return (PyObject *) result;
}

int PyInit_Py3dVector3(PyObject *module) {
    Py3dVector3_Type.tp_as_number = &Py3dVector3_NumberMethods;
    if (PyType_Ready(&Py3dVector3_Type) < 0) return 0;

    if (PyModule_AddObjectRef(module, "Vector3", (PyObject *) &Py3dVector3_Type) < 0) return 0;

    return 1;
}

struct Py3dVector3 *Py3dVector3_New(float x, float y, float z) {
    PyObject *mathModule = PyImport_ImportModule("py3dengine.math");
    if (mathModule == NULL) return NULL;

    PyObject *vector3ctor = PyObject_GetAttrString(mathModule, "Vector3");
    Py_CLEAR(mathModule);
    if (vector3ctor == NULL) return NULL;

    if (!PyCallable_Check(vector3ctor)) {
        PyErr_SetString(PyExc_TypeError, "Vector3 ctor must be callable");
        Py_CLEAR(vector3ctor);
        return NULL;
    }

    PyObject *args = Py_BuildValue("(fff)", &x, &y, &z);
    PyObject *ret = PyObject_Call(vector3ctor, args, NULL);
    Py_CLEAR(args);
    Py_CLEAR(vector3ctor);
    if (ret == NULL) return NULL;

    if (!Py3dVector3_Check(ret)) {
        // WTF???
        Py_CLEAR(ret);
        PyErr_SetString(PyExc_TypeError, "Vector3 ctor did not return a Vector3");
        return NULL;
    }

    return (struct Py3dVector3 *) ret;
}

int Py3dVector3_Check(PyObject *obj) {
    if (obj == NULL) return 0;

    int ret = PyObject_IsInstance(obj, (PyObject *) &Py3dVector3_Type);
    if (ret == -1) {
        PyErr_Clear();
        return 0;
    }

    return ret;
}