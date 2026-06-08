/*

main
│
├─ leerConfiguracion()
│
├─ crearTableroVacio()
│
├─ colocarInicio()
│
├─ colocarSalida()
│
├─ colocarPremios()
│
├─ colocarVidasExtra()
│
├─ colocarOasis()
│
├─ colocarTormentas()
│
├─ colocarBandidos()
│
├─ crearJugador()
│
└─ guardarTableroInicial()

*/
#ifndef TIPOS_H_INCLUDED
#define TIPOS_H_INCLUDED

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TDA/TDA_Lista/Lista.h"
#include "TDA/TDA_ListaCircularDoblementeEnlazada/ListaCircularD.h"
#include "TDA/TDA_Cola/Cola.h"


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

typedef struct {
    char terreno;       /* inicio, salida, oasis, tormenta, vacio*/
    char item;          /* premio, vida, vacio*/
    int bandidos;       /* 1 si hay un bandido emboscando, 0 si no*/
    int jugador;        /* 1 si el jugador está pisando esta casilla, 0 si no*/
} Casilla;

typedef struct
{
    int vidas;
    int puntos;
    int protegido; /* 0 = no, 1 = sí*/
    int pierdeTurno; /* 0 = no, 1 = sí*/
    tNodoD *posActual;
}
Jugador;

typedef struct
{
    int id;
    int estado; /* si esta eliminado el bandido o no*/
    tNodoD *posActual;
}
Bandido;

typedef struct
{
    char movimento[2];
}
MovimientoLog; /*esto es el logeo que se muestra al final en una LISTA*/

typedef struct
{
    char dir;
    int pasos;
}
Movimiento;


#endif
