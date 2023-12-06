#!/bin/bash

# Compile the program
cd server
g++ -std=c++11 -o ../executables/socketServer main.cpp TCPServer.cpp UDPServer.cpp

cd ../client
g++ -std=c++11 -pthread -o ../executables/socketClient main.cpp TCPClient.cpp UDPClient.cpp