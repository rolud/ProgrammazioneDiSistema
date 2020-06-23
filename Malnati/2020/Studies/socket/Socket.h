//
// Created by rolud on 21/06/2020.
//

#ifndef STUDIES_SOCKET_H
#define STUDIES_SOCKET_H


#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

class Socket {

public:

    Socket(const Socket&) = delete;
    Socket&operator=(const Socket&) = delete;

    Socket() {
        this->m_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (this->m_sockfd < 0)
            throw std::runtime_error("Cannot create socket");
        std::cout << "Socket " << this->m_sockfd << " created" << std::endl;
    }

    Socket(Socket& other) : m_sockfd(other.m_sockfd) {
        other.m_sockfd = 0;
    }

    ~Socket() {
        if (this->m_sockfd != 0) {
            std::cout << "Socket " << this->m_sockfd << " closed" << std::endl;
            ::close((int)this->m_sockfd);
        }
    }

    Socket&operator=(Socket&& other) {
        if (this->m_sockfd != 0) close((int)this->m_sockfd);
        this->m_sockfd = other.m_sockfd;
        other.m_sockfd = 0;
        return *this;
    }

    ssize_t read(char *buffer, size_t len, int options) {
        ssize_t  res = ::recv(this->m_sockfd, buffer, (int)len, options);
        if (res < 0) throw std::runtime_error("Cannot receive from socket");
        return res;
    }

    ssize_t write(char *buffer, size_t len, int options) {
        ssize_t  res = ::send(this->m_sockfd, buffer, (int)len, options);
        if (res < 0) throw std::runtime_error("Cannot write to socket");
        return res;
    }

    void connect(struct sockaddr_t *addr, unsigned int len) {
        if (::connect(this->m_sockfd, reinterpret_cast<struct sockaddr*>(addr), len) != 0)
            throw std::runtime_error("Cannot connect to remote socket");
    }


private:
    SOCKET m_sockfd;

    explicit Socket(SOCKET sockfd) : m_sockfd(sockfd) {
        std::cout << "Socket " << sockfd << " created" << std::endl;
    }

    friend class ServerSocket;
};


#endif //STUDIES_SOCKET_H
