#include "Logger.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <chrono>
#include <netinet/in.h>
#include <arpa/inet.h>

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;
const std::string SERVER_IP = "141.76.47.8";

std::string generateMessage() {
    return "Hello from client!";
}

int main() {
    Logger logger("client.log", true);
    logger.log("Client is starting...", INFO);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        logger.log("Socket creation failed", ERROR);
        return 1;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP.c_str(), &serv_addr.sin_addr) <= 0) {
        logger.log("Invalid address", ERROR);
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        logger.log("Connection failed", ERROR);
        return 1;
    }
    auto end = std::chrono::high_resolution_clock::now();
    logger.log("Connected to server", INFO);

    std::string msg = generateMessage();
    send(sock, msg.c_str(), msg.length(), 0);
    logger.log("Message sent", INFO);

    char buffer[BUFFER_SIZE] = {0};
    int bytes = read(sock, buffer, BUFFER_SIZE);
    if (bytes > 0) {
        std::string response(buffer, bytes);
        logger.log("Received: " + response, INFO);
    } else {
        logger.log("Failed to receive response", WARNING);
    }

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Connection setup time: " << elapsed.count() << " seconds\n";

    close(sock);
    logger.log("Client shutdown", INFO);
    return 0;
}
