#include <unistd.h>
#include <cstring>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <stdexcept>

class UDPServer {
private:
    int socket_fd;
    sockaddr_in servaddr, cliaddr;
    int port;
public:
    // Create server with port
    UDPServer(int port);
    UDPServer(const UDPServer& other);
    void operator=(const UDPServer& other);

    // Close socket
    ~UDPServer();
    
    // Bind socket
    void bind();

    // Listen and wait for connections
    void read(void* buffer, size_t size, int flags = MSG_WAITALL);

    // Send message
    void send(const void* buffer, size_t size, int flags = MSG_CONFIRM);
};