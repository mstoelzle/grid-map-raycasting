cmake_minimum_required(VERSION 3.12)
project(grid_map_raycasting)

# Find packages **************************************************************************************************
find_package(Eigen3 REQUIRED)
find_package(pybind11 REQUIRED)
find_package(raisim CONFIG REQUIRED)

# Include **************************************************************************************************
include_directories(${EIGEN3_INCLUDE_DIRS} )
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${pybind11_INCLUDE_DIR})

pybind11_add_module(grid_map_raycasting src/main.cpp)
target_link_libraries(grid_map_raycasting PUBLIC raisim::raisim pthread ${OpenCV_LIBS})

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(grid_map_raycasting PRIVATE VERSION_INFO=${GRID_MAP_RAYCASTING_VERSION_INFO})
