#ifndef DATOS_H_INCLUDED
#define DATOS_H_INCLUDED

#include "Tipos.h"
#include "TDA/TDA_Arbol/arbol.h"

#define ARCHIVO_JUGADORES "jugadores.dat"
#define ARCHIVO_PARTIDAS  "partidas.dat"
#define ARCHIVO_BST       "bst.dat"

int cmpNombre(const void* a, const void* b);

void cargarBST(tArbol* arbol);
void persistirBST(tArbol* arbol);

int guardarJugador(const registroJugador* j);
int buscarOCrearJugador(tArbol* arbol, const char* nombre, registroJugador* resultado);

int guardarPartida(const registroPartida* p);
int obtenerPuntosJugador(int idJugador);

void mostrarRanking(tArbol* arbol);

#endif
