#pragma OPENCL EXTENSION cl_intel_printf : enable

__kernel void search_naive_newline(
__global char *data,
__global char *pattern,
int patternLen,
__global int *result,
__global int *offsets) {
    int idx = get_global_id(0);
    int begin, end, j, found;
    found = 0;

    // first line
    if (idx == 0) {
        begin = 0;
    } else {
        begin = offsets[idx-1]+1;
    }
    end = offsets[idx];

    for(int i=begin; i<=end; i++) {
        j = 0;
        while(j < patternLen && data[j+i] == pattern[j]) {
            j++;
        }
        if(j == patternLen) {
            result[idx] = 1;
            found = 1;
            break;
        }

    }

    if (found == 0) {
            result[idx] = 0;
    }
}