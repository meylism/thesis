#include "stdio.h"
#include "common.h"

int add(int platform) {
    const char* programSource =
"__kernel void vecadd(__global int *A, __global int *B, __global int *C)"
"{\n"
"int idx = get_global_id(0);\n"
"C[idx] = A[idx] + B[idx];\n"
"}";

    printf("Device %d, Platform %d\n", DEVICE, platform);
    const int elements = 50000000;
    size_t datasize = sizeof(int)*elements;

    int *A = malloc(datasize);
    int *B = malloc(datasize);
    int *C = malloc(datasize);
    
    for (int i=0; i<elements; i++) {
        A[i] = i+1;
        B[i] = i+1;
    }

    cl_int status;
    cl_uint numPlatforms, numDevices;
    cl_platform_id *platforms;
    cl_device_id *devices;

    status = clGetPlatformIDs(0, NULL, &numPlatforms);
    platforms = malloc(numPlatforms*sizeof(cl_platform_id));
    status = clGetPlatformIDs(numPlatforms, platforms, NULL);
    PRNT("%d\n", status);

    status = clGetDeviceIDs(platforms[platform], CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
    devices = malloc(numDevices*sizeof(cl_device_id));
    status = clGetDeviceIDs(platforms[platform], CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);
    PRNT("%d\n", status);


    cl_context context = clCreateContext(NULL, 1, devices, NULL, NULL, &status);
    PRNT("%d\n", status);

    cl_command_queue cmdQueue = clCreateCommandQueueWithProperties(context, devices[DEVICE], 0, &status);
    PRNT("%d\n", status);

    cl_mem bufA = clCreateBuffer(context, CL_MEM_READ_ONLY, datasize, NULL, &status);
    cl_mem bufB = clCreateBuffer(context, CL_MEM_READ_ONLY, datasize, NULL, &status);
    cl_mem bufC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, datasize, NULL, &status);

    status = clEnqueueWriteBuffer(cmdQueue, bufA, CL_FALSE, 0, datasize, A, 0, NULL, NULL);
    status = clEnqueueWriteBuffer(cmdQueue, bufB, CL_FALSE, 0, datasize, B, 0, NULL, NULL);
    
    cl_program program = clCreateProgramWithSource(context, 1, &programSource, NULL, &status);

    status = clBuildProgram(program, numDevices, devices, NULL, NULL, NULL);
    cl_kernel kernel = clCreateKernel(program, "vecadd", &status);

    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
    status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufC);

    size_t indexSpace[1], workGroupSize[1];
    indexSpace[0] = elements;
    workGroupSize[0] = 256;

    status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, indexSpace, workGroupSize, 0, NULL, NULL);

    status = clEnqueueReadBuffer(cmdQueue, bufC, CL_TRUE, 0, datasize, C, 0, NULL, NULL);

    // for (int i=0; i<elements; i++) {
    //     printf("%d ", C[i]);
    // }

    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(cmdQueue);
    clReleaseMemObject(bufA);
    clReleaseMemObject(bufB);
    clReleaseMemObject(bufC);
    clReleaseContext(context);

    free(A);
    free(B);
    free(C);

    return 0;
}