#include <iostream>
#include <cstring>
#include <stdexcept>
#include "TCPServer.h"
#include "UDPServer.h"

#define PORT 8080
#define MAXSIZE 1024

int runTCPServer(TCPServer& server);
int runUDPServer(UDPServer& server);
void processBuffer(char* buffer);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        throw std::runtime_error("Too few arguments");
    }

    char* protocol = argv[1];
    int port = argc > 2 ? atoi(argv[2]) : PORT;

    if (strcmp(protocol, "tcp") == 0) {
        TCPServer server(port);
        server.bind();
        printf("\033[32mServer started on port %d\033[0m\n\n", port);

        server.listen();
        printf("Listening for connections...\n\n");

        int res = 0;
        do {
            res = runTCPServer(server);
        } while(res != -1);
    }
    else if (strcmp(protocol, "udp") == 0) {
        UDPServer server(port);
        server.bind();
        printf("\033[32mServer started on port %d\033[0m\n\n", port);

        int res = 0;
        do {
            res = runUDPServer(server);
        } while(res != -1);
    }
    else {
        throw std::runtime_error("Invalid protocol");
    }
    
    return 0;
}

int runTCPServer(TCPServer& server) {
    server.accept();
    printf("\033[32mClient connected\033[0m\n\n");

    while (true) {
        char buffer[MAXSIZE] = {0};
        
        server.read(buffer, MAXSIZE);
        printf("Client: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            printf("\033[31mClient disconnected\033[0m\n\n");
            return 0;
        }
        else if (strcmp(buffer, "shutdown") == 0) {
            printf("Shutting down server...\n\n");
            return -1;
        }

        printf("Processing...");
        processBuffer(buffer);
        printf("\033[32mDone\033[0m\n");

        server.send(buffer, strlen(buffer));
        printf("Message \"%s\" sent\n\n", buffer);
    }
}

int runUDPServer(UDPServer& server) {
    while (true) {
        char buffer[MAXSIZE] = {0};
        server.read(buffer, MAXSIZE);
        printf("Client: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            printf("\033[31mClient disconnected\033[0m\n\n");
            return 0;
        }
        else if (strcmp(buffer, "shutdown") == 0) {
            printf("Shutting down server...\n\n");
            return -1;
        }

        printf("Processing...");
        processBuffer(buffer);
        printf("\033[32mDone\033[0m\n");

        server.send(buffer, strlen(buffer));
        printf("Message \"%s\" sent\n\n", buffer);
    }
}

void processBuffer(char* buffer) {
    for (int i = 0; i < strlen(buffer); i++) {
        if (isalpha(buffer[i]) == 0) {
            continue;
        }
        int ascii = (int)buffer[i];
        buffer[i] = i % 2 == 0 ? ascii + 4 : ascii - 4;
    }
}
