cmake_minimum_required(VERSION 3.31.0)
project(untitled)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

add_executable(untitled main.cpp
        CImg.h
        funtions.cpp
        similarity.cpp
        histogram.cpp
        histogram.h
        reyleigh.cpp
        reyleigh.h
        lowpass_filter.cpp
        lowpass_filter.h
        perf.cpp
        perf.h
        LLFilter.cpp
        LLFilter.h
        regionGrowth.cpp
        regionGrowth.h
        morphology.cpp
        morphology.h
        MorphologicalM4.cpp
        MorphologicalM4.h
)
