__kernel void search_naive_register(
__global char *data,
__global char *pattern,
int patternLen,
__global int *result,
__global int *offsets) {
    int idx = get_global_id(0);
    int begin, end, j, found;
    found = 0;

    char4 vp = vload4(0, pattern);
    char4 vs;

    // first line
    if (idx == 0) {
        begin = 0;
    } else {
        begin = offsets[idx-1]+1;
    }
    end = offsets[idx];

    for(int i=begin; i<=end; i++) {
        j = 0;
        vs = vload4(0, data+i);

        if((vs.x == vp.x) && (vs.y == vp.y) && (vs.z == vp.z) && (vs.w == vp.w)) {
            result[idx] = 1;
            found = 1;
        }

    }

    if (found == 0) {
        result[idx] = 0;
    }
}