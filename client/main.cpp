#include <iostream>
#include <cstring>
#include <list>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <chrono>
#include "TCPClient.h"
#include "UDPClient.h"

#define PORT 8080
#define MAXSIZE 1024
#define HISTORY_SIZE 10

std::mutex clientMtx, ioMtx;
std::list<char[MAXSIZE]> messages(HISTORY_SIZE);

void runTCPClient(TCPClient& client);
void runUDPClient(UDPClient& client);

void sendTCPMessage(TCPClient& client, bool& startupComplete);
void sendUDPMessage(UDPClient& client);

void readTCPResponse(TCPClient& client);
void readUDPResponse(UDPClient& client);

void setUsername(TCPClient& client);
void setUsername(UDPClient& client);

void render();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        throw std::runtime_error("Too few arguments");
    }

    char* protocol = argv[1];
    int port = argc > 2 ? atoi(argv[2]) : PORT;
    const char* target = argc > 3 ? argv[3] : "127.0.0.1";

    if (strcmp(protocol, "tcp") == 0) {
        TCPClient client(port);
        client.connect(target);
        printf("\033[32mConnected to %s:%d\033[0m\n\n", target, port);

        {
            std::unique_lock<std::mutex> clientLock(clientMtx);
            std::unique_lock<std::mutex> ioLock(ioMtx);
            setUsername(client);
        }

        runTCPClient(client);
    }
    else if (strcmp(protocol, "udp") == 0) {
        UDPClient client(port);
        printf("\033[32mConnected to %s:%d\033[0m\n\n", target, port);

        setUsername(client);

        runUDPClient(client);
    }
    else {
        throw std::runtime_error("Invalid protocol");
    }

    return 0;
}

void runTCPClient(TCPClient& client) {
    bool startupComplete = false;
    std::thread sendThread(sendTCPMessage, std::ref(client), std::ref(startupComplete));

    while (!startupComplete) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::thread readThread(readTCPResponse, std::ref(client));

    // TODO - Renderer Thread
    // Every x ms interval, clear console and rerender

    sendThread.join();
    readThread.detach();
}

void runUDPClient(UDPClient& client) {
    while (true) {
        std::string buffer = "";
        char response[MAXSIZE];

        printf("Enter message: ");
        std::cin.clear();
        std::getline(std::cin, buffer);

        client.send(buffer.c_str(), buffer.length());
        printf("Message sent\n");

        if (buffer == "exit") {
            printf("Exiting...\n");
            break;
        }
        else if (buffer == "shutdown") {
            printf("Shutting down server...\n\n");
            return;
        }

        client.read(response, MAXSIZE);
        printf("Response: %s\n\n", response);
    }
}

void sendTCPMessage(TCPClient& client, bool& startupComplete) {
    while (true) {
        std::string buffer = "";

        { // TODO - Move to renderer thread
            std::unique_lock<std::mutex> ioLock(ioMtx);
            printf("Enter message: ");
            std::cin.clear();
            std::getline(std::cin, buffer);
        }

        {
            std::unique_lock<std::mutex> clientLock(clientMtx);
            if (!startupComplete) startupComplete = true;
            client.send(buffer.c_str(), buffer.length());
        }

        {
            // TODO - Move to renderer thread
            // std::unique_lock<std::mutex> ioLock(ioMtx);
            if (buffer == "exit") {
                printf("Exiting...\n");
                return;
            }
            else if (buffer == "shutdown") {
                printf("Shutting down server...\n\n");
                return;
            }
        }
    }
}

void readTCPResponse(TCPClient& client) {
    while (true) {
        char response[MAXSIZE] = {0};

        {
            std::unique_lock<std::mutex> clientLock(clientMtx);
            client.read(response, MAXSIZE);
            messages.push_back(response);
            messages.pop_front();
        }

        { // TODO - Move to renderer thread
            std::unique_lock<std::mutex> ioLock(ioMtx);
            printf("Response: %s\n\n", response);
        }
    }
}

void setUsername(TCPClient& client) {
    do {
        std::string username = "";
        char ack[4];

        printf("Choose a Username: ");
        std::cin.clear();
        std::getline(std::cin, username);

        client.send(username.c_str(), username.length());

        client.read(ack, 4);
        if (strcmp(ack, "ACK") == 0) break;
        else if (strcmp(ack, "DAK") != 0) printf("Username already taken. Please try again.");
        else if (strcmp(ack, "NAK") != 0) printf("Invalid response. Please try again.");
        else printf("An error occurred. Please try again.");
    } while (true);
    
    printf("Username Set\n\n");
}

void setUsername(UDPClient& client) {
    do {
        std::string username = "";
        char ack[4];

        printf("Choose a Username: ");
        std::cin.clear();
        std::getline(std::cin, username);

        client.send(username.c_str(), username.length());

        client.read(ack, 4);
        if (strcmp(ack, "ACK") == 0) break;
        else if (strcmp(ack, "DAK") != 0) printf("Username already taken. Please try again.");
        else if (strcmp(ack, "NAK") != 0) printf("Invalid response. Please try again.");
        else printf("An error occurred. Please try again.");
    } while (true);
    
    printf("Username Set\n\n");
}

void render() {
    // TODO - Clear console and render messages
}