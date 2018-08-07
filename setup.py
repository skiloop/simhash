#!/usr/bin/env python
# -*- coding:utf8 -*-
#
# created by skiloop@gmail.com 2018/7/27
#

from six import PY3
from distutils.core import setup, Extension

__via_cmake__ = '${CMAKE_CURRENT_SOURCE_DIR}'[2:] != "CMAKE_CURRENT_SOURCE_DIR}"

# the c++ extension module
libraries = []
libraries.append('boost_python36' if PY3 else 'boost_python27')
sources = ["${CMAKE_CURRENT_SOURCE_DIR}/py_simhash.cpp"] if __via_cmake__ else ["py_simhash.cpp"]
extension_mod = Extension("pysimhash", sources, extra_compile_args=['-std=c++11'],
                          libraries=libraries)

setup(name="pysimhash", ext_modules=[extension_mod])
