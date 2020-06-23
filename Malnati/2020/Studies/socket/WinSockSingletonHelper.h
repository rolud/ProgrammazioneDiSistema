//
// Created by rolud on 21/06/2020.
//

#ifndef STUDIES_WINSOCKSINGLETONHELPER_H
#define STUDIES_WINSOCKSINGLETONHELPER_H

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <bits/exception.h>
#include <sstream>
#include <stdexcept>

#pragma comment(lib, "Ws2_32.lib")

class WinSockSingletonHelper {

public:
    static void init() {
        if (instance == nullptr)
            instance = new WinSockSingletonHelper();
    }

protected:
    static WinSockSingletonHelper* instance;

private:
    WinSockSingletonHelper() {
        WSADATA info;
        int wsas_r = WSAStartup(MAKEWORD(2,2), &info);
        if (wsas_r != 0) {
            std::stringstream ss;
            ss << "Error initializing ws2 with error " << wsas_r;
            throw std::runtime_error(ss.str());
        }
    }
    ~WinSockSingletonHelper() {
        WSACleanup();
        delete instance;
    }

};

WinSockSingletonHelper* WinSockSingletonHelper::instance = nullptr;

#endif //STUDIES_WINSOCKSINGLETONHELPER_H
