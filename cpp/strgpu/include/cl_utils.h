#ifndef STRGPU_B_UTILS_H
#define STRGPU_B_UTILS_H

#include "stdio.h"

# include "common.h"

char* readProgramFile(const char *filename) {
    FILE *fp;
    char *fileData;
    long fileSize;

    fp = fopen(filename, "r");
    if(!fp) {
        PRNT("Couldn't open the following file: %s\n", filename);
        exit(-1);
    }

    if(fseek(fp, 0, SEEK_END)) {
        PRNT("Error reading the file\n");
        exit(-1);
    }

    fileSize = ftell(fp);
    if(fileSize < 0) {
        PRNT("Error reading the file\n");
        exit(-1);
    }

    if(fseek(fp, 0, SEEK_SET)) {
        PRNT("Error reading the file\n");
        exit(-1);
    }

    fileData = (char*)malloc(fileSize+1);
    if (!fileData) {
        PRNT("Error allocating memory for the file\n");
        exit(-1);
    }

    if(fread(fileData, fileSize, 1, fp) != 1) {
        PRNT("Error reading the file\n");
        exit(-1);
    }

    // end string
    fileData[fileSize] = '\0';

    if (fclose(fp)) {
        PRNT("Error closing the file\n");
        exit(-1);
    }
    return fileData;
}

char** splitByNewline(char* text, int* numLines) {
    char **res  = NULL;
    char *p = strtok (text, "\n");
    *numLines = 0;

    while (p) {
        res = realloc(res, sizeof (char*) * ++(*numLines));
        if (res == NULL)
            exit (-1);
        res[(*numLines)-1] = malloc(sizeof(char)*strlen(p));

        strcpy(res[(*numLines)-1],p);
        p = strtok (NULL, "\n");
    }

    return res;
}

int* splitByNewlineAndCalculateOffsets(char* text, int* numLines) {
    int *res  = NULL;
    char *p = strtok (text, "\n");
    *numLines = 0;
    int first = 1;

    while (p) {
        res = realloc(res, sizeof (int) * ++(*numLines));
        if (res == NULL)
            exit (-1);

//        res[(*numLines)-1] = p-text;
        if (first == 1) {
            first = 0;
            res[(*numLines)-1] = strlen(p)-1;
        } else {
            res[(*numLines)-1] = res[(*numLines)-2]+strlen(p)+1;
        }

        p = strtok (NULL, "\n");
    }

    return res;
}

void printCompilerError(cl_program program, cl_device_id device) {
    PRNT("OPENCL Compiler Logs:\n");
    cl_int status;
    size_t logSize;
    char *log;

    status = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
    PRNT("Status: %d\n", status);

    log = (char*) malloc(logSize);
    if(!log)
        exit(-1);

    status = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, logSize, log, NULL);
    PRNT("Status: %d\n", status);

    printf("Log: %s\n", log);
}

#endif