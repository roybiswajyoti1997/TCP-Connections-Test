#include "logger.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <chrono>
#include <netinet/in.h>
#include <arpa/inet.h>

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

std::string getWelcomeMessage() {
    return "Hello from server!";
}

int createAndBindServerSocket(int port, sockaddr_in& address, Logger& logger) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        logger.log("Socket creation failed", ERROR);
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        logger.log("Binding failed", ERROR);
        return -1;
    }

    return server_fd;
}

int main() {
    Logger logger("server.log", true);
    logger.log("Server is starting...", INFO);

    sockaddr_in server_addr{}, client_addr{};
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};

    int server_fd = createAndBindServerSocket(PORT, server_addr, logger);
    if (server_fd == -1) return 1;

    if (listen(server_fd, 5) < 0) {
        logger.log("Listen failed", ERROR);
        return 1;
    }
    logger.log("Server listening on port " + std::to_string(PORT), INFO);

    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (client_fd < 0) {
        logger.log("Accept failed", ERROR);
        return 1;
    }

    logger.log("Client connected from IP: " + std::string(inet_ntoa(client_addr.sin_addr)), INFO);
    auto start = std::chrono::high_resolution_clock::now();

    int bytes_read = read(client_fd, buffer, BUFFER_SIZE);
    if (bytes_read > 0) {
        std::string client_message(buffer, bytes_read);
        logger.log("Received: " + client_message, INFO);

        if (client_message == "Hello from client!") {
            logger.log("Functional Test Passed", INFO);
        } else {
            logger.log("Functional Test Failed", WARNING);
        }

        std::string response = getWelcomeMessage();
        send(client_fd, response.c_str(), response.length(), 0);
        logger.log("Sent response to client", INFO);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Interaction time: " << elapsed.count() << " seconds\n";

    close(client_fd);
    close(server_fd);
    logger.log("Server shutdown", INFO);
    return 0;
}
