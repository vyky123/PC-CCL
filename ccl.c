#include "ccl.h"

unsigned char *ccl(pgm *pgm_image) {
    int *label;
    unsigned char *output_data;
    linked_list *equivalency_list = NULL;

    label = (int *) malloc((pgm_image->height * pgm_image->width) * sizeof(int));

    if (!label) {
        printf("ERR#3: Memory allocation was unsuccessful!\n");
        free_pgm(pgm_image);
        exit(ERR_3);
    }
    memset(label, 0, (pgm_image->height * pgm_image->width) * sizeof(int));

    /* First pass */
    find_foreground(pgm_image, &equivalency_list, label);

    /* Second pass */
    replace_equivalent(pgm_image, equivalency_list, label);

    list_free(&equivalency_list);

    output_data = (unsigned char *) malloc((pgm_image->width * pgm_image->height) * sizeof(unsigned char));

    if (!output_data) {
        printf("ERR#3: Memory allocation was unsuccessful!\n");
        free(label);
        free_pgm(pgm_image);
        exit(ERR_3);
    }
    memset(output_data, 0, (pgm_image->width * pgm_image->height) * sizeof(unsigned char));

    /* Sets colors of pixels in output_data array */
    repaint_image(output_data, pgm_image, label);

    free(label);
    label = NULL;
    return output_data;
}

void find_foreground(pgm *pgm_image, linked_list **equivalency_list, int *label) {
    unsigned int i, j;
    int left_label = 0, up_label = 0, left_diagonal_label = 0, right_diagonal_label = 0;
    int label_counter = 1;
    int lowest, biggest;
    int has_neighbors;

    /* Iterate through pixels of PGM image */
    for (i = 0; i < pgm_image->height; i++) {
        for (j = 0; j < pgm_image->width; j++) {

            /* If the current pixel is white (foreground) */
            if (pgm_image->data[(i * pgm_image->width) + j]) {
                /* Sets lowest value to a next counter value that must always be bigger than any neighbor */
                lowest = label_counter + 1;
                /* Sets biggest value to 0, which is always smaller than any neighbor */
                biggest = 0;

                /* Finds the lowest and biggest neighbor label and adds equivalency. If no neighbor is white pixel,
                 * has_neighbors is set to 0 (FALSE) and label is equal to an incremented label value. */

                /* Checks all neighbors and sets their label values, finds biggest and lowest label and returns
                 * whether the pixel has any neighbors at all or not. */
                has_neighbors = find_neighbors(i, j, pgm_image, label, &up_label, &left_label, &left_diagonal_label,
                                               &right_diagonal_label, &lowest, &biggest);

                /* All neighboring pixels are black (background), set label to a current value of the label counter
                 * and increment it */
                if (!has_neighbors) {
                    label[(i * pgm_image->width) + j] = label_counter;
                    label_counter++;
                }
                /* At least one of neighboring pixels are white (foreground), set label to the value of the
                 * lowest neighbor label and update equivalency list */
                else {
                    label[(i * pgm_image->width) + j] = lowest;
                    update_equivalencies(equivalency_list, biggest, lowest);
                }
            }
        }
    }
}

int find_neighbors(unsigned int i, unsigned int j, pgm *pgm_image, int *label, int *up_label, int *left_label,
                   int *left_diagonal_label, int *right_diagonal_label, int *lowest, int *biggest) {
    int has_neighbors = FALSE;
    /* Upper neighbor */
    if (i > 0 && pgm_image->data[((i - 1) * pgm_image->width) + j]) {
        *up_label = label[((i - 1) * pgm_image->width) + j];
        label[(i * pgm_image->width) + j] = *up_label;
        *lowest = *up_label;
        *biggest = *up_label;
        has_neighbors = TRUE;
    }
    /* Left neighbor */
    if (j > 0 && pgm_image->data[(i * pgm_image->width) + (j - 1)]) {
        *left_label = label[(i * pgm_image->width) + (j - 1)];
        label[(i * pgm_image->width) + j] = *left_label;
        if (*left_label < *lowest) {
            *lowest = *left_label;
        }
        if (*left_label > *biggest) {
            *biggest = *left_label;
        }
        has_neighbors = TRUE;
    }
    /* Left diagonal neighbor */
    if (i > 0 && j > 0 && pgm_image->data[((i - 1) * pgm_image->width) + (j - 1)]) {
        *left_diagonal_label = label[((i - 1) * pgm_image->width) + (j - 1)];
        label[(i * pgm_image->width) + j] = *left_diagonal_label;
        if (*left_diagonal_label < *lowest) {
            *lowest = *left_diagonal_label;
        }
        if (*left_diagonal_label > *biggest) {
            *biggest = *left_diagonal_label;
        }
        has_neighbors = TRUE;
    }
    /* Right diagonal neighbor */
    if (i > 0 && (j < pgm_image->width - 1) && pgm_image->data[((i - 1) * pgm_image->width) + (j + 1)]) {
        *right_diagonal_label = label[((i - 1) * pgm_image->width) + (j + 1)];
        label[(i * pgm_image->width) + j] = *right_diagonal_label;
        if (*right_diagonal_label < *lowest) {
            *lowest = *right_diagonal_label;
        }
        if (*right_diagonal_label > *biggest) {
            *biggest = *right_diagonal_label;
        }
        has_neighbors = TRUE;
    }
    return has_neighbors;
}


void replace_equivalent(pgm *pgm_image, linked_list *equivalency_list, int *label) {
    unsigned int i;
    linked_list *it;
    it = equivalency_list;

    /* Sorts the equivalency list in descending order by its first values so all equivalencies are in the right order */
    list_sort(&equivalency_list);

    /* Iterates through equivalency list and replaces equivalent labels in label array */
    while (it) {
        for (i = 0; i < pgm_image->width * pgm_image->height; i++) {
            if (it->value == label[i]) {
                label[i] = it->value2;
            }
        }
        it = it->next;
    }
}

void repaint_image(unsigned char *output_data, pgm *input_image, int *label) {
    unsigned int i;
    int colors_count = 0;
    int *colors;
    linked_list *color_list = NULL, *it;

    /* Colors and their amount are calculated in compute_colors() function */
    colors = compute_colors(input_image, label, &colors_count, &color_list);

    it = color_list;
    /* Iterates through color list until it finds label equal to current color and sets pixel corresponding
     * to this color in output_data */
    while (it) {
        for (i = 0; i < input_image->width * input_image->height; i++) {
            if (label[i] == it->value) {
                output_data[i] = (unsigned char) colors[it->value2];
            }
        }
        it = it->next;
    }

    list_free(&color_list);
    free(colors);
    colors = NULL;
}

int *compute_colors(pgm *pgm_image, int *label, int *colors_count,
                    linked_list **color_list) {
    int i;
    int color_counter = 0;
    int *colors;

    /* Finds all unique labels, adds them to color list and increases color counter */
    for (i = 0; i < pgm_image->height * pgm_image->width; i++) {
        if (label[i] && !list_contains(*color_list, label[i])) {
            list_add(&(*color_list), label[i], color_counter);
            color_counter++;
        }
    }

    /* Changes value of *colors_count, so that it can still be used outside this function */
    *colors_count = color_counter;

    if (*colors_count >= (WHITE_VALUE - MAX_GRAY_OFFSET)) {
        printf("ERR#7: The image contains too many components to be colored independently!\n");
        exit(ERR_7);
    }

    colors = (int *) malloc((*colors_count) * sizeof(int));

    if (!colors) {
        printf("ERR#3: Memory allocation was unsuccesful!\n");
        exit(ERR_3);
    }
    memset(colors, 0, (*colors_count) * sizeof(int));

    /* If at least 2 components are being colored, distribute their grey values by integer division */
    if (*colors_count >= 2) {
        for (i = 0; i < (*colors_count); i++) {
            colors[i] = ((int) (WHITE_VALUE - MAX_GRAY_OFFSET) / (*colors_count)) * (i + 1);
        }
    }
    /* If only one component is being colored, use the default gray color (96) */
    else {
        colors[0] = DEFAULT_GRAY;
    }
    return colors;
}
