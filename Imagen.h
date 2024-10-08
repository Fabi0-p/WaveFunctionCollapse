#ifndef IMAGEN_H
#define IMAGEN_H

#include<stdbool.h>

#define TODO_OK 0
#define ARCHIVO_NO_ENCONTRADO 10
#define NO_SE_PUEDE_CREAR_ARCHIVO 20
#define ARCHIVO_INVALIDO 30
#define SIN_MEM 40
#define IMG_NULL 50

typedef struct {
    unsigned short int tipoDeFichero;
    unsigned int tamArchivo;
    unsigned int __reservado; // Este campo no va a ser utilizado, pero necesito dejar 4 bytes entre los otros campos
    unsigned int inicioImagen;
    // Lo de arriba es el header del archivo, lo ed abajo es el DIB: https://en.wikipedia.org/wiki/BMP_file_format
    unsigned int tamCabecera;
    unsigned int ancho;
    unsigned int alto;
    // Los siguientes tres campos no se usan directamente, pero es información que el formato necesita, y por lo tanto los tenemos que guardar
    unsigned short int numPlanos;
    unsigned short int tamPunto;
    unsigned int compresion;
    unsigned int tamImagen;
    unsigned int resHorizontal;
    unsigned int resVertical;
    unsigned int tamTablaDeColor;
    unsigned int contColoresImportantes;
}t_encabezado;

typedef struct {
    t_encabezado encabezado;
    unsigned char** pixeles;
}t_imagen;

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
}t_color;

unsigned long anchoDeFilaPixelABytes(unsigned long anchoEnPixeles);
int cargarImagen(t_imagen* img, char ruta[]);
int guardarImagen(t_imagen* img, char ruta[]);
void eliminarImagen(t_imagen* img);
bool crearImagen(t_imagen* img, unsigned long ancho, unsigned long alto, unsigned int profundidad);
bool obtenerPixel(t_imagen* const img, t_color* color, unsigned long coordX, unsigned long coordY);
bool colorearPixel(t_imagen* img, t_color* const color, unsigned long coordX, unsigned long coordY);
unsigned int imagenAlto(t_imagen* const img);
unsigned int imagenAncho(t_imagen* const img);

#endif