#include "ListaCircularD.h"

void crearLista(tListaCircularD *p)
{
    *p = NULL;
}

int listaVacia(const tListaCircularD *p)
{
    return *p == NULL;
}

int vaciarLista(tListaCircularD *p)
{
    tNodoD *act, *sig, *prim;

    if(!*p)
        return 0;

    prim = *p;
    act = prim->sig;

    while(act != prim)
    {
        sig = act->sig;

        free(act->info);
        free(act);

        act = sig;
    }

    free(prim->info);
    free(prim);

    *p = NULL;

    return TODO_OK;
}

int insertarEnLista(tListaCircularD *p, const void *d, unsigned cantBytes)
{
    tNodoD *nuevo;
    void *info;

    info = malloc(cantBytes);

    if(!info)
        return ERR_MEMORIA;

    memcpy(info, d, cantBytes);

    nuevo = malloc(sizeof(tNodo));

    if(!nuevo)
    {
        free(info);
        return ERR_MEMORIA;
    }

    nuevo->info = info;
    nuevo->tamInfo = cantBytes;

    if(!*p)
    {
        nuevo->sig = nuevo;
        nuevo->ant = nuevo;

        *p = nuevo;

        return TODO_OK;
    }

    tNodoD *ult = (*p)->ant;

    nuevo->sig = *p;
    nuevo->ant = ult;

    ult->sig = nuevo;
    (*p)->ant = nuevo;

    return TODO_OK;
}

int main()
{
    return 0;
}
