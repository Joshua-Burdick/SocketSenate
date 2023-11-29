#include "TCPClient.h"

TCPClient::TCPClient(int port) {
    this->port = port;

    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->socket_fd < 0) {
        throw std::runtime_error("Socket error");
    }

    this->serverAddr.sin_family = AF_INET;
    this->serverAddr.sin_port = htons(this->port);
}

TCPClient::TCPClient(const TCPClient& other) {
    this->serverAddr = other.serverAddr;
    this->port = other.port;

    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->socket_fd < 0) {
        throw std::runtime_error("Socket error");
    }
}

TCPClient::~TCPClient() {
    close(this->socket_fd);
}

void TCPClient::connect(const char* target) {
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, target, &this->serverAddr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid address/ Address not supported");
    }

    // Connect to server
    if (::connect(this->socket_fd, (sockaddr*)&this->serverAddr, sizeof(this->serverAddr)) < 0) {
        throw std::runtime_error("Connection Failed");
    }
}

void TCPClient::send(const void* buffer, size_t len, int flags) {
    ::send(this->socket_fd, buffer, len, flags);
}

void TCPClient::read(void* buffer, size_t len) {
    ::read(this->socket_fd, buffer, len);
}