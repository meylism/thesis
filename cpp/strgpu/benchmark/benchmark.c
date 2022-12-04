#include "CL/cl.h"
#include "ubench.h"

#define GPU_PLATFORM 1
#define CPU_PLATFORM 0
UBENCH_STATE();

#include "opencl_helpers.h"

// Benchmarks - start
#include "b_add_number.h"
// Benchmarks - end


opencl_stub *stub;

opencl_stub* opencl_init() {
    PRNT("### INIT ###\n");
    opencl_stub *ltub = malloc(sizeof(opencl_stub));
    cl_int status;
    cl_uint numPlatforms, numDevices;

    status = clGetPlatformIDs(0, NULL, &numPlatforms);
    ltub->platforms = malloc(numPlatforms * sizeof(cl_platform_id));
    status = clGetPlatformIDs(numPlatforms, ltub->platforms, NULL);
    PRNT("INIT(%d): %d platforms have been found\n", status, numPlatforms);
    ltub->numPlatforms = numPlatforms;

    status = clGetDeviceIDs(ltub->platforms[GPU_PLATFORM], CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
    ltub->gpu = malloc(numDevices * sizeof(cl_device_id));
    status = clGetDeviceIDs(ltub->platforms[GPU_PLATFORM], CL_DEVICE_TYPE_GPU, numDevices, ltub->gpu, NULL);
    PRNT("INIT(%d): GPU has been selected\n", status);

    status = clGetDeviceIDs(ltub->platforms[CPU_PLATFORM], CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);
    ltub->cpu = malloc(numDevices * sizeof(cl_device_id));
    status = clGetDeviceIDs(ltub->platforms[CPU_PLATFORM], CL_DEVICE_TYPE_CPU, numDevices, ltub->cpu, NULL);
    PRNT("INIT(%d): CPU has been selected\n", status);

    return ltub;
}

void opencl_free(opencl_stub* lstub) {
    free(lstub->platforms);
    free(lstub->gpu);
    free(lstub->cpu);
}

int main(int argc, const char** argv) {
    stub = opencl_init();

    int bench_status = ubench_main(argc, argv);

    opencl_free(stub);

    return bench_status;
}