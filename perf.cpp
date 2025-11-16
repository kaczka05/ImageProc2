//
// Created by HPOMEN on 16.11.2025.
//

#include "perf.h"
#include <iostream>


namespace perf {
    uint64_t additions = 0;
    uint64_t multiplications = 0;
    size_t extra_memory_bytes = 0;


    void reset() {
        additions = 0;
        multiplications = 0;
        extra_memory_bytes = 0;
    }


    void report() {
        std::cout << "\n--- PERFORMANCE REPORT ---\n";
        std::cout << "Additions: " << additions << "\n";
        std::cout << "Multiplications: " << multiplications << "\n";
        std::cout << "Estimated extra memory (bytes): " << extra_memory_bytes << "\n";
        std::cout << "--------------------------\n";
    }


    void add_memory(size_t bytes) {
        extra_memory_bytes += bytes;
    }
}