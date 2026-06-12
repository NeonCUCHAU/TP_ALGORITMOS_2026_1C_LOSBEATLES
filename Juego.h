#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "Tipos.h"
#include "Datos.h"

#define TODO_OK 1
#define CONTINUAR 1              /* Continuar iteración en callback */
#define DETENER_ITERACION 0      /* Detener iteración en callback */
#define JUGADOR_PROTEGIDO 2
#define JUGADOR_PIERDEVIDA 3

//Estructura auxiliar para pasar parámetros al callback
typedef struct
{
    tListaCircularD tablero;
    const Jugador *jugador;
    tCola *cola;
} sParamsBandido;

//Estructura auxiliar para CrearBandidos
typedef struct
{
    tLista *listaBandidos;
    unsigned *posicion;
    int *idBandido;
} sParamsCrearBandido;

//Estructura auxiliar para verificarColisionBandido
typedef struct
{
    Jugador *jugador;
    tLista *listaBandidos;
    tListaCircularD tablero;
    int posInicio;
    int resultado;  /* Resultado de la colisión */
    int bandidoId;  /* ID del bandido en colisión */
    int bandidoEncontrado;  /* 1 si se encontró colisión, 0 si no */
} sParamsVerificarColision;

int  menuPrincipal(Config config, const char* archivoTablero, registroJugador* jugadorReg, tArbol* arbol);

int  lanzarDado();
void CrearBandidos(tListaCircularD ruta, tLista* listaBandidos);
int procesarBandidoCallback(const void *dato, void *parametro);
void encolarMovBandidos(tListaCircularD tablero, tLista listaBandidos, const Jugador *jugador, tCola *cola);
int verificarColisionBandido(Jugador *jugador, tLista *listaBandidos, tListaCircularD tablero, int posInicio);

void mostrarEstado(Jugador *jugador, int turno);
void mostrarMovimientos(tLista *log);
void iniciarNuevaPartida(Config config, const char* archivoTablero, registroJugador* jugadorReg, tArbol* arbol);


#endif
