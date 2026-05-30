#include "Tablero.h"

void cambiarTipo(void *info, void *param)
{
    Casilla *c = info;

    c->tipo = *(char*)param;
}

int LeerConfig(const char* archivo, Config *cfg)
{
    char clave[64];
    int valor;
    FILE* pf = fopen(archivo,"rt");
    if(!pf)
        return ERROR_ARCH;

    while(fscanf(pf,"%63[^:]: %d\n",clave,&valor)== 2)
    {
        if(strcmp(clave, "cantidad_posiciones") == 0)
            cfg->cantidadPosiciones = valor;
        else if (strcmp(clave, "vidas_inicio") == 0)
            cfg->vidasInicio = valor;
        else if (strcmp(clave, "maximo_bandidos") == 0)
            cfg->maxBandidos = valor;
        else if (strcmp(clave, "maximo_premios") == 0)
            cfg->maxPremios = valor;
        else if (strcmp(clave, "maximo_vidas_extra") == 0)
            cfg->maxVidasExtra = valor;
        else if (strcmp(clave, "maximo_oasis") == 0)
            cfg->maxOasis = valor;
        else if (strcmp(clave, "maximo_tormentas") == 0)
            cfg->maxTormentas = valor;
    }

    fclose(pf);
    return TODO_OK;
}

int crearTableroVacio(tListaCircularD* tablero, int cantPos)
{
    unsigned i;
    Casilla c = {'.', 1};

    crearListaCircularD(tablero);

    for(i=0; i < cantPos; i++)
    { 
        if(insertarEnListaCircularDAlFinal(tablero, &c, sizeof(Casilla)) != TODO_OK)
            return ERR_MEMORIA;
    }

    return TODO_OK;
}

int generarInicioYFin(tListaCircularD* tablero, int cantPos)
{
    char c = 'I';

    alterarNodo(tablero, 0, cambiarTipo, &c);

    c = 'S';

    alterarNodo(tablero, cantPos - 1, cambiarTipo, &c);

    return TODO_OK;
}