#ifndef PILA_H
#define PILA_H

#include "../Comun/Comun.h"

#define PILA_VACIA 1
#define PILA_LLENA 0

typedef tNodo* tPila;

void crearPila(tPila* p);
int pilaLlena(const tPila* p, unsigned cantBytes);
int pilaVacia(const tPila* p);
void vaciarPila(tPila* p);
int apilar(tPila* p, const void* d, unsigned cantBytes);
int desapilar(tPila* p, void* d, unsigned cantBytes);
int verTope(const tPila* p, void* d, unsigned cantBytes);


#endif
