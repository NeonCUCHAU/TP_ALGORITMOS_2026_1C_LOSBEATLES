#ifndef LISTACIRCULARD_H
#define LISTACIRCULARD_H

#include "../Comun/Comun.h"

typedef tNodoD *tListaCircularD;

void crearLista(tListaCircularD *p);
int listaVacia(const tListaCircularD *p);
int vaciarLista(tListaCircularD *p);
int insertarEnLista(tListaCircularD *p, const void *d, unsigned cantBytes);


#endif
