#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "../Comun/Comun.h"


typedef tNodoA* tArbol;

void crearArbol(tArbol *a);
void vaciarArbol(tArbol *a);
int  insertarEnArbol(tArbol *a, const void *elem, size_t tamElem, tCmp cmp);
int  buscarEnArbol(const tArbol *a, const void *clave, void *resultado, size_t tamElem, tCmp cmp);
void recorrerIn(const tArbol *a, tMostrar mostrar);
void recorrerInAccion(tArbol *a, tAccion accion, void *param);
void recorrerPreAccion(tArbol *a, tAccion accion, void *param);
void recorrerPre(const tArbol *a, tMostrar mostrar);
void recorrerPos(const tArbol *a, tMostrar mostrar);
int  contarNodosA(tArbol *a);
void eliminarHojas(tArbol *a);

#endif
