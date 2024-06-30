#ifndef VECTOR_3_H
#define VECTOR_3_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

struct Py3dVector3 {
    PyObject_HEAD
    float elements[3];
};
extern PyTypeObject Py3dVector3_Type;

extern int PyInit_Py3dVector3(PyObject *module);
extern PyObject *Py3dVector3_Repr(struct Py3dVector3 *self);
extern struct Py3dVector3 *Py3dVector3_New(float x, float y, float z);
extern int Py3dVector3_Check(PyObject *obj);

extern PyObject *Py3dVector3_GetX(struct Py3dVector3 *self, void *closure);
extern PyObject *Py3dVector3_GetY(struct Py3dVector3 *self, void *closure);
extern PyObject *Py3dVector3_GetZ(struct Py3dVector3 *self, void *closure);
extern PyObject *Py3dVector3_Dot(struct Py3dVector3 *self, PyObject *args, PyObject *kwds);
extern PyObject *Py3dVector3_Length(struct Py3dVector3 *self, PyObject *args);
extern PyObject *Py3dVector3_Normalize(struct Py3dVector3 *self, PyObject *args);
extern PyObject *Py3dVector3_Fill(struct Py3dVector3 *self, PyObject *args, PyObject *kwds);
extern PyObject *Py3dVector3_Copy(struct Py3dVector3 *self, PyObject *args);
extern PyObject *Py3dVector3_Add(struct Py3dVector3 *self, PyObject *other);
extern PyObject *Py3dVector3_Sub(struct Py3dVector3 *self, PyObject *other);
extern PyObject *Py3dVector3_Mult(struct Py3dVector3 *self, PyObject *other);
extern PyObject *Py3dVector3_Div(struct Py3dVector3 *self, PyObject *other);

#endif
