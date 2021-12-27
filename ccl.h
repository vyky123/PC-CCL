#ifndef __CCL_H__
#define __CCL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "linked_list.h"
#include "pgm.h"

/* Buffer size */
#define BUFFER_SIZE 64
/* Integer value of white pixel */
#define WHITE_VALUE 255
/* Integer value of gray pixel that is used when only 1 color is needed for coloring */
#define DEFAULT_GRAY 96
/* Offset for maximal gray value of pixel, e.g. 5 means output colors will not exceed 250 (255 - 5) */
#define MAX_GRAY_OFFSET 5

/* Wrong input error */
#define ERR_1 1
/* Wrong PGM format error */
#define ERR_2 2
/* Unsuccessful memory allocation error */
#define ERR_3 3
/* Writing to file error */
#define ERR_4 4
/* Too many components error */
#define ERR_5 5

#define TRUE 1
#define FALSE 0

/* ____________________________________________________________________________

    unsigned char *ccl(pgm *pgm_image)

    Starts the CCL algorithm. Runs the first pass in find_foreground()
    function, the second pass in replace_equivalent() function, then
    computes colors in compute_colors() function and sets corresponding
    colors to output data in repaint_image() function.
   ____________________________________________________________________________
*/
unsigned char *ccl(pgm *pgm_image);

/* ____________________________________________________________________________

    void find_foreground(pgm *pgm_image, linked_list **equivalency_list,
                        int *label)

    Function representing the first pass of the CCL algorithm. Checks every
    pixel of the PGM image and if it is white, checks left, left diagonal,
    upper and right diagonal neighbours of the pixels and labels the pixel
    according to the CCL algorithm.
   ____________________________________________________________________________
*/
void find_foreground(pgm *pgm_image, linked_list **equivalency_list, int *label);
/* ____________________________________________________________________________

    int find_neighbors(int i, int j, pgm *pgm_image, int *label, int *up_label,
                       int *left_label, int *left_diagonal_label,
                       int *right_diagonal_label, int *lowest, int *biggest)

    Checks all 4 neighbor labels ("the mask") and saves their value. Also
    changes biggest and lowest values if conditions are met. Part of the first
    pass of CCL in find_foreground() function. It is called for every white
    pixel (foreground).
    Returns 1 (TRUE) if pixel has any neighboring labels.
    Return 0 (FALSE) if pixel has no neighbors.
   ____________________________________________________________________________
*/
int find_neighbors(int i, int j, pgm *pgm_image, int *label, int *up_label, int *left_label, int *left_diagonal_label,
                   int *right_diagonal_label, int *lowest, int *biggest);
/* ____________________________________________________________________________

    void replace_equivalent(pgm *pgm_image, linked_list *equivalency_list,
                            int *label)

    Function representing the second pass of the CCL algorithm. Iterates
    through equivalency list and replaces all equivalent labels of the PGM
    image with the smaller one.
   ____________________________________________________________________________
*/
void replace_equivalent(pgm *pgm_image, linked_list *equivalency_list, int *label);

/* ____________________________________________________________________________

    int *compute_colors(pgm *pgm_image, int *label, int *total_colors,
                    linked_list **color_list)

    Function that gets the number of colors needed to color all components.
    After that, colors are equally distributed to labels by integer division.
    Colors are stored in color_list, where value1 of the list is label
    and value2 is a counter value equal to an index of a color in an array
    of colors.
    The value of total_colors is saved and used again later in
    repaint_image() function as an indicator of size of the array of colors.
    Returns integer array of colors.
   ____________________________________________________________________________
*/
int *compute_colors(pgm *pgm_image, int *label, int *total_colors,
                    linked_list **color_list);

/* ____________________________________________________________________________

    void repaint_image(unsigned char *output_data, pgm *pgm_image, int *label)

    "Repaints" the image by assigning colors from color list and an array of
    colors and saves the image as unsigned char array output_data that is
    later used in save_pgm() function.
   ____________________________________________________________________________
*/
void repaint_image(unsigned char *output_data, pgm *pgm_image, int *label);

#endif