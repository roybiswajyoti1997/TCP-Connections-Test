#include "Connection.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

Connection::Connection() : socket_fd(-1) {}

Connection::~Connection() {
    closeConnection();
}

// ========== Client Side ==========
bool Connection::connectToServer(const std::string& ip, int port) {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        std::cerr << "[Connection] Failed to create socket.\n";
        return false;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "[Connection] Invalid IP address.\n";
        return false;
    }

    if (connect(socket_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "[Connection] Failed to connect to server.\n";
        return false;
    }

    return true;
}

// ========== Server Side ==========
bool Connection::bindAndListen(int port) {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        std::cerr << "[Connection] Failed to create socket.\n";
        return false;
    }

    int opt = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(socket_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "[Connection] Bind failed.\n";
        return false;
    }

    if (listen(socket_fd, 1) < 0) {
        std::cerr << "[Connection] Listen failed.\n";
        return false;
    }

    return true;
}

bool Connection::acceptConnection() {
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);

    client_fd = accept(socket_fd, (sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        std::cerr << "[Connection] Accept failed.\n";
        return false;
    }

    return true;
}

// ========== Shared ==========
ssize_t Connection::sendData(const void* buffer, size_t size) {
    return send(socket_fd, buffer, size, 0);
}

ssize_t Connection::receiveData(void* buffer, size_t size) {
    int fd = (client_fd != -1) ? client_fd : socket_fd;
    return recv(fd, buffer, size, 0);
}

void Connection::closeConnection() {
    if (client_fd != -1) {
        close(client_fd);
        client_fd = -1;
    }
    if (socket_fd != -1) {
        close(socket_fd);
        socket_fd = -1;
    }
}

