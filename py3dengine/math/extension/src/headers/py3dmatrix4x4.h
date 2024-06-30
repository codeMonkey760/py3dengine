#ifndef PY3DMATRIX4X4_H
#define PY3DMATRIX4X4_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

struct Py3dMatrix4x4 {
    PyObject_HEAD
    float elements[16];
};
extern PyTypeObject Py3dMatrix4x4_Type;

extern int PyInit_Py3dMatrix4x4(PyObject *module);
extern PyObject *Py3dMatrix4x4_Repr(struct Py3dMatrix4x4 *self);
extern struct Py3dMatrix4x4 *Py3dMatrix4x4_New();
extern int Py3dMatrix4x4_Check(PyObject *obj);

extern PyObject *Py3dMatrix4x4_Copy(struct Py3dMatrix4x4 *self, PyObject *args);
extern PyObject *Py3dMatrix4x4_Transpose(struct Py3dMatrix4x4 *self, PyObject *args);
extern PyObject *Py3dMatrix4x4_Inverse(struct Py3dMatrix4x4 *self, PyObject *args);

extern PyObject *Py3dMatrix4x4_Fill(struct Py3dMatrix4x4 *self, PyObject *args, PyObject *kwds);
extern PyObject *Py3dMatrix4x4_Translation(struct Py3dMatrix4x4 *self, PyObject *args, PyObject *kwds);
extern PyObject *Py3dMatrix4x4_RotationX(struct Py3dMatrix4x4 *self, PyObject *args, PyObject *kwds);
extern PyObject *Py3dMatrix4x4_RotationY(struct Py3dMatrix4x4 *self, PyObject *args, PyObject *kwds);
extern PyObject *Py3dMatrix4x4_RotationZ(struct Py3dMatrix4x4 *self, PyObject *args, PyObject *kwds);
extern PyObject *Py3dMatrix4x4_RotationAxis(struct Py3dMatrix4x4 *self, PyObject *args, PyObject *kwds);
extern PyObject *Py3dMatrix4x4_RotationQuaternion(struct Py3dMatrix4x4 *self, PyObject *args, PyObject *kwds);
extern PyObject *Py3dMatrix4x4_Scaling(struct Py3dMatrix4x4 *self, PyObject *args, PyObject *kwds);
extern PyObject *Py3dMatrix4x4_LookAtLH(struct Py3dMatrix4x4 *self, PyObject *args, PyObject *kwds);
extern PyObject *Py3dMatrix4x4_Mult(struct Py3dMatrix4x4 *self, PyObject *other);

#endif
