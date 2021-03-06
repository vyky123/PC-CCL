#include "ccl.h"

void load_pgm(char *filename, pgm **pgm_image) {
    unsigned char ch;
    char value_buffer[BUFFER_SIZE];
    unsigned int i, j;
    unsigned int value[3];
    unsigned char char1, char2, char3;
    unsigned char *ptr, *pixels;
    unsigned int width, height, max_value;


    /* Opens a file stream to variable f in a "read binary" mode */
    FILE *f = fopen(filename, "rb");

    if (!f) {
        printf("ERR#5: Error reading a file!\n");
        exit(ERR_5);
    }

    /* Loads first 3 chars into char variables */
    fscanf(f, "%c%c%c", &char1, &char2, &char3);

    /* Checks if the first line is "P5\n" */
    if (char1 != (unsigned char) 'P' || (char2 != (unsigned char) '5') || (char3 != (unsigned char) '\n')) {
        printf("ERR#2: Wrong PGM format!");
        if (fclose(f) != 0) {
            printf("ERR6#: Error when closing a file!\n");
            exit(ERR_6);
        }
        exit(ERR_2);
    }
    /* Goes char by char through second and third line and gets 3 values from these lines */
    for (i = 0; i < 3; i++) {
        j = 0;
        ch = (char) fgetc(f);
        value_buffer[j] = (char) ch;
        j++;
        /* Gets char and adds it into the buffer until the value ends
         * (until the next char is not a space or new line) */
        while (ch != (unsigned char) ' ' && ch != (unsigned char) '\n') {
            ch = (char) fgetc(f);
            if ((ch < (unsigned char) '0' || ch > (unsigned char) '9') &&
                (ch != (unsigned char) ' ' && ch != (unsigned char) '\n')) {
                printf("ERR#2: Wrong PGM format!");
                fclose(f);
                exit(ERR_2);
            }
            value_buffer[j] = (char) ch;
            j++;
        }

        value[i] = atoi(value_buffer);
    }
    /* 3 values gotten from previous loop are width, height and max value */
    width = value[0];
    height = value[1];
    max_value = value[2];

    /* If max value of PGM is not white, exits the program */
    if (max_value != WHITE_VALUE) {
        printf("ERR#2: Wrong PGM format!");
        fclose(f);
        exit(ERR_2);
    }

    pixels = (unsigned char *) malloc(height * width);

    if (!pixels) {
        printf("ERR#3: Memory allocation was unsuccessful!\n");
        exit(ERR_3);
    }

    ptr = pixels;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            fscanf(f, "%c", &ch);
            /* If pixel is not black or white, PGM format is wrong */
            if (((unsigned char) ch != (unsigned char) WHITE_VALUE && (unsigned char) ch != (unsigned char) 0)) {
                printf("ERR#2: Wrong PGM format!");
                free(pixels);
                exit(ERR_2);
            }
            *ptr++ = ch;
        }
    }
    *pgm_image = (pgm *) malloc(sizeof(pgm));

    if (!*pgm_image) {
        printf("ERR#3: Memory allocation was unsuccessful!\n");
        exit(ERR_3);
    }

    (*pgm_image)->height = height;
    (*pgm_image)->width = width;
    (*pgm_image)->data = (unsigned char *) malloc((width * height) * sizeof(unsigned char));

    if (!(*pgm_image)->data) {
        printf("ERR#3: Memory allocation was unsuccessful!\n");
        free(pixels);
        free(*pgm_image);
        exit(ERR_3);
    }

    memcpy((*pgm_image)->data, pixels, width * height * sizeof(unsigned char));

    if (fclose(f) != 0) {
        printf("ERR6#: Error when closing a file!\n");
        exit(ERR_6);
    }

    free(pixels);
}

void save_pgm(char *filename, pgm *pgm_image, unsigned char *output_data) {
    unsigned int i, j;
    char *first_line = "P5\n";
    FILE *f = fopen(filename, "w");

    if (!f) {
        printf("ERR4#: Error writing to file!\n");
        exit(ERR_4);
    }

    /* Prints the first 3 lines to a file */
    fprintf(f, "%s", first_line);
    fprintf(f, "%u %u\n", pgm_image->width, pgm_image->height);
    fprintf(f, "%u\n", WHITE_VALUE);

    /* Prints image data to the file */
    for (i = 0; i < pgm_image->height; i++) {
        for (j = 0; j < pgm_image->width; j++) {
            fprintf(f, "%c", output_data[(i * pgm_image->width) + j]);
        }
    }

    if (fclose(f) != 0) {
        printf("ERR6#: Error when closing a file!\n");
        exit(ERR_6);
    }
}

void free_pgm(pgm *pgm_image) {
    free(pgm_image->data);
    free(pgm_image);
}