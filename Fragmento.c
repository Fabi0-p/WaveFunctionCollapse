#include "Fragmento.h"
#include "Imagen.h"

int copiarCelda(t_imagen* imgDest, t_imagen* const imgOrig, int x, int y){
    t_color color;
    if(!imgDest || !imgOrig || !imagenAncho(imgOrig) || !imagenAlto(imgOrig))
        return IMG_NULL;
    for(int i = 0; i < imagenAncho(imgOrig); i++){
        for(int j = 0; j < imagenAlto(imgOrig); j++){
            obtenerPixel(imgOrig, &color, i, j);
            colorearPixel(imgDest, &color, x * imagenAncho(imgOrig) + i, y * imagenAlto(imgOrig) + j);
        }
    }
    return TODO_OK;
}