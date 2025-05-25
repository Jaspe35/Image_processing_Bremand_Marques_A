// Fichier .c : Gestion des 3 menus
// Auteure : Flavie BREMAND
// Fichier externe au main gérant le menu utilisateur


#include <stdio.h>
#include "menu.h"
#include <unistd.h>

int type_img() { // Menu pour choisir le type de l'image
    printf("Veuillez choisir une option :\n");
    printf("\t8. bmp8 (Images en nuances de gris)\n");
    printf("\t24. bmp24 (Images en couleurs)\n");

    int type;
    scanf("%d",&type);
    while (type!=8 && type!=24) {
        printf("Veuillez choisir une option valide (soit 8, soit 24) :\n");
        scanf("%d",&type);
        sleep(10);
    }

    printf(">>> Votre choix : %d\n",type);

    return type;
}

int menu() { // Menu principal
    int option=0;

    printf("Veuillez choisir une option :\n");
    printf("\t1. Ouvrir une image\n");
    printf("\t2. Sauvegarder une image\n");
    printf("\t3. Appliquer un filtre\n");
    printf("\t4. Afficher les informations de l'image\n");
    printf("\t5. Egalisation d'histogramme\n");
    printf("\t6. Changer le type de l'image (bmp8/bmp24)\n");
    printf("\t7. Quitter\n");

    scanf("%d",&option);
    while (option<1 || option>7) {
        printf("Veuillez choisir une option valide (entre 1 et 7) :\n");
        scanf("%d",&option);
        sleep(10);
    }
    printf(">>> Votre choix : %d\n",option);

    return option;
}

int menu_3() { // Menu de tous les filtres possibles
	int option_3=0;

    printf("Veuillez choisir un filtre :\n");
    printf("\t1. Negatif\n");
    printf("\t2. Luminosite\n");
    printf("\t3. Binarisation (bmp8)/Conversion en gris (bmp24)\n");
    printf("\t4. Flou\n");
    printf("\t5. Flou gaussien\n");
    printf("\t6. Nettete\n");
    printf("\t7. Contours\n");
    printf("\t8. Relief\n");
    printf("\t9. Retourner au menu precedent\n");

    scanf("%d",&option_3);
    while (option_3<1 || option_3>9) {
        printf("Veuillez choisir une option valide (entre 1 et 9) :\n");
        scanf("%d",&option_3);
    }
    printf(">>> Votre choix : %d\n",option_3);

    return option_3;
}
