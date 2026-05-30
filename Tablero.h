#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

#include "Tipos.h"


void cambiarTipo(void *info, void *param);

int LeerConfig(const char* archivo, Config *cfg);
int crearTableroVacio(tListaCircularD* tablero, int cantPos);
int generarInicioYFin(tListaCircularD* tablero, int cantPos);



#endif // TABLERO_H_INCLUDED
