import os, sys

from distutils.core import setup, Extension
from distutils import sysconfig

cpp_args = ['-std=c++11', '-stdlib=libc++', '-mmacosx-version-min=10.7']

sfc_module = Extension(
    'BondMathLib2', sources=['BondMath.cpp','PythonBondMathWrapper.cpp'],
    include_dirs=['pybind11/include'],
    language='c++',
    extra_compile_args=cpp_args,
    )

setup(
    name='BondMathLib2',
    version='1.0',
    description='Python package with BondMathLib2 C++ extension (PyBind11) Mike Jang',
    ext_modules=[sfc_module],
)