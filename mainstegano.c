#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stegano.h"

int main(int nbarg, char *argv[]) {
    int i;
    char *arg_img = NULL;
    char *arg_text = NULL;
    char *arg_sortie = NULL;

    if (nbarg < 2) {
        printf("Erreur: nombre d'arguments insuffisant.\n");
        return 1;
    }

    switch (argv[1][1]) { 
        case 'E': // Encodage
            for (i = 2; i < nbarg; i++) {
                if (!strcmp(argv[i], "-i") && (i + 1 < nbarg)) {
                    arg_img = argv[i + 1];
                } else if (!strcmp(argv[i], "-t") && (i + 1 < nbarg)) {
                    arg_text = argv[i + 1];
                } else if (!strcmp(argv[i], "-o") && (i + 1 < nbarg)) {
                    arg_sortie = argv[i + 1];
                }
            }

            if (arg_img && arg_text && arg_sortie) {
                Encoding(arg_img, arg_text, arg_sortie);
            } else {
                printf("Erreur: arguments manquants pour l'encodage.\n");
                printf("./stega -E -i <image.bmp> -t <texte.txt> -o <image_out.bmp>\n");
                return 1;
            }
            break;

        case 'D': // Décodage
            for (i = 2; i < nbarg; i++) {
                if (!strcmp(argv[i], "-i") && (i + 1 < nbarg)) {
                    arg_img = argv[i + 1];
                } else if (!strcmp(argv[i], "-o") && (i + 1 < nbarg)) {
                    arg_sortie = argv[i + 1];
                }
            }

            if (arg_img && arg_sortie) {
                Decoding(arg_img, arg_sortie);
            } else {
                printf("Erreur: arguments manquants pour le decodage.\n");
                printf("./stega -D -i <image.bmp> -o <texte_out.txt>\n");
                return 1;
            }
            break;

        default:
            printf("Erreur: option invalide. Utilisez -E pour encoder ou -D pour decoder.\n");
            return 1;
    }

    return 0;
}
