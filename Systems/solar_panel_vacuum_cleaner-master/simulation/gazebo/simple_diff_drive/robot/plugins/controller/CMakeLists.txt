cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS} includes includes/framework includes/statemachines)

link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

file(GLOB CONTROLLER_SRC
     "*.cpp"
     "statemachines/*.cpp"
     "framework/*.cpp"
     "includes/*.hpp"
     "includes/framework/*.hpp"
     "includes/statemachines/*.hpp"
)

add_library(controller SHARED ${CONTROLLER_SRC})
target_link_libraries(controller ${GAZEBO_LIBRARIES})

get_property(_system_include_dirs TARGET controller PROPERTY INCLUDE_DIRECTORIES)
message("INCLUDE_DIRECTORIES: ${_system_include_dirs}")

message("CONTROLLER_SRC: ${CONTROLLER_SRC}")