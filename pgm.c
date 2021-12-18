#include "ccl.h"

void free_pgm(pgm *pgm_image) {
    free(pgm_image->data);
    free(pgm_image);
}
int load_pgm(char *filename, pgm **pgm_image) {
    unsigned char ch;
    char line_buffer[200];
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
        while(ch > ' ');
        line_buffer[i-1] = 0;
        value[j] = atoi(line_buffer);
    }

    width  = value[0];
    height = value[1];
    max_value = value[2];
    pixels  = (unsigned char *)malloc (height*width);

    if (!pixels) {
        printf("ERR#3: Memory allocation was unsuccesful!\n");
        exit(ERR_3);
    }


    ptr = pixels;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            fscanf(f,"%c", &ch);
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
int find_foreground(pgm *pgm_image, linked_list **list) {
    int i, j;
    unsigned char current;
    int left_label, up_label, diagonal_label;
    int label[pgm_image->height * pgm_image->width];
    int label_counter = 1;
    int is_set;
    memset(label, 0, (pgm_image->height * pgm_image->width) * sizeof(int));
    for (i = 0; i < pgm_image->height; i++) {
        for (j = 0; j < pgm_image->width; j++) {
            current = pgm_image->data[(i * pgm_image->width) + j];

            if (current) {
             //   printf("%d ", current/100);
                is_set = FALSE;
                left_label = 0;
                up_label = 0;
                diagonal_label = 0;

                /* Up */
                if (i > 0 && pgm_image->data[((i-1) * pgm_image->width) + j]) {
                    up_label = label[((i-1) * pgm_image->width) + j];
                    label[(i * pgm_image->width) + j] = up_label;
                    is_set = TRUE;
                }
                /* Left */
                if (j > 0 && pgm_image->data[(i*pgm_image->width) + (j-1)]) {
                    left_label = label[(i * pgm_image->width) + (j-1)];
                    label[(i * pgm_image->width) + j] = left_label;
                    is_set = TRUE;
                }
                /* Diagonal */
                if (i > 0 && j > 0 && pgm_image->data[((i-1) * pgm_image->width) + (j-1)]) {
                    diagonal_label = label[((i-1) * pgm_image->width) + (j-1)];
                    label[(i * pgm_image->width) + j] = diagonal_label;
                    is_set = TRUE;
                }

                /* Label collision */
                if (left_label) {
                    if (up_label && up_label != left_label) {
                        list_add(list, up_label);
                        list_add(list, left_label);
                       // equivalency_table_add(eq_table, left_label, up_label);
                    }
                    if (diagonal_label && diagonal_label != left_label) {
                        list_add(list, diagonal_label);
                        list_add(list, left_label);
                       // equivalency_table_add(left_label, diagonal_label);
                    }
                }
                if (up_label) {
                    if (diagonal_label && diagonal_label != up_label) {
                        list_add(list, diagonal_label);
                        list_add(list, up_label);
                       // equivalency_table_add(diagonal_label, up_label);
                    }
                }
                if (!is_set) {
                    label[(i * pgm_image->width) + j] = label_counter++;
                }
                if (label[(i * pgm_image->width) + j] == 3) {
                    printf("\n%d %d up:%d left:%d diag:%d\n",current, is_set, up_label, left_label, diagonal_label);
                }

            }
            else {
            //    label[(i * pgm_image->width) + j] = 0;
           //     printf(" ");

            }
        }
    //    printf("\n");
    }
    printf("\n");
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
    }
}
