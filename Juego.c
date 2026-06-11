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

static void imprimirMovimiento(const void *dato)
{
    const Movimiento *m = (const Movimiento*) dato;
    printf("%c%d ", m->dir, m->pasos);
}

void mostrarMovimientos(tLista *log)
{
    printf("\n===== REGISTRO DE MOVIMIENTOS =====\n");

    if(listaVacia(log))
    {
        printf("Sin movimientos registrados.\n");
        printf("\n===================================\n");
        return;
    }

    mostrarLista(log, imprimirMovimiento);
    printf("\n===================================\n");
}

void encolarMovBandidos(tLista listaBandidos, const Jugador *jugador, tCola *cola)
{
    tNodo      *actual;
    Bandido    *b;
    tNodoD     *cursor;
    Movimiento  mov;
    int         encontrado;

    actual = listaBandidos;
    while(actual != NULL)
    {
        b = (Bandido*) actual->info;

        if(b->estado == 1)
        {
            /* Avanzar desde el bandido: si llegamos al jugador antes
               de dar vuelta completa, el jugador esta adelante */
            cursor     = b->posActual->sig;
            encontrado = 0;

            while(cursor != b->posActual && !encontrado)
            {
                if(cursor == jugador->posActual)
                    encontrado = 1;
                else
                    cursor = cursor->sig;
            }

            mov.dir   = encontrado ? 'f' : 'b';//[F(Forward),B(Backward)] en minusculas para bandidos en mayusculas para jugador
            mov.pasos = lanzarDado();
            ponerEnCola(cola, &mov, sizeof(Movimiento));
        }
        actual = actual->sig;
    }
}

int verificarColisionBandido(Jugador *jugador, tLista *listaBandidos,tNodoD *nodoInicio)
//nodoInicio le mando por parametro la posicion inicial del tablero
{
    tNodo   *actual;
    Bandido *b;
    Casilla *c;

    actual = *listaBandidos;
    while(actual != NULL)
    {
        b = (Bandido*) actual->info;
        if(b->estado == 1 && b->posActual == jugador->posActual)
        {
            if(jugador->protegido == 1)
            {
                printf("  >> Bandido cerca, pero estas PROTEGIDO por el oasis!\n");
                jugador->protegido = 0;
                eliminarNodo(listaBandidos, actual);
                return JUGADOR_PROTEGIDO;
            }
            jugador->vidas--;

            /* Limpiar bandido de la casilla */
            c = (Casilla*) b->posActual->info;
            c->bandidos = 0;

            printf("  >> Un bandido te intercepto! Vidas: %d\n", jugador->vidas);
            /* Quitar jugador de la casilla actual */
            c = (Casilla*) jugador->posActual->info;
            c->jugador = 0;

            /* Volver al inicio */
            jugador->posActual = nodoInicio;
            c = (Casilla*) nodoInicio->info;
            c->jugador = 1;

            eliminarNodo(listaBandidos, actual);
            return JUGADOR_PIERDEVIDA;
        }
        actual = actual->sig;
    }
    return TODO_OK;
}