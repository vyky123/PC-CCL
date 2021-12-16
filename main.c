#include "ccl.h"

int main(int argc, char *argv[]) {
    char *input_filename = argv[1];
    char *output_filename = argv[2];
    pgm *pgmfile = NULL;
    int i, j;

    if (argc != 3) {
        printf("ERR#1: Wrong input!\n");
        return ERR_1;
    }
    load_pgm(input_filename, &pgmfile);
    for (i = 0; i < pgmfile->height; i++) {
        for (j = 0; j < pgmfile->width; j++) {
            if(pgmfile->data[(i * pgmfile->width) + j]) {
                printf(". ");
            }
            else {
//                printf("%s ", pgmfile->data[(i * pgmfile->width) + j]);
                printf("0 ");
            }
        }
        printf("\n");
    }

    free(pgmfile);
    return EXIT_SUCCESS;
}
//int process_file(char *filename) {
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