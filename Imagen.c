#include "Imagen.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int cargarImagen(t_imagen* img, char ruta[]){
    int i, error;
    FILE* f;
    f = fopen(ruta, "rb");
    if(!f)
        return ARCHIVO_NO_ENCONTRADO;

    // Tenemos que cargar todos los campos manualmente porque el short deja un padding de 2 bytes, lo que hace que se cargue mal todos los campos que siguen
    fread(&img->encabezado.tipoDeFichero, sizeof(short), 1, f);
    fread(&img->encabezado.tamArchivo, sizeof(int), 1, f);
    fread(&img->encabezado.__reservado, sizeof(int), 1, f);
    fread(&img->encabezado.inicioImagen, sizeof(int), 1, f);
    fread(&img->encabezado.tamCabecera, sizeof(int), 1, f);
    fread(&img->encabezado.ancho, sizeof(int), 1, f);
    fread(&img->encabezado.alto, sizeof(int), 1, f);
    fread(&img->encabezado.numPlanos, sizeof(short), 1, f);
    fread(&img->encabezado.tamPunto, sizeof(short), 1, f);
    fread(&img->encabezado.compresion, sizeof(int), 1, f);
    fread(&img->encabezado.tamImagen, sizeof(int), 1, f);
    fread(&img->encabezado.resHorizontal, sizeof(int), 1, f);
    fread(&img->encabezado.resVertical, sizeof(int), 1, f);
    fread(&img->encabezado.tamTablaDeColor, sizeof(int), 1, f);
    fread(&img->encabezado.contColoresImportantes, sizeof(int), 1, f);

    if(img->encabezado.tamPunto != 24)
    {
        printf("\nERROR: Solo se soportan archivos de 24 bits por pixel y el archivo seleccionado tiene %hu bits por pixel\n", img->encabezado.tamPunto);
        fclose(f);
        exit(ARCHIVO_INVALIDO);
    }

    fseek(f, img->encabezado.inicioImagen, SEEK_SET);

    img->pixeles = malloc(img->encabezado.alto * sizeof(char*));

    if(!img->pixeles)
        return SIN_MEM;

    error = 0;
    for(i = 0; i < img->encabezado.alto; i++){
        img->pixeles[i] = malloc(anchoDeFilaPixelABytes(img->encabezado.ancho));
        if(!img->pixeles[i])
            error = 1;
    }
    if(error){
        for(i = 0; i < img->encabezado.alto; i++){
            if(img->pixeles[i]){
                free(img->pixeles[i]);
                img->pixeles[i] = NULL;
            }
        }
        free(img->pixeles);
        img->pixeles = NULL;
        return SIN_MEM;
    }

    for(i = 0; i < img->encabezado.alto; i++){
    	fread(img->pixeles[i], sizeof(char), anchoDeFilaPixelABytes(img->encabezado.ancho), f);
    }

    fclose(f);

    return TODO_OK;
}

int guardarImagen(t_imagen* const img, char ruta[]){
    int i;
    FILE* f;
    
    f = fopen(ruta, "wb");
    if(!f)
        return NO_SE_PUEDE_CREAR_ARCHIVO;

    // Tenemos que guardar todos los campos manualmente porque el short deja un padding de 2 bytes, lo que hace que se guarden mal todos los campos que siguen
    fwrite(&img->encabezado.tipoDeFichero, sizeof(short), 1, f);
    fwrite(&img->encabezado.tamArchivo, sizeof(int), 1, f);
    fwrite(&img->encabezado.__reservado, sizeof(int), 1, f);
    fwrite(&img->encabezado.inicioImagen, sizeof(int), 1, f);
    fwrite(&img->encabezado.tamCabecera, sizeof(int), 1, f);
    fwrite(&img->encabezado.ancho, sizeof(int), 1, f);
    fwrite(&img->encabezado.alto, sizeof(int), 1, f);
    fwrite(&img->encabezado.numPlanos, sizeof(short), 1, f);
    fwrite(&img->encabezado.tamPunto, sizeof(short), 1, f);
    fwrite(&img->encabezado.compresion, sizeof(int), 1, f);
    fwrite(&img->encabezado.tamImagen, sizeof(int), 1, f);
    fwrite(&img->encabezado.resHorizontal, sizeof(int), 1, f);
    fwrite(&img->encabezado.resVertical, sizeof(int), 1, f);
    fwrite(&img->encabezado.tamTablaDeColor, sizeof(int), 1, f);
    fwrite(&img->encabezado.contColoresImportantes, sizeof(int), 1, f);

    fseek(f, img->encabezado.inicioImagen, SEEK_SET);
    for(i = 0; i < img->encabezado.alto; i++){
        fwrite(img->pixeles[i], sizeof(char), anchoDeFilaPixelABytes(img->encabezado.ancho), f);
    }

    fclose(f);
    return TODO_OK;
}

void eliminarImagen(t_imagen* img){
    int i;
    if(img->pixeles)
    {
        for(i = 0; i < img->encabezado.alto; i++){
            free(img->pixeles[i]);
            img->pixeles[i] = NULL;
        }
        free(img->pixeles);
        img->pixeles = NULL;
    }
}

bool obtenerPixel(t_imagen* const img, t_color* color, unsigned long coordX, unsigned long coordY){
    if(img->encabezado.tamPunto != 24)
        return false;
    if(coordX >= img->encabezado.ancho || coordY >= img->encabezado.alto)
        return false;

    coordX *= 3;
    // Las imagenes se guardan como BGR en lugar de RGB
    color->b = img->pixeles[coordY][coordX];
    color->g = img->pixeles[coordY][coordX+1];
    color->r = img->pixeles[coordY][coordX+2];

    return true;
}

bool colorearPixel(t_imagen* img, t_color* const color, unsigned long coordX, unsigned long coordY){
    if(img->encabezado.tamPunto != 24)
        return false;
    if(coordX > img->encabezado.ancho || coordY > img->encabezado.alto)
        return false;

    coordX *= 3;
    // Las imagenes se guardan como BGR en lugar de RGB
    img->pixeles[coordY][coordX] = color->b;
    img->pixeles[coordY][coordX+1] = color->g;
    img->pixeles[coordY][coordX+2] = color->r;
    return true;
}

bool crearImagen(t_imagen* img, unsigned long ancho, unsigned long alto, unsigned int profundidad){
    int i, j, error;
    unsigned long filaAnchoBytes, tamCabecera, offsetPixeles, tamImagen, tamArchivo;
    short tamTablaDeColor;

    if(profundidad != 24)
        return false;

    filaAnchoBytes = anchoDeFilaPixelABytes(ancho);
    tamCabecera = 40; // Esta cabecera es el DIB, sin incluir la primer parte de header
    offsetPixeles = tamCabecera + 14; // 14 bytes del primer header
    tamTablaDeColor = 0;

    tamImagen = filaAnchoBytes * alto;
    tamArchivo = tamCabecera + tamImagen + 14 + (tamTablaDeColor * 4); // La cabecera DIB, la otra cabecera (14 bytes), la tabla de color, y la imagen.

    img->encabezado.tipoDeFichero = 19778; // "BM"
    img->encabezado.tamArchivo = tamArchivo;
    img->encabezado.__reservado = 0;
    img->encabezado.inicioImagen = offsetPixeles;
    // Comienzo del DIB
    img->encabezado.tamCabecera = tamCabecera;
    img->encabezado.ancho = ancho;
    img->encabezado.alto = alto;
    img->encabezado.numPlanos = 1;
    img->encabezado.tamPunto = profundidad;
    img->encabezado.compresion = 0;
    img->encabezado.tamImagen = tamImagen;
    // Ambas resoluciones las dejamos en un valor por defecto. Para los fines de este TP no afecta, ya que esto es para el tamaño de impresion, lo cual no nos interesa
    // En https://stackoverflow.com/questions/17550545/bmp-image-header-bixpelspermeter se menciona que en Windows lo tradicional son 92 DPI (Dots Per Inch), que equivale a aproximadamente 3779 DPM (Dots Per Meter)
    img->encabezado.resHorizontal = 3779;
    img->encabezado.resVertical = 3779;
    img->encabezado.tamTablaDeColor = tamTablaDeColor;
    img->encabezado.contColoresImportantes = 0;

    img->pixeles = malloc(alto * sizeof(char*));
    if(!img->pixeles)
        return false;

    error = 0;
    for(i = 0; i < alto; i++){
        img->pixeles[i] = malloc(filaAnchoBytes);
        if(!img->pixeles[i])
            error = 1;
    }

    if(error)
        return false;

    for(i = 0; i < alto; i++)
        for(j = 0; j < filaAnchoBytes; j++)
            img->pixeles[i][j] = 0;

    return true;
}

unsigned int imagenAlto(t_imagen* const img){
    if(!img)
        return 0;
    return img->encabezado.alto;
}

unsigned int imagenAncho(t_imagen* const img){
    if(!img)
        return 0;
    return img->encabezado.ancho;
}

unsigned long anchoDeFilaPixelABytes(unsigned long anchoEnPixeles)
{
    // En formato BMP, el ancho de cada fila en bytes se redondea hacia arriba al proximo multiplo de 4
    // Esta operacion divide la cantidad de bytes entre 4, toma el techo, y lo multiplica por 4.
    // Esto deberia redondear al multiplo de 4 mas cercano.
    return ceil((float)(anchoEnPixeles * 3.0) / 4.0) * 4;
}
