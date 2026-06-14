#include "Juego.h"

static void pausa(void)
{
    printf(MAGENTA "\nPresione Enter para continuar..." RESET);
    getchar();
}

int menuPrincipal(Config config, const char* archivoTablero, registroJugador* jugadorReg, tArbol* arbol)
{
    int opcion, leidos;
    do
    {
        LIMPIAR_PANTALLA;
        printf(MAGENTA "===========================================\n");
        printf("         CARAVANA DEL DESIERTO\n");
        printf("===========================================\n" RESET);
        printf(CYAN " [1] " RESET " Nueva Partida\n");
        printf(CYAN " [2] " RESET " Ver Ranking\n");
        printf(RED " [0] " RESET " Salir\n");
        printf(MAGENTA "-------------------------------------------\n" RESET);
        printf("Ingrese una opcion: ");

        leidos = scanf("%d", &opcion);
        while(getchar() != '\n');

        if(leidos != 1)
            opcion = -1;

        switch(opcion)
        {
        case 1:
            iniciarNuevaPartida(config, archivoTablero, jugadorReg, arbol);
            pausa();
            break;
        case 2:
            LIMPIAR_PANTALLA;
            mostrarRanking(arbol);
            pausa();
            break;
        case 0:
            printf(GREEN "\nGracias por jugar. Vuelve pronto!\n" RESET);
            pausa();
            break;
        default:
            printf(RED "\n[!] Opcion invalida. Intente de nuevo.\n" RESET);
            pausa();
        }
    }
    while(opcion != 0);

    return TODO_OK;
}

int aplicarMovBandidoCallback(void *dato, void *parametro)
{
    Bandido *b = (Bandido*) dato;
    sParamsAplicarMov *p = (sParamsAplicarMov*) parametro;
    Casilla *c;
    int i;
    unsigned nuevaPos;

    if(b->estado != 1 || b->id != p->idBuscado)
        return TODO_OK;

    c = (Casilla*) obtenerInfoPorIndiceD(p->tablero, b->posActual);
    if(c)
        c->bandidos = 0;

    nuevaPos = b->posActual;
    for(i = 0; i < p->pasos; i++)
        nuevaPos = (nuevaPos + 1) % p->cantNodos;

    b->posActual = nuevaPos;

    c = (Casilla*) obtenerInfoPorIndiceD(p->tablero, b->posActual);
    if(c)
        c->bandidos = 1;

    return TODO_OK;
}

void iniciarNuevaPartida(Config config, const char* archivoTablero, registroJugador* jugadorReg, tArbol* arbol)
{
    tListaCircularD tablero;
    tLista listaBandidos;
    tLista logMovimientos;
    tCola cola;
    Jugador jugador;
    registroPartida partida;
    unsigned cantNodos;
    int turno = 1, gano = 0;

    /* 1. y 2. Generar y Cargar tablero */
    if(cargarTableroATxt(config, archivoTablero) != TODO_OK || cargarTableroDesdeTxt(&tablero, archivoTablero) != TODO_OK)
    {
        printf("Error al generar o cargar el tablero.\n");
        return;
    }

    cantNodos = contarNodosListaCircularD(&tablero);
    CrearBandidos(tablero, &listaBandidos);
    crearLista(&logMovimientos);

    /* 3. Inicializar jugador */
    jugador.vidas       = config.vidasInicio;
    jugador.puntos      = 0;
    jugador.protegido   = 0;
    jugador.pierdeTurno = 0;
    jugador.posActual   = 0;

    /* ================= LOOP PRINCIPAL ================= */
    while(jugador.vidas > 0 && !gano)
    {
        LIMPIAR_PANTALLA;
        printf(MAGENTA "===========================================\n");
        printf("              TURNO %d - JUGANDO           \n", turno);
        printf("===========================================\n" RESET);

        mostrarTablero(&tablero);
        mostrarEstado(&jugador, turno);
        crearCola(&cola);

        /* Desgaste del escudo */
        if(jugador.protegido > 0) {
            jugador.protegido--;
            if(jugador.protegido == 0)
                printf(YELLOW "(!) El efecto protector del oasis se ha desvanecido.\n" RESET);
            else
                printf(CYAN "(*) Proteccion activa. Turnos restantes: %d\n" RESET, jugador.protegido);
        }

        /* Lógica del Turno: Jugador y Bandidos */
        if(jugador.pierdeTurno) {
            printf(YELLOW "Perdiste el turno por la tormenta, pero los bandidos acechan...\n" RESET);
            jugador.pierdeTurno = 0; /* Se limpia el estado, NO encolamos movimiento del jugador */
        } else {
            pedirMovimientoJugador(&jugador, &cola);
        }

        /* Los bandidos SIEMPRE se encolan, sin importar si el jugador perdió el turno */
        encolarMovBandidos(&tablero, &listaBandidos, &jugador, &cola);

        /* Desencolar y aplicar todos los movimientos */
        procesarColaMovimientos(&cola, &jugador, &tablero, &listaBandidos, &logMovimientos, cantNodos, &gano);

        vaciarCola(&cola);
        printf(MAGENTA "\n--------------------------------------\n" RESET);
        pausa();
        turno++;
    }
    /* ================= FIN LOOP PRINCIPAL ================= */

    /* 4. Mostrar log y guardar partida */
    mostrarMovimientos(&logMovimientos);

    partida.id          = 0;
    partida.idJugador   = jugadorReg->id;
    partida.puntos      = jugador.puntos;
    partida.movimientos = turno;
    partida.gano        = gano;
    guardarPartida(&partida);

    /* 5. Fin de Partida y Limpieza */
    LIMPIAR_PANTALLA;
    printf(MAGENTA "==================\n");
    if(gano)
        printf(GREEN "    VICTORIA! \n" RESET);
    else
        printf(RED "    GAME OVER \n" RESET);
    printf(MAGENTA "==================\n");

    printf(GREEN "Puntos finales: %d\n", jugador.puntos);

    vaciarLista(&logMovimientos);
    vaciarLista(&listaBandidos);
    vaciarListaCircularD(&tablero);
}
    /* 7. Mostrar log */
    mostrarMovimientos(&logMovimientos);

    /* 8. Guardar partida */
    partida.id          = 0;
    partida.idJugador   = jugadorReg->id;
    partida.puntos      = jugador.puntos;
    partida.movimientos = turno;
    partida.gano        = gano;
    guardarPartida(&partida);

    /* FIN DE PARTIDA */
    LIMPIAR_PANTALLA;
    printf(MAGENTA "==================\n");
    if(gano)
        printf(GREEN "    VICTORIA! \n" RESET);
    else
        printf(RED "     GAME OVER \n" RESET);
    printf(MAGENTA "==================\n");

    printf(GREEN "Puntos finales: %d\n", jugador.puntos);
    vaciarLista(&logMovimientos);
    vaciarLista(&listaBandidos);
    vaciarListaCircularD(&tablero);
}

int lanzarDado(void)
{
    return (rand() % 6) + 1;
}

int procesarCasillaParaBandidoCallback(void *dato, void *parametro)
{
    Casilla *c = (Casilla*) dato;
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
    return TODO_OK;
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

int procesarBandidoCallback(void *dato, void *parametro)
{
    Bandido *b = (Bandido*) dato;
    sParamsBandido *params = (sParamsBandido*) parametro;
    Movimiento mov;
    int encontrado;
    unsigned posicion;
    unsigned cantNodos;

    if(b->estado != 1)
        return CONTINUAR;

    cantNodos = contarNodosListaCircularD(params->tablero);
    if(cantNodos == 0)
        return ERROR;

    posicion = (b->posActual + 1) % cantNodos;
    encontrado = 0;

    while(posicion != b->posActual && !encontrado)
    {
        if(posicion == params->jugador->posActual)
            encontrado = 1;
        else
            posicion = (posicion + 1) % cantNodos;
    }

    mov.dir = encontrado ? 'f' : 'b';
    mov.pasos = lanzarDado();
    ponerEnCola(params->cola, &mov, sizeof(Movimiento));

    return TODO_OK;
}

void encolarMovBandidos(tListaCircularD *tablero, tLista *listaBandidos, const Jugador *jugador, tCola *cola)
{
    sParamsBandido params;

    params.tablero = tablero;
    params.jugador = jugador;
    params.cola = cola;

    recorrerLista(listaBandidos, procesarBandidoCallback, &params);
}

int compararBandidoPorId(const void *dato, void *criterio)
{
    const Bandido *b = (const Bandido*) dato;
    int *idBuscado = (int*) criterio;
    return b->id == *idBuscado;
}

int verificarColisionBandidoCallback(void *dato, void *parametro)
{
    Bandido *b = (Bandido*) dato;
    sParamsVerificarColision *params = (sParamsVerificarColision*) parametro;
    Casilla *c;

    if(b->estado != 1 || b->posActual != params->jugador->posActual)
        return CONTINUAR;

    if(params->jugador->protegido > 0)
    {
        printf("\nBandido cerca, pero estas PROTEGIDO por el oasis!\n");
        params->jugador->protegido = 0; /*Al usarlo contra el bandido, se consume el escudo*/
        params->resultado = JUGADOR_PROTEGIDO;
        params->bandidoEncontrado = 0; /*No eliminamos al bandido*/
        return DETENER_ITERACION;
    }

    /*Si llega acá, hay colisión real*/
    params->bandidoEncontrado = 1;
    params->bandidoId = b->id;
    params->jugador->vidas--;

    /*Limpiar bandido de la casilla usando el puntero*/
    c = (Casilla*) obtenerInfoPorIndiceD(params->tablero, b->posActual);
    if(c)
        c->bandidos = 0;

    printf("\nUn bandido te intercepto! Vidas: %d\n", params->jugador->vidas);

    /*Quitar jugador de la casilla actual*/
    c = (Casilla*) obtenerInfoPorIndiceD(params->tablero, params->jugador->posActual);
    if(c)
        c->jugador = 0;

    /*Volver al inicio*/
    params->jugador->posActual = params->posInicio;
    c = (Casilla*) obtenerInfoPorIndiceD(params->tablero, params->posInicio);
    if(c)
        c->jugador = 1;

    params->resultado = JUGADOR_PIERDEVIDA;
    return DETENER_ITERACION;
}

int verificarColisionBandido(Jugador *jugador, tLista *listaBandidos, tListaCircularD *tablero, int posInicio)
{
    sParamsVerificarColision params;

    params.jugador = jugador;
    params.tablero = tablero;
    params.posInicio = posInicio;
    params.resultado = TODO_OK;
    params.bandidoId = -1;
    params.bandidoEncontrado = 0;

    recorrerLista(listaBandidos, verificarColisionBandidoCallback, &params);

    if(params.bandidoEncontrado)
        eliminarPorCondicion(listaBandidos, compararBandidoPorId, &params.bandidoId);

    return params.resultado;
}

void pedirMovimientoJugador(Jugador* jugador, tCola* cola)
{
    char dirInput;
    int dado;
    MovimientoEncolado movEnc;

    dado = lanzarDado();

    printf(CYAN "Dado: " RESET "%d\n", dado);

    do {
        printf(CYAN "Direccion" RESET " (F, B): ");
        scanf(" %c", &dirInput);
        while(getchar() != '\n');
        dirInput = (char)toupper((unsigned char)dirInput);
        
         /* VALIDACIÓN ESTRICTA: Si el dado supera la posición actual, prohibir ir hacia atrás */
        if (dirInput == 'B' && dado > (int)jugador->posActual) {
            printf(RED "[!] No puedes retroceder. El dado (%d) supera tu distancia al inicio (%d).Debes ir hacia adelante (F).\n" RESET, dado, jugador->posActual);
            dirInput = 'X'; /* Forzamos a que el bucle se repita */
        }
    } while(dirInput != 'F' && dirInput != 'B');

    movEnc.esBandido = 0;
    movEnc.idBandido = -1;
    movEnc.dir       = dirInput;
    movEnc.pasos     = dado;

    ponerEnCola(cola, &movEnc, sizeof(MovimientoEncolado));
}

/* 2. PROCESAR INTERACCIONES DEL JUGADOR  */
/* Maneja la recolección de items y los efectos del terreno */
void procesarCasillaJugador(Jugador* jugador, Casilla* casilla, int* gano)
{
    if (!casilla) return;

    /* ITEMS */
    if(casilla->item == CELDA_PREMIO) {
        jugador->puntos += 1;
        printf(GREEN "[+] Premio encontrado! +1 punto.\n" RESET);
        casilla->item = CELDA_VACIA;
    }
    else if(casilla->item == CELDA_VIDA) {
        jugador->vidas++;
        printf(GREEN "[V] Vida extra recuperada!\n" RESET);
        casilla->item = CELDA_VACIA;
    }

    /* TERRENO */
    if(casilla->terreno == CELDA_SALIDA) {
        printf(GREEN "\n======================================\n");
        printf("  Llegaste a la salida! HAS GANADO!\n");
        printf("======================================\n" RESET);
        *gano = 1;
    }
    else if(casilla->terreno == CELDA_OASIS) {
        printf(CYAN "[~] Oasis! Estas protegido durante este y el proximo turno.\n" RESET);
        jugador->protegido = 2;
    }
    else if(casilla->terreno == CELDA_TORMENTA) {
        if(jugador->protegido > 0) {
            printf(CYAN "[~] Tormenta! Pero el escudo te ha protegido.\n" RESET);
        } else {
            printf(RED "[!] Tormenta! Perderas el proximo turno.\n" RESET);
            jugador->pierdeTurno = 1;
        }
        jugador->protegido = 0; /* El escudo se consume tras la tormenta */
    }
}

/* 3. PROCESAR COLA DE MOVIMIENTOS */
/* Desencola y ejecuta los movimientos del jugador y los bandidos */
void procesarColaMovimientos(tCola* cola, Jugador* jugador, tListaCircularD* tablero, tLista* listaBandidos, tLista* logMovimientos, unsigned cantNodos, int* gano)
{
    MovimientoEncolado movEnc;
    Movimiento mov;
    Casilla* casilla;
    sParamsAplicarMov paramsAplicar;

    while(!colaVacia(cola))
    {
        sacarDeCola(cola, &movEnc, sizeof(MovimientoEncolado));

        if(!movEnc.esBandido)
        {
            /* MOVIMIENTO DEL JUGADOR */
            casilla = (Casilla*) obtenerInfoPorIndiceD(tablero, jugador->posActual);
            if(casilla)
                casilla->jugador = 0;

            if(movEnc.dir == 'F') {
                unsigned nuevaPos = jugador->posActual + movEnc.pasos;
                if(nuevaPos >= cantNodos)
                    nuevaPos = (cantNodos - 1) - (nuevaPos - (cantNodos - 1));
                jugador->posActual = nuevaPos;
            } else {
                jugador->posActual -= movEnc.pasos; /* Ya validamos en pedirMovimientoJugador que no sea < 0 */
            }

            casilla = (Casilla*) obtenerInfoPorIndiceD(tablero, jugador->posActual);
            if(casilla)
                casilla->jugador = 1;

            /* Registrar movimiento */
            mov.dir   = movEnc.dir;
            mov.pasos = movEnc.pasos;
            ponerAlFinal(logMovimientos, &mov, sizeof(Movimiento));

            /* Procesar terreno e items */
            procesarCasillaJugador(jugador, casilla, gano);

            /* Verificar colisión inmediatamente tras moverse */
            if(!(*gano) && jugador->vidas > 0)
                verificarColisionBandido(jugador, listaBandidos, tablero, 0);
        }
        else if(!(*gano) && jugador->vidas > 0)
        {
            /* MOVIMIENTO DE BANDIDO */
            paramsAplicar.tablero   = tablero;
            paramsAplicar.cantNodos = cantNodos;
            paramsAplicar.idBuscado = movEnc.idBandido;
            paramsAplicar.pasos     = movEnc.pasos;
            paramsAplicar.dir       = (char)tolower((unsigned char)movEnc.dir);

            recorrerLista(listaBandidos, aplicarMovBandidoCallback, &paramsAplicar);

            /* Verificar colisión tras moverse el bandido */
            verificarColisionBandido(jugador, listaBandidos, tablero, 0);
        }
    }
}

static void imprimirMovimiento(const void *dato)
{
    const Movimiento *m = (const Movimiento*) dato;
    char dir = (char)toupper((unsigned char)m->dir);

    if(dir == 'F')
        printf(GREEN "F" RESET "(%d) ", m->pasos);
    else
        printf(YELLOW "B" RESET "(%d) ", m->pasos);
}

void mostrarMovimientos(tLista *log)
{
    printf(MAGENTA "\n========== REGISTRO DE MOVIMIENTOS ==========\n" RESET);

    if(listaVacia(log))
    {
        printf(YELLOW "   Sin movimientos registrados.\n" RESET);
        printf(MAGENTA "=============================================\n" RESET);
        return;
    }

    mostrarLista(log, imprimirMovimiento);

    printf(MAGENTA "=============================================\n" RESET);
}

void mostrarEstado(Jugador *jugador, int turno)
{
    printf(MAGENTA "\n-------------- TURNO %2d --------------\n" RESET, turno);
    printf(CYAN "Vidas: " RESET "%-12d " CYAN "Puntos: " RESET "%d\n", jugador->vidas, jugador->puntos);

    printf(CYAN "Estado: " RESET);
    if(jugador->protegido > 0)
        printf(GREEN "PROTEGIDO (Oasis activo)\n" RESET);
    else if(jugador->pierdeTurno)
        printf(RED "PIERDE TURNO (Tormenta)\n" RESET);
    else
        printf("Normal\n");

    printf(MAGENTA "--------------------------------------\n" RESET);
}

