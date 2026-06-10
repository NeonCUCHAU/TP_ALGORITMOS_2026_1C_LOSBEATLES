#include "Datos.h"

int cmpNombre(const void* a, const void* b)
{
    return strcmp(((InfoBST*)a)->nombre, ((InfoBST*)b)->nombre);
}

static int serializarNodo(void* dato, void* param)
{
    FILE *pf = (FILE*) param;
    return fwrite(dato, sizeof(InfoBST), 1, pf) == 1 ? TODO_OK : ERROR_ARCH;
}

/*
static int contarPartidas()
{
    FILE *pf = fopen(ARCHIVO_PARTIDAS, "rb");
    int cant = 0;

    if(!pf)
        return 0;

    fseek(pf, 0, SEEK_END);
    cant = (int)(ftell(pf) / sizeof(registroPartida));
    fclose(pf);

    return cant;
}
*/
static int contarJugadores()
{
    FILE *pf = fopen(ARCHIVO_JUGADORES, "rb");
    int cant = 0;

    if(!pf)
        return 0;

    fseek(pf, 0, SEEK_END);
    cant = (int)(ftell(pf) / sizeof(registroJugador));
    fclose(pf);

    return cant;
}

static int verificarPin(registroJugador* resultado)
{
    int pinIngresado, intentos;

    intentos = 0;
    do
    {
        printf("Ingrese su PIN: ");
        scanf("%d", &pinIngresado);
        while(getchar() != '\n');

        if(pinIngresado == resultado->pin)
        {
            printf("Bienvenido de nuevo, %s!\n", resultado->nombre);
            return TODO_OK;
        }
        printf("PIN incorrecto.\n");
        intentos++;
    }
    while(intentos < 5);

    printf("Demasiados intentos fallidos.\n");
    return ERROR;
}

static int nuevoPin()
{
    int pin;

    do
    {
        printf("Elija un PIN de 4 digitos: ");
        scanf("%d", &pin);
        while(getchar() != '\n');

        if(pin < 1000 || pin > 9999)
            printf("El PIN debe tener exactamente 4 digitos.\n");
    } while(pin < 1000 || pin > 9999);

    return pin;
}

/**/

void persistirBST(tArbol* arbol)
{
    FILE *pf = fopen(ARCHIVO_BST, "wb");
    if(!pf)
        return;

    recorrerPreAccion(arbol, serializarNodo, pf);

    fclose(pf);
}

void cargarBST(tArbol* arbol)
{
    FILE *pf;
    InfoBST info;

    if(!arbol)
        return;

    crearArbol(arbol);

    pf = fopen(ARCHIVO_BST, "rb");
    if(!pf)
        return;

    while(fread(&info, sizeof(InfoBST), 1, pf) == 1)
        insertarEnArbol(arbol, &info, sizeof(InfoBST), cmpNombre);

    fclose(pf);
}

/**/

int guardarJugador(const registroJugador* j)
{
    FILE *pf = fopen(ARCHIVO_JUGADORES, "ab");
    if(!pf)
        return ERROR_ARCH;

    fwrite(j, sizeof(registroJugador), 1, pf);

    fclose(pf);
    return TODO_OK;
}

static int registrarJugador(tArbol* arbol, const char* nombre, registroJugador* resultado)
{
    InfoBST infoNueva;
    int cantActual;

    cantActual = contarJugadores();

    resultado->id = cantActual;
    strncpy(resultado->nombre, nombre, 31);
    resultado->nombre[31] = '\0';
    resultado->pin = nuevoPin();

    strncpy(infoNueva.nombre, nombre, 31);
    infoNueva.id = cantActual;
    infoNueva.nombre[31] = '\0';
    infoNueva.offset = (long)(cantActual * sizeof(registroJugador));

    if(guardarJugador(resultado) != TODO_OK)
        return ERROR_ARCH;

    insertarEnArbol(arbol, &infoNueva, sizeof(InfoBST), cmpNombre);
    persistirBST(arbol);

    printf("Bienvenido, %s! Se ha registrado su usuario.\n", nombre);
    return TODO_OK;
}

int buscarOCrearJugador(tArbol* arbol, const char* nombre, registroJugador* resultado)
{
    InfoBST clave;
    InfoBST infoNueva;
    FILE *pf;

    strncpy(clave.nombre, nombre, 31);
    clave.nombre[31] = '\0';

    if(buscarEnArbol(arbol, &clave, &infoNueva, sizeof(InfoBST), cmpNombre) == TODO_OK)
    {
        pf = fopen(ARCHIVO_JUGADORES, "rb");
        if(!pf)
            return ERROR_ARCH;
        fseek(pf, infoNueva.offset, SEEK_SET);
        fread(resultado, sizeof(registroJugador), 1, pf);
        fclose(pf);

        return verificarPin(resultado);
    }

    return registrarJugador(arbol, nombre, resultado);
}

/**/

int guardarPartida(const registroPartida* p)
{
    FILE *pf = fopen(ARCHIVO_PARTIDAS, "ab");
    if(!pf)
        return ERROR_ARCH;

    fwrite(p, sizeof(registroPartida), 1, pf);

    fclose(pf);
    return TODO_OK;
}

int obtenerPuntosJugador(int idJugador) /* se considera las partidas perdidas?*/
{
    FILE *pf = fopen(ARCHIVO_PARTIDAS, "rb");
    registroPartida p;
    int total = 0;

    if(!pf)
        return 0;

    while(fread(&p, sizeof(registroPartida), 1, pf))
    {
        if(p.idJugador == idJugador)
            total += p.puntos;
    }

    fclose(pf);
    return total;
}

/**/

static void ordenarRanking(registroRanking* entradas, int cant)
{
    int i, j;
    registroRanking tmp;

    for(i = 0; i < cant - 1; i++)
    {
        for(j = i + 1; j < cant; j++)
        {
            if(entradas[j].puntos > entradas[i].puntos)
            {
                tmp = entradas[i];
                entradas[i] = entradas[j];
                entradas[j] = tmp;
            }
        }
    }
}

static int cargarRegistroRanking(void* dato, void* param)
{
    InfoBST *info = (InfoBST*) dato;
    paramRanking *pr = (paramRanking*) param;
    registroRanking *e;

    e = &pr->registro[pr->cantidad];

    strncpy(e->nombre, info->nombre, 31);
    e->nombre[31] = '\0';
    e->puntos = obtenerPuntosJugador(info->id);

    pr->cantidad++;

    return TODO_OK;
}

void mostrarRanking(tArbol* arbol)
{
    int cantJugadores, i;
    paramRanking pr;

    cantJugadores = contarNodosA(arbol);
    if(!cantJugadores)
    {
        printf("\nNo hay jugadores registrados.\n");
        return;
    }

    pr.registro = malloc(cantJugadores * sizeof(registroRanking));
    if(!pr.registro)
    {
        printf("Error de memoria.\n");
        return;
    }

    pr.cantidad = 0;

    recorrerInAccion(arbol, cargarRegistroRanking, &pr);
    ordenarRanking(pr.registro, pr.cantidad);

    printf("\n========== RANKING ==========\n");
    printf("%-4s %-20s %s\n", "POS", "JUGADOR", "PUNTOS");
    printf("------------------------------\n");

    for(i = 0; i < pr.cantidad; i++)
        printf("%-4d %-20s %d\n", i + 1, pr.registro[i].nombre, pr.registro[i].puntos);

    printf("==============================\n");

    free(pr.registro);
}


