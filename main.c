#include "pgm.h"
#include "ccl.h"

/* ____________________________________________________________________________

    MAIN PROGRAM
   ____________________________________________________________________________
*/
int main(int argc, char *argv[]) {
    char *input_filename = argv[1];
    char *output_filename = argv[2];
    pgm *pgm_image = NULL;
    unsigned char *output_data = NULL;

    if (argc != 3 || !input_filename || !output_filename) {
        printf("ERR#1: Wrong input!\n");
        return ERR_1;
    }

    /* Loads PGM file into pgm_image */
    load_pgm(input_filename, &pgm_image);

    /* Runs connected component labeling (CCL) algorithm and returns pixel data of colored image */
    output_data = ccl(pgm_image);

    /* Saves colored image into the output file */
    save_pgm(output_filename, pgm_image, output_data);

    /* Frees allocated memory */
    free(output_data);
    output_data = NULL;
    free_pgm(pgm_image);
    pgm_image = NULL;

    return EXIT_SUCCESS;
}