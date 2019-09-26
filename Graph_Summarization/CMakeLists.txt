project(SWeG)
# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.8)

#SET(CMAKE_CXX_FLAGS "-std=c++11 -O3 -Wall")
#SET(CMAKE_CXX_FLAGS "-std=c++11 -O3 -w -pthread")
SET(CMAKE_CXX_FLAGS "-I/usr/local/include -L/usr/local/lib -std=c++11 -O3 -DNDEBUG -w -pthread -lboost_serialization -lboost_filesystem -lboost_system")


#find_package(Threads)
find_package(Boost REQUIRED  unit_test_framework filesystem system date_time program_options)

#message(STATUS "Boost_FOUND: ${Boost_FOUND}")
#message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
#message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
#message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")


set(HEADER utility.hpp config.hpp algo.hpp graph.hpp)
set(LIB utility.cpp config.cpp algo.cpp graph.cpp)

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}")


#add_library(mylib ${PROJECT_SOURCE_DIR}/mylib.cpp ${PROJECT_SOURCE_DIR}/mylib.h)
#add_library(config ${PROJECT_SOURCE_DIR}/config.cpp ${PROJECT_SOURCE_DIR}/config.h)


#add_executable(sweg ${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/utility.cpp ${PROJECT_SOURCE_DIR}/config.cpp ${PROJECT_SOURCE_DIR}/algo.cpp)

add_executable(sweg main.cpp ${LIB} ${HEADER})

target_link_libraries(sweg  boost_serialization boost_filesystem boost_system)

