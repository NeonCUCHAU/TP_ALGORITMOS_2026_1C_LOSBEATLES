#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "Tipos.h"
#include "Datos.h"

int  menuPrincipal(Config config, const char* archivoTablero, registroJugador* jugadorReg, tArbol* arbol);

int  lanzarDado();
void CrearBandidos(tListaCircularD ruta, tLista* listaBandidos);

void mostrarEstado(Jugador *jugador, int turno);
void mostrarMovimientos(tLista *log);
void iniciarNuevaPartida(Config config, const char* archivoTablero, registroJugador* jugadorReg, tArbol* arbol);

#endif
