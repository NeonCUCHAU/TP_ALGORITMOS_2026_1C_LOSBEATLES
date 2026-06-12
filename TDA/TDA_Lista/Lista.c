#include "Lista.h"

void crearLista(tLista *p)
{
    *p = NULL;
}

int listaVacia(const tLista *p)
{
    return *p == NULL;
}

int listaLlena(const tLista *p, unsigned cantBytes)
{
    tNodo *aux = (tNodo*)malloc(sizeof(tNodo));
    void *info = malloc(cantBytes);

    free(aux);
    free(info);
    return aux == NULL || info == NULL;
}

void vaciarLista(tLista *p)
{
    while(*p)
    {
        tNodo *aux = *p;

        *p = aux->sig;
        free(aux->info);
        free(aux);
    }
}

int ponerAlComienzo(tLista *p, const void *d, unsigned cantBytes)
{
    tNodo *nue;

    if((nue = (tNodo*)malloc(sizeof(tNodo))) == NULL || (nue->info = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return ERR_MEMORIA;
    }
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = *p;
    *p = nue;
    return TODO_OK;
}

int sacarPrimeroLista(tLista *p, void *d, unsigned cantBytes)
{
    tNodo *aux = *p;

    if(aux == NULL)
        return ERR_MEMORIA;
    *p = aux->sig;
    memcpy(d, aux->info, minimo(cantBytes, aux->tamInfo));
    free(aux->info);
    free(aux);
    return TODO_OK;
}

int verPrimeroLista(const tLista *p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return LISTA_VACIA;
    memcpy(d, (*p)->info, minimo(cantBytes, (*p)->tamInfo));
    return TODO_OK;
}

int ponerAlFinal(tLista *p, const void *d, unsigned cantBytes)
{
    tNodo *nue;

    while(*p)
        p = &(*p)->sig;

    if((nue = (tNodo*)malloc(sizeof(tNodo))) == NULL || (nue->info = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return ERR_MEMORIA;
    }
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = *p;
    *p = nue;
    return TODO_OK;
}

int sacarUltimoLista(tLista *p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return LISTA_VACIA;
    while((*p)->sig)
        p = &(*p)->sig;

    memcpy(d, (*p)->info, minimo(cantBytes, (*p)->tamInfo));
    free((*p)->info);
    free(*p);
    *p = NULL;
    return TODO_OK;
}

int verUltimoLista(const tLista *p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;
    while((*p)->sig)
        p = &(*p)->sig;
    memcpy(d, (*p)->info, minimo(cantBytes, (*p)->tamInfo));
    return TODO_OK;
}

void recorrerLista(const tLista *p, tAccion accion, void *param)
{
    const tNodo *aux = *p;
    while(aux != NULL)
    {
        accion(aux->info, param);
        aux = aux->sig;
    }
}

void mostrarLista(const tLista *p, tMostrar mostrar)
{
    const tNodo *aux = *p;
    while(aux != NULL)
    {
        mostrar(aux->info);
        aux = aux->sig;
    }
}

int eliminarNodo(tLista *p, tNodo *nodoEliminar)
{
    tNodo *actual, *anterior;

    if(!nodoEliminar || !p)
        return ERROR;

    anterior = NULL;
    actual = *p;

    while(actual != NULL && actual != nodoEliminar)
    {
        anterior = actual;
        actual = actual->sig;
    }

    if(actual != NULL)
    {
        if(anterior == NULL)
            *p = actual->sig;
        else
            anterior->sig = actual->sig;
        free(actual->info);
        free(actual);
        return TODO_OK;
    }

    return NO_ENCONTRADO;
}

int eliminarPorCondicion(tLista *p, tComparador comparador, void *criterio)
{
    tNodo *actual, *anterior;

    if(!p || !comparador)
        return ERROR;

    anterior = NULL;
    actual = *p;

    while(actual != NULL)
    {
        if(comparador(actual->info, criterio))
        {
            /* Encontrado - eliminar */
            if(anterior == NULL)
                *p = actual->sig;
            else
                anterior->sig = actual->sig;
            free(actual->info);
            free(actual);
            return TODO_OK;
        }
        anterior = actual;
        actual = actual->sig;
    }

    return NO_ENCONTRADO;
}
