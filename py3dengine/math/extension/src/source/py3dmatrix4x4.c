#include "py3dvector3.h"
#include "py3dquaternion.h"
#include "py3dmatrix4x4.h"

#include <structmember.h>

#include <matrix.h>

#define MAX_STRING_SIZE 256

static int Py3dMatrix4x4_Init(struct Py3dMatrix4x4 *self, PyObject *args, PyObject *kwds);
static void Py3dMatrix4x4_Dealloc(struct Py3dMatrix4x4 *self);

static PyMethodDef Py3dMatrix4x4_Methods[] = {
    {"copy", (PyCFunction) Py3dMatrix4x4_Copy, METH_NOARGS, "Copy an existing Matrix4x4"},
    {"transpose", (PyCFunction) Py3dMatrix4x4_Transpose, METH_NOARGS, "Copy an existing Matrix4x4"},
    {"inverse", (PyCFunction) Py3dMatrix4x4_Inverse, METH_NOARGS, "Calculate the inverse of a Matrix4x4"},

    {"Fill", (PyCFunction) Py3dMatrix4x4_Fill, METH_VARARGS | METH_STATIC, "Create a new Matrix4x4 and fill it with a value"},
    {"Translation", (PyCFunction) Py3dMatrix4x4_Translation, METH_VARARGS | METH_STATIC, "Create a new translation Matrix4x4 with the provided Vector3"},
    {"RotationX", (PyCFunction) Py3dMatrix4x4_RotationX, METH_VARARGS | METH_STATIC, "Create a new x axis rotation Matrix4x4 with the provided angle in degrees"},
    {"RotationY", (PyCFunction) Py3dMatrix4x4_RotationY, METH_VARARGS | METH_STATIC, "Create a new y axis rotation Matrix4x4 with the provided angle in degrees"},
    {"RotationZ", (PyCFunction) Py3dMatrix4x4_RotationZ, METH_VARARGS | METH_STATIC, "Create a new z axis rotation Matrix4x4 with the provided angle in degrees"},
    {"RotationAxis", (PyCFunction) Py3dMatrix4x4_RotationAxis, METH_VARARGS | METH_STATIC, "Create a new rotation Matrix4x4 with the provided Vector3 axis angle in degrees"},
    {"RotationQuaternion", (PyCFunction) Py3dMatrix4x4_RotationQuaternion, METH_VARARGS | METH_STATIC, "Create a new rotation Matrix4x4 with the provided quaternion"},
    {"Scaling", (PyCFunction) Py3dMatrix4x4_Scaling, METH_VARARGS | METH_STATIC, "Create a new scaling Matrix4x4"},
    {"LookAtLH", (PyCFunction) Py3dMatrix4x4_LookAtLH, METH_VARARGS | METH_STATIC, "Create a new left handed look at Matrix4x4 using the supplied target and up vectors in world space"},
    {NULL}
};

static PyNumberMethods Py3dMatrix4x4_NumberMethods = {
    .nb_multiply = (binaryfunc) Py3dMatrix4x4_Mult,
};

PyTypeObject Py3dMatrix4x4_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "py3dmath.Matrix4x4",
    .tp_doc = "A 4x4 Matrix",
    .tp_basicsize = sizeof(struct Py3dMatrix4x4),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_init = (initproc) Py3dMatrix4x4_Init,
    .tp_methods = Py3dMatrix4x4_Methods,
    .tp_dealloc = (destructor) Py3dMatrix4x4_Dealloc,
    .tp_new = PyType_GenericNew,
    .tp_repr = (reprfunc) Py3dMatrix4x4_Repr,
    .tp_str = (reprfunc) Py3dMatrix4x4_Repr,
};

int PyInit_Py3dMatrix4x4(PyObject *module) {
    Py3dMatrix4x4_Type.tp_as_number = &Py3dMatrix4x4_NumberMethods;
    if (PyType_Ready(&Py3dMatrix4x4_Type) < 0) return 0;

    if (PyModule_AddObjectRef(module, "Matrix4x4", (PyObject *) &Py3dMatrix4x4_Type) < 0) return 0;

    return 1;
}

static int Py3dMatrix4x4_Init(struct Py3dMatrix4x4 *self, PyObject *Py_UNUSED(args), PyObject *Py_UNUSED(kwds)) {
    Mat4Identity(self->elements);

    return 0;
}

static void Py3dMatrix4x4_Dealloc(struct Py3dMatrix4x4 *self) {
    Py_TYPE(self)->tp_free((PyObject *) self);
}

struct Py3dMatrix4x4 *Py3dMatrix4x4_New() {
    PyObject *mathModule = PyImport_ImportModule("py3dengine.math");
    if (mathModule == NULL) return NULL;

    PyObject *mat4ctor = PyObject_GetAttrString(mathModule, "Matrix4x4");
    Py_CLEAR(mathModule);
    if (mat4ctor == NULL) return NULL;

    if (!PyCallable_Check(mat4ctor)) {
        PyErr_SetString(PyExc_TypeError, "Matrix4x4 ctor must be callable");
        Py_CLEAR(mat4ctor);
        return NULL;
    }

    PyObject *ret = PyObject_CallNoArgs(mat4ctor);
    Py_CLEAR(mat4ctor);

    if (ret == NULL) return NULL;

    if (!Py3dMatrix4x4_Check(ret)) {
        // WTF???
        Py_CLEAR(ret);
        PyErr_SetString(PyExc_TypeError, "Matrix4x4 ctor did not return a Matrix4x4");
        return NULL;
    }

    return (struct Py3dMatrix4x4 *) ret;
}

int Py3dMatrix4x4_Check(PyObject *obj) {
    if (obj == NULL) return 0;

    int ret = PyObject_IsInstance(obj, (PyObject *) &Py3dMatrix4x4_Type);
    if (ret == -1) {
        PyErr_Clear();
        return 0;
    }

    return ret;
}

PyObject *Py3dMatrix4x4_Repr(struct Py3dMatrix4x4 *self) {
    char buffer[MAX_STRING_SIZE];
    Mat4ToCStr(buffer, self->elements, MAX_STRING_SIZE);

    return PyUnicode_FromString(buffer);
}

PyObject *Py3dMatrix4x4_Copy(struct Py3dMatrix4x4 *self, PyObject *Py_UNUSED(args)) {
    struct Py3dMatrix4x4 *result = Py3dMatrix4x4_New();
    if (result == NULL) return NULL;

    Mat4Copy(result->elements, self->elements);

    return (PyObject *) result;
}

PyObject *Py3dMatrix4x4_Transpose(struct Py3dMatrix4x4 *self, PyObject *Py_UNUSED(args)) {
    struct Py3dMatrix4x4 *result = Py3dMatrix4x4_New();
    if (result == NULL) return NULL;

    Mat4Transpose(result->elements, self->elements);

    return (PyObject *) result;
}

PyObject *Py3dMatrix4x4_Inverse(struct Py3dMatrix4x4 *self, PyObject *Py_UNUSED(args)) {
    struct Py3dMatrix4x4 *result = Py3dMatrix4x4_New();
    if (result == NULL) return NULL;

    int success = Mat4Inverse(result->elements, self->elements);
    if (!success) {
        Py_CLEAR(result);
        PyErr_SetString(PyExc_ArithmeticError, "Matrix has no inverse");
        return NULL;
    }

    return (PyObject *) result;
}

PyObject *Py3dMatrix4x4_Fill(struct Py3dMatrix4x4 *Py_UNUSED(self), PyObject *args, PyObject *Py_UNUSED(kwds)) {
    PyObject *valueAsObj = NULL;
    if (PyArg_ParseTuple(args, "O", &valueAsObj) != 1) return NULL;

    valueAsObj = PyNumber_Float(valueAsObj);
    if (valueAsObj == NULL) return NULL;

    float value = (float) PyFloat_AsDouble(valueAsObj);
    Py_CLEAR(valueAsObj);

    struct Py3dMatrix4x4 *result = Py3dMatrix4x4_New();
    if (result == NULL) return NULL;

    Mat4Fill(result->elements, value);

    return (PyObject *) result;
}

PyObject *Py3dMatrix4x4_Translation(struct Py3dMatrix4x4 *Py_UNUSED(self), PyObject *args, PyObject *Py_UNUSED(kwds)) {
    struct Py3dVector3 *displacement = NULL;
    if (PyArg_ParseTuple(args, "O!", &Py3dVector3_Type, &displacement) != 1) return NULL;

    struct Py3dMatrix4x4 *result = Py3dMatrix4x4_New();
    if (result == NULL) return NULL;

    Mat4TranslationFA(result->elements, displacement->elements);

    return (PyObject *) result;
}

static PyObject *do_Rotation_Matrix(char axis, PyObject *args) {
    PyObject *thetaAsObj = NULL;
    if (PyArg_ParseTuple(args, "O", &thetaAsObj) != 1) return NULL;

    thetaAsObj = PyNumber_Float(thetaAsObj);
    if (thetaAsObj == NULL) return NULL;

    float theta = (float) PyFloat_AsDouble(thetaAsObj);
    Py_CLEAR(thetaAsObj);

    struct Py3dMatrix4x4 *result = Py3dMatrix4x4_New();
    if (result == NULL) return NULL;

    switch (axis) {
        case 'x':
            Mat4RotationX(result->elements, theta);
            break;
        case 'y':
            Mat4RotationY(result->elements, theta);
            break;
        case 'z':
            Mat4RotationZ(result->elements, theta);
            break;
        default:
            Py_CLEAR(result);
            PyErr_SetString(PyExc_AssertionError, "Bad axis specification in do_Rotation_Matrix");
    }

    return (PyObject *) result;
}

PyObject *Py3dMatrix4x4_RotationX(struct Py3dMatrix4x4 *Py_UNUSED(self), PyObject *args, PyObject *Py_UNUSED(kwds)) {
    return do_Rotation_Matrix('x', args);
}

PyObject *Py3dMatrix4x4_RotationY(struct Py3dMatrix4x4 *Py_UNUSED(self), PyObject *args, PyObject *Py_UNUSED(kwds)) {
    return do_Rotation_Matrix('y', args);
}

PyObject *Py3dMatrix4x4_RotationZ(struct Py3dMatrix4x4 *Py_UNUSED(self), PyObject *args, PyObject *Py_UNUSED(kwds)) {
    return do_Rotation_Matrix('z', args);
}

PyObject *Py3dMatrix4x4_RotationAxis(struct Py3dMatrix4x4 *Py_UNUSED(self), PyObject *args, PyObject *Py_UNUSED(kwds)) {
    struct Py3dVector3 *axis = NULL;
    PyObject *thetaAsObj = NULL;
    if (PyArg_ParseTuple(args, "O!O", &Py3dVector3_Type, &axis, &thetaAsObj) != 1) return NULL;

    thetaAsObj = PyNumber_Float(thetaAsObj);
    if (thetaAsObj == NULL) return NULL;

    float theta = (float) PyFloat_AsDouble(thetaAsObj);
    Py_CLEAR(thetaAsObj);

    struct Py3dMatrix4x4 *result = Py3dMatrix4x4_New();
    if (result == NULL) return NULL;

    Mat4RotationAxisFA(result->elements, axis->elements, theta);

    return (PyObject *) result;
}

PyObject *Py3dMatrix4x4_RotationQuaternion(struct Py3dMatrix4x4 *Py_UNUSED(self), PyObject *args, PyObject *Py_UNUSED(kwds)) {
    struct Py3dQuaternion *quat = NULL;
    if (PyArg_ParseTuple(args, "O!O", &Py3dQuaternion_Type, &quat) != 1) return NULL;

    struct Py3dMatrix4x4 *result = Py3dMatrix4x4_New();
    if (result == NULL) return NULL;

    Mat4RotationQuaternionFA(result->elements, quat->elements);

    return (PyObject *) result;
}

PyObject *Py3dMatrix4x4_Scaling(struct Py3dMatrix4x4 *Py_UNUSED(self), PyObject *args, PyObject *Py_UNUSED(kwds)) {
    struct Py3dVector3 *factors = NULL;
    if (PyArg_ParseTuple(args, "O!", &Py3dVector3_Type, &factors) != 1) return NULL;

    struct Py3dMatrix4x4 *result = Py3dMatrix4x4_New();
    if (result == NULL) return NULL;

    Mat4ScalingFA(result->elements, factors->elements);

    return (PyObject *) result;
}

PyObject *Py3dMatrix4x4_LookAtLH(struct Py3dMatrix4x4 *Py_UNUSED(self), PyObject *args, PyObject *Py_UNUSED(kwds)) {
    struct Py3dVector3 *camPosW = NULL, *camTargetW = NULL, *camUpW = NULL;
    if (
        PyArg_ParseTuple(
            args, "O!O!O!",
            &Py3dVector3_Type, &camPosW,
            &Py3dVector3_Type, &camTargetW,
            &Py3dVector3_Type, &camUpW
        ) != 1
    ) return NULL;

    struct Py3dMatrix4x4 *result = Py3dMatrix4x4_New();
    if (result == NULL) return NULL;

    Mat4LookAtLH(result->elements, camPosW->elements, camTargetW->elements, camUpW->elements);

    return (PyObject *) result;
}

PyObject *Py3dMatrix4x4_Mult(struct Py3dMatrix4x4 *self, PyObject *other) {
    if (!Py3dMatrix4x4_Check(other)) {
        PyErr_SetString(PyExc_TypeError, "Second operand must be of type Matrix4x4");
        return NULL;
    }

    struct Py3dMatrix4x4 *operand = (struct Py3dMatrix4x4 *) other;
    struct Py3dMatrix4x4 *result = Py3dMatrix4x4_New();
    if (result == NULL) return NULL;

    Mat4Mult(result->elements, self->elements, operand->elements);

    return (PyObject *) result;
}
