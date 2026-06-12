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

int procesarCasillaParaBandidoCallback(const void *dato, void *parametro)
{
    const Casilla *c = (const Casilla*) dato;
    sParamsCrearBandido *params = (sParamsCrearBandido*) parametro;
    Bandido b;

    if(c->bandidos == 1)
    {
        b.id        = (*params->idBandido)++;
        b.estado    = 1;
        b.posActual = *params->posicion;
        ponerAlFinal(params->listaBandidos, &b, sizeof(Bandido));
    }
    
    (*params->posicion)++;
    return CONTINUAR;
}

void CrearBandidos(tListaCircularD ruta, tLista* listaBandidos)
{
    sParamsCrearBandido params;
    unsigned posicion;
    int idBandido;

    crearLista(listaBandidos);

    if(!ruta)
        return;

    posicion = 0;
    idBandido = 0;
    params.listaBandidos = listaBandidos;
    params.posicion = &posicion;
    params.idBandido = &idBandido;

    recorrerListaCircularD(&ruta, procesarCasillaParaBandidoCallback, &params);
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

int procesarBandidoCallback(const void *dato, void *parametro)
{
    const Bandido *b = (const Bandido*) dato;
    sParamsBandido *params = (sParamsBandido*) parametro;
    Movimiento mov;
    int encontrado;
    unsigned posicion;
    unsigned cantNodos;

    if(b->estado != 1)
        return CONTINUAR;

    //Contar nodos para iterar
    cantNodos = contarNodosListaCircularD(&params->tablero);
    if(cantNodos == 0)
        return ERROR;

    //Avanzar desde el bandido: si llegamos al jugador antes
    // de dar vuelta completa, el jugador esta adelante
    posicion = (b->posActual + 1) % cantNodos;
    encontrado = 0;

    while(posicion != b->posActual && !encontrado)
    {
        if(posicion == params->jugador->posActual)
            encontrado = 1;
        else
            posicion = (posicion + 1) % cantNodos;
    }

    mov.dir   = encontrado ? 'f' : 'b';
    mov.pasos = lanzarDado();
    ponerEnCola(params->cola, &mov, sizeof(Movimiento));

    return TODO_OK;
}

void encolarMovBandidos(tListaCircularD tablero, tLista listaBandidos, const Jugador *jugador, tCola *cola)
{
    sParamsBandido params;

    params.tablero = tablero;
    params.jugador = jugador;
    params.cola = cola;

    recorrerLista(&listaBandidos, procesarBandidoCallback, &params);
}

/* Comparador para buscar bandido por ID */
int compararBandidoPorId(const void *dato, void *criterio)
{
    const Bandido *b = (const Bandido*) dato;
    int *idBuscado = (int*) criterio;
    return b->id == *idBuscado;
}

int verificarColisionBandidoCallback(const void *dato, void *parametro)
{
    const Bandido *b = (const Bandido*) dato;
    sParamsVerificarColision *params = (sParamsVerificarColision*) parametro;
    Casilla *c;

    if(b->estado != 1 || b->posActual != params->jugador->posActual)
        return CONTINUAR;

    /* Colisión detectada */
    params->bandidoEncontrado = 1;
    params->bandidoId = b->id;
    
    if(params->jugador->protegido == 1)
    {
        printf("Bandido cerca, pero estas PROTEGIDO por el oasis!\n");
        params->jugador->protegido = 0;
        params->resultado = JUGADOR_PROTEGIDO;
        return ERROR;  // Detener iteración
    }

    params->jugador->vidas--;

    /* Limpiar bandido de la casilla */
    c = (Casilla*) obtenerInfoPorIndiceD(&params->tablero, b->posActual);
    if(c)
        c->bandidos = 0;

    printf("Un bandido te intercepto! Vidas: %d\n", params->jugador->vidas);
    
    /* Quitar jugador de la casilla actual */
    c = (Casilla*) obtenerInfoPorIndiceD(&params->tablero, params->jugador->posActual);
    if(c)
        c->jugador = 0;

    /* Volver al inicio */
    params->jugador->posActual = params->posInicio;
    c = (Casilla*) obtenerInfoPorIndiceD(&params->tablero, params->posInicio);
    if(c)
        c->jugador = 1;

    params->resultado = JUGADOR_PIERDEVIDA;
    return TODO_OK;
}

int verificarColisionBandido(Jugador *jugador, tLista *listaBandidos, tListaCircularD tablero, int posInicio)

{
    sParamsVerificarColision params;

    params.jugador = jugador;
    params.listaBandidos = listaBandidos;
    params.tablero = tablero;
    params.posInicio = posInicio;
    params.resultado = TODO_OK;
    params.bandidoId = -1;
    params.bandidoEncontrado = 0;

    //Procesar colisiones con callback
    recorrerLista(listaBandidos, verificarColisionBandidoCallback, &params);

    // Si se encontró colisión, eliminar el bandido por ID (sin exponer tNodo)
    if(params.bandidoEncontrado)
    {
        eliminarPorCondicion(listaBandidos, compararBandidoPorId, &params.bandidoId);
        return params.resultado;
    }

    return TODO_OK;
}