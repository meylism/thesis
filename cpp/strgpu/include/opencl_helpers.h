//
// Created by meylism on 2022.12.02..
//

#ifndef STRGPU_OPENCL_HELPERS_H
#define STRGPU_OPENCL_HELPERS_H
#include "CL/cl.h"
#include "common.h"

typedef struct opencl_stub {
    cl_int numPlatforms;
    cl_platform_id *platforms;

    cl_device_id *gpu;
    cl_device_id *cpu;
} opencl_stub;

extern struct opencl_stub *stub;
#endif //STRGPU_OPENCL_HELPERS_H
