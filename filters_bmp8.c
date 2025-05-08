// Fichier .c : Gestion des filtres
// Auteure : Flavie BREMAND


#include <stdlib.h>

#include "filters_bmp8.h"

float **allocate_kernel(float values[3][3], float factor) { // Fonction pour allouer dynamiquement les tableaux des filtres
    float **kernel = malloc(3 * sizeof(float *));
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = values[i][j] * factor;
        }
    }
    return kernel;
}

float **create_box_blur_kernel() {
    float values[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    return allocate_kernel(values, 1.0f / 9.0f); // Facteur de 1/9 d'après le document de la partie 1
}

float **create_gaussian_blur_kernel() {
    float values[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };
    return allocate_kernel(values, 1.0f / 16.0f); // Facteur de 1/16 d'après le document de la partie 1
}

float **create_sharpen_kernel() {
    float values[3][3] = {
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
    };
    return allocate_kernel(values, 1.0f);
}

float **create_outline_kernel() {
    float values[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };
    return allocate_kernel(values, 1.0f);
}

float **create_emboss_kernel() {
    float values[3][3] = {
        {-2, -1, 0},
        {-1,  1, 1},
        { 0,  1, 2}
    };
    return allocate_kernel(values, 1.0f);
}

void free_kernel(float **kernel, int size) {
    for (int i = 0; i < size; i++) {
        free(kernel[i]);
    }
    free(kernel);
}
