#ifndef COMUN_H
#define COMUN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO_OK 0
#define ERROR 1
#define ERR_MEMORIA 2
#define ERROR_ARCH 3
#define NO_ENCONTRADO 4
#define HAY_ESPACIO 1

#define minimo(X, Y) ((X) <= (Y) ? (X) : (Y))

typedef struct sNodo
{
    void *info;
    unsigned tamInfo;
    struct sNodo *sig;
} tNodo;

typedef struct sNodoD
{
    void *info;
    unsigned tamInfo;
    struct sNodoD *sig;
    struct sNodoD *ant;
} tNodoD;

typedef int (*tCmp)(const void* a, const void* b);
typedef int (*tAccion)(void* a, void* param);
typedef void (*tMostrar)(const void* a);

#endif
