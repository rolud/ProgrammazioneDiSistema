cmake_minimum_required(VERSION 3.13)
project(Exams)

set(CMAKE_CXX_STANDARD 17)

add_executable(Exams main.cpp DurationLogger.h 2016/Exchanger.h tests.h functions.h nodate/GameStanding.h nodate/Phaser.h 2019/Executor.h 2019/Context.h nodate/Dispatcher.h)