// Fichier .h : Prototypes et structures de la 2e partie
// Auteures : Noémie MARQUES (2.4 et 2.6) et Flavie BREMAND (2.3 et 2.5)


#ifndef T_BMP24_H
#define T_BMP24_H


#include <stdint.h>
#include <stdio.h>


typedef struct {  // structure d'un pixel
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} t_pixel;

typedef struct {  // structure du header
	uint16_t type;
	uint32_t size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t offset;
} t_bmp_header;

typedef struct {  // structure des infos de l'image
	uint32_t size;
	int32_t width;
	int32_t height;
	uint16_t planes;
	uint16_t bits;
	uint32_t compression;
	uint32_t imagesize;
	int32_t xresolution;
	int32_t yresolution;
	uint32_t ncolors;
	uint32_t importantcolors;
} t_bmp_info;

typedef struct {  // structure du type de l'image
    t_bmp_header header; // En-tête de l'image
    t_bmp_info header_info; // Infos de l'image
    int width; // Largeur de l'image
    int height; // Hauteur de l'image
    int colorDepth; // Profondeur de couleur de l'image (24 bits)
    t_pixel **data; // Matrice avec les données de l'image
} t_bmp24;

typedef struct {  // convolution pour la luminance
  float Y;
  float U;
  float V;
} t_yuv;

t_pixel ** bmp24_allocateDataPixels (int width, int height);
void bmp24_freeDataPixels (t_pixel ** pixels, int height);
t_bmp24 * bmp24_allocate (int width, int height, int colorDepth);
void bmp24_free (t_bmp24 * img);

void bmp24_printInfo(t_bmp24 * img);


void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file);
void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file);

void bmp24_readPixelValue (t_bmp24 * image, int x, int y, FILE * file);
void bmp24_readPixelData (t_bmp24 * image, FILE * file);
void bmp24_writePixelValue (t_bmp24 * image, int x, int y, FILE * file);
void bmp24_writePixelData (t_bmp24 * image, FILE * file);

t_bmp24 * bmp24_loadImage (const char * filename);
void bmp24_saveImage (t_bmp24 * img, const char * filename);


void bmp24_negative (t_bmp24 * img);
void bmp24_grayscale (t_bmp24 * img);
void bmp24_brightness (t_bmp24 * img, int value);


t_pixel bmp24_convolution (t_bmp24 * img, int x, int y, float ** kernel, int kernelSize);
void bmp24_boxBlur (t_bmp24 * img);
void bmp24_gaussianBlur (t_bmp24 * img);
void bmp24_outline (t_bmp24 * img);
void bmp24_emboss (t_bmp24 * img);
void bmp24_sharpen (t_bmp24 * img);

void bmp24_equalize(t_bmp24 * img) ;


#endif //T_BMP24_H
