#include "ccl.h"

void debug_print(pgm *pgm_image, int label[pgm_image->height * pgm_image->width]) { /* TODO smazat */
    int i, j;
    for (i = 0; i < pgm_image->height; i++) {
        for (j = 0; j < pgm_image->width; j++) {
            if (label[(i * pgm_image->width) + j]) {
//                output_data[(i * pgm_image->width) + j] = pgm_image->max_value;
                printf("%3u", (label[(i * pgm_image->width) + j]));
            }
            else {
                //output_data[(i * pgm_image->width) + j] = 0;
                printf("  .");
            }
        }
        printf("\n");
    }
}

void free_pgm(pgm *pgm_image) {
    free(pgm_image->data);
    free(pgm_image);
}
int load_pgm(char *filename, pgm **pgm_image) {
    unsigned char ch;
    char line_buffer[BUFFER_SIZE];
    int  i, j, value[3];
    unsigned char char1, char2;
    unsigned char *ptr, *pixels;
    unsigned int width, height, max_value;
    FILE *f = fopen(filename, "rb"); /* Opens a file stream to variable f in a "read binary" mode */


    fscanf(f, "%c%c", &char1, &char2);
    if (char1 != 'P' || (char2 != '5')) {
        printf("ERR#2: Wrong PGM format!");
        fclose(f);
        exit(ERR_2);
    }

    for (j = 0; j < 3; j++) {
        i = 0;
        ch = (char)fgetc(f);
        line_buffer[i] = 0;

        i = 0;
        line_buffer[i++] = ch;
        do {
            ch = (char)fgetc(f);
            line_buffer[i++] = ch;
        }
        while(ch >= '0' && ch <= '9');
        line_buffer[i - 1] = 0;
        value[j] = atoi(line_buffer);
    }

    width  = value[0];
    height = value[1];
    max_value = value[2];
    pixels = (unsigned char *)malloc (height * width);

    if (!pixels) {
        printf("ERR#3: Memory allocation was unsuccesful!\n");
        exit(ERR_3);
    }


    ptr = pixels;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            fscanf(f,"%c", &ch);
            if ( ((unsigned char) ch != max_value && (unsigned char) ch != 0) ) {
                printf("ERR#2: Wrong PGM format!");
                exit(ERR_2);
            }
            *ptr++ = ch;
        }
    }
    /*   for (i = 0; i < height; i++) {
           for (j = 0; j < width; j++) {
               if(pixels[(i * width) + j] == 255) {
                   printf(". ");
               }
               else {
                   printf("%d ", pixels[(i * width) + j]);
               }
           }
           printf("\n");
       }
   */
    *pgm_image = (pgm *)malloc(sizeof(pgm));

    if (!*pgm_image) {
        printf("ERR#3: Memory allocation was unsuccesful!\n");
        exit(ERR_3);
    }

    (*pgm_image)->height = height;
    (*pgm_image)->width = width;
    (*pgm_image)->max_value = max_value;
    (*pgm_image)->data = malloc((width * height) * sizeof(unsigned char));
    memcpy((*pgm_image)->data, pixels, width * height * sizeof(unsigned char));

    fclose(f);
    free(pixels);
    return TRUE;
}
int save_pgm(char *filename, pgm *pgm_image, unsigned char *output_data) {
    int i, j;
    FILE *f = fopen(filename, "w");
    pgm *output_image;
    char *first_line = "P5\n";

    if (!f) {
        printf("ERR4#: Error writing to file!\n");
        exit(ERR_4);
    }

    /* Print to file */
    fprintf(f, "%s", first_line);
    fprintf(f, "%d %d\n", pgm_image->width, pgm_image->height);
    fprintf(f, "%u\n", pgm_image->max_value);

    for (i = 0; i < pgm_image->height; i++) {
        for (j = 0; j < pgm_image->width; j++) {
            fprintf(f, "%c", output_data[(i * pgm_image->width) + j]);
        }
    }

    fclose(f);
}
unsigned char *ccl(pgm *pgm_image) {
    int i, j;
    int label[pgm_image->height * pgm_image->width];
    unsigned char *output_data;
    linked_list *list = NULL;

    memset(label, 0, (pgm_image->height * pgm_image->width) * sizeof(int));
    find_foreground(pgm_image, &list, label);
    printf("First iteration:\n");
    debug_print(pgm_image, label);
    linked_list *it = list;
    while(it) {
        printf("[%d->%d] ", it->value, it->value2);
        it = it->next;
    }
    printf("\n");
    replace_equivalent(pgm_image, list, label);
    printf("Second iteration:\n");
    debug_print(pgm_image, label);
    it = list;
    while(it) {
        printf("[%d->%d] ", it->value, it->value2);
        it = it->next;
    }
    printf("\n");

    output_data = malloc(sizeof(unsigned char) * (pgm_image->width * pgm_image->height));
    memset(output_data, 0, sizeof(unsigned char) * (pgm_image->width * pgm_image->height));
    repaint_image(output_data, pgm_image, label);

    printf("Final data:\n");
    debug_print(pgm_image, label);

    list_free(&list);
    list = NULL;

    return output_data;
}
void find_foreground(pgm *pgm_image, linked_list **list, int label[pgm_image->height * pgm_image->width]) {
    int i, j;
    unsigned char current;
    int left_label, up_label, left_diagonal_label, right_diagonal_label;
    int label_counter = 1;
    int lowest;
    int is_set;

    for (i = 0; i < pgm_image->height; i++) {
        for (j = 0; j < pgm_image->width; j++) {
            current = pgm_image->data[(i * pgm_image->width) + j];

            if (current) {
                left_label = 0;
                up_label = 0;
                left_diagonal_label = 0;
                right_diagonal_label = 0;
                is_set = FALSE;
                lowest = INT_MAX;

                /* Up */
                if (i > 0 && pgm_image->data[((i-1) * pgm_image->width) + j]) {
                    up_label = label[((i-1) * pgm_image->width) + j];
                    label[(i * pgm_image->width) + j] = up_label;
                    lowest = up_label;
                    is_set = TRUE;
                }
                /* Left */
                if (j > 0 && pgm_image->data[(i * pgm_image->width) + (j - 1)]) {
                    left_label = label[(i * pgm_image->width) + (j - 1)];
                    label[(i * pgm_image->width) + j] = left_label;
                    if (left_label < lowest) {
                        lowest = left_label;
                    }
                    is_set = TRUE;
                }
                /* Left diagonal */
                if (i > 0 && j > 0 && pgm_image->data[((i-1) * pgm_image->width) + (j-1)]) {
                    left_diagonal_label = label[((i-1) * pgm_image->width) + (j-1)];
                    label[(i * pgm_image->width) + j] = left_diagonal_label;
                    if (left_diagonal_label < lowest) {
                        lowest = left_diagonal_label;
                    }
                    is_set = TRUE;
                }
                /* Right diagonal */
                if (i > 0 && pgm_image->data[((i - 1) * pgm_image->width) + (j + 1)]) {
                    right_diagonal_label = label[((i - 1) * pgm_image->width) + (j + 1)];
                    label[(i * pgm_image->width) + j] = right_diagonal_label;
                    if (right_diagonal_label < lowest) {
                        lowest = right_diagonal_label;
                    }
                    is_set = TRUE;
                }
                /* All neighboring pixels are black (background) */
                if (!is_set) {
                    label[(i * pgm_image->width) + j] = label_counter;
                    label_counter++;
                }
                else {
                    label[(i * pgm_image->width) + j] = lowest;
                }

                /* Label collision */
                if (left_label) {
                    add_equivalency(list, left_label, lowest);
                    if (left_diagonal_label && left_diagonal_label != left_label) {
                        add_equivalency(list, left_label, lowest);
                        add_equivalency(list, left_diagonal_label, lowest);

                    }
                    if (up_label && up_label != left_label) {
                        add_equivalency(list, left_label, lowest);
                        add_equivalency(list, up_label, lowest);
                    }
                    if (right_diagonal_label && right_diagonal_label != left_label) {
                        add_equivalency(list, left_label, lowest);
                        add_equivalency(list, right_diagonal_label, lowest);
                    }
                }
                if (left_diagonal_label) {
                    add_equivalency(list, left_diagonal_label, lowest);
                    if (up_label && up_label != left_diagonal_label) {
                        add_equivalency(list, left_diagonal_label, lowest);
                        add_equivalency(list, up_label, lowest);
                    }
                    if (right_diagonal_label && right_diagonal_label != left_diagonal_label) {
                        add_equivalency(list, left_diagonal_label, lowest);
                        add_equivalency(list, right_diagonal_label, lowest);
                    }
                }
                if (up_label) {
                    add_equivalency(list, up_label, lowest);
                    if (right_diagonal_label && right_diagonal_label != up_label) {
                        add_equivalency(list, up_label, lowest);
                        add_equivalency(list, right_diagonal_label, lowest);
                    }
                }
                if (right_diagonal_label) {
                    add_equivalency(list, right_diagonal_label, lowest);
                }


                if (j == 30 && i == 116) {
                    printf("\n");
                    linked_list *it = *list;
                    while(it) {
                        printf("[%d->%d] ", it->value, it->value2);
                        it = it->next;
                    }
                    printf("\n");
                    printf("%d---left:%d left_diag:%d up:%d right_diag:%d\n", label[(i * pgm_image->width) + j], left_label, left_diagonal_label, up_label, right_diagonal_label);
                    printf("\n");
                }
            }
        }
    //    printf("\n");
    }


    /*  printf("\n");
      for (i = 0; i < pgm_image->height; i++) {
          for (j = 0; j < pgm_image->width; j++) {
              if (label[(i * pgm_image->width) + j]) {
                  printf("%u  ", (label[(i * pgm_image->width) + j])/1);
              }
              else {
                  printf(".  ");
              }
          }
          printf("\n");
      }*/
}
void replace_equivalent(pgm *pgm_image, linked_list *list, int label[pgm_image->height * pgm_image->width]) {
    int i, j;
    linked_list *it;

    for (j = 0; j < 1; j++) { /* TODO smazat tenhle testovaci cyklus */
        it = list;
        while (it) {
            for (i = 0; i < pgm_image->width * pgm_image->height; i++) {
                if (it->value == label[i]) {
                    label[i] = it->value2;
                }
            }
            it = it->next;
        }
    }


}
void repaint_image(unsigned char *output_data, pgm *input_image, int label[input_image->width * input_image->height]) {
    int i;
    int colors_size;
    int *colors;
    linked_list *color_list = NULL, *it;
    colors = compute_colors(input_image, label, &colors_size, &color_list);

    it = color_list;
    while(it) {
        for (i = 0; i < input_image->width * input_image->height; i++) {
            if (label[i] == it->value) {
        //        printf("AAAA: %d\n", colors[it->value2]);
                output_data[i] = colors[it->value2 - 1]; /* TODO - 1 */
            }
        }
        it = it->next;
    }

    list_free(&color_list);
    free(colors);
    colors = NULL;
}
int *compute_colors(pgm *pgm_image, int label[pgm_image->height * pgm_image->width], int *colors_size,
                    linked_list **color_list) {
    int i;
    int color_counter = 1;
    int *colors;

    for (i = 0; i < pgm_image->height * pgm_image->width; i++) {
        if(label[i] && !list_contains(*color_list, label[i])) {
            list_add(&(*color_list), label[i], color_counter);
            color_counter++;
        }
    }
    *colors_size = color_counter - 1; /* TODO +1 nebo ne */
    colors = malloc(sizeof(int) * (*colors_size));
    
    if (!colors) {
        printf("ERR#3: Memory allocation was unsuccesful!\n");
        exit(ERR_3);
    }
    memset(colors, 0, sizeof(int) * (*colors_size));

    if (*colors_size >= 2) {
        printf("COLOR SIZE: %d\n", *colors_size);
        for (i = 0; i < (*colors_size); i++) {
            colors[i] = ((int) (pgm_image->max_value - MAX_GRAY_OFFSET) / (*colors_size)) * (i + 1);
            printf("Barva: %d\n",colors[i]);
        }
    }
    else {
        printf("NE, COLOR SIZE: %d\n", *colors_size);
        colors[0] = DEFAULT_GRAY;
    }
    return colors;
}