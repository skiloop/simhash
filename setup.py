#!/usr/bin/env python
# -*- coding:utf8 -*-
#
# created by skiloop@gmail.com 2018/7/27
#
import io
import os
import platform
import sys

try:
    from setuptools import setup
except ImportError:
    from distutils.core import setup
try:
    FileNotFoundError
except NameError:
    FileNotFoundError = IOError
from distutils.extension import Extension

PY_VERSION = platform.python_version_tuple()

NAME = 'pysimhash'
DESCRIPTION = 'a simhash module in cpp for python'
URL = 'https://github.com/skiloop/simhash'
EMAIL = 'skiloop@gmail.com'
AUTHOR = 'Skiloop'
VERSION = "1.1.1"
SYSTEM = platform.system()
if SYSTEM == "Windows":
    raise EnvironmentError("not support for windows")

try:
    with io.open('README.md', encoding='utf-8') as f:
        long_description = '\n' + f.read()
except IOError:
    long_description = DESCRIPTION


def get_library(lp):
    bn = os.path.basename(lp)
    bb = bn.split('.', 2)
    return bb[0]


def find_file(path, func):
    for fn in os.listdir(path):
        if os.path.isfile(path + "/" + fn) and func(fn):
            return fn


def scan_argv(argv, feat):
    for i in range(len(argv)):
        arg = argv[i]
        if arg == feat:
            del argv[i]
            return True
    return False


boost_lib_prefix = "libboost_python"


def get_boost_python_root(default_root):
    boost_python_path = os.environ.get("BOOST_PYTHON_PATH")
    if boost_python_path is None or boost_python_path == "":
        boost_python_path = default_root
    return boost_python_path


def find_boost_library_osx(boost_lib_path):
    boost_lib_name = ''.join(['libboost_python', PY_VERSION[0], PY_VERSION[1], '.dylib'])
    boost_library = find_file(boost_lib_path, lambda s: s == boost_lib_name)
    if boost_library is None:
        msg = "No boost-python library built with python %s.%s found. " \
              "This happens when boost-python is not in common paths. " \
              "Or you mix the versions, for example use a boost-python " \
              "built with python 3.9 to build pysimhash for python 3.7. " \
              "You may set BOOST_PYTHON_PATH to where the correct boost-python is installed." % PY_VERSION[:2]
        raise FileNotFoundError(msg)
    return boost_library.split(".", 1)[0][3:]


def src_path(fn):
    return os.path.join("src", fn)


# the c++ extension module
libraries = []
extra_compile_flags = ['-std=c++11']
extra_link_flags = []


def update_build_flags(default_boost_python_root=None):
    root_path = get_boost_python_root(default_boost_python_root)
    if root_path is None or root_path == "":
        return root_path

    extra_compile_flags.append("-I" + root_path + "/include")
    extra_link_flags.append("-L" + root_path + "/lib")
    return root_path


if SYSTEM == "Darwin":
    boost_python_root = update_build_flags("/usr/local/")
    libraries.append(find_boost_library_osx(boost_python_root + "/lib"))
elif SYSTEM == "Windows":
    raise RuntimeError("not support for Windows")
else:
    boost_python_root = update_build_flags()
    libraries.append("boost_python" if PY_VERSION[0] == "2" else "boost_python3")
sources = [src_path("py_simhash.cpp"), src_path("SimHashBase.cpp")]


def run_setup():
    extension_mod = Extension(NAME, sources, extra_compile_args=extra_compile_flags,
                              extra_link_args=extra_link_flags,
                              libraries=libraries)
    setup(
        name=NAME,
        version=VERSION,
        description=DESCRIPTION,
        author=AUTHOR,
        long_description=long_description,
        author_email=EMAIL,
        url=URL,
        license='MIT',
        classifiers=[
            'Operating System :: MacOS :: MacOS X',
            'Operating System :: POSIX :: Linux',
            'License :: OSI Approved :: MIT License',
            'Programming Language :: Python',
            'Programming Language :: Python :: 2',
            'Programming Language :: Python :: 2.7',
            'Programming Language :: Python :: 3',
            'Programming Language :: Python :: 3.6',
            'Programming Language :: Python :: 3.7',
            'Programming Language :: Python :: 3.8',
            'Programming Language :: Python :: 3.9',
            "Topic :: Utilities",
            'Topic :: Software Development :: Libraries :: Python Modules',
        ],
        long_description_content_type="text/markdown",
        ext_modules=[extension_mod])


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
