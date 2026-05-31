#include "Tablero.h"

void cambiarTipo(void *info, void *param)
{
    Casilla *c = info;

    c->tipo = *(char*)param;
}

int generarAleatorioRango(int inicio, int fin)
{
    if(inicio > fin)
        return ERROR;

    return rand() % (fin - inicio + 1) + inicio;
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

int posicionValida(const char* tablero, int cantPos, int pos, char elemento, int factorProximidad, int considerarProximidad)
{
    if(pos <= 0 || pos >= cantPos - 1 || tablero[pos] != CELDA_VACIA)
        return POSICION_INVALIDA;
    
    if(considerarProximidad == CONSIDERAR_PROXIMIDAD)
    {
        for(unsigned i = 1; i <= factorProximidad; i++)
        {
        if(pos >= i && tablero[pos - i] == elemento)
            return POSICION_INVALIDA;

        if(pos + i < cantPos && tablero[pos + i] == elemento)
             return POSICION_INVALIDA;
         }
    }

    return POSICION_VALIDA;
}

void generarElementosDistribuidos(char* tablero, int cantPos, int cantElementos, char elemento, int considerarProximidad)
{
    int rangoRand = cantPos / cantElementos, sectorRandIni = 0, sectorRandFin, posRand, factorProximidad = 0;

    if(considerarProximidad == CONSIDERAR_PROXIMIDAD)
        factorProximidad = rangoRand / 2;

    for(unsigned i = 0; i < cantElementos; i++)
    {
        sectorRandFin = sectorRandIni + rangoRand - 1;

        do posRand = generarAleatorioRango(sectorRandIni, sectorRandFin);
        while(posicionValida(tablero, cantPos, posRand, elemento, factorProximidad, considerarProximidad) == POSICION_INVALIDA);

        tablero[posRand] = elemento;
        sectorRandIni += rangoRand;
    }
}

void generarBandidos(char* tablero, int cantPos, int cantBandidos)
{
    int rangoIni = cantPos / 4, rangoFin = cantPos - cantPos / 4 - 1, posRand;

    for(unsigned i = 0; i < cantBandidos; i++)
    {
        do posRand = generarAleatorioRango(rangoIni, rangoFin);
        while(tablero[posRand] != CELDA_VACIA);

        tablero[posRand] = CELDA_BANDIDO;
    }
}

int cargarTableroATxt(const Config* config, const char* nombreArchivo)
{
    char* tablero = malloc(config.cantidadPosiciones);
    int rangoRand, sectorRandIni, sectorRandFin, posRand, factorProximidad;

    if(!tablero)
        return ERR_MEMORIA;

    tablero[0] = CELDA_INICIO;
    tablero[config.cantidadPosiciones - 1] = CELDA_SALIDA;

    for(unsigned i = 1; i < config.cantidadPosiciones - 1; i++)
        tablero[i] = CELDA_VACIA;
    
    generarElementosDistribuidos(tablero, config.cantidadPosiciones, config.maxOasis, CELDA_OASIS, CONSIDERAR_PROXIMIDAD);
    generarElementosDistribuidos(tablero, config.cantidadPosiciones, config.maxTormentas, CELDA_TORMENTA, CONSIDERAR_PROXIMIDAD);
    generarElementosDistribuidos(tablero, config.cantidadPosiciones, config.maxPremios, CELDA_PREMIO, NO_CONSIDERAR_PROXIMIDAD);
    generarElementosDistribuidos(tablero, config.cantidadPosiciones, config.maxVidasExtra, CELDA_VIDA, NO_CONSIDERAR_PROXIMIDAD);
    generarBandidos(tablero, config.cantidadPosiciones, config.maxBandidos);

    free(tablero);
}
