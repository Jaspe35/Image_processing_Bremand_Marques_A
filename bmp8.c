// Fichier .c : Déclarations des fonctions
// Auteures : Noémie MARQUES (N) et Flavie BREMAND (F)


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "bmp8.h"



t_bmp8 * bmp8_loadImage(const char * filename) {
    FILE * file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Impossible de charger le fichier\n");
        printf(" %s\n",filename); //debug
        fclose(file);
        return NULL;
    }
    else {
        printf("Charger avec succes !\n");
	}

    // lecture de l'en-tête
    unsigned char header[54];
    if (fread(header, sizeof(unsigned char), 54, file) != 54) {
        printf("Erreur lors de la lecture de l'en-tete\n");
        fclose(file);
        return NULL;
    }

    // Vérification de l'image (8 bits)
    if (header[28] != 8) {
        printf ("L'image n'est pas en niveau de gris");
       fclose(file);
        return NULL;
    }

    // Récupération des informations de l'en-tête
    unsigned int width = *(unsigned int*)&header[18];
    unsigned int height = *(unsigned int*)&header[22];
    unsigned int colorDepth=*(unsigned int*)&header[28];
    unsigned int dataSize = *(unsigned int*)&header[34];

    // Allocation de l'image
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

    // Allocation mémoire pour l'image
    image->data = malloc(dataSize); // pas image->dataSize ici
    image->data = (unsigned char *)malloc(image->dataSize);
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
    };

    // Initialisation des champs de l'image
    image->width = width;
    image->height = height;
    image->colorDepth = colorDepth;
    image->dataSize = dataSize;

    fclose(file);
    return image;
}

void bmp8_saveImage(const char * filename, t_bmp8 * image) {
      FILE *file = fopen(filename, "wb"); // Ouvrir le fichier en écriture binaire (car fichier .bmp)
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


void bmp8_printInfo(t_bmp8 * img) {
  printf("Info de l'image chargee : \n");
  printf("\tWidth : %d \n", img->width);
  printf("\tHeight : %d \n", img->height);
  printf("\tColor Depth : %d \n", img->colorDepth);
  printf("\tDataSize : %d \n", img->dataSize);
}

void bmp8_free(t_bmp8 * img) {
  free(img);
}

void bmp8_negative(t_bmp8 * img) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            unsigned int index = i * img->width + j;   // utilisation de la variable index pour simuler un tableau 2D
            img->data[index] = 255 - img->data[index];  //(car data est en réalité un tableau 1D)
        }
    }
}

void bmp8_brightness(t_bmp8 * img, int value) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
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
}


void bmp8_threshold(t_bmp8 * img, int threshold) {
  for (int i = 0; i < img->height; i++) {
    for (int j = 0; j < img->width; j++) {
      unsigned int index = i * img->width + j;
      if (img->data[index] >= threshold) {
        img->data[index] = 255;
      }
      else {
        img->data[index] = 0;
      }
    }
  }
}


void bmp8_applyFilter(t_bmp8 * img, float **kernel, int kernelSize) {
    int offset = kernelSize / 2; // Centre le noyau sur chaque pixel

    // Création d'une copie de l'image initiale
    unsigned char *newImg = (unsigned char *)malloc(img->dataSize);
    if (newImg == NULL) {
        printf("Erreur : allocation memoire pour le filtre\n");
        return;
    }
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            newImg[i * img->width + j] = img->data[i * img->width + j];
        }
    }

    // Convolution
    for (int i = offset; i < img->height - offset; i++) {
        for (int j = offset; j < img->width - offset; j++) {
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
    for (int i = 0; i < img->dataSize; i++) {
        img->data[i] = newImg[i];
    }

    free(newImg);
}
