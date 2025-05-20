// Fichier .c : Gestion du main
// Auteure : Flavie BREMAND


#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "bmp8.h"
#include "filters_bmp8.h"


int verif_img_null(t_bmp8 *image) { // Fonction de vérification: image vide?
    if (image == NULL) {
        printf("Aucune image chargee.\n");
        return 1;
    }
    return 0;
}


int main() {
    int image_processing = 1;
    t_bmp8 * image = NULL;
    char * filename = malloc(50*sizeof(char));

    while (image_processing) {
        int option_choisie = menu(); // Fonction du menu principal
        switch (option_choisie){

            case 1:{ // Ouvrir une image
                printf("Chemin du fichier : ");
                scanf(" %s",filename);
                image = bmp8_loadImage(filename);
                bmp8_saveImage(filename,image);
                break;
            }

            case 2:{ // Sauvegarder l'image
                if (verif_img_null(image)) break;
                printf("Chemin du fichier : ");
                scanf(" %s",filename);
                bmp8_saveImage(filename, image);
                break;
            }

            case 3:{ // Appliquer un filtre
                if (verif_img_null(image)) break;

                int option_3=menu_3(); // Fonction du menu résultant au 3 du menu principal
                float **kernel = NULL;

                switch (option_3) {

                    case 1: { // Négatif
                        bmp8_negative(image);
                        bmp8_saveImage(filename, image);
                        break;
                    }

                    case 2: { // Luminosité
                        int val;
                        do {
                            printf("Valeur de luminosité (+/-) : ");
                            scanf("%d", &val);
                        } while (val < -255 || val > 255);

                        bmp8_brightness(image, val);
                        bmp8_saveImage(filename, image);
                        break;
                    }

                    case 3: { // Binarisation
                        int seuil;
                        do {
                            printf("Seuil (0-255) : ");
                            scanf("%d", &seuil);
                        } while (seuil<0 || seuil>255);

                        bmp8_threshold(image, seuil);
                        bmp8_saveImage(filename, image);
                        break;
                    }

                    case 4: { // Flou (Box blur kernel)
                        kernel = create_box_blur_kernel();
                        bmp8_applyFilter(image, kernel, 3);
                        free_kernel(kernel, 3);
                        bmp8_saveImage(filename, image);
                        break;
                    }

                    case 5: { // Flou gaussien (Gaussian blur)
                        kernel = create_gaussian_blur_kernel();
                        bmp8_applyFilter(image, kernel, 3);
                        free_kernel(kernel, 3);
                        bmp8_saveImage(filename, image);
                        break;
                    }

                    case 6: { // Netteté (Sharpen)
                        kernel = create_sharpen_kernel();
                        bmp8_applyFilter(image, kernel, 3);
                        free_kernel(kernel, 3);
                        bmp8_saveImage(filename, image);
                        break;
                    }

                    case 7: { // Contours (Outline)
                        kernel = create_outline_kernel();
                        bmp8_applyFilter(image, kernel, 3);
                        free_kernel(kernel, 3);
                        bmp8_saveImage(filename, image);
                        break;
                    }

                    case 8: { // Reliefs (Emboss)
                        kernel = create_emboss_kernel();
                        bmp8_applyFilter(image, kernel, 3);
                        free_kernel(kernel, 3);
                        bmp8_saveImage(filename, image);
                        break;
                    }

                    case 9: { // Retour au menu précédent
                        break;
                    }

                    default : printf("Erreur dans la saisie du filtre choisi");
                }
                break;
            }

            case 4:{ // Affichage des infos de l'image
                if (verif_img_null(image)) break;

                bmp8_printInfo(image);
                break;
            }

            case 5:{ // Quitter
                image_processing = 0;
                break;
            }

            default : printf("Erreur dans la saisie de l'option choisie");
        }

    }

    // Libérations finales
    if (filename != NULL)
        free(filename);
    if (image != NULL)
        bmp8_free(image);

    return 0;
}
