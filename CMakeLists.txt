cmake_minimum_required(VERSION 3.8)
project(simhash_cpp)

set(CMAKE_CXX_STANDARD 11)

include(FindPythonInterp)
include(FindPythonLibs)

message("-- python libraries: ${Python_LIBRARIES}")
message("-- python libraries dirs: ${Python_LIBRARY_DIRS}")
message("-- python version: ${Python_VERSION}")

if (PYTHONLIBS_FOUND)
    message("-- use python libraries")
    string(REPLACE "." ";" VERSION_LIST ${PYTHONLIBS_VERSION_STRING})
    list(GET VERSION_LIST 0 PYTHON_VERSION_MAJOR)
    list(GET VERSION_LIST 1 PYTHON_VERSION_MINOR)
    list(GET VERSION_LIST 2 PYTHON_VERSION_PATCH)
    MESSAGE("-- PYTHON VERSION MAJOR: ${PYTHON_VERSION_MAJOR}")
    MESSAGE("-- PYTHON VERSION MINOR: ${PYTHON_VERSION_MINOR}")
    MESSAGE("-- PYTHON VERSION PATCH: ${PYTHON_VERSION_PATCH}")
    set(CMAKE_CXX_FLAGS -I${PYTHON_INCLUDE_DIR})
    if (${PYTHONLIBS_VERSION_STRING} MATCHES "^3.")
        set(CMAKE_EXE_LINKER_FLAGS "-lboost_python36")
    else ()
        set(CMAKE_EXE_LINKER_FLAGS "-lboost_python27")
    endif ()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpython${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR} -L${PYTHON_INCLUDE_DIR}/../../lib/")
    message("-- CMAKE EXE LINKER FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
endif ()

add_executable(simhash_cpp SimHashBase.cpp main.cpp)


if (PYTHONINTERP_FOUND)
    message("-- python interpreter found")
    set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py")
    set(SETUP_PY "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
    set(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/build/timestamp")
    set(PYTHONPATH "${CMAKE_CURRENT_BINARY_DIR}/${BUILD_LIB_DIR}")


    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/build")
    configure_file(${SETUP_PY_IN} ${SETUP_PY})

    add_custom_command(OUTPUT ${OUTPUT}
            COMMAND ${Python_EXECUTABLE}
            ARGS ${SETUP_PY} build)

    add_custom_target(pysimhash ALL DEPENDS ${OUTPUT})

    install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} install)")


    # TEST CONFIG
    set(TEST_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/test.py")
    set(TEST_PY "${CMAKE_CURRENT_BINARY_DIR}/test.py")
    configure_file(${TEST_PY_IN} ${TEST_PY})
    message("-- python path: ${PYTHONPATH}")
    add_test(NAME simhash_test COMMAND ${PYTHON_EXECUTABLE} ${TEST_PY})
    set_tests_properties(simhash_test PROPERTIES ENVIRONMENT PYTHONPATH=${PYTHONPATH})
endif ()
