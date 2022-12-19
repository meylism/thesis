__kernel void search_naive_split_step1(
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

    end = begin+(end-begin)/2;

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

__kernel void search_naive_split_step2(
__global char *data,
__global char *pattern,
int patternLen,
__global int *result,
__global int *startoffsets,
__global int *offsets,
__global int *notFoundIndices,
int indicesLen) {
    int idx = get_global_id(0);
    int nFI = notFoundIndices[idx];
    int begin, end, j, found;
    found = 0;
    if (idx >= indicesLen) return;

    begin = startoffsets[nFI];
    end = offsets[nFI];

    begin += (end-begin)/2-(patternLen-1);


    for(int i=begin; i<=end; i++) {
        j = 0;
        while(j < patternLen && data[j+i] == pattern[j]) {
            j++;
        }
        if(j == patternLen) {
            result[nFI] = 1;
            found = 1;
            break;
        }

    }

    if (found == 0) {
        result[nFI] = 0;
    }
}