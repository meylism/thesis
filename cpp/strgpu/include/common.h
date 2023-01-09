#define CL_TARGET_OPENCL_VERSION 110

// defaults to PLATFORM 0
#ifndef PLATFORM
#define PLATFORM 0
#endif

// defaults to DEVICE 0
#ifndef DEVICE
#define DEVICE 0
#endif

#ifdef __APPLE__
    #include "OpenCL/cl.h"
#else
    #include "CL/cl.h"
#endif

#ifdef DEBUG
#define PRNT(...) fprintf(stderr, __VA_ARGS__)
#else
#define PRNT(...) do{} while (1 == 2)
#endif