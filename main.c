// Fichier .c : Gestion du main
// Auteure : Flavie BREMAND


#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "bmp8.h"
#include "filters_bmp8.h"
#include "t_bmp24.h"


int verif_img_null_8(t_bmp8 *image) { // Fonction de vérification: image vide?
    if (image == NULL) {
        printf("Aucune image chargee.\n");
        return 1;
    }
    return 0;
}

int verif_img_null_24(t_bmp24 *image) { // Fonction de vérification: image vide?
    if (image == NULL) {
        printf("Aucune image chargee.\n");
        return 1;
    }
    return 0;
}

int Save_new_8(char * newname_f,t_bmp8 *image8) {
    printf("Chemin/nom de la nouvelle image : ");
    scanf("%s",newname_f);
    bmp8_saveImage(newname_f,image8);
    return 0;
}

int Save_new_24(char * newname_f,t_bmp24 *image24) {
    printf("Chemin/nom de la nouvelle image : ");
    scanf("%s",newname_f);
    bmp24_saveImage(image24,newname_f);
    return 0;
}


int main() {
    int image_processing = 1;
    char * filename = malloc(50*sizeof(char));
    char * newname = malloc(55*sizeof(char));

    int type = type_img(); // Fonction pour savoir si l'on va travailler avec une image grise ou en couleurs
    t_bmp8 * image8 = NULL;
    t_bmp24 * image24 = NULL;

    while (image_processing) {
        int option_choisie = menu(); // Fonction du menu principal
        switch (option_choisie){

            case 1:{ // Ouvrir une image
                printf("Chemin du fichier : ");
                scanf("%s",filename);
                printf("debug-1 : %s\n",filename);
                if (type==8) {
                    printf("debug0");
                    //image8 = bmp8_loadImage(filename);
                    printf("debug1");
                    Save_new_8(newname,image8);
                }
                else if (type==24) {
                    image24 = bmp24_loadImage(filename);
                    Save_new_24(newname,image24);
                }
                break;
            }

            case 2:{ // Sauvegarder l'image
                if (type==8) {
                    if (verif_img_null_8(image8)) break;
                    bmp8_saveImage(newname,image8);
                    //Save_new_8(newname,image8);
                }
                else if (type==24) {
                    if (verif_img_null_24(image24)) break;
                    bmp24_saveImage(image24,filename);
                    //Save_new_24(newname,image24);
                }
                break;
            }

            case 3:{ // Appliquer un filtre
                if (type==8) {
                    if (verif_img_null_8(image8)) break;
                }
                else if (type==24) {
                    if (verif_img_null_24(image24)) break;
                }

                int option_3=menu_3(); // Fonction du menu résultant au 3 du menu principal
                float **kernel = NULL;

                switch (option_3) {

                    case 1: { // Négatif
                        if (type==8) {
                            bmp8_negative(image8);
                            Save_new_8(newname,image8);
                        }
                        else if (type==24) {
                            bmp24_negative(image24);
                            Save_new_24(newname,image24);
                        }
                        break;
                    }

                    case 2: { // Luminosité
                        int val;
                        do {
                            printf("Valeur de luminosite (+/-) : ");
                            scanf("%d", &val);
                        } while (val < -255 || val > 255);

                        if (type==8) {
                            bmp8_brightness(image8, val);
                            Save_new_8(newname,image8);
                        }
                        else if (type==24) {
                            bmp24_brightness(image24, val);
                            Save_new_24(newname,image24);
                        }
                        break;
                    }

                    case 3: { // Binarisation (bmp8) / Conversion en gris (bmp24)
                        if (type==8) {
                            int seuil;
                            do {
                                printf("Seuil (0-255) : ");
                                scanf("%d", &seuil);
                            } while (seuil<0 || seuil>255);

                            bmp8_threshold(image8, seuil);
                            Save_new_8(newname,image8);
                        }
                        else if (type==24) {
                            bmp24_grayscale(image24);
                            Save_new_24(newname,image24);
                        }
                        break;
                    }

                    case 4: { // Flou (Box blur kernel)
                        if (type==8) {
                            kernel = create_box_blur_kernel();
                            bmp8_applyFilter(image8, kernel, 3);
                            free_kernel(kernel, 3);
                            Save_new_8(newname,image8);
                        }
                        else if (type==24) {
                            bmp24_boxBlur(image24);
                            Save_new_24(newname,image24);
                        }
                        break;
                    }

                    case 5: { // Flou gaussien (Gaussian blur)
                        if (type==8) {
                            kernel = create_gaussian_blur_kernel();
                            bmp8_applyFilter(image8, kernel, 3);
                            free_kernel(kernel, 3);
                            Save_new_8(newname,image8);
                        }
                        else if (type==24) {
                            bmp24_gaussianBlur(image24);
                            Save_new_24(newname,image24);
                        }
                        break;
                    }

                    case 6: { // Netteté (Sharpen)
                        if (type==8) {
                            kernel = create_sharpen_kernel();
                            bmp8_applyFilter(image8, kernel, 3);
                            free_kernel(kernel, 3);
                            Save_new_8(newname,image8);
                        }
                        else if (type==24) {
                            //bmp24_sharpen(image24);
                            Save_new_24(newname,image24);
                        }
                        break;
                    }

                    case 7: { // Contours (Outline)
                        if (type==8) {
                            kernel = create_outline_kernel();
                            bmp8_applyFilter(image8, kernel, 3);
                            free_kernel(kernel, 3);
                            Save_new_8(newname,image8);
                        }
                        else if (type==24) {
                            bmp24_outline(image24);
                            Save_new_24(newname,image24);
                        }
                        break;
                    }

                    case 8: { // Reliefs (Emboss)
                        if (type==8) {
                            kernel = create_emboss_kernel();
                            bmp8_applyFilter(image8, kernel, 3);
                            free_kernel(kernel, 3);
                            Save_new_8(newname,image8);
                        }
                        else if (type==24) {
                            bmp24_emboss(image24);
                            Save_new_24(newname,image24);
                        }
                        break;
                    }

                    case 9: { // Retour au menu précédent
                        break;
                    }

                    default : printf("Erreur dans la saisie du filtre choisi\n");
                }
                break;
            }

            case 4:{ // Affichage des infos de l'image
                if (type==8) {
                    if (verif_img_null_8(image8)) break;
                    bmp8_printInfo(image8);
                }
                else if (type==24) {
                    printf("L'affichage des infos d'une image en couleur n'est pas prevue ici.\n");
                }
                break;
            }

            case 5:{ // Egalisation d'histogramme
                if (type==8) {
                    bmp8_equalize(image8);
                    Save_new_8(newname,image8);
                }
                else if (type==24) {
                    bmp24_equalize(image24);
                    Save_new_24(newname,image24);
                }
                break;
            }

            case 6:{ // Quitter
                image_processing = 0;
                break;
            }

            default : printf("Erreur dans la saisie de l'option choisie\n");
        }
    }

    // Libérations finales
    if (filename != NULL)
        free(filename);
    if (image8 != NULL)
        bmp8_free(image8);
    if (image24 != NULL)
        bmp24_free(image24);

    return 0;
}
