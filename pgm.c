#include <sys/stat.h>
#include "ccl.h"
#include <sys/stat.h>

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
void ccl(pgm *pgm_image, linked_list **list, unsigned char *output_data) {
    int i, j;
    int label[pgm_image->height * pgm_image->width];
    find_foreground(pgm_image, list, label);
    replace_equivalent(pgm_image, *list, label);

    for (i = 0; i < pgm_image->height; i++) {
        for (j = 0; j < pgm_image->width; j++) {

            if (label[(i * pgm_image->width) + j] != label[0]) { /* TODO ne label[0], barvicky */
                output_data[(i * pgm_image->width) + j] = pgm_image->max_value;
                printf("%u  ", (label[(i * pgm_image->width) + j]));
            }
            else {
                output_data[(i * pgm_image->width) + j] = 0;

                printf(".  ");
            }
        }
        printf("\n");
    }

}
void find_foreground(pgm *pgm_image, linked_list **list, int label[pgm_image->height * pgm_image->width]) {
    int i, j;
    unsigned char current;
    int left_label, up_label, left_diagonal_label, right_diagonal_label;
    int is_set;
    int label_counter = 1;

    memset(label, 0, (pgm_image->height * pgm_image->width) * sizeof(int));
    for (i = 0; i < pgm_image->height; i++) {
        for (j = 0; j < pgm_image->width; j++) {
            current = pgm_image->data[(i * pgm_image->width) + j];

            if (current) {
                is_set = FALSE;
                left_label = 0;
                up_label = 0;
                left_diagonal_label = 0;
                right_diagonal_label = 0;

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
                /* Left diagonal */
                if (i > 0 && j > 0 && pgm_image->data[((i-1) * pgm_image->width) + (j-1)]) {
                    left_diagonal_label = label[((i-1) * pgm_image->width) + (j-1)];
                    label[(i * pgm_image->width) + j] = left_diagonal_label;
                    is_set = TRUE;
                }
                /* Right diagonal */
                if (i > 0 && pgm_image->data[((i-1) * pgm_image->width) + (j+1)]) {
                    right_diagonal_label = label[((i-1) * pgm_image->width) + (j+1)];
                    label[(i * pgm_image->width) + j] = right_diagonal_label;
                    is_set = TRUE;
                }

                /* Label collision */
                if (left_label) {
                    if (up_label && up_label != left_label) {
                        list_add(list, up_label, left_label);
                       // equivalency_table_add(eq_table, left_label, up_label);
                    }
                    else if (left_diagonal_label && left_diagonal_label != left_label) {
                        list_add(list, left_diagonal_label, left_label);
                       // equivalency_table_add(left_label, diagonal_label);
                    }
                    else if (right_diagonal_label && right_diagonal_label != left_label) {
                        list_add(list, right_diagonal_label, left_label);
                    }

                }
                else if (up_label) {
                    if (left_diagonal_label && left_diagonal_label != up_label) {
                        list_add(list, left_diagonal_label, up_label);
                       // equivalency_table_add(diagonal_label, up_label);
                    }
                    else if (right_diagonal_label && right_diagonal_label != up_label) {
                        list_add(list, right_diagonal_label, up_label);
                    }
                }
                else if (left_diagonal_label) {
                    if (right_diagonal_label && right_diagonal_label != left_diagonal_label) {
                        list_add(list, left_diagonal_label, right_diagonal_label);
                    }
                }
                /* All neighboring pixels are white (background) */
                if (!is_set) {
                    label[(i * pgm_image->width) + j] = label_counter++;
                }
                if (label[(i * pgm_image->width) + j] == 2) {
             //       printf("\n%d %d up:%d left:%d diag:%d\n",current, is_set, up_label, left_label, diagonal_label);
                } /* TODO smazat */
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
    unsigned char current;
    linked_list *it;

    for (i = 0; i < pgm_image->height; i++) {
        for (j = 0; j < pgm_image->width; j++) {
            current = pgm_image->data[(i * pgm_image->width) + j];

            if (current) {
                it = list;
                while(it->next) { /* TODO tady */
                    if (it->value == label[(i * pgm_image->width) + j]) {
                        label[(i * pgm_image->width) + j] = list->value2;
                    }
                    it = it->next;
                }
            }
        }
    }
}