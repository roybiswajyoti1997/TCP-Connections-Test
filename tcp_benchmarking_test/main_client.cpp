#include <iostream>
#include "Connection.hpp"
#include "DataProvider.hpp"
#include "Benchmark.hpp"

int main() {
    const std::string serverIP = "141.76.47.8";  // change to localhost for local test
    const int port = 8080;

    Connection clientConn;
    if (!clientConn.connectToServer(serverIP, port)) {
        std::cerr << "[Client] Failed to connect to server.\n";
        return 1;
    }

    // Generate 1MB dummy data
    DataProvider dp;
    dp.generateDummyData(1024 * 1024 / sizeof(uint64_t));  // 1MB = 1M bytes = (1M / 8) uint64_t

    // Run throughput benchmark
    Benchmark& bench = Benchmark::getInstance();
    bench.runThroughput(clientConn, dp);

    clientConn.closeConnection();
    return 0;
}

