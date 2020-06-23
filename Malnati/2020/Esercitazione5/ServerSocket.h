//
// Created by rolud on 21/06/2020.
//

#ifndef STUDIES_SERVERSOCKET_H
#define STUDIES_SERVERSOCKET_H

#include <winsock2.h>

#include "Socket.h"
#pragma comment(lib, "Ws2_32.lib")

class ServerSocket : private Socket {
public:
    ServerSocket(int port) {
        struct sockaddr_in sockaddrIn;
        sockaddrIn.sin_port = ::htons((u_short)port);
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_addr.s_addr = ::htonl(INADDR_ANY);

        if (::bind(this->m_sockfd, reinterpret_cast<struct sockaddr*>(&sockaddrIn), sizeof(sockaddrIn)) != 0)
            throw std::runtime_error("Cannot bind port ");
        if (::listen(this->m_sockfd, 8) != 0)
            throw std::runtime_error("Cannot listen on port ");

    }

    Socket accept(struct sockaddr_in* addr, unsigned int* len) {
        SOCKET fd = ::accept(this->m_sockfd, reinterpret_cast<struct sockaddr*>(addr), (int*) len);
        if (fd < 0)
            throw std::runtime_error("Cannot accept socket");
        return Socket(fd);
    }
};


#endif //STUDIES_SERVERSOCKET_H
