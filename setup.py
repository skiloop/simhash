"""
setup.py
use with pyproject.toml
"""

# !/usr/bin/env python
# -*- coding:utf8 -*-
#
# created by skiloop@gmail.com 2018/7/27
#
import platform
import sys

try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.extension import Extension
    from distutils.core import setup

from pybind11.setup_helpers import Pybind11Extension, build_ext


def scan_argv(argv, feat):
    for i in range(len(argv)):
        arg = argv[i]
        if arg == feat:
            del argv[i]
            return True
    return False


# the c++ extension module
libraries = []
extra_compile_flags = ['/std:c++11' if platform.system() == 'Windows' else '-std=c++11']
extra_link_flags = []

sources = ["src/py_simhash.cpp", "src/SimHashBase.cpp"]


def run_setup():
    extension_mod = Pybind11Extension(
        "pysimhash", sources,
        extra_compile_args=extra_compile_flags,
        extra_link_args=extra_link_flags,
        libraries=libraries)
    setup(ext_modules=[extension_mod], cmdclass={"build_ext": build_ext})


help_text = """  
"""


def strip_local_options(argv):
    options = ["--help", "-h", "--debug"]
    for option in options:
        scan_argv(argv, option)


if __name__ == "__main__":
    if "--help" in sys.argv or "-h" in sys.argv:
        print(help_text)
        strip_local_options(sys.argv)
        run_setup()
        sys.exit()
    if scan_argv(sys.argv, "--debug"):
        extra_compile_flags.append("-DDEBUG")
    run_setup()
