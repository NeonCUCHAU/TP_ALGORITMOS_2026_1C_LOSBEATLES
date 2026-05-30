#include "Juego.h"

int lanzarDado()
{
    return (rand() % 6) + 1;
}

void CrearBandidos(tListaCircularD ruta, tNodo** listaBandidos)// ruta es la listacircular del tablero
{
    tNodoD *actual;
    Casilla *c;
    Bandido b;
    int id = 0;

    crearLista(listaBandidos);
    if(!ruta)
        return;

    actual = ruta;

    do{
        c = (Casilla*) actual->info;
        if(c->tipo == CELDA_BANDIDO)
        {
            b.id = id++;
            b.estado = 1;
            b.posActual = actual;
            ponerAlFinal(listaBandidos, &b, sizeof(Bandido));
        }
        actual = actual->sig;
    }while(actual != ruta);
}



