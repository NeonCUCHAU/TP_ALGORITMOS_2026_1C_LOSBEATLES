#ifndef TIPOS_H_INCLUDED
#define TIPOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TDA/TDA_Lista/Lista.h"
#include "TDA/TDA_ListaCircularDoblementeEnlazada/ListaCircularD.h"
#include "TDA/TDA_Cola/Cola.h"

/*Tipos de celdas*/
#define CELDA_VACIA '.'
#define CELDA_INICIO 'I'
#define CELDA_SALIDA 'S'
#define CELDA_PREMIO 'P'
#define CELDA_VIDA 'V'
#define CELDA_OASIS 'O'
#define CELDA_TORMENTA 'T'
#define CELDA_BANDIDO 'B'

#define MAX_MOVIMIENTOS 200

/*Colores*/
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#ifdef _WIN32
#include <windows.h>
#define LIMPIAR_PANTALLA system("cls")
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
#define LIMPIAR_PANTALLA system("clear")
#endif

/* TABLERO */
typedef struct
{
    int cantidadPosiciones;
    int vidasInicio;
    int maxBandidos;
    int maxPremios;
    int maxVidasExtra;
    int maxOasis;
    int maxTormentas;
}
Config;

typedef struct
{
    char terreno;       /* inicio, salida, oasis, tormenta, vacio*/
    char item;          /* premio, vida, vacio*/
    int bandidos;       /* 1 si hay un bandido emboscando, 0 si no*/
    int jugador;        /* 1 si el jugador está pisando esta casilla, 0 si no*/
}
Casilla;

typedef struct
{
    int vidas;
    int puntos;
    int protegido; /* 0 = no, 1 = sí*/
    int pierdeTurno; /* 0 = no, 1 = sí*/
    int posActual;
}
Jugador;

typedef struct
{
    int id;
    int estado; /* si esta eliminado el bandido o no( 1 esta vivo , 0 esta muerto)*/
    int posActual;
}
Bandido;

typedef struct
{
    char dir;
    int pasos;
}
Movimiento;

typedef struct
{
    Casilla** vector;
    unsigned posActual;
}
ParamCargaVisual;

/* DATOS */
typedef struct
{
    int id;
    char nombre[32];
    long offset; /* posicion en jugadores.dat */
}
InfoBST;

typedef struct
{
    int id;
    char nombre[32];
    int pin;
}
registroJugador;

typedef struct
{
    int id; /* por ahora no se usa */
    int idJugador;
    int puntos;
    int movimientos;
    int gano;
}
registroPartida;

typedef struct
{
    char nombre[32];
    int puntos;
}
registroRanking;


#endif
