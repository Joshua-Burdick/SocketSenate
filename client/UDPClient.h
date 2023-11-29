#include <unistd.h>
#include <cstring>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <stdexcept>

class UDPClient {
private:
    int socket_fd;
    sockaddr_in servaddr;
    int port;
public:
    // Create Client
    UDPClient(int port);
    UDPClient(const UDPClient& other);
    void operator=(const UDPClient& other);

    // Close socket
    ~UDPClient();

    // Read from socket
    void read(void* buffer, size_t size, int flags = MSG_WAITALL);

    // Send to socket
    void send(const void* buffer, size_t size, int flags = MSG_CONFIRM);
};