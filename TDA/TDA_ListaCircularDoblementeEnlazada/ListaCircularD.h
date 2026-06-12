#ifndef LISTACIRCULARD_H
#define LISTACIRCULARD_H

#include "../Comun/Comun.h"

typedef tNodoD *tListaCircularD;

void crearListaCircularD(tListaCircularD *p);
int listaCircularDVacia(const tListaCircularD *p);
int vaciarListaCircularD(tListaCircularD *p);
int insertarEnListaCircularDAlFinal(tListaCircularD *p, const void *d, unsigned cantBytes);
int alterarNodoEnListaCircularD(tListaCircularD* p, unsigned pos, tAccion accion, void* param);
int recorrerListaCircularD(const tListaCircularD* p, tAccion accion, void* param);
unsigned contarNodosListaCircularD(const tListaCircularD* p);
tNodoD* obtenerNodoPorIndiceD(const tListaCircularD* p, unsigned indice);
void* obtenerInfoPorIndiceD(const tListaCircularD* p, unsigned indice);
int modificarInfoPorIndiceD(tListaCircularD* p, unsigned indice, const void *datos, unsigned tamDatos);

#endif
