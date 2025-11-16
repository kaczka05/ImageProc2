//
// Created by HPOMEN on 16.11.2025.
//

#ifndef PERF_H
#define PERF_H



#include <cstddef>
#include <cstdint>


// Simple performance counters (globals for easy use, in the simple style requested)
namespace perf {
    extern uint64_t additions;
    extern uint64_t multiplications;
    extern size_t extra_memory_bytes; // estimated additional memory used during filtering


    void reset();
    void report();
    void add_memory(size_t bytes);
}



#endif //PERF_H
