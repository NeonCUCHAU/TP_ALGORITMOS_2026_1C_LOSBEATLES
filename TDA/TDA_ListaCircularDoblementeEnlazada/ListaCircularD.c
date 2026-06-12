#include "ListaCircularD.h"

void crearListaCircularD(tListaCircularD *p)
{
    *p = NULL;
}

int listaCircularDVacia(const tListaCircularD *p)
{
    return *p == NULL;
}

int vaciarListaCircularD(tListaCircularD *p)
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

int insertarEnListaCircularDAlFinal(tListaCircularD *p, const void *d, unsigned cantBytes)
{
    tNodoD *nuevo;
    void *info;

    info = malloc(cantBytes);

    if(!info)
        return ERR_MEMORIA;

    memcpy(info, d, cantBytes);

    nuevo = malloc(sizeof(tNodoD));

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

int alterarNodoEnListaCircularD(tListaCircularD* p, unsigned pos, tAccion accion, void* param)
{
    tNodoD *act;
    unsigned i;

    if(!p || !*p || !accion)
        return ERROR;

    act = *p;

    for(i = 0; i < pos; i++)
    {
        act = act->sig;

        if(act == *p)
            return NO_ENCONTRADO;
    }

    return accion(act->info, param);
}

int recorrerListaCircularD(const tListaCircularD* p, tAccion accion, void* param)
{
    tNodoD *act;

    if(!p || !*p || !accion)
        return ERROR;

    act = *p;

    do
    {
        if(accion(act->info, param) != TODO_OK)
            return ERROR;

        act = act->sig;
    }
    while(act != *p);

    return TODO_OK;
}

unsigned contarNodosListaCircularD(const tListaCircularD* p)
{
    tNodoD* act;
    unsigned cant = 0;

    if(!p || !*p)
        return 0;

    act = *p;

    do
    {
        cant++;
        act = act->sig;
    }
    while(act != *p);

    return cant;
}

tNodoD* obtenerNodoPorIndiceD(const tListaCircularD* p, unsigned indice)
{
    tNodoD *act;
    unsigned pos;

    if(!p || !*p)
        return NULL;

    act = *p;
    pos = 0;

    do
    {
        if(pos == indice)
            return act;
        pos++;
        act = act->sig;
    }
    while(act != *p);

    return NULL;
}

void* obtenerInfoPorIndiceD(const tListaCircularD* p, unsigned indice)
{
    tNodoD *act;
    unsigned pos;

    if(!p || !*p)
        return NULL;

    act = *p;
    pos = 0;

    do
    {
        if(pos == indice)
            return act->info;
        pos++;
        act = act->sig;
    }
    while(act != *p);

    return NULL;
}

int modificarInfoPorIndiceD(tListaCircularD* p, unsigned indice, const void *datos, unsigned tamDatos)
{
    tNodoD *act;
    unsigned pos;

    if(!p || !*p || !datos)
        return ERROR;

    act = *p;
    pos = 0;

    do
    {
        if(pos == indice)
        {
            memcpy(act->info, datos, minimo(tamDatos, act->tamInfo));
            return TODO_OK;
        }
        pos++;
        act = act->sig;
    }
    while(act != *p);

    return NO_ENCONTRADO;
}
