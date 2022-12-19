#ifndef STRGPU_OPENCL_HELPERS_H
#define STRGPU_OPENCL_HELPERS_H
#include "common.h"

typedef struct opencl_stub {
    cl_int numPlatforms;
    cl_platform_id *platforms;

    cl_device_id *gpu;
    cl_device_id *cpu;
} opencl_stub;

typedef struct benchmark_data {
    char *data;
    long data_len;
    long data_size;
    // pattern being looked for
    char *pattern;
    long pattern_len;
    long pattern_size;
    int *offsets; // end incides
    int *start_indices;
//    char** dataByLine;
    long numLines;

} benchmark_data;

typedef struct configuration {
    cl_device_id *device;
    size_t localSize[1];
} configuration;

extern struct opencl_stub *stub;
extern struct benchmark_data *bench_data;
extern struct configuration *conf;
#endif //STRGPU_OPENCL_HELPERS_H
