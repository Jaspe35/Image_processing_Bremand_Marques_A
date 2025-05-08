// Fichier .h : Gestion des filtres
// Auteure : Flavie BREMAND


#ifndef FILTERS_H
#define FILTERS_H

float **allocate_kernel(float values[3][3], float factor);

float **create_box_blur_kernel();

float **create_gaussian_blur_kernel();

float **create_sharpen_kernel();

float **create_outline_kernel();

float **create_emboss_kernel();

void free_kernel(float **kernel, int size);


#endif //FILTERS_H
