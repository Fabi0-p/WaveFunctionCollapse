#ifndef FRAGMENTO_H
#define FRAGMENTO_H

#include "Imagen.h"

#define IMG_NOMBRE_TAM 100

typedef struct{
    char nombre[IMG_NOMBRE_TAM];
    t_imagen img;
}t_frag;

int copiarCelda(t_imagen* imgDest, t_imagen* const imgOrig, int x, int y);

#endif