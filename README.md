# Image_processing_Bremand_Marques_A

Projet C - P1 A - BREMAND Flavie et MARQUES Noémie


# Remarques

Nous avons rencontré quelques problèmes pendant notre projet, nous avons donc créé une nouvelle branche contenant le projet fonctionnel (master), puis finalement, nous avons créé un nouveau projet sur GitHub à cause de problèmes persistants.


# Liste des fichiers

*lena_gray.bmp*, *lena_color.bmp*, *barbara_gray.bmp*, *flowers_color.bmp* : Les 4 images du projet pour tester le programme

*main.c* : Fichier principal de l'exécution du code - BREMAND Flavie

*menu.c* et *menu.h* : Fichiers de gestion des fonctions utiles pour pour le fonctionnement du menu - BREMAND Flavie

*ex_affichage.txt* : Affichage du menu de sélection pour visualiser le fonctionnement des menus - BREMAND Flavie

*bmp8.c* et *bmp8.h* : Fichiers contenant la *Partie 1* et la *Partie 3* du projet concernant les images en noir et blanc - BREMAND Flavie et MARQUES Noémie

*filters_bmp8.c* et *filters_bmp8.h* : Fichiers contenant les filtres et leurs noyaux de la *Partie 1* - BREMAND Flavie

*bmp_constant.h* : Fichier de constantes pour la *Partie 2* - MARQUES Noémie

*t_bmp24.c* et *t_bmp24.h* : Fichiers contenant la *Partie 2* et la *Partie 3* du projet concernant les images en couleurs - BREMAND Flavie et MARQUES Noémie


# Carnet de bord

19/03/2025 - BREMAND Flavie : Initial commit
Implémentation des images et création de l'arborescence
Début de *main.c* et des *menu.c* et *menu.h*

22/03/2025 - MARQUES Noémie : Implémentation de la fonction *loadImage* et *free*

24/03/2025 - BREMAND Flavie : Implémentation de la fonction *saveImage* et finitions de *main.c*

13/04/2025 - MARQUES Noémie : Implémentation des types *t_bmp_header*, *t_bmp_info* et *t_pixel*. Puis implémentation des fonctions *loadImage*, *file_rawRead*, *file_rawWrite*

15/04/2025 - MARQUES Noémie : Implémentation des filtres *bmp24_boxBlur*, *gaussianBlur* et outline de la *Partie 2*

18/04/2025 - BREMAND Flavie : Implémentation des filtres *applyFilter* et les noyaux des filtres de la *Partie 1*
Fin de la *Partie 1* (il reste à la tester)

19/04/2025 - MARQUES Noémie : Implémentation des filtres *emboss* et *sharpen* de la *Partie 2*

23/04/2025 - BREMAND Flavie : Fin de la *Partie 2* (il reste à la tester)

24/03/2025 - MARQUES Noémie : Amélioration de la lisibilité des filtres de la *Partie 2*

24/03/2025 - BREMAND Flavie : Eclaircissement de *main.c*

25/04/2025 : Test de la *Partie 1*, mais problème interne à CLion rendant les tests impossible à réaliser

30/04/2025 - MARQUES Noémie : Implémentation des prototypes des fonctions de la *Partie 3*

02/05/2025 - MARQUES Noémie : Essai de résolution du problème et création de la branche *master*

05/05/2025 - MARQUES Noémie : Implémentation de la fonction *bmp24_equalise* sans pouvoir la tester

08/05/2025 - BREMAND Flavie : A cause de problèmes persistants dûs à la configuration de CLion (sans solutions trouvées par nos professeurs de TD et le coordinateur du module), nouveau projet créé en copiant tous les fichiers et nouveau lien GitHub créé

12/05/2025 - BREMAND Flavie : Ecriture de phrases de debug pour trouver le problème

20/05/2025 - BREMAND Flavie : Tests de bmp8 (liés aux images en gris) fructueux, partie 3 finalisée, *main* complété avec les fonctions de bmp24 (couleurs), tests bmp24 échoués...

22/05/2025 - BREMAND Flavie : Isolation des problèmes pour bmp24 (*saveImage* et *convolution*), améliorations dans le *main*

23/05/2025 - MARQUES Noémie : Reprise complète de *saveImage* et essai fructueux de la fonction. Modification de *convolution* qui devient fonctionnelle

24/05/2025 - BREMAND Flavie : Automatisation de l'écriture des chemins des images, tests bmp24 terminés et fructueux, ajout du fichier .zip pour avoir les commits du dépôt intermédiaire, *READ.ME* complété

24/05/2025 - MARQUES Noémie : Ajout des commentaires du code, finalisation du *READ.ME* et implémentation de la fonction *printInfobmp_24*

