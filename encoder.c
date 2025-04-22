#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "stegano.h"

int taille_image(FILE *fimg) {
    int largeur, hauteur;

    // Aller à la position 0x12 dans le fichier BMP (offset de la largeur dans l'en-tête)
    fseek(fimg, 0x12, SEEK_SET);

  
    fread(&largeur, sizeof(int), 1, fimg);
    fread(&hauteur, sizeof(int), 1, fimg);
    printf("Dimensions de l'image : %d x %d\n", largeur, hauteur);

    // Revenir au début du fichier
    fseek(fimg, 0L, SEEK_SET);

    // Retourner le nombre d'octets utilisables pour l'encodage
    return ((largeur * hauteur * 3) / 8);
}

int taille_texte(FILE *ftxt) {
    fseek(ftxt, 0, SEEK_END); // Positionner le curseur à la fin du fichier texte
    int size_txt = ftell(ftxt); // Obtenir la position actuelle (taille du fichier)
    fseek(ftxt, 0, SEEK_SET); // Revenir au début du fichier
    return size_txt;
}

void texte_bit(FILE *ftxt) {
    char ch;
    int i;
    printf("Texte a inserer : \n");

    // Début du fichier
    fseek(ftxt, 0, SEEK_SET);

    while ((ch = fgetc(ftxt)) != EOF) {
        printf("%c", ch);
    }

    printf("\n\nFormat binaire du texte : \n");

    fseek(ftxt, 0, SEEK_SET); // Revenir au début pour lire à nouveau

    while ((ch = fgetc(ftxt)) != EOF) {
        for (i = 7; i >= 0; i--) {
            printf("%d", (ch >> i) & 1);
        }
        printf(" ");
    }

    printf("\n\n");
    fseek(ftxt, 0, SEEK_SET); // Revenir au début 
    return;
}

int verifier_bmp(FILE *fp) {
    uint16_t signature;

    //Lire les deux premiers octets (signature)
    fseek(fp, 0, SEEK_SET); //  au début du fichier
    fread(&signature, sizeof(uint16_t), 1, fp);

    //Vérifie la signature "BM"
    if (signature != BMP_TYPE) {
        printf( "N'est pas un fichier BMP\n"); //stderr ?
        return 0; // Fichier invalide
    }

    return 1; // Fichier valide
}
void encode_message(FILE *fimg, FILE *ftxt, FILE *fsimg) {
    unsigned char pixel[NB_OCTETS]; 
    char bit; // Un caractère du texte à encoder
    int i;    

    while ((bit = fgetc(ftxt)) != EOF) { // Lire chaque caractère du fichier texte
        for (i = 0; i < BITS_PER_BYTE; i++) { // Parcourir les 8 bits du caractère
           
            fread(pixel, sizeof(unsigned char), NB_OCTETS, fimg);

            // Étape 1 : Mettre à zéro le LSB
            pixel[0] &= 0xFE;
            pixel[1] &= 0xFE; 
            pixel[2] &= 0xFE; 
            // Étape 2 : Extraire le bit correspondant du caractère à encoder
            unsigned char bit_rouge = (bit >> (7 - i)) & 0x01; 
            unsigned char bit_vert = (bit >> (6 - i)) & 0x01;  
            unsigned char bit_bleu = (bit >> (5 - i)) & 0x01;  

            // Étape 3 : Insérer les bits extraits dans le LSB 
            pixel[0] |= bit_rouge; 
            pixel[1] |= bit_vert;  
            pixel[2] |= bit_bleu;  

            // Écrire le pixel modifié dans le fichier de sortie
            fwrite(pixel, sizeof(unsigned char), NB_OCTETS, fsimg);
        }
    }

    // Ajouter le caractère de fin '\0'
    bit = '\0';  
    for (i = 0; i < BITS_PER_BYTE; i++) { 
        fread(pixel, sizeof(unsigned char), NB_OCTETS, fimg);

        // Mettre à zéro le LSB du pixel
        pixel[0] &= 0xFE;
        pixel[1] &= 0xFE;
        pixel[2] &= 0xFE;

        // Extraire et insérer les bits du caractère '\0'
        unsigned char bit_rouge = (bit >> (7 - i)) & 0x01; 
        unsigned char bit_vert = (bit >> (6 - i)) & 0x01;  
        unsigned char bit_bleu = (bit >> (5 - i)) & 0x01;  

        // Insérer les bits extraits 
        pixel[0] |= bit_rouge; 
        pixel[1] |= bit_vert;  
        pixel[2] |= bit_bleu;  

     
        fwrite(pixel, sizeof(unsigned char), NB_OCTETS, fsimg);
    }

    //Copie du reste des données pixel
    while (fread(pixel, sizeof(unsigned char), NB_OCTETS, fimg) == NB_OCTETS) {
        fwrite(pixel, sizeof(unsigned char), NB_OCTETS, fsimg);
    }
}

 //Ajouter une erreur si fail 
void Encoding(const char *arg_img, const char *arg_text, const char *arg_sortie) {
    FILE *fimg = fopen(arg_img, "rb");
    FILE *ftxt = fopen(arg_text, "r");
    FILE *fsimg = fopen(arg_sortie, "wb");

    if (!fimg || !ftxt || !fsimg) {
        printf("Erreur : Impossible d'ouvrir un ou plusieurs fichiers/ou un fichier inexistant.\n");
        if (fimg) fclose(fimg);
        if (ftxt) fclose(ftxt);
        if (fsimg) fclose(fsimg);
        return;
    }

    //Vérifier si le fichier image est un fichier BMP
    if (!verifier_bmp(fimg)) {
        fclose(fimg);
        fclose(ftxt);
        fclose(fsimg);
        return;
    }

    //Vérif la taille de l'image
    int img_size = taille_image(fimg);
    int text_size = taille_texte(ftxt);

    if (text_size > img_size) {
        printf("Erreur : Le texte est trop long pour etre encode dans l'image.\n");
        fclose(fimg);
        fclose(ftxt);
        fclose(fsimg);
        return;
    }
    // Pour afficher le texte et son format binaire
    texte_bit(ftxt);

    // Copier l'en-tête BMP dans le fichier de sortie
    unsigned char header[BMP_HEADER];
    fread(header, sizeof(unsigned char), BMP_HEADER, fimg);
    fwrite(header, sizeof(unsigned char), BMP_HEADER, fsimg);

    // Encoder le message
    encode_message(fimg, ftxt, fsimg);
    printf("Le texte a ete encode.\n");

    fclose(fimg);
    fclose(ftxt);
    fclose(fsimg);
}


void Decoding(const char *arg_img, const char *arg_sortie) {
    FILE *fimg = fopen(arg_img, "rb");
    FILE *fout = fopen(arg_sortie, "w");

    if (!fimg || !fout) { //if  ==NULL
        printf("Erreur : Impossible d'ouvrir le fichier image ou de sortie.\n");
        if (fimg) fclose(fimg);
        if (fout) fclose(fout);
        return;
    }

    //On ignore l'en-tête BMP
    unsigned char header[BMP_HEADER];
    fread(header, sizeof(unsigned char), BMP_HEADER, fimg);

    //Lire les bits cachés et reconstruire le texte
    unsigned char pixel[NB_OCTETS];
    char ch = 0;
    int ctp_bit = 0;

    while (fread(pixel, sizeof(unsigned char), NB_OCTETS, fimg) == NB_OCTETS) {
        //recuperer le bit 
        ch = (ch << 1) | (pixel[0] & LSB_MASK);
        ctp_bit++;

        if (ctp_bit == BITS_PER_BYTE) { // Un caractère complet reconstruit
            if (ch == '\0') break; 
            fputc(ch, fout);
            printf("%c", ch); 
            ch = 0;
            ctp_bit = 0;
        }
    }

    printf("\nDecodage termine. Texte extrait ecrit dans : %s\n", arg_sortie);

    fclose(fimg);
    fclose(fout);
    return;
}
