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
int cargarTableroATxt(const Config* config, const char* nombreArchivo);


#endif // TABLERO_H_INCLUDED
