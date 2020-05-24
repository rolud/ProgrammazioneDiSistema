cmake_minimum_required(VERSION 3.13)
project(Esercitazione4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)
find_package(Threads)

add_executable(Esercitazione4 main.cpp JobScheduler.cpp JobScheduler.h Job.cpp Job.h)