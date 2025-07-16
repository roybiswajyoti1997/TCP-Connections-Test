#include <iostream>
#include <vector>
#include "Connection.hpp"

int main() {
    const int port = 8080;

    Connection serverConn;
    if (!serverConn.bindAndListen(port)) {
        std::cerr << "[Server] Failed to bind and listen on port " << port << "\n";
        return 1;
    }

    std::cout << "[Server] Waiting for client connection...\n";
    if (!serverConn.acceptConnection()) {
        std::cerr << "[Server] Failed to accept connection.\n";
        return 1;
    }

    std::cout << "[Server] Client connected. Receiving data...\n";

    const size_t bufferSize = 1024 * 1024;  // 1MB buffer
    std::vector<char> buffer(bufferSize);
    size_t totalReceived = 0;

    for (int i = 0; i < 10; ++i) {
        ssize_t received = serverConn.receiveData(buffer.data(), bufferSize);
        if (received <= 0) {
            std::cerr << "[Server] Error or disconnect.\n";
            break;
        }
        totalReceived += received;
        std::cout << "[Server] Received " << received << " bytes in iteration " << (i + 1) << "\n";
    }

    std::cout << "[Server] Total received: " << totalReceived / (1024 * 1024.0) << " MB\n";
    serverConn.closeConnection();
    return 0;
}

