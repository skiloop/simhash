cmake_minimum_required(VERSION 3.8)
project(simhash_cpp)

set(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(PythonInterp)

if (PYTHONINTERP_FOUND)
    if (UNIX AND NOT APPLE)
        if (PYTHON_VERSION_MAJOR EQUAL 3)
            FIND_PACKAGE(Boost COMPONENTS python${PYTHON_VERSION_SUFFIX})
            FIND_PACKAGE(PythonInterp 3)
            FIND_PACKAGE(PythonLibs 3 REQUIRED)
        else ()
            FIND_PACKAGE(Boost COMPONENTS python)
            FIND_PACKAGE(PythonInterp)
            FIND_PACKAGE(PythonLibs REQUIRED)
        endif ()
    else ()
        if (PYTHON_VERSION_MAJOR EQUAL 3)
            FIND_PACKAGE(Boost COMPONENTS python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
            FIND_PACKAGE(PythonInterp 3)
            FIND_PACKAGE(PythonLibs 3 REQUIRED)
        else ()
            FIND_PACKAGE(Boost COMPONENTS python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
            FIND_PACKAGE(PythonInterp)
            FIND_PACKAGE(PythonLibs REQUIRED)
        endif ()
    endif ()
else ()
    message("Python not found")
endif ()

message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
LINK_LIBRARIES(${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

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
            COMMAND ${PYTHON_EXECUTABLE}
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
