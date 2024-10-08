#ifndef VECTOR_H
#define VECTOR_H

#include<stdbool.h>
#include<stdlib.h>

#define VEC_TODO_OK 0
#define VEC_ES_NULL 1
#define VEC_SIN_MEM 2

typedef int (*Cmp)(void* const, void* const);
typedef void (*AccIndividual)(void*);

typedef struct {
    void* v;
    int ce;
    int cap;
    bool ordenado;
    size_t tamElem;
}t_vector;

int vectorCrear(t_vector* vec, int cap, size_t tamElem);
int vectorInsertar(t_vector* vec, void* elem);
int vectorVaciar(t_vector* vec);
void vectorEliminar(t_vector* vec, AccIndividual destructor);
char* vectorCodAStr(int cod);
void* vectorBuscar(t_vector* const vec, Cmp cmp, void* clave);

#endif
