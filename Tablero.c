#include "Tablero.h"

int LeerConfig(const char* archivo, Config *cfg)
{
    char clave[64];
    int valor;
    FILE* pf = fopen(archivo,"rt");
    if(!pf)
        return ERR_ARCH;

    while(fscanf(pf,"%63[^:]: %d\n",clave,&valor)== 2)
    {
        if(strcmp(clave, "cantidad_posiciones") == 0)
            cfg->cantidad_posiciones = valor;
        else if (strcmp(clave, "vidas_inicio") == 0)
            cfg->vidas_inicio = valor;
        else if (strcmp(clave, "maximo_bandidos") == 0)
            cfg->maximo_bandidos = valor;
        else if (strcmp(clave, "maximo_premios") == 0)
            cfg->maximo_premios = valor;
        else if (strcmp(clave, "maximo_vidas_extra") == 0)
            cfg->maximo_vidas_extra = valor;
        else if (strcmp(clave, "maximo_oasis") == 0)
            cfg->maximo_oasis = valor;
        else if (strcmp(clave, "maximo_tormentas") == 0)
            cfg->maximo_tormentas = valor;
    }

    fclose(pf);
    return TODO_OK;
}



