#include "ccl.h"

int main(int argc, char *argv[]) {
    char *input_filename = argv[1];
    char *output_filename = argv[2];
    pgm *pgm_image = NULL, *output_image = NULL;
    unsigned char *output_data = NULL;
    linked_list *list = NULL;
    int i, j;

    if (argc != 3) {
        printf("ERR#1: Wrong input!\n");
        return ERR_1;
    }
    load_pgm(input_filename, &pgm_image);


    output_data = ccl(pgm_image, &list);


    save_pgm(output_filename, pgm_image, output_data); /* TODO output image */
    free(output_data);
    linked_list *it = list;
    printf("\n");
    while(it) {
        printf("[%d->%d] ", it->value, it->value2);
        it = it->next;
    }
    list_free(&list);
    free_pgm(pgm_image);
    return EXIT_SUCCESS;
}