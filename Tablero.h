#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

#include "Tipos.h"

#define CONSIDERAR_PROXIMIDAD 1
#define NO_CONSIDERAR_PROXIMIDAD 0
#define POSICION_INVALIDA -1
#define POSICION_VALIDA 0


void cambiarTipo(void *info, void *param);
int generarAleatorioRango(int inicio, int fin);

int LeerConfig(const char* archivo, Config *cfg);
int posicionValida(const char* tablero, int cantPos, int pos, char elemento, int factorProximidad, int considerarProximidad);
void generarElementosDistribuidos(char* tablero, int cantPos, int cantElementos, char elemento, int considerarProximidad);
void generarBandidos(char* tablero, int cantPos, int cantBandidos);
int guardarTableroATxt(const char* nombreArchivo, const char* tablero, int cantPos);
int cargarTableroATxt(const Config config, const char* nombreArchivo);


#endif // TABLERO_H_INCLUDED
