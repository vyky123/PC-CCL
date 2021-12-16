#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "ccl.h"


void load_pgm(char *filename, pgm *pgmfile) {
    char 		ch;
    char 		line_buffer[200];
    int 		i, j, value[3], val;
    char		c1, c2;
    unsigned char *ptr, *pixels;
    unsigned int width, height, max_value;
    FILE *f = fopen(filename, "rb"); /* Opens a file line_buffeream to variable f in a "read binary" mode */


    fscanf(f, "%c%c", &c1, &c2);
    if (c1 != 'P' || (c2 != '2' && c2 != '5')) {
        printf("ERR#2: Wrong PGM format!");
        fclose(f);
        exit(ERR_2);
    }

    for (j = 0; j < 3; j++) {
        do {
            i = 0;
            ch = fgetc(f);
            if (ch == '#') do {
                    line_buffer[i++]=fgetc(f);
                } while (line_buffer[i-1]!= 'n');
            line_buffer[i] = 0;
        } while(ch < '0' || ch > '9');
        i = 0;
        line_buffer[i++] = ch;
        do {
            ch = fgetc(f);
            line_buffer[i++] = ch;
        } while(ch >= '0' && ch <= '9');
        line_buffer[i-1] = 0;
        value[j] = atoi(line_buffer);
        i=0;
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
    for (i = 0; i < height; i++) {
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
    pgmfile = (pgm *)malloc(sizeof(pgm));

    pgmfile->height = height;
    pgmfile->width = width;
    pgmfile->max_value = max_value;
    pgmfile->data = &pixels;
    fclose(f);
}

int main(int argc, char *argv[]) {
    char *input_filename = argv[1];
    char *output_filename = argv[2];
    pgm *pgmfile = NULL;
    int i;

    if (argc != 3) {
        printf("ERR#1: Wrong input!\n");
        return ERR_1;
    }
    if (!load_pgm(input_filename, pgmfile)) {
        printf("ERR# : !\n");
        return ERR_
    }
    load_pgm(input_filename, pgmfile);
    for (i = 0; i < pgmfile->width; ++i) {

    }
}
int process_file(char *filename) {
    /* Opens a file stream to variable f in a "read binary" mode */
    FILE *f = fopen(filename, "rb");
    char first_line[BUFFER_SIZE];
    char *loaded;
    unsigned char u;
    unsigned int height, width, max_value;
    int i;

    if (f == NULL) {
        printf("ERR#5: File was not found.");
        return ERR_5;
    }
    /* Read first line of PGM file */
    if (fscanf(f, "%s", first_line)) {
        printf("FirstLine: %s\n", first_line);
    }
    else {
        printf("Something is wrong I can feel it\n");
    }
    /* Read width and height of PGM file */
    if (fscanf(f, "%u %u", &width, &height)) {
        printf("Width Height: %u %u\n", width, height);
    }
    /* Read the max value of gray of PGM file */
    if (fscanf(f, "%u", &max_value)) {
        printf("MaxColor: %u\n", max_value);
    }
    printf("\n");

    /* Read image data (levels of gray) */
    loaded = malloc(sizeof(char) * width * height);
    if (fscanf(f, "%s", loaded)) {
    //    printf("%s", line_buffer);
        for (i = 0; i < strlen(loaded); i++) {
            if (loaded[i] != (unsigned int)4294967295) {
                printf("%u ", loaded[i]);
            }
            if ((unsigned char)loaded[i] < 255) {
         //       printf("%u ", (unsigned char)line_buffer[i]);
             //  printf("%02X", ((unsigned char) line_buffer[i] ));
            }
        }
    }

    fclose(f); /* Zavre soubor */
    return 0;
}