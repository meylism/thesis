//
// Created by meylism on 2022.12.02..
//
#ifndef STRGPU_B_ADD_NUMBER_H
#define STRGPU_B_ADD_NUMBER_H
#include "ubench.h"
#include "opencl_helpers.h"
#include "cl_utils.h"


struct data {
    size_t size;
    int elements;
    int *a;
    int *b;
    int *sum;
};

UBENCH_F_SETUP(data) {
    PRNT("Setting up data\n");
    const int elements = 1024*1024*64;
    size_t size = sizeof(int) * elements;

    ubench_fixture->elements = elements;
    ubench_fixture->size = size;
    ubench_fixture->a = malloc(size);
    ubench_fixture->b = malloc(size);
    ubench_fixture->sum = malloc(size);

    for (int i=0; i<elements; i++) {
        ubench_fixture->a[i] = i+1;
        ubench_fixture->b[i] = i+1;
    }
}

UBENCH_F_TEARDOWN(data) {
    free(ubench_fixture->a);
    free(ubench_fixture->b);
    free(ubench_fixture->sum);
    PRNT("Freed benchmark data\n");
}

UBENCH_EX_F(data, add_number_gpu) {
    cl_int status;
    cl_context context = clCreateContext(NULL, 1, stub->gpu, NULL, NULL, &status);
    PRNT("Context created(%d)\n", status);

//    cl_command_queue cmdQueue = clCreateCommandQueueWithProperties(context, *stub->cpu, 0, &status);
    cl_command_queue cmdQueue = clCreateCommandQueue(context, *stub->gpu, 0, &status);
    PRNT("Queue created(%d)\n", status);

    cl_mem bufA = clCreateBuffer(context, CL_MEM_READ_ONLY, ubench_fixture->size, NULL, &status);
    cl_mem bufB = clCreateBuffer(context, CL_MEM_READ_ONLY, ubench_fixture->size, NULL, &status);
    cl_mem bufC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, ubench_fixture->size, NULL, &status);
    PRNT("Buffers created(%d)\n", status);

    status = clEnqueueWriteBuffer(cmdQueue, bufA, CL_FALSE, 0, ubench_fixture->size, ubench_fixture->a, 0, NULL, NULL);
    status = clEnqueueWriteBuffer(cmdQueue, bufB, CL_FALSE, 0, ubench_fixture->size, ubench_fixture->b, 0, NULL, NULL);
    PRNT("Buffers written(%d)\n", status);

    char* src = readProgramFile("../benchmark/b_add_number.cl");

    cl_program program = clCreateProgramWithSource(context, 1, &src, NULL, &status);
    PRNT("Program created(%d)\n", status);

    status = clBuildProgram(program, 1, stub->gpu, NULL, NULL, NULL);
    PRNT("Program built(%d)\n", status);

    cl_kernel kernel = clCreateKernel(program, "vecadd", &status);
    PRNT("Kernel created(%d)\n", status);

    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
    status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufC);
    PRNT("Kernel's parameters set(%d)\n", status);

    size_t indexSpace[1], workGroupSize[1];
    indexSpace[0] = ubench_fixture->elements;
    workGroupSize[0] = 256;

    UBENCH_DO_BENCHMARK() {
        status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, indexSpace, workGroupSize, 0, NULL, NULL);
        PRNT("Queued kernel(%d)\n", status);

        status = clEnqueueReadBuffer(cmdQueue, bufC, CL_TRUE, 0, ubench_fixture->size, ubench_fixture->sum, 0, NULL, NULL);
        PRNT("Read resulting buffer(%d)\n", status);
    }

    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(cmdQueue);
    clReleaseMemObject(bufA);
    clReleaseMemObject(bufB);
    clReleaseMemObject(bufC);
    clReleaseContext(context);
    PRNT("Freed Opencl resources\n");
}
#endif //STRGPU_B_ADD_NUMBER_H
