from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            name="py3dengine.math",
            sources=[
                "mathmodule.c",
                "src/source/py3dvector3.c",
                "src/source/py3dquaternion.c",
                "src/source/py3dmatrix4x4.c"
            ],
            include_dirs=['src/headers', '../lib/src/headers'],
            library_dirs=['../lib/cmake-build-debug'],
            libraries=['py3dmath']
        )
    ]
)