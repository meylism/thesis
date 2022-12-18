#ifndef STRGPU_B_STRING_SEARCH_NAIVE_H
#define STRGPU_B_STRING_SEARCH_NAIVE_H

#include "ubench.h"
#include "opencl_helpers.h"
#include "cl_utils.h"


UBENCH_EX(string_search, naive) {
    printf("\t########## Benchmark - String Search Naive ##########\n");
    cl_int status;
    cl_context context = clCreateContext(NULL, 1, stub->gpu, NULL, NULL, &status);
    PRNT("Context created(%d)\n", status);

    cl_command_queue cmdQueue = clCreateCommandQueue(context, *stub->gpu, 0, &status);
    PRNT("Queue created(%d)\n", status);

    int result_size = sizeof(int)*bench_data->data_size;
    int *result = (int*)malloc(result_size);

    cl_mem cl_data = clCreateBuffer(context, CL_MEM_READ_ONLY, bench_data->data_size, NULL, &status);
    cl_mem cl_pattern = clCreateBuffer(context, CL_MEM_READ_ONLY, bench_data->pattern_size, NULL, &status);
    cl_mem cl_result = clCreateBuffer(context, CL_MEM_READ_WRITE, bench_data->data_size*sizeof(int), NULL, &status);
    PRNT("Buffers created(%d)\n", status);

    status = clEnqueueWriteBuffer(cmdQueue, cl_data, CL_FALSE, 0, bench_data->data_size, bench_data->data, 0, NULL, NULL);
    status = clEnqueueWriteBuffer(cmdQueue, cl_pattern, CL_FALSE, 0, bench_data->pattern_size, bench_data->pattern, 0, NULL, NULL);
    status = clEnqueueWriteBuffer(cmdQueue, cl_result, CL_FALSE, 0, bench_data->data_size*sizeof(int), result, 0, NULL, NULL);
    PRNT("Buffers written(%d)\n", status);

    char* src = readProgramFile("../benchmark/b_string_search_naive.cl");

    cl_program program = clCreateProgramWithSource(context, 1, &src, NULL, &status);
    PRNT("Program created(%d)\n", status);

    status = clBuildProgram(program, 1, stub->gpu, NULL, NULL, NULL);
    PRNT("Program built(%d)\n", status);

    cl_kernel kernel = clCreateKernel(program, "search_naive", &status);
    PRNT("Kernel created(%d)\n", status);

    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_data);
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &cl_pattern);
    status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &cl_result);
    PRNT("Kernel's parameters set(%d)\n", status);

    size_t indexSpace[1], workGroupSize[1];
    indexSpace[0] = bench_data->data_len;
    workGroupSize[0] = 1;

    UBENCH_DO_BENCHMARK() {
        status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, indexSpace, workGroupSize, 0, NULL, NULL);
        PRNT("Queued kernel(%d)\n", status);

        status = clEnqueueReadBuffer(cmdQueue, cl_result, CL_TRUE, 0, result_size, result, 0, NULL, NULL);
        PRNT("Read resulting buffer(%d)\n", status);
    }
    int x = 0, i=0;
    while (i < bench_data->data_len) {
        i++;
        if (result[i] == 1)
            x++;
    }
    PRNT("Occurrences %d\n", x);


    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(cmdQueue);
    clReleaseMemObject(cl_result);
    clReleaseMemObject(cl_pattern);
    clReleaseMemObject(cl_data);
    clReleaseContext(context);

    free(result);

    PRNT("Freed resources\n");
}

#endif