#ifndef LISTA_H
#define LISTA_H

#include "../Comun/Comun.h"

#define LISTA_VACIA 1

typedef tNodo *tLista;

void crearLista(tLista *p);
int listaVacia(const tLista *p);
int listaLlena(const tLista *p, unsigned cantBytes);
void vaciarLista(tLista *p);
int ponerAlComienzo(tLista *p, const void *d, unsigned cantBytes);
int sacarPrimeroLista(tLista *p, void *d, unsigned cantBytes);
int verPrimeroLista(const tLista *p, void *d, unsigned cantBytes);
int ponerAlFinal(tLista *p, const void *d, unsigned cantBytes);
int sacarUltimoLista(tLista *p, void *d, unsigned cantBytes);
int verUltimoLista(const tLista *p, void *d, unsigned cantBytes);
void recorrerLista(const tLista *p, tAccion accion, void *param);
void mostrarLista(const tLista *p, tMostrar mostrar);
int eliminarNodo(tLista *p, tNodo *nodoEliminar);

/* Comparador: retorna 1 si se cumple la condición, 0 si no */
typedef int (*tComparador)(const void *dato, void *criterio);

/* Busca y elimina el primer nodo que cumple la condición */
int eliminarPorCondicion(tLista *p, tComparador comparador, void *criterio);

#endif
