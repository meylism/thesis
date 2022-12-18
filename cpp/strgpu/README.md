# Benchmarks for OpenCL-Based String Search Algorithms

The repository contains benchmarks for string search algorithms written in OpenCL. Benchmarks are built upon [ubench](https://github.com/sheredom/ubench.h) framework.

## Instructions

The project uses Cmake as a build system.

Make sure you have:

- Cmake
- OpenCL [ICD](https://registry.khronos.org/OpenCL/specs/2.2/html/OpenCL_ICD_Installation.html)(Installabe Client Driver) - is an attempt to unify access of different OpenCL implementations
- a GPU and an OpenCL driver for it

Then:

1. `cmake -S . -B build`
2. `cmake --build build`
3. Then run the executable in `./build/benchmark`
    - Example run: `./build/benchmark gharchive1000.json "great" --confidence=10%`
    - Refer to [this](https://sphweb.bumc.bu.edu/otlt/MPH-Modules/BS/BS704_Confidence_Intervals/BS704_Confidence_Intervals_print.html) or [ubench](https://github.com/sheredom/ubench.h) to understand confidence level
    - Define `-DDEBUG` to run the executable in debugging mode
