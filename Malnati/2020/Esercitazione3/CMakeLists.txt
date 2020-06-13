cmake_minimum_required(VERSION 3.16)
project(Esercitazione3)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.66.0 COMPONENTS system filesystem)

add_executable(Esercitazione3 main.cpp MapperInputT.h ResultT.h ReducerInputT.h)

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(Esercitazione3 ${Boost_LIBRARIES})