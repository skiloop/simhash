#!/usr/bin/env python
# -*- coding:utf8 -*-
#
# created by skiloop@gmail.com 2018/7/27
#
import io
import os
from setuptools import setup
from distutils.core import Extension

from six import PY2

__via_cmake__ = '${CMAKE_CURRENT_SOURCE_DIR}'[2:] != "CMAKE_CURRENT_SOURCE_DIR}"

NAME = 'pysimhash'
DESCRIPTION = 'a simhash module in cpp for python'
URL = 'https://github.com/skiloop/simhash'
EMAIL = 'skiloop@gmail.com'
AUTHOR = 'Skiloop'
VERSION = "1.0.5"

try:
    src_path = "${CMAKE_CURRENT_SOURCE_DIR}/" if __via_cmake__ else ""
    with io.open(os.path.join(src_path, 'README.md'), encoding='utf-8') as f:
        long_description = '\n' + f.read()
except IOError:
    long_description = DESCRIPTION

# the c++ extension module
libraries = ['boost_python27' if PY2 else 'boost_python36']
sources = ["py_simhash.cpp", "SimHashBase.cpp"]
if __via_cmake__:
    sources = ["${CMAKE_CURRENT_SOURCE_DIR}/{}".format(s) for s in sources]
extension_mod = Extension("pysimhash", sources, extra_compile_args=['-std=c++11'],
                          libraries=libraries)

setup(name=NAME, version=VERSION, description=DESCRIPTION, author=AUTHOR, long_description=long_description,
      author_email=EMAIL, url=URL, license='MIT', long_description_content_type="text/markdown",
      ext_modules=[extension_mod], requires=['six'])
