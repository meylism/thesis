__kernel void search_naive_window(
__global char *data,
__global char *pattern,
int patternLen,
__global int *result,
__global int *offsets) {
    int idx = get_global_id(0);
    int begin, end, found;
    found = 0;
    unsigned int mask1 = 0xffff;
    unsigned int mask2 = 0xffff0000;

    char8 string;
    char4 substring = vload4(0, pattern);
     unsigned int lsubstring, res;
     unsigned long lstring;

    lsubstring = ( unsigned int)substring.s0 << 24 |
        ( unsigned int)substring.s1 << 16 |
        ( unsigned int)substring.s2 << 8 |
        ( unsigned int)substring.s3 << 0;


    // first line
    if (idx == 0) {
        begin = 0;
    } else {
        begin = offsets[idx-1]+1;
    }
    end = offsets[idx];

    for(int i=begin; i<=end; i+=6) {
            if (found == 1) break;
            string = vload8(0, data+i);

        lstring = (unsigned long long)string.s0 << 56 |
            (unsigned long long)string.s1 << 48 |
            (unsigned long long)string.s2 << 40 |
            (unsigned long long)string.s3 << 32 |
            (unsigned long long)string.s4 << 24 |
            (unsigned long long)string.s5 << 16 |
            (unsigned long long)string.s6 << 8 |
            (unsigned long long)string.s7 << 0;

            res = ~(lstring^lsubstring);

            for (int xx=1; xx<=4; xx++) {
                if ((res&mask1) == mask1 || (res&mask2) == mask2) found = 1;

                lstring = lstring >> 8;
                res = ~(lstring^lsubstring);
            }
    }

    if (found == 0) {
        result[idx] = 0;
    } else {
        result[idx] = 1;
    }
}