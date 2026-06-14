#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "Tipos.h"
#include "Datos.h"
#include "Tablero.h"
#include <ctype.h>

#define CONTINUAR 1
#define DETENER_ITERACION 0
#define JUGADOR_PROTEGIDO 2
#define JUGADOR_PIERDEVIDA 3

#define SIMBOLO_CORAZON  3
#define SIMBOLO_DIAMANTE 4
#define SIMBOLO_TREBOL   5
#define SIMBOLO_PICA     6

typedef struct
{
    char dir;
    int pasos;
    int esBandido;
    int idBandido;
}
MovimientoEncolado;

typedef struct
{
    tLista *listaBandidos;
    unsigned *posicion;
    int *idBandido;
}
sParamsCrearBandido;

typedef struct
{
    Jugador *jugador;
    tListaCircularD *tablero;
    int posInicio;
    int resultado;
    int bandidoId;
    int bandidoEncontrado;
}
sParamsVerificarColision;

typedef struct
{
    tListaCircularD *tablero;
    const Jugador *jugador;
    tCola *cola;
}
sParamsBandido;

typedef struct
{
    tListaCircularD *tablero;
    unsigned cantNodos;
    int idBuscado;
    int pasos;
}
sParamsAplicarMov;

int menuPrincipal(Config config, const char* archivoTablero, registroJugador* jugadorReg, tArbol* arbol);
void iniciarNuevaPartida(Config config, const char* archivoTablero, registroJugador* jugadorReg, tArbol* arbol);

int lanzarDado(void);
void CrearBandidos(tListaCircularD ruta, tLista* listaBandidos);
int procesarBandidoCallback(void *dato, void *parametro);

void encolarMovBandidos(tListaCircularD *tablero, tLista *listaBandidos, const Jugador *jugador, tCola *cola);
int verificarColisionBandido(Jugador *jugador, tLista *listaBandidos, tListaCircularD *tablero, int posInicio);
int aplicarMovBandidoCallback(void *dato, void *parametro);

void mostrarEstado(Jugador *jugador, int turno);
void mostrarMovimientos(tLista *log);

void pedirMovimientoJugador(Jugador* jugador, tCola* cola);
void procesarColaMovimientos(tCola* cola, Jugador* jugador, tListaCircularD* tablero, tLista* listaBandidos, tLista* logMovimientos, unsigned cantNodos, int* gano);
void procesarCasillaJugador(Jugador* jugador, Casilla* casilla, int* gano);

#endif
