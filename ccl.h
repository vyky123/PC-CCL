#ifndef __CCL_H__
#define __CCL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define BUFFER_SIZE 256

#define ERR_1 1
#define ERR_2 2
#define ERR_3 3
#define ERR_4 4
#define ERR_5 5

#define TRUE 1
#define FALSE 0

typedef struct thepgm {
    unsigned char *data;
    unsigned int height;
    unsigned int width;
    unsigned int max_value;
} pgm;

//int process_file(char *filename);
int load_pgm(char *filename, pgm **pgmfile);

#endif
