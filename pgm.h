#ifndef __PGM_H__
#define __PGM_H__

/* Struct representing PGM image */
typedef struct thepgm {
    unsigned char *data;
    unsigned int height;
    unsigned int width;
} pgm;

/* ____________________________________________________________________________

    int load_pgm(char *filename, pgm **pgm_image)

    Loads PGM image width, height and pixel data into a PGM struct.
    Checks whether the PGM file is valid and if not, exits the program.
   ____________________________________________________________________________
*/
void load_pgm(char *filename, pgm **pgm_image);

/* ____________________________________________________________________________

    void save_pgm(char *filename, pgm *pgm_image, unsigned char *data)

    Saves the colored PGM into the output file.
   ____________________________________________________________________________
*/
void save_pgm(char *filename, pgm *pgm_image, unsigned char *data);

/* ____________________________________________________________________________

    void free_pgm(pgm *pgm_image)

    Frees all memory that was allocated for the PGM image.
   ____________________________________________________________________________
*/
void free_pgm(pgm *pgm_image);

#endif
