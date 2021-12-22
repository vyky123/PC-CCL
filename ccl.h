#ifndef __CCL_H__
#define __CCL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "linked_list.h"

#define BUFFER_SIZE 256
#define DEFAULT_GRAY 64

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

typedef struct theequivalency_set {
    int color;

} equivalency_set;

//int process_file(char *filename);
unsigned char *ccl(pgm *pgm_image, linked_list **list);
int load_pgm(char *filename, pgm **pgm_image);
int save_pgm(char *filename, pgm *pgm_image, unsigned char *data);
void free_pgm(pgm *pgm_image);
void find_foreground(pgm *pgm_image, linked_list **list, int label[pgm_image->height * pgm_image->width]);
void replace_equivalent(pgm *pgm_image, linked_list *list, int label[pgm_image->height * pgm_image->width]);
void repaint_image(unsigned char* output_data, pgm *pgm_image, int label[pgm_image->width * pgm_image->height]);
int *compute_colors(pgm *pgm_image, int label[pgm_image->height * pgm_image->width], int *total_colors,
                    linked_list **color_list);
#endif
