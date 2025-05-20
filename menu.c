// Fichier .c : Gestion des 2 menus
// Auteure : Flavie BREMAND


#include <stdio.h>
#include "menu.h"

int type_img() {
    printf("Veuillez choisir une option :\n");
    printf("\t8. bmp8 (Images en nuances de gris)\n");
    printf("\t24. bmp24 (Images en couleurs)\n");

    int type;
    scanf("%d",&type);
    while (type!=8 && type!=24) {
        printf("Veuillez choisir une option valide (soit 8, soit 24) :\n");
        scanf("%d",&type);
    }

    printf(">>> Votre choix : %d\n",type);

    return type;
}

int menu() {
    int option=0;

    printf("Veuillez choisir une option :\n");
    printf("\t1. Ouvrir une image\n");
    printf("\t2. Sauvegarder une image\n");
    printf("\t3. Appliquer un filtre\n");
    printf("\t4. Afficher les informations de l'image\n");
    printf("\t5. Egalisation d'histogramme\n");
    printf("\t6. Quitter\n");

    scanf("%d",&option);
    while (option<1 || option>6) {
      printf("Veuillez choisir une option valide (entre 1 et 6) :\n");
      scanf("%d",&option);
    }
    printf(">>> Votre choix : %d\n",option);

    return option;
}

int menu_3() {
	int option_3=0;

    // Menu de tous les filtres possibles:
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
        //getchar();
    }
    printf(">>> Votre choix : %d\n",option_3);

    return option_3;
}



/* CONTENU DU MENU:

Veuillez choisir une option :
1. Ouvrir une image
2. Sauvegarder une image
3. Appliquer un filtre
4. Afficher les informations de l'image
5. Quitter
>>> Votre choix : 1
Chemin du fichier : lena_gray.bmp
Image chargée avec succès !

Veuillez choisir une option :
1. Ouvrir une image
2. Sauvegarder une image
3. Appliquer un filtre
4. Afficher les informations de l'image
5. Quitter
>>> Votre choix : 3
Veuillez choisir un filtre :
1. Négatif
2. Luminosité
3. Binarisation
4. Flou
5. Flou gaussien
5. Netteté
6. Contours
7. Relief
8. Retourner au menu précédent
>>> Votre choix : 1
Filtre appliqué avec succès !

Veuillez choisir une option :
1. Ouvrir une image
2. Sauvegarder une image
3. Appliquer un filtre
4. Afficher les informations de l'image
5. Quitter
>>> Votre choix : 2
Chemin du fichier : lena_gray_negatif.bmp
Image sauvegardée avec succès !

Veuillez choisir une option :
1. Ouvrir une image
2. Sauvegarder une image
3. Appliquer un filtre
4. Afficher les informations de l'image
5. Quitter
>>> Votre choix : 5
*/