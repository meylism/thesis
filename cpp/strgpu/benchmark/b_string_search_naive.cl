__kernel void search_naive(__global char *data, __global char *pattern, __global int *result) {
    int idx = get_global_id(0);
    if (data[idx] == 'a') {
        result[idx] = 1;
    } else {
        result[idx] = 0;
    }
}