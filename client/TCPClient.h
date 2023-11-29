#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdexcept>

class TCPClient {
private:
    int socket_fd;
    sockaddr_in serverAddr;
    int port;
public:
    TCPClient(int port);
    TCPClient(const TCPClient& other);
    ~TCPClient();

    void connect(const char* target);

    void send(const void* buffer, size_t len, int flags = 0);
    void read(void* buffer, size_t len);
};