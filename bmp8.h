// Fichier .h : Déclarations des prototypes des fonctions
// Auteure : Noémie MARQUES

#ifndef T_BMP8_H
#define T_BMP8_H


typedef struct {
    unsigned char header[54]; // en-tête du fichier BMP
    unsigned char colorTable[1024];  // table de couleur de l’image
    unsigned char * data; // données de l’image, sous forme de tableau

    unsigned int width;  // largeur de l’image en pixels
    unsigned int height;  // la hauteur de l’image
    unsigned int colorDepth;  // profondeur de couleur de l’image
    unsigned int dataSize;  //  la taille des données de l’image en octets
} t_bmp8;


t_bmp8 *bmp8_loadImage(const char * filename);  //N

void bmp8_saveImage(const char * filename, t_bmp8 * image); //F

void bmp8_free(t_bmp8 * img); //N

void bmp8_printInfo(t_bmp8 * img); //N

void bmp8_negative(t_bmp8 * img); //N

void bmp8_brightness(t_bmp8 * img, int value); //N

void bmp8_threshold(t_bmp8 * img, int threshold); //N

void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize); //F

unsigned int * bmp8_computeHistogram(t_bmp8 * img); // A faire 3.3.1

unsigned int * bmp8_computeCDF(unsigned int * hist); // A faire 3.3.2

void bmp8_equalize(t_bmp8 * img, unsigned int * hist_eq); // A faire 3.3.3

// Ne pas oublier de changer le main pour que les égalisations d'histogramme soient disponibles.

#endif //T_BMP8_H
