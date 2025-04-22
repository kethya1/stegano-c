# 🔐 Projet – Stéganographie en C

## Description
Ce projet réalisé en binôme implémente une méthode simple de stéganographie en langage C.

La stéganographie est l’art de cacher un message dans un support anodin (image, vidéo, son…), de manière à ce que la présence même du message passe inaperçue.
Le message sera caché dans un fichier image .bmp sans que sa qualité soit altérée à l'oeil nu.


## Fonctionnalités

- Lecture et écriture de fichiers '.bmp'.
- Encodage d’un message texte dans les LSB (Least Significant Bits) de l’image.
- Décodage du message caché.

## Structure du projet

- `mainstegano.c` : Gestion des arguments et des cas encode/decode  
- `encoder.c` : Fonctions d'encodage et de décodage  
- `stegano.h` : Prototypes et constantes  
- `image.bmp` : Image d'exemple  
- `texte.txt` : Message à cacher

##  Utilisation

### Options 
+ "-E" (Encode): Permet d'encoder un texte dans une image BMP.
+ "-D" (Decode): Permet de décoder une image BMP.
+ "-i" <image> : Chemin de l'image BMP d'entrée.
+ "-t" <texte> : Chemin du fichier texte à encoder.
+ "-o" <output> : Chemin de l'image BMP de sortie après encodage/ texte décodé dans le décodage.

### Compilation

```bash
gcc mainstegano.c encoder.c -o stegano
```
### Encodage
```bash
./stegano -E -i image.bmp -t texte.txt -o output.bmp
```


### Decodage 
```bash
./stegano -D -i output.bmp -o textedecode.txt
```

## Pistes d'amélioration

- **Support d'autres formats d'images**  
  Étendre le projet pour permettre l'encodage et le décodage dans des formats plus courants comme JPEG, PNG, etc., en plus du BMP.

- **Ajout d'un chiffrement**  
  Intégrer une phase de chiffrement du message (ex : chiffrement symétrique avec une clé partagée) avant l'encodage pour renforcer la sécurité et la confidentialité.

- **Vérification de la présence d’un message**  
  Introduire une "magic string"ou signature spécifique au début du message encodé, afin de valider automatiquement si une information cachée est réellement présente lors du décodage.

- **Encodage aléatoire des bits**  
  Modifier l’algorithme pour répartir les bits du message de façon pseudo-aléatoire dans l’image, ce qui rendrait la stéganographie encore plus difficile à détecter.


## Collaborateurs

- [@Nadine Ouelhazi](https://www.linkedin.com/in/nadine-ouelhazi/)






