#include "ccl.h"


int load_pgm(char *filename, pgm **pgmfile) {
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
    *pgmfile = (pgm *)malloc(sizeof(pgm));

    if (!*pgmfile) {
        printf("ERR#3: Memory allocation was unsuccesful!\n");
        exit(ERR_3);
    }

    (*pgmfile)->height = height;
    (*pgmfile)->width = width;
    (*pgmfile)->max_value = max_value;
    (*pgmfile)->data = malloc((width * height) * sizeof(unsigned char));
    memcpy((*pgmfile)->data, pixels, width * height * sizeof(unsigned char));

    fclose(f);
    free(pixels);
    return TRUE;
}