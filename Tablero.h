#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

#include "Tipos.h"

#define CONSIDERAR_PROXIMIDAD 1
#define NO_CONSIDERAR_PROXIMIDAD 0
#define POSICION_INVALIDA -1
#define POSICION_VALIDA 0


int generarAleatorioRango(int inicio, int fin);
int LeerConfig(const char* archivo, Config *cfg);

/* =========================================================
   Generacion del tablero (arrays temporales de chars)

   Se usan DOS arrays separados:
     terreno[] -> INICIO, SALIDA, OASIS, TORMENTA, VACIA
     items[]   -> PREMIO, VIDA, VACIA
========================================================= */
int posicionValida(const char* tablero, unsigned cantPos, unsigned pos, char elemento, unsigned factorProximidad, int considerarProximidad);
int posicionValidaItem(const char* items, const char* terreno, unsigned cantPos, unsigned pos);
void generarElementosDistribuidos(char* tablero, unsigned cantPos, unsigned cantElementos, char elemento, int considerarProximidad);
void generarBandidos(char* tablero, unsigned cantPos, unsigned cantBandidos);

int guardarTableroATxt(const char* nombreArchivo, const char* terreno, const char* items, unsigned cantPos);
int cargarTableroATxt(const Config config, const char* nombreArchivo);
int cargarTableroDesdeTxt(tListaCircularD* tablero, const char* nombreArchivo);


#endif
