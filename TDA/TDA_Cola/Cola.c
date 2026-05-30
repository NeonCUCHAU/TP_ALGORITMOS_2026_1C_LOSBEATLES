#include "Cola.h"

void crearCola(tCola *p)
{
    p->pri = NULL;
    p->ult = NULL;
}

int colaLlena(const tCola *p, unsigned cantBytes)
{
    return TODO_OK;
}

int ponerEnCola(tCola *p, const void *d, unsigned cantBytes)
{
    tNodo *nue = (tNodo*) malloc(sizeof(tNodo));

    if(nue == NULL || (nue->info = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return ERR_MEMORIA;
    }
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = NULL;
    if(p->ult)
        p->ult->sig = nue;
    else
        p->pri = nue;
    p->ult = nue;

    return TODO_OK;
}

int verPrimeroCola(const tCola* p, void *d, unsigned cantBytes)
{
    if(p->pri == NULL)
        return ERR_COLA_VACIA;
    memcpy(d, p->pri->info, minimo(cantBytes, p->pri->tamInfo));

    return TODO_OK;
}

int colaVacia(const tCola *p)
{
    return p->pri == NULL;
}

int sacarDeCola(tCola *p, void *d, unsigned cantBytes)
{
    tNodo *aux = p->pri;
    if(aux == NULL)
        return ERR_COLA_VACIA;

    p->pri = aux->sig;
    memcpy(d, aux->info, minimo(aux->tamInfo, cantBytes));
    free(aux->info);
    free(aux);
    if(p->pri == NULL)
        p->ult = NULL;

    return TODO_OK;
}

void vaciarCola(tCola *p)
{
    while(p->pri)
    {
        tNodo *aux = p->pri;
        p->pri = aux->sig;
        free(aux->info);
        free(aux);
    }
    p->ult = NULL;
}

int main()
{
    return 0;
}
