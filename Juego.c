#include "Juego.h"

int menuPrincipal()
{
    int opcion = 0;

    do
    {
        printf("\n");
        printf("---------------------------------\n");
        printf("     CARAVANA DEL DESIERTO\n");
        printf("---------------------------------\n");
        printf("1 - Nueva partida(proximamente)\n");
        printf("2 - Ver ranking(proximamente)\n");
        printf("0 - Salir\n");
        printf("\n");
        printf("Ingrese una opcion: ");

        scanf("%d", &opcion);

        switch(opcion)
        {
            case 1:
                printf("proximamente\n");
                //iniciarNuevaPartida();
                break;

            case 2:
                printf("proximamenteX2\n");
                //mostrarRanking();
                break;

            case 0:
                printf("\nHasta luego.\n");
                break;

            default:
                printf("\nOpcion invalida.\n");
        }

    } while(opcion != 0);

    return TODO_OK;
}

int lanzarDado()
{
    return (rand() % 6) + 1;
}

void CrearBandidos(tListaCircularD ruta, tNodo** listaBandidos)// ruta es la listacircular del tablero
{
    tNodoD *actual;
    Casilla *c;
    Bandido b;
    int id = 0;

    crearLista(listaBandidos);
    if(!ruta)
        return;

    actual = ruta;

    do{
        c = (Casilla*) actual->info;
        if(c->tipo == CELDA_BANDIDO)
        {
            b.id = id++;
            b.estado = 1;
            b.posActual = actual;
            ponerAlFinal(listaBandidos, &b, sizeof(Bandido));
        }
        actual = actual->sig;
    }while(actual != ruta);
}



