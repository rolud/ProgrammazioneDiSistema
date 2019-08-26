cmake_minimum_required(VERSION 3.13)
project(Esercitazione5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Charts REQUIRED)

list(APPEND LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Charts)
list(REMOVE_DUPLICATES LIBRARIES)

add_executable(Esercitazione5 main.cpp DiskStatusBar.cpp DiskStatusBar.h SearchBar.cpp SearchBar.h FileSystemModel.cpp FileSystemModel.h DirEntry.cpp DirEntry.h)
target_link_libraries(Esercitazione5 ${LIBRARIES})