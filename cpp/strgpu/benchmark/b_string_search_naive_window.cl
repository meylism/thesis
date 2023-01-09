__kernel void search_naive_window(
__global char *data,
__global char *pattern,
int patternLen,
__global int *result,
__global int *offsets) {
    int idx = get_global_id(0);
    int begin, end, j, found;
    found = 0;
    unsigned long long mask1 = 0xffffffff;
    unsigned long long mask2 = 0xffffffff00000000;

    char8 string;
    char8 substring = vload8(0, pattern);
    unsigned long long lstring, lsubstring;

    lsubstring = (unsigned long long)substring.s0 << 56 |
        (unsigned long long)substring.s1 << 48 |
        (unsigned long long)substring.s2 << 40 |
        (unsigned long long)substring.s3 << 32 |
        (unsigned long long)substring.s4 << 24 |
        (unsigned long long)substring.s5 << 16 |
        (unsigned long long)substring.s6 << 8 |
        (unsigned long long)substring.s7 << 0;


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

        unsigned long long res = ~(lstring^lsubstring);
            for (int xx=0; xx<=3; xx++) {
                if ((res&mask1) == mask1 || (res&mask2) == mask2) {
                    found = 1;
                }
                lstring = lstring << 8;
                res = ~(lstring^lsubstring);
            }
    }

    if (found == 0) {
        result[idx] = 0;
    } else {
        result[idx] = 1;
    }
}