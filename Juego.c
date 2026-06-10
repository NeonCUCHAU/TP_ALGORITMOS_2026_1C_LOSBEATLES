#include "Juego.h"

int menuPrincipal(Config config, const char* archivoTablero, registroJugador* jugadorReg, tArbol* arbol)
{
    int opcion, leidos;

    do
    {
        printf("\n");
        printf("---------------------------------\n");
        printf("     CARAVANA DEL DESIERTO\n");
        printf("---------------------------------\n");
        printf("1 - Nueva partida\n");
        printf("2 - Ver ranking\n");
        printf("0 - Salir\n\n");
        printf("Ingrese una opcion: ");

        leidos = scanf("%d", &opcion);
        while(getchar() != '\n');

        if(leidos != 1)
            opcion = -1;

        switch(opcion)
        {
        case 1:
            iniciarNuevaPartida(config, archivoTablero, jugadorReg, arbol);
            break;
        case 2:
            mostrarRanking(arbol);
            break;
        case 0:
            printf("\nHasta luego.\n");
            break;
        default:
            printf("\nOpcion invalida. Por favor, ingrese un numero valido.\n");
        }
    }
    while(opcion != 0);

    return TODO_OK;
}

int lanzarDado()
{
    return (rand() % 6) + 1;
}

void CrearBandidos(tListaCircularD ruta, tLista* listaBandidos)
{
    tNodoD *actual;
    Casilla *c;
    Bandido b;
    int id = 0;

    crearLista(listaBandidos);

    if(!ruta)
        return;

    actual = ruta;

    do
    {
        c = (Casilla*) actual->info;

        if(c->bandidos == 1)
        {
            b.id        = id++;
            b.estado    = 1;
            b.posActual = actual;
            ponerAlFinal(listaBandidos, &b, sizeof(Bandido));
        }
        actual = actual->sig;
    }
    while(actual != ruta);
}

void iniciarNuevaPartida(Config config, const char* archivoTablero, registroJugador* jugadorReg, tArbol* arbol)
{

}

void mostrarEstado(Jugador *jugador, int turno)
{
    printf("\n--- Turno %d ---\n", turno);
    printf("Vidas:  %d\n", jugador->vidas);
    printf("Puntos: %d\n", jugador->puntos);

    if(jugador->protegido)
        printf("Estado: PROTEGIDO por oasis\n");
    else if(jugador->pierdeTurno)
        printf("Estado: PIERDE TURNO por tormenta\n");
    else
        printf("Estado: Normal\n");
}

void mostrarMovimientos(tLista *log)
{
    tNodo *aux;
    Movimiento m;

    printf("\n===== REGISTRO DE MOVIMIENTOS =====\n");

    if(listaVacia(log))
    {
        printf("Sin movimientos registrados.\n");
        printf("\n===================================\n");
        return;
    }

    aux = *log;
    while(aux != NULL)
    {
        memcpy(&m, aux->info, sizeof(Movimiento));
        printf("%c%d ", m.dir, m.pasos);
        aux = aux->sig;
    }
    printf("\n===================================\n");
}
