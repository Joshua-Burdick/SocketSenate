#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdexcept>

class TCPServer {
private:
    int socket_fd;
    int accept_fd;
    sockaddr_in address;
    int port;
public:
    TCPServer(int port);
    TCPServer(const TCPServer& other);
    ~TCPServer();

    void bind();

    void listen(int backlog = 3);

    void accept();

    void send(const void* buffer, size_t len, int flags = 0);
    void read(void* buffer, size_t len);
};