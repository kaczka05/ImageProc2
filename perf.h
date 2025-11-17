//
// Created by HPOMEN on 16.11.2025.
//

#ifndef PERF_H
#define PERF_H


#include <chrono>
#include <cstddef>
#include <cstdint>



namespace perf {
    extern uint64_t additions;
    extern uint64_t multiplications;
    extern size_t extra_memory_bytes;

    extern uint64_t elapsed_nanoseconds;
    extern std::chrono::steady_clock::time_point start_time;

    void reset();
    void report();
    void add_memory(size_t bytes);
    void stop();
}



#endif //PERF_H
