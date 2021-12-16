#include "ccl.h"


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

    if (pixels == NULL) {
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
int find_foreground(pgm *pgm_image) {
    int i, j;
    unsigned char current;
    unsigned int left_color, up_color, diagonal_color;
    unsigned int color[pgm_image->height * pgm_image->width];

    for (i = 0; i < pgm_image->height; i++) {
        for (j = 0; j < pgm_image->width; j++) {
            current = pgm_image->data[(i * pgm_image->width) + j];
            if (current) {
                left_color = 0;
                up_color = 0;
                diagonal_color = 0;

                /* Left */
                if (i > 0 && pgm_image->data[((i-1) * pgm_image->width) + j] == current) {
                    left_color = color[((i-1) * pgm_image->width) + j];
                }
                /* Up */
                if (j > 0 && pgm_image->data[(i*pgm_image->width) + (j-1)] == current) {
                    up_color = color[(i * pgm_image->width) + (j-1)];
                }
                /* Diagonal */
                if (i > 0 && j > 0 && pgm_image->data[((i-1) * pgm_image->width) + (j-1)] == current) {
                    diagonal_color = color[((i-1) * pgm_image->width) + (j-1)];
                }

                /* Color collision */
                if (left_color) {
                    if (up_color && up_color != left_color) {
                        equivalency_table_add(left_color, up_color);
                    }
                    if (diagonal_color && diagonal_color != left_color) {
                        equivalency_table_add(left_color, diagonal_color);
                    }
                }
                if (up_color) {
                    if (diagonal_color && diagonal_color != up_color) {
                        equivalency_table_add(diagonal_color, up_color);
                    }
                }
//                color[(i * pgm_image->width) + j] = /* TODO toto */;
            }
        }
    }
}