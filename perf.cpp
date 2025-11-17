//
// Created by HPOMEN on 16.11.2025.
//

#include "perf.h"
#include <iostream>


namespace perf {
    uint64_t additions = 0;
    uint64_t multiplications = 0;
    size_t extra_memory_bytes = 0;
    uint64_t elapsed_nanoseconds = 0;
    std::chrono::steady_clock::time_point start_time;


    void reset() {
        additions = 0;
        multiplications = 0;
        extra_memory_bytes = 0;
        start_time = std::chrono::steady_clock::now();
    }

    void stop() {
        auto now = std::chrono::steady_clock::now();
        elapsed_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time).count();
    }


    void report() {
        std::cout << "\n--- PERFORMANCE REPORT ---\n";
        std::cout << "Additions: " << additions << "\n";
        std::cout << "Multiplications: " << multiplications << "\n";
        std::cout << "Estimated extra memory (bytes): " << extra_memory_bytes << "\n";
        std::cout << "Elapsed time: " << elapsed_nanoseconds << " ns " << (elapsed_nanoseconds / 1e9) << " s";
        std::cout << "--------------------------\n";
    }


    void add_memory(size_t bytes) {
        extra_memory_bytes += bytes;
    }
}