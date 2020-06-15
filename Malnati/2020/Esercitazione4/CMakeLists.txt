cmake_minimum_required(VERSION 3.13)
project(Esercitazione4)

set(CMAKE_CXX_STANDARD 17)

add_executable(Esercitazione4 main.cpp Jobs.h Line.h DurationLogger.h Result.h functions.h)

target_link_libraries(Esercitazione4 stdc++fs)