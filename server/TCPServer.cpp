#include "TCPServer.h"

TCPServer::TCPServer(int port) {
    this->port = port;

    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->socket_fd < 0) {
        throw std::runtime_error("Socket error");
    }

    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(this->port);
}

TCPServer::TCPServer(const TCPServer& other) {
    this->address = other.address;
    this->port = other.port;

    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->socket_fd < 0) {
        throw std::runtime_error("Socket error");
    }
}

TCPServer::~TCPServer() {
    close(this->accept_fd);
    close(this->socket_fd);
}

void TCPServer::bind() {
    if (::bind(this->socket_fd, (sockaddr*)&this->address, sizeof(this->address)) < 0) {
        throw std::runtime_error("Socket Bind Error");
    };
}

void TCPServer::listen(int backlog) {
    if (::listen(this->socket_fd, backlog) < 0) {
        throw std::runtime_error("Socket Listen Error");
    }
}

void TCPServer::accept() {
    int addrlen = sizeof(this->address);
    this->accept_fd = ::accept(this->socket_fd, (sockaddr*)&this->address, (socklen_t*)&addrlen);

    if (this->accept_fd < 0) {
        throw std::runtime_error("Socket Accept Error");
    }
}

void TCPServer::send(const void* buffer, size_t len, int flags) {
    ::send(this->accept_fd, buffer, len, flags);
}

void TCPServer::read(void* buffer, size_t len) {
    ::read(this->accept_fd, buffer, len);
}