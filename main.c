#include "ccl.h"
int to_2D(int i, int j, int width, unsigned char *data) {
    return data[(i * width) + j];
}
int main(int argc, char *argv[]) {
    char *input_filename = argv[1];
    char *output_filename = argv[2];
    pgm *pgm_image = NULL;
    int i, j;

    if (argc != 3) {
        printf("ERR#1: Wrong input!\n");
        return ERR_1;
    }
    load_pgm(input_filename, &pgm_image);
    find_foreground(pgm_image);
  /*  for (i = 0; i < pgm_image->height; i++) {
        for (j = 0; j < pgm_image->width; j++) {
            if(pgm_image->data[(i * pgm_image->width) + j]) {
                printf(". ");
            }
            else {
//                printf("%s ", pgm_image->data[(i * pgm_image->width) + j]);
                printf("0 ");
            }
        }
        printf("\n");
    }*/

    free(pgm_image);
    return EXIT_SUCCESS;
}