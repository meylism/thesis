__kernel void search_naive_window(
__global char *data,
__global char *pattern,
int patternLen,
__global int *result,
__global int *offsets) {
    int idx = get_global_id(0);
    int begin, end, found;
    char8 string;
    unsigned int lsubstring, res;
    unsigned long lstring;

    unsigned int mask = 0xffffffff;
    found = 0;
    char4 substring = vload4(0, pattern);

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

    for(int i=begin; i<=end; i+=5) {
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

            for (int xx=1; xx<=5; xx++) {
                if ((res&mask) == mask) found = 1;

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