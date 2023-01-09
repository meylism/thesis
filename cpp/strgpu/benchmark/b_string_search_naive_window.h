#ifndef STRGPU_B_STRING_SEARCH_NAIVE_WINDOW_H
#define STRGPU_B_STRING_SEARCH_NAIVE_WINDOW_H

#include "ubench.h"
#include "opencl_helpers.h"
#include "cl_utils.h"


UBENCH_EX(string_search, naive_newline_window) {
    printf("\t########## Benchmark - String Search Naive Window ##########\n");
    cl_int status;
    cl_context context = clCreateContext(NULL, 1, conf->device, NULL, NULL, &status);
    PRNT("Context created(%d)\n", status);

    cl_command_queue cmdQueue = clCreateCommandQueue(context, *conf->device, 0, &status);
    PRNT("Queue created(%d)\n", status);

    int result_size = sizeof(int)*bench_data->numLines;
    int offset_size = result_size;
    int *result = (int*)malloc(result_size);
     

    cl_mem cl_data = clCreateBuffer(context, CL_MEM_READ_ONLY, bench_data->data_size, NULL, &status);
    cl_mem cl_pattern = clCreateBuffer(context, CL_MEM_READ_ONLY, 8, NULL, &status);
    cl_mem cl_result = clCreateBuffer(context, CL_MEM_READ_WRITE, result_size, NULL, &status);
    cl_mem cl_offsets = clCreateBuffer(context, CL_MEM_READ_ONLY, offset_size, NULL, &status);
    PRNT("Buffers created(%d)\n", status);

    status = clEnqueueWriteBuffer(cmdQueue, cl_data, CL_FALSE, 0, bench_data->data_size, bench_data->data, 0, NULL, NULL);
    PRNT("Buffers written1(%d)\n", status);
    status = clEnqueueWriteBuffer(cmdQueue, cl_pattern, CL_FALSE, 0, 8, "commcomm", 0, NULL, NULL);
    PRNT("Buffers written2(%d)\n", status);
    status = clEnqueueWriteBuffer(cmdQueue, cl_result, CL_FALSE, 0, result_size, result, 0, NULL, NULL);
    PRNT("Buffers written3(%d)\n", status);
    status = clEnqueueWriteBuffer(cmdQueue, cl_offsets, CL_FALSE, 0, offset_size, bench_data->offsets, 0, NULL, NULL);
    PRNT("Buffers written4(%d)\n", status);

    char* src = readProgramFile("../benchmark/b_string_search_naive_window.cl");

    cl_program program = clCreateProgramWithSource(context, 1, &src, NULL, &status);
    PRNT("Program created(%d)\n", status);
    printCompilerError(program, *conf->device);
    status = clBuildProgram(program, 1, conf->device, NULL, NULL, NULL);
    PRNT("Program built(%d)\n", status);
    printCompilerError(program, *conf->device);

    cl_kernel kernel = clCreateKernel(program, "search_naive_window", &status);
    PRNT("Kernel created(%d)\n", status);

    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_data);
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &cl_pattern);
    status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &bench_data->pattern_len);
    status = clSetKernelArg(kernel, 3, sizeof(cl_mem), &cl_result);
    status = clSetKernelArg(kernel, 4, sizeof(cl_mem), &cl_offsets);
    PRNT("Kernel's parameters set(%d)\n", status);

    size_t globalSize[1];
    globalSize[0] = bench_data->numLines;

    UBENCH_DO_BENCHMARK() {
        status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, globalSize, conf->localSize, 0, NULL, NULL);
        PRNT("Queued kernel(%d)\n", status);
        clFinish(cmdQueue);

        status = clEnqueueReadBuffer(cmdQueue, cl_result, CL_TRUE, 0, result_size, result, 0, NULL, NULL);
        PRNT("Read resulting buffer(%d)\n", status);
    }
    int x=0, i=0;
    while (i < bench_data->numLines) {
        if (result[i] == 1) {
            x++;
//            PRNT("%d ", result[i]);
        }
        i++;
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