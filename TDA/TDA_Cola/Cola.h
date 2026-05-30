#ifndef COLA_H
#define COLA_H

#include "../Comun/Comun.h"

#define ERR_COLA_VACIA 2

typedef struct
{
    tNodo *pri,
          *ult;
} tCola;

void crearCola(tCola *p);
int colaLlena(const tCola *p, unsigned cantBytes);
int ponerEnCola(tCola *p, const void *d, unsigned cantBytes);
int verPrimeroCola(const tCola* p, void *d, unsigned cantBytes);
int colaVacia(const tCola *p);
int sacarDeCola(tCola *p, void *d, unsigned cantBytes);
void vaciarCola(tCola *p);

#endif
