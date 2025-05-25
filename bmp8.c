// Fichier .c : Déclarations des fonctions
// Auteures : Noémie MARQUES (N) et Flavie BREMAND (F)
// Fichier bmp8.c avec son .h :  Fonctions de la première partie, gérant les images sur 8 bits (charger, sauvegarder, appliquer un filtre et calculer l'histogramme d'une image 8 bits)

#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "bmp_constant.h"


t_bmp8 * bmp8_loadImage(const char * filename) {
    char tmp[64]=CHEMIN_IMG;
    strcat(tmp, filename);
    strcat(tmp, EXTENSION_IMG);
    // printf("Chemin : %s\n", tmp);

    FILE * file = fopen(tmp, "rb");
    if (file == NULL) {
        printf("Impossible de charger le fichier : %s\n",filename);
        fclose(file);
        return NULL;
    }

    // lecture de l'en-tête de l'image
    unsigned char header[54];
    if (fread(header, sizeof(unsigned char), 54, file) != 54) {
        printf("Erreur lors de la lecture de l'en-tete\n");
        fclose(file);
        return NULL;
    }

    // Vérification de l'image (8 bits)
    if (header[28] != 8) {
        printf ("L'image n'est pas en niveau de gris\n");
        fclose(file);
        return NULL;
    }

    // Récupération des informations de l'en-tête pour les charger
    unsigned int width = *(unsigned int*)&header[18];
    unsigned int height = *(unsigned int*)&header[22];
    unsigned int colorDepth=*(unsigned int*)&header[28];
    unsigned int dataSize = *(unsigned int*)&header[34];

    // Allocation de l'image et de son en-tête
    t_bmp8 *image = malloc(sizeof(t_bmp8)); // Allocation mémoire
    if (image == NULL) {
        printf("Erreur : Allocation memoire impossible\n");
        fclose(file);
        return NULL;
    }

    // Copie de l'en-tête dans l'image
    memcpy(image->header, header, 54);

    // Lecture de la table des couleurs
    if (fread(image->colorTable, sizeof(unsigned char), 1024, file) != 1024){
        printf("Erreur : La table de couleur ne peut pas etre chargee\n");
        free(image);
        fclose(file);
        return NULL;
    }

    // Allocation mémoire pour le contenu de l'image
    image->data = malloc(dataSize); // pas image->dataSize ici
    if (image->data == NULL) {
        printf("Erreur : Pixel non charge\n");
        free(image);
        fclose(file);
        return NULL;
    }

    // Lecture des datas
    if (fread(image->data, sizeof(unsigned char), dataSize, file) != dataSize) {
        printf("Donnees non chargees\n");
        free(image);
        fclose(file);
        return NULL;
    }

    // Initialisation des champs de l'image
    image->width = width;
    image->height = height;
    image->colorDepth = colorDepth;
    image->dataSize = dataSize;

    fclose(file);
    printf("Charger avec succes !\n");
    return image;
}


void bmp8_saveImage(const char * filename, t_bmp8 * image) {
    char tmp[64]=CHEMIN_IMG;
    strcat(tmp, filename);
    strcat(tmp, EXTENSION_IMG);
    // printf("Chemin : %s\n", tmp);

    FILE *file = fopen(tmp, "wb"); // Ouvrir le fichier en écriture binaire (car fichier .bmp)
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return;
    }

    if (fwrite(image->header, sizeof(unsigned char), 54, file) != 54) { // En-tête BMP (54 octets)
        printf("Erreur d'ecriture de l'en-tete\n");
        fclose(file);
        return;
    }

    if (fwrite(image->colorTable, sizeof(unsigned char), 1024, file) != 1024) { // Table de couleurs (1024 octets)
        printf("Erreur d'ecriture de la table de couleurs\n");
        fclose(file);
        return;
    }

    if (fwrite(image->data, sizeof(unsigned char), image->dataSize, file) != image->dataSize) { // Données de l'image (taille du tableau pointé par data)
        printf("Erreur d'ecriture des donnees de l'image\n");
        fclose(file);
        return;
    }

    fclose(file); // Fermer le fichier
    printf("Image sauvegardee avec succes dans %s\n", filename);
}


void bmp8_printInfo(t_bmp8 * img) {  // Affichage des caractéristiques de l'image
    printf("Info de l'image chargee : \n");
    printf("\tLargeur : %d \n", img->width);
    printf("\tHauteur : %d \n", img->height);
    printf("\tProfondeur de la couleur : %d \n", img->colorDepth);
    printf("\tTaille des donnees : %d \n", img->dataSize);
}


void bmp8_free(t_bmp8 * img) {
    free(img);
}


void bmp8_negative(t_bmp8 * img) {
    for (unsigned int i = 0; i < img->height; i++) {
        for (unsigned int j = 0; j < img->width; j++) {
            unsigned int index = i * img->width + j;   // utilisation de la variable index pour simuler un tableau 2D
            img->data[index] = 255 - img->data[index];  // (car data est en réalité un tableau 1D)
        }
    }
    printf("Filtre negatif applique avec succes !\n");
}


void bmp8_brightness(t_bmp8 * img, int value) {
    for (unsigned int i = 0; i < img->height; i++) {
        for (unsigned int j = 0; j < img->width; j++) {
            unsigned int index = i * img->width + j;
            int result = img->data[index] + value;
            if (result > 255) {
                img->data[index] = 255;
            } else if (result < 0) {
                img->data[index] = 0;
            } else {
                img->data[index] = (unsigned char)result;
            }
        }
    }
    printf("Filtre de luminosite applique avec succes !\n");
}


void bmp8_threshold(t_bmp8 * img, int threshold) {
    for (unsigned int i = 0; i < img->height; i++) {
        for (unsigned int j = 0; j < img->width; j++) {
            unsigned int index = i * img->width + j;
            if (img->data[index] >= threshold) {
                img->data[index] = 255;
            }
            else {
                img->data[index] = 0;
            }
        }
    }
    printf("Filtre de binarisation applique avec succes !\n");
}


void bmp8_applyFilter(t_bmp8 * img, float **kernel, int kernelSize) {
    int offset = kernelSize / 2; // Centre le noyau sur chaque pixel

    // Création d'une copie de l'image initiale
    unsigned char *newImg = (unsigned char *)malloc(img->dataSize);
    if (newImg == NULL) {
        printf("Erreur : allocation memoire pour le filtre\n");
        return;
    }

    for (unsigned int i = 0; i < img->height; i++) {
        for (unsigned int j = 0; j < img->width; j++) {
            newImg[i * img->width + j] = img->data[i * img->width + j];
        }
    }

    // Convolution
    for (unsigned int i = offset; i < img->height - offset; i++) {
        for (unsigned int j = offset; j < img->width - offset; j++) {
            float sum = 0;
            for (int ki = -offset; ki <= offset; ki++) {
                for (int kj = -offset; kj <= offset; kj++) {
                    int x = j + kj;
                    int y = i + ki;
                    int pixel = img->data[y * img->width + x];
                    sum += pixel * kernel[ki + offset][kj + offset]; // Somme des pixels qui ont été multiplié par le masque (kernel)
                }
            }

            // Les valeurs doivent être comprises dans [0;255]
            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;

            // Nouveau pixel stocké
            newImg[i * img->width + j] = (unsigned char)sum;
        }
    }

    // Remplacement des anciennes données
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = newImg[i];
    }

    printf("Filtre applique avec succes !\n");
    free(newImg);
}


unsigned int * bmp8_computeHistogram(t_bmp8 * img) {
    if (!img || !img->data) return NULL;

    // Allocation dynamique du tableau de 256 entiers
    unsigned int *hist = calloc(256, sizeof(int));
    if (!hist) {
        printf("Erreur : echec de l'allocation de l'histogramme.\n");
        return NULL;
    }

    // Remplissage de l'histogramme
    for (unsigned int i = 0; i < img->dataSize; i++) {
        unsigned char pixel = img->data[i];  // valeur entre 0 et 255
        hist[pixel]++;
    }

    return hist;
}


unsigned int * bmp8_computeCDF(unsigned int * hist) {
    if (!hist) return NULL;

    unsigned int *cdf = malloc(256 * sizeof(unsigned int));
    unsigned int *hist_eq = malloc(256 * sizeof(unsigned int));

    if (!cdf || !hist_eq) {
        fprintf(stderr, "Erreur : echec de l'allocation memoire pour la CDF.\n");
        free(cdf);
        free(hist_eq);
        return NULL;
    }

    // Étape 1 : calcul de la CDF
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    // Étape 2 : trouver cdfmin (la première valeur non nulle)
    unsigned int cdfmin = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] != 0) {
            cdfmin = cdf[i];
            break;
        }
    }

    // Étape 3 : normalisation
    unsigned int total_pixels = cdf[255];
    for (int i = 0; i < 256; i++) {
        hist_eq[i] = round(((float)(cdf[i] - cdfmin) / (total_pixels - cdfmin)) * 255);
    }

    free(cdf); // On n’a plus besoin de la CDF brute
    return hist_eq;
}


void bmp8_equalize(t_bmp8 * img) {
    if (!img || !img->data) return;

    // Calcul de l'histogramme
    unsigned int *hist = bmp8_computeHistogram(img);
    if (!hist) return;

    // Calcul de l'histogramme égalisé (CDF normalisée)
    unsigned int *hist_eq = bmp8_computeCDF(hist);
    free(hist); // on peut libérer hist après utilisation

    if (!hist_eq) return;

    // Application de l'égalisation
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = (unsigned char)hist_eq[img->data[i]];
    }

    printf("Egalisation appliquee avec succes !\n");
    free(hist_eq);
}
