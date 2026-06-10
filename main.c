#include <time.h>
#include "Tipos.h"
#include "Tablero.h"
#include "Juego.h"
#include "Datos.h"

#define CONFIG_TXT   1
#define CARAVANA_TXT 2

int main(int argc, char* argv[])
{
    Config config;
    tArbol arbol;
    registroJugador jugadorActual;
    char nombre[32];

    srand(time(NULL));

    if(LeerConfig(argv[CONFIG_TXT], &config) != TODO_OK)
    {
        printf("Error LeerConfig\n");
        return ERROR;
    }

    cargarBST(&arbol);

    printf("Ingrese su nombre: ");
    scanf("%31s", nombre);
    while(getchar() != '\n');

    /*menuPrincipal();*/

    if(buscarOCrearJugador(&arbol, nombre, &jugadorActual) != TODO_OK)
    {
        printf("Error al identificar jugador.\n");
        vaciarArbol(&arbol);
        return ERROR;
    }

    menuPrincipal(config, argv[CARAVANA_TXT], &jugadorActual, &arbol);

    vaciarArbol(&arbol);
    return TODO_OK;
}
