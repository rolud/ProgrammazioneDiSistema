cmake_minimum_required(VERSION 3.13)
project(Esercitazione1)

set(CMAKE_CXX_STANDARD 17)

add_executable(Esercitazione1 main.cpp Message.h DurationLogger.h Message.cpp MessageStore.cpp MessageStore.h)