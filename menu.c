// Fichier .c : Gestion des 2 menus
// Auteure : Flavie BREMAND


#include <stdio.h>
#include "menu.h"

int type_img() {
    printf("Veuillez choisir une option :\n");
    printf("8. bmp8 (gris)\n");
    printf("24. bmp24 (couleurs)\n");

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
    printf("1. Ouvrir une image\n");
    printf("2. Sauvegarder une image\n");
    printf("3. Appliquer un filtre\n");
    printf("4. Afficher les informations de l'image\n");
    printf("5. Quitter\n");

    scanf("%d",&option);
    while (option<1 || option>5) {
      printf("Veuillez choisir une option valide (entre 1 et 5) :\n");
      scanf("%d",&option);
    }
    printf(">>> Votre choix : %d\n",option);

    return option;
}

int menu_3() {
	int option_3=0;

    // Menu de tous les filtres possibles:
    printf("Veuillez choisir un filtre :\n");
    printf("1. Negatif\n");
    printf("2. Luminosite\n");
    printf("3. Binarisation (bmp8)/Conversion en gris (bmp24)\n");
    printf("4. Flou\n");
    printf("5. Flou gaussien\n");
    printf("6. Nettete\n");
    printf("7. Contours\n");
    printf("8. Relief\n");
    printf("9. Retourner au menu precedent\n");

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