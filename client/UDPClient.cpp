#include "UDPClient.h"

UDPClient::UDPClient(int port) {
    this->port = port;

    this->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (this->socket_fd < 0) {
        throw std::runtime_error("Socket Creation Error");
    }

    memset(&this->servaddr, 0, sizeof(this->servaddr));

    this->servaddr.sin_family = AF_INET;
    this->servaddr.sin_addr.s_addr = INADDR_ANY;
    this->servaddr.sin_port = htons(this->port);
}

UDPClient::UDPClient(const UDPClient& other) {
    this->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (this->socket_fd < 0) {
        throw std::runtime_error("Socket Creation Error");
    }

    this->servaddr = other.servaddr;
    this->port = other.port;
}

void UDPClient::operator=(const UDPClient& other) {
    this->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (this->socket_fd < 0) {
        throw std::runtime_error("Socket Creation Error");
    }

    this->servaddr = other.servaddr;
    this->port = other.port;
}

UDPClient::~UDPClient() {
    close(this->socket_fd);
}

void UDPClient::read(void* buffer, size_t size, int flags) {
    socklen_t len = sizeof(this->servaddr);
    int numWritten = recvfrom(this->socket_fd, buffer, size, flags, (sockaddr*) &this->servaddr, &len);
    ((char*)buffer)[numWritten] = '\0';
}

void UDPClient::send(const void* buffer, size_t size, int flags) {
    sendto(this->socket_fd, buffer, size, flags, (const sockaddr*) &this->servaddr, sizeof(this->servaddr));
}