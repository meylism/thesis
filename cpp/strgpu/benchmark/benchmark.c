#include "common.h"
#include "ubench.h"

#define PLATFORM 0
UBENCH_STATE();

#include "opencl_helpers.h"
#include "cl_utils.h"

// Include benchmark headers here
// Benchmarks - start

//#include "b_add_number.h"
//#include "b_string_search_naive.h"
//#include "b_string_search_naive_newline.h"
//#include "b_string_search_naive_register.h"
//#include "b_string_search_naive_split.h"
//#include "b_string_search_naive_split3.h"
#include "b_string_search_naive_window.h"

// Benchmarks - end


opencl_stub *stub;
benchmark_data *bench_data;
configuration *conf;

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

    status = clGetDeviceIDs(ltub->platforms[PLATFORM], CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
    ltub->gpu = malloc(numDevices * sizeof(cl_device_id));
    status = clGetDeviceIDs(ltub->platforms[PLATFORM], CL_DEVICE_TYPE_GPU, numDevices, ltub->gpu, NULL);
    PRNT("INIT(%d): GPU has been selected\n", status);

    status = clGetDeviceIDs(ltub->platforms[PLATFORM], CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);
    ltub->cpu = malloc(numDevices * sizeof(cl_device_id));
    status = clGetDeviceIDs(ltub->platforms[PLATFORM], CL_DEVICE_TYPE_CPU, numDevices, ltub->cpu, NULL);
    PRNT("INIT(%d): CPU has been selected\n", status);

    return ltub;
}

void opencl_free(opencl_stub *lstub) {
    free(lstub->platforms);
    free(lstub->gpu);
    free(lstub->cpu);
    free(lstub);
}

configuration* init_conf(opencl_stub *lstub) {
    configuration *lconf = malloc(sizeof(configuration));

    // uncomment corresponding device to select
    // cpu
    lconf->device = lstub->cpu;
    lconf->localSize[0] = 10;

    // gpu
//    lconf->device = lstub->gpu;
//    lconf->localSize[0] = 100;

    return lconf;
}

void free_conf(configuration *lconf) {
    free(lconf);
}

benchmark_data * load_benchmark_data(const char *filepath, const char *pattern) {
    benchmark_data *b_data = malloc(sizeof (benchmark_data));

    b_data->data = readProgramFile(filepath);
    b_data->data_len = strlen(b_data->data);
    b_data->data_size = b_data->data_len*sizeof(char);

    b_data->pattern = pattern;
    b_data->pattern_len = strlen(b_data->pattern);
    b_data->pattern_size = b_data->pattern_len*sizeof(char);

    int numLines;
    splitByNewlineAndCalculateOffsets2(b_data->data, &numLines, b_data);
    b_data->numLines = numLines;

//    for(int i=0; i<numLines-90000; i++) {
//        PRNT("%d-%d ", b_data->start_indices[i], b_data->offsets[i]);
//    }

    return b_data;
}

void free_benchmark_data(benchmark_data *b_data) {
    free(b_data->data);
    free(b_data->offsets);
    free(b_data->start_indices);
    free(b_data);
}

void print_usage(const char* program_name) {
    fprintf(stderr, "Usage:\n\t%s <path-to-the-benchmark-dataset> <pattern> <confidence level for benchmarks>", program_name);
}

int main(int argc, const char** argv) {
    if (argc != 4) {
        print_usage(argv[0]);
        return -1;
    }

    stub = opencl_init();

    bench_data = load_benchmark_data(argv[1], argv[2]);

    conf = init_conf(stub);

    int bench_status = ubench_main(argc, argv);

    opencl_free(stub);

    free_conf(conf);

    free_benchmark_data(bench_data);

    return bench_status;
}