#include <iostream>
#include <memory>
#include <unistd.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Buffer.h"
#include "patternCRTP/Amount.h"
#include "patternCRTP/Constant42.h"
#include "patternCRTP/Variable.h"
#include "smartPointer/DoublyLinkedList.h"
#include "socket/ServerSocket.h"
#include "socket/WinSockSingletonHelper.h"
#pragma comment(lib, "Ws2_32.lib")

std::weak_ptr<int> gw;

template <typename T>
void print(Amount<T> amount) {
    std::cout << amount.getValue() << std::endl;
}

void fooWP() {
    if (auto spt = gw.lock())
        std::cout << "gw: " << *spt << std::endl;
    else
        std::cout << "gw scaduto" << std::endl;
}

void patterCRTP() {
    Constant42 c42;
    Variable v;

    print(c42);
    print(v);
}

void test_shared_ptr() {
    {
        auto sp = std::make_shared<int>(42);
        gw = sp;
        fooWP();
    }
    fooWP();
}

void server() {

    WinSockSingletonHelper::init();

    ServerSocket ss(5000);

    while (true) {
        struct sockaddr_in addr;
        unsigned int len = sizeof(addr);
        std::cout << "Waiting for incoming connection..." << std::endl;
        Socket s = ss.accept(&addr, &len);
        char* name;
        if ((name = ::inet_ntoa(addr.sin_addr)) == nullptr)
            throw std::runtime_error("Cannot convert address");
        std::cout << "Got connection from " << name << ":" << ::ntohs(addr.sin_port) << std::endl;
        bool end = false;
        while (!end) {
            char buffer[1024];
            int size = (int) s.read(buffer, sizeof(buffer) - 1, 0);
            buffer[size] = '\0';
            std::string str(buffer);
             std::cout << "Received : " << str << std::endl;
            s.write(buffer, size, 0);
            if (str == "END" || size == 0) end = true;
        }
        std::cout << "Connection closed" << std::endl;
    }

}

int main() {

    server();

    return 0;
}