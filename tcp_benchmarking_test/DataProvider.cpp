#include "DataProvider.hpp"
#include <cstdlib>
#include <cstring>

DataProvider::DataProvider() : data(nullptr), currCount(0) {}

DataProvider::~DataProvider() {
    if (data) {
        free(data);
        data = nullptr;
    }
}

// Generate 'count' elements (each is uint64_t)
void DataProvider::generateDummyData(uint64_t count) {
    if (data) {
        free(data);  // Clean up previous data
    }

    data = (uint64_t*)malloc(count * sizeof(uint64_t));
    currCount = count;

    for (uint64_t i = 0; i < count; ++i) {
        data[i] = xorshift64();
    }
}

const void* DataProvider::getData() const {
    return static_cast<void*>(data);
}

uint64_t DataProvider::getSize() const {
    return currCount;
}

uint64_t DataProvider::xorshift64() {
    x64 ^= x64 << 13;
    x64 ^= x64 >> 7;
    x64 ^= x64 << 17;
    return x64;
}

