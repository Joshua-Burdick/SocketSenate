#include "UDPServer.h"

UDPServer::UDPServer(int port) {
    this->port = port;

    this->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socket_fd < 0) {
        throw std::runtime_error("Socket Creation Error");
    }

    memset(&this->servaddr, 0, sizeof(this->servaddr)); 
	memset(&this->cliaddr, 0, sizeof(this->cliaddr));

    this->servaddr.sin_family = AF_INET;
	this->servaddr.sin_addr.s_addr = INADDR_ANY; 
	this->servaddr.sin_port = htons(this->port);
}

UDPServer::UDPServer(const UDPServer& other) {
    this->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socket_fd < 0) {
        throw std::runtime_error("Socket Creation Error");
    }

    this->servaddr = other.servaddr;
    this->cliaddr = other.cliaddr;
    this->port = other.port;
}

UDPServer::~UDPServer() {
    close(this->socket_fd);
}

void UDPServer::operator=(const UDPServer& other) {
    this->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socket_fd < 0) {
        throw std::runtime_error("Socket Creation Error");
    }

    this->servaddr = other.servaddr;
    this->cliaddr = other.cliaddr;
    this->port = other.port;
}

void UDPServer::bind() {
    if (::bind(this->socket_fd, (const sockaddr*)&this->servaddr, sizeof(this->servaddr)) < 0) {
        throw std::runtime_error("Socket Bind Error");
    }
}

void UDPServer::read(void* buffer, size_t size, int flags) {
    socklen_t len = sizeof(this->cliaddr);
    int numWritten = recvfrom(this->socket_fd, buffer, size, flags, (sockaddr*)&this->cliaddr, &len);
    ((char*)buffer)[numWritten] = '\0';
}

void UDPServer::send(const void* buffer, size_t size, int flags) {
    socklen_t len = sizeof(this->cliaddr);
    sendto(this->socket_fd, buffer, size, flags, (const sockaddr*)&this->cliaddr, len);
}