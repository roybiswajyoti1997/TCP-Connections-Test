#include "Benchmark.hpp"
#include <iostream>

Benchmark& Benchmark::getInstance() {
    static Benchmark instance;
    return instance;
}

void Benchmark::runThroughput(Connection& conn, DataProvider& dp) {
    const void* buffer = dp.getData();
    const size_t size = dp.getSize() * sizeof(uint64_t);

    using clock = std::chrono::high_resolution_clock;
    auto start = clock::now();

    size_t totalBytesSent = 0;

    for (uint32_t i = 0; i < BENCH_ITERATIONS; ++i) {
        ssize_t sent = conn.sendData(buffer, size);
        if (sent < 0) {
            std::cerr << "[Benchmark] Failed to send data in iteration " << i << "\n";
            return;
        }
        totalBytesSent += sent;
    }

    auto end = clock::now();
    std::chrono::duration<double> elapsed = end - start;

    double mbSent = totalBytesSent / (1024.0 * 1024.0);
    double throughput = mbSent / elapsed.count();

    std::cout << "[Benchmark] Sent " << mbSent << " MB in " 
              << elapsed.count() << " sec → " 
              << throughput << " MB/s\n";
}

