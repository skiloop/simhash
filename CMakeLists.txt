cmake_minimum_required(VERSION 3.8)
project(simhash_cpp)

set(CMAKE_CXX_STANDARD 11)

add_executable(simhash_cpp main.cpp)

include(FindPythonInterp)

if (PYTHONINTERP_FOUND)
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
