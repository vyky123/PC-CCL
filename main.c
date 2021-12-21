#include "ccl.h"
#include "linked_list.h"

int main(int argc, char *argv[]) {
    char *input_filename = argv[1];
    char *output_filename = argv[2];
    pgm *pgm_image = NULL;
    unsigned char *output_data = NULL;
    linked_list *list = NULL;
    int i, j;

    if (argc != 3) {
        printf("ERR#1: Wrong input!\n");
        return ERR_1;
    }
    load_pgm(input_filename, &pgm_image);

    output_data = malloc(sizeof(unsigned char) * pgm_image->height * pgm_image->width);
    ccl(pgm_image, &list, output_data);


    save_pgm(output_filename, pgm_image, output_data);

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
    linked_list *it = list;
    printf("\n");
    while(it) {
        printf("[%d->%d] ", it->value, it->value2);
        it = it->next;
    }
    free_pgm(pgm_image);
    return EXIT_SUCCESS;
}