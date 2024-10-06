#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define DIR_U   0b00001000
#define DIR_L   0b00000100
#define DIR_R   0b00000010
#define DIR_D   0b00000001
#define DIR_ALL 0b00001111
#define DIR_DES 0b00010000

#define MAT_TAM 30

typedef unsigned char t_cod;

t_cod obtenerCaracter(t_cod u, t_cod l, t_cod r, t_cod d);
char codigoAChar(t_cod cod);
void iniciarMatrizDesconocida(t_cod mat[][MAT_TAM]);
void mostrarMatriz(t_cod mat[][MAT_TAM]);
void popularMatriz(t_cod mat[][MAT_TAM]);

int main(){
    t_cod mat[MAT_TAM][MAT_TAM];

    srand(time(NULL));
    iniciarMatrizDesconocida(mat);
    popularMatriz(mat);
    mostrarMatriz(mat);
}

char codigoAChar(t_cod cod){
    switch(cod){
        case 0:
            return 32;  // ' '
        case 9:
            return 179; // '│'
        case 13:
            return 180; // '┤'
        case 5:
            return 191; // '┐'
        case 10:
            return 192; // '└'
        case 14:
            return 193; // '┴'
        case 7:
            return 194; // '┬'
        case 11:
            return 195; // '├'
        case 6:
            return 196; // '─'
        case 15:
            return 197; // '┼'
        case 12:
            return 217; // '┘'
        case 3:
            return 218; // '┌'
        case 8:
            return 118; // 'v'
        case 4:
            return 62;  // '>'
        case 2:
            return 60;  // '<'
        case 1:
            return 94;  // '^'
        default:
            return 63;  // '?'
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

void mostrarMatriz(t_cod mat[][MAT_TAM]){
    FILE* f;
    f = fopen("MATRIZ.txt", "w");

    for(int i = 0; i < MAT_TAM; i++){
        for(int j = 0; j < MAT_TAM; j++){
            fprintf(f, "%c", codigoAChar(mat[i][j]));
            // printf("%4.d", mat[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
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
