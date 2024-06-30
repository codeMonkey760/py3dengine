#ifndef QUATERNION_H
#define QUATERNION_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

struct Py3dQuaternion {
    PyObject_HEAD
    float elements[4];
};
extern PyTypeObject Py3dQuaternion_Type;

extern int PyInit_Py3dQuaternion(PyObject *module);
extern PyObject *Py3dQuaternion_Repr(struct Py3dQuaternion *self);
extern struct Py3dQuaternion *Py3dQuaternion_New(float x, float y, float z, float w);
extern int Py3dQuaternion_Check(PyObject *obj);

extern PyObject *Py3dQuaternion_GetX(struct Py3dQuaternion *self, void *closure);
extern PyObject *Py3dQuaternion_GetY(struct Py3dQuaternion *self, void *closure);
extern PyObject *Py3dQuaternion_GetZ(struct Py3dQuaternion *self, void *closure);
extern PyObject *Py3dQuaternion_GetW(struct Py3dQuaternion *self, void *closure);
extern PyObject *Py3dQuaternion_FromAxisAndDegrees(struct Py3dQuaternion *self, PyObject *args, PyObject *kwds);
extern PyObject *Py3dQuaternion_Normalize(struct Py3dQuaternion *self, PyObject *args);
extern PyObject *Py3dQuaternion_Mult(struct Py3dQuaternion *self, PyObject *other);


#endif
