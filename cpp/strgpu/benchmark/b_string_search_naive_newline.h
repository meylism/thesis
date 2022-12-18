#ifndef STRGPU_B_STRING_SEARCH_NAIVE_H
#define STRGPU_B_STRING_SEARCH_NAIVE_H

#include "ubench.h"
#include "opencl_helpers.h"
#include "cl_utils.h"


UBENCH_EX(string_search, naive_newline) {
    printf("\t########## Benchmark - String Search Naive Newline ##########\n");
    cl_int status;
    cl_context context = clCreateContext(NULL, 1, conf->device, NULL, NULL, &status);
    PRNT("Context created(%d)\n", status);

    cl_command_queue cmdQueue = clCreateCommandQueue(context, *conf->device, 0, &status);
    PRNT("Queue created(%d)\n", status);

    int result_size = sizeof(int)*bench_data->numLines;
    int offset_size = result_size;
    int *result = (int*)malloc(result_size);
    int *result_out = (int*)malloc(result_size);

    cl_mem cl_data = clCreateBuffer(context, CL_MEM_READ_ONLY, bench_data->data_size, NULL, &status);
    cl_mem cl_pattern = clCreateBuffer(context, CL_MEM_READ_ONLY, bench_data->pattern_size, NULL, &status);
    cl_mem cl_result = clCreateBuffer(context, CL_MEM_READ_WRITE, bench_data->data_size*sizeof(int), NULL, &status);
    cl_mem cl_offsets = clCreateBuffer(context, CL_MEM_READ_ONLY, offset_size, NULL, &status);
    PRNT("Buffers created(%d)\n", status);

    status = clEnqueueWriteBuffer(cmdQueue, cl_data, CL_FALSE, 0, bench_data->data_size, bench_data->data, 0, NULL, NULL);
    status = clEnqueueWriteBuffer(cmdQueue, cl_pattern, CL_FALSE, 0, bench_data->pattern_size, bench_data->pattern, 0, NULL, NULL);
    status = clEnqueueWriteBuffer(cmdQueue, cl_result, CL_FALSE, 0, bench_data->data_size*sizeof(int), result, 0, NULL, NULL);
    status = clEnqueueWriteBuffer(cmdQueue, cl_offsets, CL_FALSE, 0, offset_size, bench_data->offsets, 0, NULL, NULL);
    PRNT("Buffers written(%d)\n", status);

    char* src = readProgramFile("../benchmark/b_string_search_naive_newline.cl");

    cl_program program = clCreateProgramWithSource(context, 1, &src, NULL, &status);
    PRNT("Program created(%d)\n", status);

    status = clBuildProgram(program, 1, conf->device, NULL, NULL, NULL);
    PRNT("Program built(%d)\n", status);

    cl_device_id dev = *conf->device;
    printCompilerError(program, dev);

    cl_kernel kernel = clCreateKernel(program, "search_naive_newline", &status);
    PRNT("Kernel created(%d)\n", status);

    status = clSetKernelArg(kernel, 0, sizeof   (cl_mem), &cl_data);
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

        status = clEnqueueReadBuffer(cmdQueue, cl_result, CL_TRUE, 0, result_size, result_out, 0, NULL, NULL);
        PRNT("Read resulting buffer(%d)\n", status);
    }
    int x=0, i=0;
    while (i < bench_data->numLines) {
        if (result_out[i] == 1) x++;
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
    free(result_out);

    PRNT("Freed resources\n");
}

#endif