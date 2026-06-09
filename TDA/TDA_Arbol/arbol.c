#include "arbol.h"

void crearArbol(tArbol *a)
{
    *a = NULL;
}

void vaciarArbol(tArbol *a)
{
    if(!a || !*a)
        return;

    vaciarArbol(&(*a)->izq);
    vaciarArbol(&(*a)->der);

    free((*a)->dato);
    free(*a);
    *a = NULL;
}

int insertarEnArbol(tArbol *a, const void *elem, size_t tamElem, tCmp cmp)
{
    tNodoA *nue;
    int c;

    if(!*a)
    {
        nue = malloc(sizeof(tNodoA));
        if(!nue)
            return ERR_MEMORIA;

        nue->dato = malloc(tamElem);
        if(!nue->dato)
        {
            free(nue);
            return ERR_MEMORIA;
        }

        memcpy(nue->dato, elem, tamElem);
        nue->tamDato = tamElem;
        nue->izq = NULL;
        nue->der = NULL;
        *a = nue;
        return TODO_OK;
    }

    c = cmp((*a)->dato, elem);

    if(c > 0)
        return insertarEnArbol(&(*a)->izq, elem, tamElem, cmp);
    else if(c < 0)
        return insertarEnArbol(&(*a)->der, elem, tamElem, cmp);
    else
        return DUPL;
}

int buscarEnArbol(const tArbol *a, const void *clave, void *resultado, size_t tamElem, tCmp cmp)
{
    int c;

    if(!*a)
        return NO_ENCONTRADO;

    c = cmp((*a)->dato, clave);

    if(c == 0)
    {
        memcpy(resultado, (*a)->dato, tamElem);
        return TODO_OK;
    }
    else if(c > 0)
        return buscarEnArbol(&(*a)->izq, clave, resultado, tamElem, cmp);
    else
        return buscarEnArbol(&(*a)->der, clave, resultado, tamElem, cmp);
}

void recorrerInAccion(tArbol *a, tAccion accion, void *param)
{
    if(!a || !*a)
        return;

    recorrerInAccion(&(*a)->izq, accion, param);
    accion((*a)->dato, param);
    recorrerInAccion(&(*a)->der, accion, param);
}

void recorrerPreAccion(tArbol *a, tAccion accion, void *param)
{
    if(!a || !*a)
        return;

    accion((*a)->dato, param);
    recorrerPreAccion(&(*a)->izq, accion, param);
    recorrerPreAccion(&(*a)->der, accion, param);
}

void recorrerIn(const tArbol *a, tMostrar mostrar)
{
    if(!a || !*a)
        return;

    recorrerIn(&(*a)->izq, mostrar);
    mostrar((*a)->dato);
    recorrerIn(&(*a)->der, mostrar);
}

void recorrerPre(const tArbol *a, tMostrar mostrar)
{
    if(!a || !*a)
        return;

    mostrar((*a)->dato);
    recorrerPre(&(*a)->izq, mostrar);
    recorrerPre(&(*a)->der, mostrar);
}

void recorrerPos(const tArbol *a, tMostrar mostrar)
{
    if(!a || !*a)
        return;

    recorrerPos(&(*a)->izq, mostrar);
    recorrerPos(&(*a)->der, mostrar);
    mostrar((*a)->dato);
}

int contarNodosA(tArbol *a)
{
    if(!a || !*a)
        return 0;

    return contarNodosA(&(*a)->izq) + contarNodosA(&(*a)->der) + 1;
}

void eliminarHojas(tArbol *a)
{
    if(!a || !*a)
        return;

    if(!(*a)->izq && !(*a)->der)
    {
        free((*a)->dato);
        free(*a);
        *a = NULL;
        return;
    }

    eliminarHojas(&(*a)->izq);
    eliminarHojas(&(*a)->der);
}








