#ifndef MEM_CONNECT_DATA_PROVIDER_HPP_
#define MEM_CONNECT_DATA_PROVIDER_HPP_

#include <cstdint>
#include <cstddef>

class DataProvider {
public:
    DataProvider();
    ~DataProvider();

    void generateDummyData(uint64_t count);  // Number of uint64_t elements

    const void* getData() const;
    uint64_t getSize() const;

private:
    uint64_t* data;
    uint64_t currCount;

    uint64_t x64 = 88172645463325252ull;  // Seed for PRNG

    uint64_t xorshift64();  // Fast pseudo-random number generator
};

#endif  // MEM_CONNECT_DATA_PROVIDER_HPP_
