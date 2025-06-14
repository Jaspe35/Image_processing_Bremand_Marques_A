// Fichier .c : Déclaration des fonctions de la 2e partie
// Auteures : Noémie MARQUES (2.4 et 2.6) et Flavie BREMAND (2.3 et 2.5)
// Fichier t_bmp24.c avec son .h : Fonctions de la deuxième partie, gérant les images sur 24 bits (charger, sauvegarder, appliquer un filtre et calculer l'histogramme d'une image 24 bits)


#include "t_bmp24.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bmp_constant.h"


t_pixel **bmp24_allocateDataPixels(int width, int height) // allocation mémoire des pixels
{
    t_pixel **pixels = malloc(height * sizeof(t_pixel *)); // allocation par ligne
    if (pixels == NULL) {
        printf("Erreur d'allocation memoire pour les lignes.\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) { // allocation par colonne
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

void bmp24_freeDataPixels(t_pixel **pixels, int height) { // Libération de la mémoire allouée au pixel
    if (pixels == NULL){ // Si la matrice est NULL de base
      return;
    }
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}

t_bmp24 *bmp24_allocate(int width, int height, int colorDepth) { // allocation mémoire de l'image
    t_bmp24 *img = malloc(sizeof(t_bmp24));
    if (img == NULL) {
        printf("Erreur d'allocation memoire pour l'image.\n");
        return NULL;
    }

    img->data = bmp24_allocateDataPixels(width, height); // allocation de la matrice des pixels

    if (img->data == NULL) {
        free(img);
        return NULL;
    }
    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;

    return img;
}

void bmp24_free(t_bmp24 *img) { // libération mémoire de l'image
    if (img == NULL) {
      return;
    }
    bmp24_freeDataPixels(img->data, img->height);
    free(img);
}


void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) { // fonction présente dans l'énoncé du projet
    fseek(file, (long) position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) { // fonction présente dans l'énoncé du projet
  fseek(file, (long)position, SEEK_SET);
  fwrite(buffer, size, n, file);
}

void bmp24_readPixelValue(t_bmp24 * image, int x, int y, FILE * file) { // lecture du pixel dans l'ordre BGR, ordre dans lequel est stockée l'image
    uint8_t colors[3];
    fread(colors, sizeof(uint8_t), 3, file);
    image->data[y][x].blue = colors[0];
    image->data[y][x].green = colors[1];
    image->data[y][x].red = colors[2];
}

void bmp24_readPixelData(t_bmp24 * image, FILE * file) { // lecture de la matrice avec la nécessité de gérer le padding
    int padding = (4 - (image->width * 3) % 4) % 4;

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            bmp24_readPixelValue(image, j, i, file);
        }
        fseek(file, padding, SEEK_CUR);
    }
}

void bmp24_writePixelValue(t_bmp24 * image, int x, int y, FILE * file) { // écriture du pixel dans l'ordre BGR, ordre dans lequel est stockée l'image
    uint8_t colors[3];
    colors[0] = image->data[y][x].blue;
    colors[1] = image->data[y][x].green;
    colors[2] = image->data[y][x].red;
    fwrite(colors, sizeof(uint8_t), 3, file);
}

void bmp24_writePixelData(t_bmp24 * image, FILE * file) { // écriture de la matrice avec nécessité de gérer le padding
    int padding = (4 - (image->width * 3) % 4) % 4;
    uint8_t pad[3] = {0, 0, 0}; // 3 octets de padding (au pire)

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            bmp24_writePixelValue(image, j, i, file);
        }
        fwrite(pad, sizeof(uint8_t), padding, file);
    }
}

t_bmp24 * bmp24_loadImage(const char * filename) { // chargement de l'image
    char tmp[64]=CHEMIN_IMG; // l'utilisateur tape seulement le nom de l'image
    strcat(tmp, filename);
    strcat(tmp, EXTENSION_IMG);

    FILE *f = fopen(tmp, "rb");
    if (f == NULL) {
        return NULL;
    }

    unsigned char header[54];
    fread(header, 1, 54, f); // lecture du header

    int32_t width = *(int32_t *)&header[BITMAP_WIDTH]; // assignation des champs les plus importants de l'image : la largeur, la longueur et la profondeur des couleurs
    int32_t height = *(int32_t *)&header[BITMAP_HEIGHT];
    uint16_t colorDepth = *(uint16_t *)&header[BITMAP_DEPTH];

    t_bmp24 *img = bmp24_allocate(width, height, colorDepth);
    if (img == NULL) {
        fclose(f);
        return NULL;
    }

    uint32_t offset = *(uint32_t *)&header[BITMAP_OFFSET]; // assignation du bon offset pour une bonne lecture du header
    fseek(f, offset, SEEK_SET);

    bmp24_readPixelData(img, f);

    fclose(f);
    printf("Charger avec succes !\n");
    return img;
}

void bmp24_saveImage(t_bmp24 * img, const char * filename) { // sauvegarde de l'image
    char tmp[64]=CHEMIN_IMG; // l'utilisateur tape seulement le nom de la nouvelle image
    strcat(tmp, filename);
    strcat(tmp, EXTENSION_IMG);

    FILE *f = fopen(tmp, "wb");
    if (f == NULL) {
        return;
    }

    // Réécriture des informations de l'image
    int padding = (4 - (img->width * 3) % 4) % 4;
    uint32_t rowSize = img->width * 3 + padding;
    uint32_t imageSize = rowSize * img->height;
    uint32_t fileSize = HEADER_SIZE + INFO_SIZE + imageSize;

    t_bmp_header header;
    header.type = BMP_TYPE;
    header.size = fileSize;
    header.reserved1 = 0;
    header.reserved2 = 0;
    header.offset = HEADER_SIZE + INFO_SIZE;

    fwrite(&header.type, sizeof(uint16_t), 1, f);
    fwrite(&header.size, sizeof(uint32_t), 1, f);
    fwrite(&header.reserved1, sizeof(uint16_t), 1, f);
    fwrite(&header.reserved2, sizeof(uint16_t), 1, f);
    fwrite(&header.offset, sizeof(uint32_t), 1, f);

    t_bmp_info info;
    info.size = INFO_SIZE;
    info.width = img->width;
    info.height = img->height;
    info.planes = 1;
    info.bits = 24;
    info.compression = 0;
    info.imagesize = imageSize;
    info.xresolution = 0x0B13;
    info.yresolution = 0x0B13;
    info.ncolors = 0;
    info.importantcolors = 0;

    fwrite(&info, sizeof(t_bmp_info), 1, f);

    bmp24_writePixelData(img, f);

    fclose(f);
    printf("Image sauvegardee avec succes dans %s\n", filename);
}


// Filtres que l'on peut appliquer à une image :

void bmp24_negative(t_bmp24 *img) { // Inverser les couleurs de l'image
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            img->data[i][j].red   = 255 - img->data[i][j].red;
            img->data[i][j].green = 255 - img->data[i][j].green;
            img->data[i][j].blue  = 255 - img->data[i][j].blue;
        }
    }
    printf("Filtre negatif applique avec succes !\n");
}

void bmp24_grayscale(t_bmp24 *img) { // Mettre en teinte de gris
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

void bmp24_brightness(t_bmp24 *img, int value) { // Augmenter ou baisser la luminosité
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

// Application des filtres qui nécessitent un noyau
t_pixel bmp24_convolution (t_bmp24 * img, int x, int y, float ** kernel, int kernelSize) {
    float red = 0, green = 0, blue = 0 ;

    if (img == NULL || kernel == NULL) {
        printf("Erreur : image ou noyau NULL\n");
        t_pixel erreur = {0, 0, 0};
        return erreur;
    }

    for (int i = -(kernelSize/2); i <= (kernelSize/2); i++) { // calcul de la manière dont les pixels vont être regroupés pour subir une modification
        for (int j = -(kernelSize/2); j <= (kernelSize/2); j++) {
            int xi = x + i;
            int yj = y + j;

            if (xi >= 0 && xi < img->width && yj >= 0 && yj < img->height) { // calcul des nouveaux pixels
                t_pixel voisin = img->data[yj][xi];
                float coeff = kernel[i + (kernelSize/2)][j + kernelSize/2];

                red += (float) voisin.red * coeff;
                green += (float) voisin.green * coeff;
                blue += (float) voisin.blue * coeff;
            }
        }
    }
    if (red > 255) red = 255; // recalibrage de la couleur des pixels si la couleur calculée dépasse les valeurs limites
    if (green > 255) green = 255;
    if (blue > 255) blue = 255;
    if (red < 0) red = 0;
    if (green < 0) green = 0;
    if (blue < 0) blue = 0;

    t_pixel final;
    final.red = (uint8_t) red;
    final.green = (uint8_t) green;
    final.blue = (uint8_t) blue;

    return final;
}

void bmp24_boxBlur(t_bmp24 *img) { // Flou
    int kernelSize = 3;
    int offset = kernelSize / 2;

    float reference[3][3] = {
    {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
    {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
    {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
    };

    float **kernel = malloc(kernelSize * sizeof(float *)); // allocation dynamique du noyau en fonction des matrices de référence
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

    for (int y = offset; y < img->height - offset; y++) { // calcul de la nouvelle image
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

// Les fonctions de filtres ne diffèrent pas beaucoup, à part sur la base du noyau considéré

void bmp24_gaussianBlur (t_bmp24 * img){ // Flou gaussien
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

    for (int y = offset; y < img->height - offset; y++) {
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

void bmp24_outline (t_bmp24 * img) { // Détection des contours de l'image
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

void bmp24_emboss (t_bmp24 * img) { // Relief
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

void bmp24_sharpen (t_bmp24 * img) { // Netteté
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

// Création de l'histogramme
void bmp24_equalize(t_bmp24 * img) {
    // Création de la luminance et calcul de la CDF comme pour les niveaux de gris
    // Allocation un tableau YUV de la luminance
    t_yuv * data = malloc(img->width * img->height * sizeof(t_yuv));
    int hist[256] = {0};
    int cpt = 0;
    int cdf[256] = {0};

    for (int y = 0; y < img->height; y++) {  // recalcul de la luminance de chaque pixel
        for (int x = 0; x < img->width; x++) {
            float Y = (float) (0.299 * img->data[y][x].red + 0.587 * img->data[y][x].green + 0.114 * img->data[y][x].blue) ;

            int Y_arrondi = (int) roundf(Y);
            if (Y_arrondi > 255) Y_arrondi = 255;
            if (Y_arrondi < 0) Y_arrondi = 0;

            data[cpt].Y = Y;

            hist[Y_arrondi]++;
            cpt++;
        }
    }

    cdf[0] = hist[0]; // histogramme cumulé
    for (int x = 1; x < 256; x++) {
        cdf[x] = cdf[x-1] + hist[x];
    }

    int cdf_min = cdf[0]; // recherche du minimum pour faire la formule
    for (int x = 0; x < 256; x++) {
        if (cdf[x] < cdf_min) {
            cdf_min = cdf[x];
        }
    }

    int normalisation[256]; // formule indiquée dans le cours
    for (int i = 0; i < 256; i++) {
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

            float ratio = Y_eq/Y ; // rapport de rééchelonnage

            int newR = (int) round((double)img->data[y][x].red * ratio); // rééchelonnage
            int newG = (int) round((double)img->data[y][x].green * ratio);
            int newB = (int) round((double)img->data[y][x].blue * ratio);

            img->data[y][x].red   = newR > 255 ? 255 : newR;
            img->data[y][x].green = newG > 255 ? 255 : newG;
            img->data[y][x].blue  = newB > 255 ? 255 : newB;

            cpt++;
        }
    }

    free(data);
    printf("Egalisation appliquee avec succes !\n");
}

void bmp24_printInfo(t_bmp24 * img) { // Affichage des caractéristiques de l'image
    printf("Info de l'image chargee : \n");
    printf("\tLargeur : %d \n", img->header_info.width);
    printf("\tHauteur : %d \n", img->header_info.height);
    printf("\tTaille : %d \n", img->header_info.size);
    printf("\tBpp : %d \n", img->header_info.bits);
    printf("\tTaille des donnees : %d \n", img->header_info.imagesize);
    printf("\tNombre de couleur : %d \n", img->header_info.ncolors);
}