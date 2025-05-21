// Fichier .c : Déclaratin des fonctions de la 2e partie
// Auteures : Noémie MARQUES (2.4 et 2.6) et Flavie BREMAND (2.3 et 2.5)


#include "t_bmp24.h"
#include <stdlib.h>
#include <math.h>
#include "bmp_constant.h"


t_pixel **bmp24_allocateDataPixels(int width, int height) {
    t_pixel **pixels = malloc(height * sizeof(t_pixel *));
    if (pixels == NULL) {
        printf("Erreur d'allocation memoire pour les lignes.\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(width * sizeof(t_pixel));
        if (pixels[i] == NULL) {
            printf("Erreur d'allocation memoire pour les colonnes.\n");
            for (int j = 0; j < i; j++) {
              free(pixels[j]);
            }
            free(pixels);
            return NULL;
        }
    }
    return pixels;
}

void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (pixels == NULL){ // Si la matrice est NULL de base
      return;
    }

    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}

t_bmp24 *bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = malloc(sizeof(t_bmp24));
    if (img == NULL) {
        printf("Erreur d'allocation memoire pour l'image.\n");
        return NULL;
    }

    img->data = bmp24_allocateDataPixels(width, height);
    if (img->data == NULL) {
        free(img);
        return NULL;
    }
    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;

    return img;
}

void bmp24_free(t_bmp24 *img) {
    if (img == NULL) {
      return;
    }

    bmp24_freeDataPixels(img->data, img->height);
    free(img);
}


void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, (long) position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
  fseek(file, (long)position, SEEK_SET);
  fwrite(buffer, size, n, file);
}

void bmp24_readPixelValue (t_bmp24 * image, int x, int y, FILE * file) {
	if (x < 0 || x >= image->width || y < 0 || y >= image->height) {
  		printf("Coordonnees de pixel hors limites.\n");
    	return;
	}

    // Calcul de la position du pixel dans le fichier BMP
    // BITMAP_OFFSET : défini à 10, indique où commencent les données d’image
    // lignes sont stockées de bas en haut (donc à l'envers), et chaque pixel prend 3 octets (B, G, R) d'où le *3
    uint32_t pixelPosition = BITMAP_OFFSET + ((image->height - y - 1) * image->width + x) * 3;

    // Lecture des composantes du pixel (ordre BGR, c'est stocké à l'envers, me demande pas pourquoi...)
    // la fonction file_rawRead est donné par le PDF
    file_rawRead(pixelPosition, &image->data[y][x].blue, sizeof(uint8_t), 1, file);
    file_rawRead(pixelPosition + 1, &image->data[y][x].green, sizeof(uint8_t), 1, file);
    file_rawRead(pixelPosition + 2, &image->data[y][x].red, sizeof(uint8_t), 1, file);

}

void bmp24_readPixelData (t_bmp24 * image, FILE * file) {
    for (int y = image->height - 1; y >= 0; y--) {
        for (int x = 0; x < image->width; x++) {
            bmp24_readPixelValue(image, x, y, file);
        }
    }

}

void bmp24_writePixelValue (t_bmp24 * image, int x, int y, FILE * file) {
	if (x < 0 || x >= image->width || y < 0 || y >= image->height) {
  		printf("Coordonnees de pixel hors limites.\n");
    	return;
	}

  t_pixel pixel = image->data[y][x];

  uint32_t pixelPosition = BITMAP_OFFSET + ((image->height - y - 1) * image->width + x) * 3;

  file_rawWrite(pixelPosition, &pixel.blue, sizeof(uint8_t), 1, file);
  file_rawWrite(pixelPosition + 1, &pixel.green, sizeof(uint8_t), 1, file);
  file_rawWrite(pixelPosition + 2, &pixel.red, sizeof(uint8_t), 1, file);

}

void bmp24_writePixelData (t_bmp24 * image, FILE * file) {

    for (int y = image->height - 1; y >= 0; y--) {
        for (int x = 0; x < image->width; x++) {
        bmp24_writePixelValue(image, x, y, file);
        }
    }
}

t_bmp24 * bmp24_loadImage (const char * filename){
    FILE * file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Erreur : impossible d’ouvrir le fichier.\n");
        return NULL;
    }
    int32_t width, height;
    uint16_t depth;
    file_rawRead(BITMAP_WIDTH,&width, sizeof(int32_t), 1, file);
    file_rawRead(BITMAP_HEIGHT,&height, sizeof(int32_t), 1, file);
    file_rawRead(BITMAP_DEPTH,&depth, sizeof(uint16_t), 1, file);

    t_bmp24 * image = bmp24_allocate(width, height, depth);
    if (image == NULL) {
        fclose(file);
        return NULL;
    }
    file_rawRead(0, &image->header, HEADER_SIZE, 1, file);
    file_rawRead(HEADER_SIZE, &image->header_info, sizeof(t_bmp_info), 1, file);

    bmp24_readPixelData (image, file);
    fclose(file);
    printf("Charger avec succes !\n");
    return image;
}

void bmp24_saveImage (t_bmp24 * img, const char * filename) {
    FILE * file = fopen(filename, "wb");

    file_rawWrite(0, &img->header, HEADER_SIZE, 1, file);
    file_rawWrite(HEADER_SIZE, &img->header_info, INFO_SIZE, 1, file);


    bmp24_writePixelData(img, file);

    if (file == NULL) {
        printf("Erreur : impossible d’ouvrir le fichier.\n");
        return;
    }

    fclose(file);
    printf("Charger avec succes !\n");
}

/*
// Sauvegarde image BMP 24 bits
void bmp24_saveImage(t_bmp24 *image, const char *filename) {
FILE *file = fopen(filename, "wb");
if (!file) return;

// Recalculer header et info
uint32_t imageSize = image->width * image->height * 3;
image->header.size = image->header.offset + imageSize;
image->info.imageSize = imageSize;

// Écrire header BMP
file_rawWrite(OFFSET_TYPE,        &image->header.type,       2, 1, file);
file_rawWrite(OFFSET_FILESIZE,    &image->header.size,       4, 1, file);
uint32_t reserved = 0;
file_rawWrite(4, &reserved, 2, 1, file);
file_rawWrite(6, &reserved, 2, 1, file);
file_rawWrite(OFFSET_DATA_OFFSET, &image->header.offset,     4, 1, file);

// Écrire BITMAPINFOHEADER
file_rawWrite(OFFSET_INFO_SIZE,        &image->info.size,        4,1,file);
file_rawWrite(OFFSET_INFO_WIDTH,       &image->info.width,       4,1,file);
file_rawWrite(OFFSET_INFO_HEIGHT,      &image->info.height,      4,1,file);
file_rawWrite(OFFSET_INFO_PLANES,      &image->info.planes,      2,1,file);
file_rawWrite(OFFSET_INFO_BITS,        &image->info.bits,        2,1,file);
file_rawWrite(OFFSET_INFO_COMPRESSION, &image->info.compression, 4,1,file);
file_rawWrite(OFFSET_INFO_IMAGE_SIZE,  &image->info.imageSize,   4,1,file);
file_rawWrite(38, &image->info.xPixelsPerMeter,4,1,file);
file_rawWrite(42, &image->info.yPixelsPerMeter,4,1,file);
file_rawWrite(46, &image->info.colorsUsed,    4,1,file);
file_rawWrite(50, &image->info.importantColors,4,1,file);

// Écrire pixels
bmp24_writePixelData(image, file);
fclose(file);
}
*/

void bmp24_negative(t_bmp24 *img) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            img->data[i][j].red   = 255 - img->data[i][j].red;
            img->data[i][j].green = 255 - img->data[i][j].green;
            img->data[i][j].blue  = 255 - img->data[i][j].blue;
        }
    }
    printf("Filtre negatif applique avec succes !\n");
}

void bmp24_grayscale(t_bmp24 *img) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            uint8_t moyenne = (img->data[i][j].red + img->data[i][j].green + img->data[i][j].blue) / 3;
            img->data[i][j].blue = moyenne;
            img->data[i][j].green = moyenne;
            img->data[i][j].red = moyenne;
        }
    }
    printf("Filtre de conversion en gris applique avec succes !\n");
}

void bmp24_brightness(t_bmp24 *img, int value) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int r = img->data[i][j].red + value;
            int g = img->data[i][j].green + value;
            int b = img->data[i][j].blue + value;

            if (r > 255) img->data[i][j].red = 255;
            else if (r < 0) img->data[i][j].red = 0;
            else img->data[i][j].red = r;

            if (g > 255) img->data[i][j].green = 255;
            else if (g < 0) img->data[i][j].green = 0;
            else img->data[i][j].green = g;

            if (b > 255) img->data[i][j].blue = 255;
            else if (b < 0) img->data[i][j].blue = 0;
            else img->data[i][j].blue = b;
        }
    }
    printf("Filtre de luminosite applique avec succes !\n");
}


t_pixel bmp24_convolution (t_bmp24 * img, int x, int y, float ** kernel, int kernelSize) {
    float red = 0, green = 0, blue = 0 ;

    if (img == NULL || kernel == NULL) {
        printf("Erreur : image ou noyau NULL\n");
        t_pixel erreur = {0, 0, 0};
        return erreur;
    }

    for (int i = -(kernelSize/2); i <= (kernelSize/2); i++) {
        for (int j = -(kernelSize/2); j <= (kernelSize/2); j++) {
            int xi = x + i;
            int yj = y + j;

            if (xi >= 0 && xi < img->width && yj >= 0 && yj < img->height) {
                t_pixel voisin = img->data[yj][xi];
                float coeff = kernel[i + (kernelSize/2)][j + kernelSize/2];

                red += (float) voisin.red * coeff;
                green += (float) voisin.green * coeff;
                blue += (float) voisin.blue * coeff;
            }
        }
    }
    if (red > 255) red = 255;
    if (green > 255) green = 255;
    if (blue > 255) blue = 255;
    if (red < 0) red = 0;
    if (green < 0) green = 0;
    if (blue < 0) blue = 0;

    t_pixel final;
    final.red = (uint8_t) red;
    final.green = (uint8_t) green;
    final.blue = (uint8_t) blue;

    printf("Convolution appliquee avec succes\n");
    return final;
}

void bmp24_boxBlur(t_bmp24 *img) {
    int kernelSize = 3;
    int offset = kernelSize / 2;

    float reference[3][3] = {
    {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
    {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
    {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
    };

    float **kernel = malloc(kernelSize * sizeof(float *));
    for (int i = 0; i < kernelSize; i++) {
        kernel[i] = malloc(kernelSize * sizeof(float));
        for (int j = 0; j < kernelSize; j++) {
		    kernel[i][j] = reference[i][j];
        }
    }

    t_bmp24 *temp = bmp24_allocate(img->width, img->height, img->colorDepth);
    if (temp == NULL) {
        fprintf(stderr, "Erreur : allocation de l’image temporaire echouee.\n");
        for (int i = 0; i < kernelSize; i++) {
          free(kernel[i]);
        }
        free(kernel);
        return;
    }

    for (int y = offset; y < img->height - offset; y++) { // on s'arrête avant les bords pour qu'il n'y ait pas de bugs
        for (int x = offset; x < img->width - offset; x++) {
            temp->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
        }
    }

    for (int y = offset; y < img->height - offset; y++) {
        for (int x = offset; x < img->width - offset; x++) {
            img->data[y][x] = temp->data[y][x];
        }
    }

    for (int i = 0; i < kernelSize; i++) {
        free(kernel[i]);
    }
    free(kernel);
    bmp24_free(temp);
    printf("Filtre applique avec succes !\n");
}

void bmp24_gaussianBlur (t_bmp24 * img){
    int kernelSize = 3;
    int offset = kernelSize / 2;

    float reference[3][3] = {
    {1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f},
    {2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f},
    {1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f},
    };

    float **kernel = malloc(kernelSize * sizeof(float *));
    for (int i = 0; i < kernelSize; i++) {
        kernel[i] = malloc(kernelSize * sizeof(float));
        for (int j = 0; j < kernelSize; j++) {
		    kernel[i][j] = reference[i][j];
        }
    }

    t_bmp24 *temp = bmp24_allocate(img->width, img->height, img->colorDepth);
    if (temp == NULL) {
        fprintf(stderr, "Erreur : allocation de l’image temporaire echouee.\n");
        for (int i = 0; i < kernelSize; i++) {
          free(kernel[i]);
        }
        free(kernel);
        return;
    }

    for (int y = offset; y < img->height - offset; y++) { // on s'arrête avant les bords pour qu'il n'y est pas de bugs
        for (int x = offset; x < img->width - offset; x++) {
            temp->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
        }
    }

    for (int y = offset; y < img->height - offset; y++) {
        for (int x = offset; x < img->width - offset; x++) {
            img->data[y][x] = temp->data[y][x];
        }
    }

    for (int i = 0; i < kernelSize; i++) {
        free(kernel[i]);
    }
    free(kernel);
    bmp24_free(temp);
    printf("Filtre applique avec succes !\n");
}

void bmp24_outline (t_bmp24 * img) {
    int kernelSize = 3;
    int offset = kernelSize / 2;

    float reference[3][3] = {
    {-1, -1, -1},
    {-1, 8, -1},
    {-1, -1, -1},
    };

    float **kernel = malloc(kernelSize * sizeof(float *));
    for (int i = 0; i < kernelSize; i++) {
        kernel[i] = malloc(kernelSize * sizeof(float));
        for (int j = 0; j < kernelSize; j++) {
		    kernel[i][j] = reference[i][j];
        }
    }

    t_bmp24 *temp = bmp24_allocate(img->width, img->height, img->colorDepth);
    if (temp == NULL) {
  		fprintf(stderr, "Erreur : allocation de l’image temporaire echouee.\n");
        for (int i = 0; i < kernelSize; i++) {
        	free(kernel[i]);
        }
       	free(kernel);
        return;
    }

    for (int y = offset; y < img->height - offset; y++) { // on s'arrête avant les bords pour qu'il n'y est pas de bugs
       for (int x = offset; x < img->width - offset; x++) {
            temp->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
       }
    }

    for (int y = offset; y < img->height - offset; y++) {
        for (int x = offset; x < img->width - offset; x++) {
            img->data[y][x] = temp->data[y][x];
        }
    }

    for (int i = 0; i < kernelSize; i++) {
        free(kernel[i]);
    }
    free(kernel);
    bmp24_free(temp);
    printf("Filtre applique avec succes !\n");
}

void bmp24_emboss (t_bmp24 * img) {
    int kernelSize = 3;
    int offset = kernelSize / 2;

    float reference[3][3] = {
    {-2, -1, 0},
    {-1, 1, 1},
    {0, 1, 2},
    };

    float **kernel = malloc(kernelSize * sizeof(float *));
    for (int i = 0; i < kernelSize; i++) {
        kernel[i] = malloc(kernelSize * sizeof(float));
        for (int j = 0; j < kernelSize; j++) {
		    kernel[i][j] = reference[i][j];
        }
    }
    t_bmp24 *temp = bmp24_allocate(img->width, img->height, img->colorDepth);
    if (temp == NULL) {
        fprintf(stderr, "Erreur : allocation de l’image temporaire echouee.\n");
        for (int i = 0; i < kernelSize; i++) {
          free(kernel[i]);
        }
        free(kernel);
        return;
    }

    for (int y = offset; y < img->height - offset; y++) { // on s'arrête avant les bords pour qu'il n'y est pas de bugs
        for (int x = offset; x < img->width - offset; x++) {
            temp->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
        }
    }

    for (int y = offset; y < img->height - offset; y++) {
        for (int x = offset; x < img->width - offset; x++) {
            img->data[y][x] = temp->data[y][x];
        }
    }

    for (int i = 0; i < kernelSize; i++) {
        free(kernel[i]);
    }

    free(kernel);
    bmp24_free(temp);
    printf("Filtre applique avec succes !\n");
}

void bmp_24_sharpen (t_bmp24 * img) {
    int kernelSize = 3;
    int offset = kernelSize / 2;

    float reference[3][3] = {
    {0, -1, 0},
    {-1, 5, -1},
    {0, -1, 0},
    };

    float **kernel = malloc(kernelSize * sizeof(float *));
    for (int i = 0; i < kernelSize; i++) {
        kernel[i] = malloc(kernelSize * sizeof(float));
        for (int j = 0; j < kernelSize; j++) {
		    kernel[i][j] = reference[i][j];
        }
    }

    t_bmp24 *temp = bmp24_allocate(img->width, img->height, img->colorDepth);
    if (temp == NULL) {
  		fprintf(stderr, "Erreur : allocation de l’image temporaire echouee.\n");
        for (int i = 0; i < kernelSize; i++) {
        	free(kernel[i]);
        }
       	free(kernel);
        return;
    }

    for (int y = offset; y < img->height - offset; y++) { // on s'arrête avant les bords pour qu'il n'y ait pas de bugs
       for (int x = offset; x < img->width - offset; x++) {
            temp->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
       }
    }

    for (int y = offset; y < img->height - offset; y++) {
        for (int x = offset; x < img->width - offset; x++) {
            img->data[y][x] = temp->data[y][x];
        }
    }

    for (int i = 0; i < kernelSize; i++) {
        free(kernel[i]);
    }

    free(kernel);
    bmp24_free(temp);
    printf("Filtre applique avec succes !\n");
}

void bmp24_equalize(t_bmp24 * img) { // Creation de la luminance et calcul de la CDF comme pour les niveaux de gris
    // Allocation un tableau YUV de la luminance
    t_yuv * data = malloc(img->width * img->height * sizeof(t_yuv));
    int hist[256] = {0};
    int cpt = 0;
    int cdf[255] = {0};
    for (int x = 0; x < img->width; x++) {
        for (int y = 0; y < img->height; y++) {
            float Y = (float) (0.299 * img->data[y][x].red + 0.587 * img->data[y][x].green + 0.114 * img->data[y][x].blue) ;
            float U = (float) (-0.14713 * img->data[y][x].red - 0.28886 * img->data[y][x].green + 0.436 * img->data[y][x].blue) ;
            float V = (float) (0.615 * img->data[y][x].red - 0.51499 * img->data[y][x].green - 0.10001 * img->data[y][x].blue) ;

            int Y_arrondi = (int) roundf(Y);
            if (Y_arrondi > 255) Y_arrondi = 255;
            if (Y_arrondi < 0) Y_arrondi = 0;

            data[cpt].Y = Y;
            data[cpt].U = U;
            data[cpt].V = V;

            hist[Y_arrondi]++;
            cpt++;
        }
    }

    // Histogramme cumulé
    cdf[0] = hist[0];
    for (int x = 1; x < 255; x++) {
        cdf[x] = cdf[x-1] + hist[x];
    }

    // Recherche du minimum pour faire la formule
    int cdf_min = cdf[0];
    for (int x = 0; x < 255; x++) {
        if (cdf[x] < cdf_min) {
            cdf_min = cdf[x];
        }
    }

    int normalisation[256];
    for (int i = 0; i < 255; i++) {
        normalisation[i] = (int) roundf((float)(cdf[i] - cdf_min) / (float)(img->height * img->width - cdf_min) * 255);
        if (normalisation[i] < 0) normalisation[i] = 0;
        if (normalisation[i] > 255) normalisation[i] = 255;
    }

    cpt = 0;
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            float Y = data[cpt].Y;
            int Y_arrondi = (int)roundf(Y);
            if (Y_arrondi > 255) Y_arrondi = 255;
            if (Y_arrondi < 0) Y_arrondi = 0;

            float Y_eq = (float)normalisation[Y_arrondi];

            // Rapport de rééchelonnage
            float ratio = Y_eq/Y ;

            // Rééchelonnage
            int newR = (int) round((double)img->data[y][x].red * ratio);
            int newG = (int) round((double)img->data[y][x].green * ratio);
            int newB = (int) round((double)img->data[y][x].blue * ratio);

            // Saturation à 255
            img->data[y][x].red   = newR > 255 ? 255 : newR;
            img->data[y][x].green = newG > 255 ? 255 : newG;
            img->data[y][x].blue  = newB > 255 ? 255 : newB;

            cpt++;
        }
    }

    free(data);
    printf("Egalisation appliquee avec succes !\n");
}
