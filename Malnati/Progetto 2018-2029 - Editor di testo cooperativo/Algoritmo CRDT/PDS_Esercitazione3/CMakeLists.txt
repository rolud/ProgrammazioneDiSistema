cmake_minimum_required(VERSION 3.13)
project(Esercitazione3)

set(CMAKE_CXX_STANDARD 17)

add_executable(Esercitazione3 main.cpp NetworkServer.cpp NetworkServer.h Message.cpp Message.h Symbol.cpp Symbol.h SharedEditor.cpp SharedEditor.h)