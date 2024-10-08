#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#include "Imagen.h"
#include "Vector.h"
#include "Fragmento.h"

#define DIR_U   0b00001000
#define DIR_L   0b00000100
#define DIR_R   0b00000010
#define DIR_D   0b00000001
#define DIR_ALL 0b00001111
#define DIR_DES 0b00010000

#define MAT_TAM 50
#define FRAG_TAM 5

typedef unsigned char t_cod;

t_cod obtenerCaracter(t_cod u, t_cod l, t_cod r, t_cod d);
void codigoAImagen(t_cod cod, char* rutaImagen);
void iniciarMatrizDesconocida(t_cod mat[][MAT_TAM]);
void guardarMatriz(t_cod mat[][MAT_TAM], t_vector* const fragmentos);
void popularMatriz(t_cod mat[][MAT_TAM]);
void popularVecImg(t_vector* vec);
void eliminarFrag(void* elem);

int cmpFrag(void* const elem1, void* const elem2){
    return strcmp(((t_frag*)elem1)->nombre, elem2);
}

int main(){
    t_vector vec;
    t_cod mat[MAT_TAM][MAT_TAM];

    vectorCrear(&vec, 16, sizeof(t_frag));
    popularVecImg(&vec);

    srand(time(NULL));
    iniciarMatrizDesconocida(mat);
    popularMatriz(mat);
    guardarMatriz(mat, &vec);

    vectorEliminar(&vec, eliminarFrag);
    return 0;
}

void codigoAImagen(t_cod cod, char* rutaImagen){
    switch(cod){
        case 0:
            strcpy(rutaImagen, "none.bmp\0");  // ' '
            break;
        case 9:
            strcpy(rutaImagen, "UD.bmp\0"); // '│'
            break;
        case 13:
            strcpy(rutaImagen, "ULD.bmp\0"); // '┤'
            break;
        case 5:
            strcpy(rutaImagen, "LD.bmp\0"); // '┐'
            break;
        case 10:
            strcpy(rutaImagen, "UR.bmp\0"); // '└'
            break;
        case 14:
            strcpy(rutaImagen, "ULR.bmp\0"); // '┴'
            break;
        case 7:
            strcpy(rutaImagen, "LRD.bmp\0"); // '┬'
            break;
        case 11:
            strcpy(rutaImagen, "URD.bmp\0"); // '├'
            break;
        case 6:
            strcpy(rutaImagen, "LR.bmp\0"); // '─'
            break;
        case 15:
            strcpy(rutaImagen, "ULRD.bmp\0"); // '┼'
            break;
        case 12:
            strcpy(rutaImagen, "UL.bmp\0"); // '┘'
            break;
        case 3:
            strcpy(rutaImagen, "RD.bmp\0"); // '┌'
            break;
        case 8:
            strcpy(rutaImagen, "D.bmp\0"); // 'v'
            break;
        case 4:
            strcpy(rutaImagen, "R.bmp\0");  // '>'
            break;
        case 2:
            strcpy(rutaImagen, "L.bmp\0");  // '<'
            break;
        case 1:
            strcpy(rutaImagen, "U.bmp\0");  // '^'
            break;
        default:
            strcpy(rutaImagen, "error.bmp\0");  // '?'
            break;
    }
}

t_cod obtenerCaracter(t_cod u, t_cod l, t_cod r, t_cod d){
    t_cod cod = 0;

    // Revisamos los vecinos para setear en 1 o dejar en cero. Si el vecino es desconocido elegimos cualquiera al azar.
    if(d == DIR_DES)
        cod |= DIR_D * (rand() % 2);
    else if(d & DIR_U)
        cod |= DIR_D;

    if(r == DIR_DES)
        cod |= DIR_R * (rand() % 2);
    else if(r & DIR_L)
        cod |= DIR_R;

    if(l == DIR_DES)
        cod |= DIR_L * (rand() % 2);
    else if(l & DIR_R)
        cod |= DIR_L;

    if(u == DIR_DES)
        cod |= DIR_U * (rand() % 2);
    else if(u & DIR_D)
        cod |= DIR_U;

    cod &= ~DIR_DES;

    return cod;
}

void iniciarMatrizDesconocida(t_cod mat[][MAT_TAM]){
    for(int i = 0; i < MAT_TAM; i++)
        for(int j = 0; j < MAT_TAM; j++)
            mat[i][j] = DIR_DES;
}

void guardarMatriz(t_cod mat[][MAT_TAM], t_vector* const fragmentos){
    t_imagen imgFinal;
    char nombreImagen[100];
    crearImagen(&imgFinal, FRAG_TAM * MAT_TAM, FRAG_TAM * MAT_TAM, 24);
    for(int i = 0; i < MAT_TAM; i++){
        for(int j = 0; j < MAT_TAM; j++){
            codigoAImagen(mat[MAT_TAM - j - 1][i], nombreImagen);
            copiarCelda(&imgFinal, &((t_frag*)vectorBuscar(fragmentos, cmpFrag, nombreImagen))->img, i, j);
        }
    }
    guardarImagen(&imgFinal, "Resultado.bmp");
    eliminarImagen(&imgFinal);
}

void popularMatriz(t_cod mat[][MAT_TAM]){
    t_cod u, l, r, d;
    for(int i = 0; i < MAT_TAM; i++){
        for(int j = 0; j < MAT_TAM; j++){
            if(i == 0)
                u = DIR_DES;
            else
                u = mat[i-1][j];
            if(i == MAT_TAM - 1)
                d = DIR_DES;
            else
                d = mat[i+1][j];
            if(j == 0)
                l = DIR_DES;
            else
                l = mat[i][j-1];
            if(j == MAT_TAM - 1)
                r = DIR_DES;
            else
                r = mat[i][j + 1];

            mat[i][j] = obtenerCaracter(u, l, r, d);
        }
    }
}

void popularVecImg(t_vector* vec){

    t_frag fragTemp;

    strcpy(fragTemp.nombre, "U.bmp");
    cargarImagen(&fragTemp.img, "assets/U.bmp");
    vectorInsertar(vec, &fragTemp);
    strcpy(fragTemp.nombre, "UL.bmp");
    cargarImagen(&fragTemp.img, "assets/UL.bmp");
    vectorInsertar(vec, &fragTemp);
    strcpy(fragTemp.nombre, "UR.bmp");
    cargarImagen(&fragTemp.img, "assets/UR.bmp");
    vectorInsertar(vec, &fragTemp);
    strcpy(fragTemp.nombre, "UD.bmp");
    cargarImagen(&fragTemp.img, "assets/UD.bmp");
    vectorInsertar(vec, &fragTemp);
    strcpy(fragTemp.nombre, "ULR.bmp");
    cargarImagen(&fragTemp.img, "assets/ULR.bmp");
    vectorInsertar(vec, &fragTemp);
    strcpy(fragTemp.nombre, "URD.bmp");
    cargarImagen(&fragTemp.img, "assets/URD.bmp");
    vectorInsertar(vec, &fragTemp);
    strcpy(fragTemp.nombre, "ULD.bmp");
    cargarImagen(&fragTemp.img, "assets/ULD.bmp");
    vectorInsertar(vec, &fragTemp);

    strcpy(fragTemp.nombre, "L.bmp");
    cargarImagen(&fragTemp.img, "assets/L.bmp");
    vectorInsertar(vec, &fragTemp);
    strcpy(fragTemp.nombre, "LR.bmp");
    cargarImagen(&fragTemp.img, "assets/LR.bmp");
    vectorInsertar(vec, &fragTemp);
    strcpy(fragTemp.nombre, "LD.bmp");
    cargarImagen(&fragTemp.img, "assets/LD.bmp");
    vectorInsertar(vec, &fragTemp);
    strcpy(fragTemp.nombre, "LRD.bmp");
    cargarImagen(&fragTemp.img, "assets/LRD.bmp");
    vectorInsertar(vec, &fragTemp);

    strcpy(fragTemp.nombre, "R.bmp");
    cargarImagen(&fragTemp.img, "assets/R.bmp");
    vectorInsertar(vec, &fragTemp);
    strcpy(fragTemp.nombre, "RD.bmp");
    cargarImagen(&fragTemp.img, "assets/RD.bmp");
    vectorInsertar(vec, &fragTemp);
    
    strcpy(fragTemp.nombre, "D.bmp");
    cargarImagen(&fragTemp.img, "assets/D.bmp");
    vectorInsertar(vec, &fragTemp);
    
    strcpy(fragTemp.nombre, "ULRD.bmp");
    cargarImagen(&fragTemp.img, "assets/ULRD.bmp");
    vectorInsertar(vec, &fragTemp);
    strcpy(fragTemp.nombre, "none.bmp");
    cargarImagen(&fragTemp.img, "assets/none.bmp");
    vectorInsertar(vec, &fragTemp);
    strcpy(fragTemp.nombre, "error.bmp");
    cargarImagen(&fragTemp.img, "assets/error.bmp");
    vectorInsertar(vec, &fragTemp);
}

void eliminarFrag(void* elem){
    eliminarImagen(&((t_frag*)elem)->img);
}