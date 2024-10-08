#include"Vector.h"
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<math.h>

int vectorCrear(t_vector* vec, int cap, size_t tamElem){
    vec->tamElem = tamElem;
    vec->ordenado = false;
    vec->cap = cap;
    vec->ce = 0;
    vec->v = malloc(cap * tamElem);
    if(!vec->v)
        return VEC_SIN_MEM;
    return VEC_TODO_OK;
}

int vectorInsertar(t_vector* vec, void* elem){
    if(!vec || !vec->v)
        return VEC_ES_NULL;

    if((float)vec->ce > (float)vec->cap * 0.9){
        vec->cap = ceil(vec->cap * 1.35);
        vec->v = realloc(vec->v, vec->cap * vec->tamElem);
        if(!vec->v)
            return VEC_SIN_MEM;
    }

    memcpy(vec->v + vec->ce * vec->tamElem, elem, vec->tamElem);
    vec->ce++;
    vec->ordenado = false;
    return VEC_TODO_OK;
}

char* vectorCodAStr(int cod){
    switch(cod){
        case VEC_TODO_OK:
            return "TODO OK";
        case VEC_ES_NULL:
            return "VECTOR ES NULL";
        case VEC_SIN_MEM:
            return "SIN MEMORIA";
        default:
            return "";
    }
}

int vectorVaciar(t_vector* vec){    
    if(!vec || !vec->v)
        return VEC_ES_NULL;
    vec->ce = 0;
    vec->ordenado = true;

    return VEC_TODO_OK;
}

void* vectorBuscar(t_vector* const vec, Cmp cmp, void* clave){
    for(int i = 0; i < vec->ce; i++){
        if(!cmp(vec->v + i * vec->tamElem, clave))
            return vec->v + i * vec->tamElem;
    }
    return NULL;
}

void vectorEliminar(t_vector* vec, AccIndividual destructor){
    if(!vec || !vec->v)
        return;
    
    for(int i; i < vec->ce; i++){
        destructor(vec->v + vec->tamElem * i);
    }

    free(vec->v);
    vec->v = NULL;
    vec->ce = 0;
    vec->cap = 0;
    vec->tamElem = 0;
}
