#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "Tipos.h"
#include "Datos.h"

#define TODO_OK 1
#define JUGADOR_PROTEGIDO 2
#define JUGADOR_PIERDEVIDA 3

int  menuPrincipal(Config config, const char* archivoTablero, registroJugador* jugadorReg, tArbol* arbol);

int  lanzarDado();
void CrearBandidos(tListaCircularD ruta, tLista* listaBandidos);
void encolarMovBandidos(tLista listaBandidos, const Jugador *jugador, tCola *cola);
int verificarColisionBandido(Jugador *jugador, tLista *listaBandidos,tNodoD *nodoInicio);

void mostrarEstado(Jugador *jugador, int turno);
void mostrarMovimientos(tLista *log);
void iniciarNuevaPartida(Config config, const char* archivoTablero, registroJugador* jugadorReg, tArbol* arbol);


#endif
