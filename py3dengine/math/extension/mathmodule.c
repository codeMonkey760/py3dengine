#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "py3dvector3.h"
#include "py3dquaternion.h"
#include "py3dmatrix4x4.h"

static struct PyModuleDef py3dmathModuleDef = {
    PyModuleDef_HEAD_INIT,
    .m_name = "py3dengine.math",
    .m_doc = "Contains 3d math related objects and functions",
    .m_size = -1,
};

extern PyMODINIT_FUNC PyInit_math(void) {
    PyObject *newModule = PyModule_Create(&py3dmathModuleDef);
    if (newModule == NULL) return NULL;

    if (!PyInit_Py3dVector3(newModule)) {
        Py_CLEAR(newModule);
        return NULL;
    }

    if (!PyInit_Py3dQuaternion(newModule)) {
        Py_CLEAR(newModule);
        return NULL;
    }

    if (!PyInit_Py3dQuaternion(newModule)) {
        Py_CLEAR(newModule);
        return NULL;
    }

    if (!PyInit_Py3dMatrix4x4(newModule)) {
        Py_CLEAR(newModule);
        return NULL;
    }

    return newModule;
}
