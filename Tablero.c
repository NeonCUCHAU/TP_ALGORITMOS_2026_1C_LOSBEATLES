#include "Tablero.h"


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
    FILE* pf = fopen(archivo, "rt");
    if(!pf)
        return ERROR_ARCH;

    while(fscanf(pf,"%63[^:]: %d\n", clave, &valor) == 2)
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

/* Valida posicion para terreno */
int posicionValida(const char* tablero, unsigned cantPos, unsigned pos, char elemento, unsigned factorProximidad, int considerarProximidad)
{
    unsigned i;

    if(pos == 0 || pos >= cantPos - 1 || tablero[pos] != CELDA_VACIA)
        return POSICION_INVALIDA;

    if(considerarProximidad == CONSIDERAR_PROXIMIDAD)
    {
        for(i = 1; i <= factorProximidad; i++)
        {
            if(pos >= i && tablero[pos - i] == elemento)
                return POSICION_INVALIDA;

            if(pos + i < cantPos && tablero[pos + i] == elemento)
                return POSICION_INVALIDA;
        }
    }

    return POSICION_VALIDA;
}
/* Valida posicion para items: acepta cualquier terreno excepto inicio/salida */
int posicionValidaItem(const char* items, const char* terreno, unsigned cantPos, unsigned pos)
{
    if(pos == 0 || pos >= cantPos - 1)
        return POSICION_INVALIDA;

    if(terreno[pos] == CELDA_INICIO || terreno[pos] == CELDA_SALIDA)
        return POSICION_INVALIDA;

    if(items[pos] != CELDA_VACIA)
        return POSICION_INVALIDA;

    return POSICION_VALIDA;
}


void generarElementosDistribuidos(char* tablero, unsigned cantPos, unsigned cantElementos, char elemento, int considerarProximidad)
{
    int esValida, intentos, sectorRandFin, posRand, factorProximidad = 0;
    unsigned rangoRand, sectorRandIni = 0, i;

    rangoRand = cantPos / cantElementos;

    if(considerarProximidad == CONSIDERAR_PROXIMIDAD)
        factorProximidad = rangoRand / 2;

    for(i = 0; i < cantElementos; i++)
    {
        sectorRandFin = sectorRandIni + rangoRand - 1;
        intentos = 0;

        do
        {
            posRand = generarAleatorioRango(sectorRandIni, sectorRandFin);
            intentos++;
            esValida = posicionValida(tablero, cantPos, posRand, elemento, factorProximidad, considerarProximidad);
        }
        while(esValida == POSICION_INVALIDA && intentos < 100);

        if(esValida == POSICION_VALIDA)
            tablero[posRand] = elemento;

        sectorRandIni += rangoRand;
    }
}

void generarItemsDistribuidos(char* items, const char* terreno, unsigned cantPos, unsigned cantElementos, char elemento)
{
    int esValida, intentos, sectorRandFin, posRand;
    unsigned rangoRand, sectorRandIni, i;

    rangoRand     = cantPos / cantElementos;
    sectorRandIni = 0;

    for(i = 0; i < cantElementos; i++)
    {
        sectorRandFin = sectorRandIni + rangoRand - 1;
        intentos = 0;

        do
        {
            posRand = generarAleatorioRango(sectorRandIni, sectorRandFin);
            intentos++;
            esValida = posicionValidaItem(items, terreno, cantPos, posRand);
        }
        while(esValida == POSICION_INVALIDA && intentos < 100);

        if(esValida == POSICION_VALIDA)
            items[posRand] = elemento;

        sectorRandIni += rangoRand;
    }
}

void generarBandidos(char* tablero, unsigned cantPos, unsigned cantBandidos)
{
    unsigned i, posRand, intentos;
    unsigned rangoIni = cantPos / 4;
    unsigned rangoFin = cantPos - cantPos / 4 - 1;

    for(i = 0; i < cantBandidos; i++)
    {
        intentos = 0;
        do
        {
            posRand = generarAleatorioRango(rangoIni, rangoFin);
            intentos++;
        }
        while(tablero[posRand] != CELDA_VACIA && intentos < 100);

        if(tablero[posRand] == CELDA_VACIA)
            tablero[posRand] = CELDA_BANDIDO;
    }
}


int guardarTableroATxt(const char* nombreArchivo, const char* terreno, const char* items, unsigned cantPos)
{
    unsigned i;
    FILE* pf = fopen(nombreArchivo, "wt");

    if(!pf)
        return ERROR_ARCH;

    fprintf(pf, "%02d:[I J]\n", 1);

    for(i = 1; i < cantPos; i++)
    {
        if(terreno[i] != CELDA_VACIA && items[i] != CELDA_VACIA)
            fprintf(pf, "%02d:[%c %c]\n", i + 1, terreno[i], items[i]);
        else if(terreno[i] != CELDA_VACIA)
            fprintf(pf, "%02d:[%c]\n", i + 1, terreno[i]);
        else if(items[i] != CELDA_VACIA)
            fprintf(pf, "%02d:[%c]\n", i + 1, items[i]);
        else
            fprintf(pf, "%02d:[.]\n", i + 1);
    }

    fclose(pf);
    return TODO_OK;
}

int cargarTableroATxt(const Config config, const char* nombreArchivo)
{
    unsigned i;
    char* terreno;
    char* items;

    terreno = malloc(config.cantidadPosiciones);
    items = malloc(config.cantidadPosiciones);

    if(!terreno || !items)
    {
        free(terreno);
        free(items);
        return ERR_MEMORIA;
    }

    /* inicializar terreno */
    terreno[0] = CELDA_INICIO;
    terreno[config.cantidadPosiciones - 1] = CELDA_SALIDA;
    for(i = 1; i < config.cantidadPosiciones - 1; i++)
        terreno[i] = CELDA_VACIA;

    /* inicializar items */
    for(i = 0; i < config.cantidadPosiciones; i++)
        items[i] = CELDA_VACIA;

    /* generar terreno */
    generarElementosDistribuidos(terreno, config.cantidadPosiciones, config.maxOasis, CELDA_OASIS, CONSIDERAR_PROXIMIDAD);
    generarElementosDistribuidos(terreno, config.cantidadPosiciones, config.maxTormentas, CELDA_TORMENTA, CONSIDERAR_PROXIMIDAD);
    /* generar items (pueden superponerse con terreno) */
    generarItemsDistribuidos(items, terreno, config.cantidadPosiciones, config.maxPremios, CELDA_PREMIO);
    generarItemsDistribuidos(items, terreno, config.cantidadPosiciones, config.maxVidasExtra, CELDA_VIDA);
    /* generar bandidos */
    generarBandidos(terreno, config.cantidadPosiciones, config.maxBandidos);


    if(guardarTableroATxt(nombreArchivo, terreno, items, config.cantidadPosiciones) != TODO_OK)
    {
        free(terreno);
        free(items);
        return ERROR_ARCH;
    }

    free(terreno);
    free(items);
    return TODO_OK;
}

int cargarTableroDesdeTxt(tListaCircularD* tablero, const char* nombreArchivo)
{
    FILE* pf = fopen(nombreArchivo, "rt");
    char linea[16];
    char* sep;
    char ch;
    Casilla c;

    if(!pf)
        return ERROR_ARCH;

    crearListaCircularD(tablero);

    /* inicio con jugador*/
    if(fgets(linea, sizeof(linea), pf))
    {
        c.terreno  = CELDA_INICIO;
        c.item     = CELDA_VACIA;
        c.bandidos = 0;
        c.jugador  = 1;

        if(insertarEnListaCircularDAlFinal(tablero, &c, sizeof(Casilla)) != TODO_OK)
        {
            fclose(pf);
            return ERR_MEMORIA;
        }
    }

    while(fgets(linea, sizeof(linea), pf))
    {
        c.terreno  = CELDA_VACIA;
        c.item     = CELDA_VACIA;
        c.bandidos = 0;
        c.jugador  = 0;

        sep = strchr(linea, '[');

        if(sep != NULL)
        {
            ch = *(sep + 1);

            /* procesar el primer caracter(T o B)*/
            if(ch == CELDA_BANDIDO)
                c.bandidos = 1;
            else if(ch == CELDA_VACIA)
                c.terreno = CELDA_VACIA;
            else
                c.terreno = ch;

            /* procesar el segundo car�cter si existe*/
            if(*(sep + 2) == ' ' && *(sep + 3) != ']')
                c.item = *(sep + 3);
        }
        if(insertarEnListaCircularDAlFinal(tablero, &c, sizeof(Casilla)) != TODO_OK)
        {
            fclose(pf);
            return ERR_MEMORIA;
        }
    }
    fclose(pf);
    return TODO_OK;
}

//FUNCIONES PARA MOSTRAR EL TABLERO EN PANTALLA:
int cargarVectorVisual(void* info, void* param)
{
    ParamCargaVisual* p = (ParamCargaVisual*)param;

<<<<<<< HEAD
    p->vector[p->posActual++] = (Casilla*)info;

    return TODO_OK;
} //funcion tipo tAccion para cargar un vector de Casilla* a partir de la lista circular del tablero

void mostrarTablero(const tListaCircularD* tablero)
{
    unsigned cantCasillas = contarNodosListaCircularD(tablero), casillasPorFila = 10, col, inicioFila;
    Casilla** vec;
    ParamCargaVisual param;

    vec = malloc(sizeof(Casilla*) * cantCasillas);

    if(!vec)
        return;

    param.vector = vec;
    param.posActual = 0;

    recorrerListaCircularD(tablero, cargarVectorVisual, &param);

    for(inicioFila = 0; inicioFila < cantCasillas; inicioFila += casillasPorFila)
    {
        /* -------- Borde superior -------- */

        for(col = 0; col < casillasPorFila && inicioFila + col < cantCasillas; col++)
        {
            printf("+-------+ ");
        }

        printf("\n");

        /* -------- Línea item -------- */

        for(col = 0; col < casillasPorFila && inicioFila + col < cantCasillas; col++)
        {
            Casilla* c = vec[inicioFila + col];

            if(c->item != CELDA_VACIA)
                printf("|   %c   | ", c->item);
            else
                printf("|       | ");
        }

        printf("\n");

        /* -------- Línea terreno-------- */

        for(col = 0; col < casillasPorFila && inicioFila + col < cantCasillas; col++)
        {
            Casilla* c = vec[inicioFila + col];

            printf("|   %c   | ", c->terreno);
        }

        printf("\n");

        /* -------- Línea ocupantes -------- */

        for(col = 0; col < casillasPorFila && inicioFila + col < cantCasillas; col++)
        {
            Casilla* c = vec[inicioFila + col];

            if(c->jugador)
                printf("|   J   | ");

            else if(c->bandidos == 1)
                printf("|   B   | ");

            else if(c->bandidos > 1)
                printf("|  Bx%d | ", c->bandidos);

            else
                printf("|       | ");
        }

        printf("\n");

        /* -------- Borde inferior -------- */

        for(col = 0; col < casillasPorFila && inicioFila + col < cantCasillas; col++)
        {
            printf("+-------+ ");
        }

        printf("\n");

        /* -------- Numeros -------- */
        for(col = 0; col < casillasPorFila && inicioFila + col < cantCasillas; col++)
        {
            printf("   %02u     ", inicioFila + col + 1);
        }

        printf("\n\n");
    }

    free(vec);
}
=======
void mostrarTablero(tListaCircularD ruta)
{
    tNodoD *actual;
    Casilla *c;
    char contenido[16];
    int pos = 1, len, i;

    if(!ruta)
        return;

    actual = ruta;

    printf("\n===== TABLERO =====\n");
    do
    {
        c = (Casilla*) actual->info;
        len = 0;

        if(c->terreno != CELDA_VACIA)
            contenido[len++] = c->terreno;
        if(c->item != CELDA_VACIA)
            contenido[len++] = c->item;
        for(i = 0; i < c->bandidos; i++)
            contenido[len++] = CELDA_BANDIDO;
        if(c->jugador)
            contenido[len++] = 'J';

        if(!len)
            contenido[len++] = CELDA_VACIA;
        contenido[len] = '\0';

        printf("%02d:[%s]\n", pos, contenido);

        actual = actual->sig;
        pos++;
    }
    while(actual != ruta);

    printf("===================\n");
}

>>>>>>> 3ca397f (Implementacion de carga de datos, ranking, partidas y jugadores)
