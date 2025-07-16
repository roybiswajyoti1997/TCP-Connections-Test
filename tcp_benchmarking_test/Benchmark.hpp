#ifndef MEM_CONNECT_BENCHMARK_HPP_
#define MEM_CONNECT_BENCHMARK_HPP_

#include "Connection.hpp"
#include "DataProvider.hpp"
#include <chrono>
#include <cstdint>
#include <cstddef>

class Benchmark {
public:
    static Benchmark& getInstance();  // Singleton pattern

    void runThroughput(Connection& conn, DataProvider& dp);

private:
    Benchmark() = default;
    Benchmark(const Benchmark&) = delete;
    Benchmark& operator=(const Benchmark&) = delete;

    const uint32_t BENCH_ITERATIONS = 10;
};

#endif  // MEM_CONNECT_BENCHMARK_HPP_

