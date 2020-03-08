# /**
#     @date 2020/03/08
# **/
project(dsdana CXX)

# CMake
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++1z -Wall")

# Pybind11
find_package(pybind11 REQUIRED)
set(PYBIND11_CPP_STANDARD "-std=c++11")

# Uninstallation
configure_file(cmake_uninstall.cmake.in cmake_uninstall.cmake IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND "${CMAKE_COMMAND}" -P cmake_uninstall.cmake)

find_package(anlcross)
find_package(ROOT)
include_directories(${ROOT_INCLUDE_DIRS})

# --------------------

# --------------------

# Build

add_executable(${PROJECT_NAME} main.cpp ApplyDatabase.cpp CoupleHit.cpp MergeAdjacent.cpp ReadDatabase.cpp ReadDatabaseText.cpp ReadEventtree.cpp WriteHittree.cpp WriteHittreeAll.cpp)
install(
  TARGETS dsdana
  DESTINATION bin
  LIBRARY DESTINATION lib
  INCLUDES DESTINATION include
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
  )
target_link_libraries(${PROJECT_NAME} anlcross ${ROOT_LIBRARIES} -lreadline)

# pybind11_add_module(${PROJECT_NAME}py main.cpp ApplyDatabase.cpp CoupleHit.cpp MergeAdjacent.cpp ReadDatabase.cpp ReadDatabaseText.cpp ReadEventtree.cpp WriteHittree.cpp WriteHittreeAll.cpp)
# install(
#   TARGETS ${PROJECT_NAME}py
#   DESTINATION lib
#   LIBRARY DESTINATION lib
#   INCLUDES DESTINATION include
#   PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}py
#   )

