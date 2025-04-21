#ifndef STEGANO_H
#define STEGANO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BMP_HEADER 54  // Taille de l'en-tête BMP
#define BITS_PER_BYTE 8 // Nombre de bits par octet
#define NB_OCTETS 3 // Nombre d'octets par pixel (RVB)
#define LSB_MASK 0x01 // Masque pour récupérer le LSB du caractère message
#define MASK 0xFE // Masque pour effacer le LSB
#define BMP_TYPE 0x4D42 // Type de fichier BMP


int taille_image(FILE *fimg);
int taille_texte(FILE *ftxt);
int verifier_bmp(FILE *fp);
void encode_message(FILE *fimg, FILE *ftxt, FILE *fsimg) ;
void texte_bit(FILE *ftxt);
void Encoding(const char *arg_img, const char *arg_text, const char *arg_sortie);
void Decoding(const char *arg_img, const char *arg_sortie);


#endif
 
