cmake_minimum_required(VERSION 3.13)
project(Studies)

set(CMAKE_CXX_STANDARD 17)
link_libraries(ws2_32 wsock32)
add_executable(Studies main.cpp Buffer.cpp Buffer.h MyString.cpp MyString.h patternCRTP/Amount.h patternCRTP/Constant42.h patternCRTP/Variable.h smartPointer/smart_ptr.h smartPointer/DoublyLinkedList.h smartPointer/DoubnlyLinkedNode.h socket/Socket.h socket/ServerSocket.h socket/WinSockSingletonHelper.h)