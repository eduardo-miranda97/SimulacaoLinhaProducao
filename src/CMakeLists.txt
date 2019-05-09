cmake_minimum_required(VERSION 3.5)

project(Simulation_DUDU)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake") # CMAKE_CURRENT_LIST_DIR is the directory holding the CMakeLists.txt we are currently using
set(SOURCE_FILES artesao.cpp especialista.cpp TAD.cpp vaso.cpp)

add_library(Simulation_DUDU ${SOURCE_FILES})

include(FindPkgConfig)
find_package(Simulation_DUDU REQUIRED)
include_directories(${Simulation_DUDU_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${Simulation_DUDU_LIBRARY})