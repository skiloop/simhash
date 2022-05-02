cmake_minimum_required(VERSION 3.12)
project(simhash_cpp)

set(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(Python COMPONENTS Interpreter Development)

message(STATUS "Python_FOUND = ${Python_FOUND}")
message(STATUS "Python_Development_FOUND = ${Python_Development_FOUND}")

IF (NOT Python_FOUND)
    message(FATAL_ERROR "Python not found")
ENDIF ()
IF (NOT Python_Development_FOUND)
    message(FATAL_ERROR "Python development not found")
ENDIF ()
find_package(Boost COMPONENTS python REQUIRED)

if (NOT Boost_FOUND)
    message(FATAL_ERROR "boost-python not found")
endif ()

message(STATUS "Python_LIBRARY_DIRS = ${Python_LIBRARY_DIRS}")
message(STATUS "Python_EXECUTABLE = ${Python_EXECUTABLE}")
message(STATUS "Python_INCLUDE_DIRS = ${Python_INCLUDE_DIRS}")
message(STATUS "Python_LIBRARIES = ${Python_LIBRARIES}")
message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Python_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(src)
LINK_LIBRARIES(${Boost_LIBRARIES} ${Python_LIBRARIES})

add_executable(simhash_cpp src/SimHashBase.cpp src/main.cpp)
