#include <time.h>
#include "Tipos.h"
#include "Tablero.h"
#include "Juego.h"

#define CONFIG_TXT 1
#define CARAVANA_TXT 2

int main(int argc, char* argv[])
{
    Config configuracionTest;
    tListaCircularD listaTest;

    srand(time(NULL));

    if(LeerConfig(argv[CONFIG_TXT], &configuracionTest) != TODO_OK)
        printf("Error LeerConfig\n");

    if(cargarTableroATxt(configuracionTest, argv[CARAVANA_TXT]) != TODO_OK)
        printf("Error cargarTableroATxt\n");

    if(cargarTableroDesdeTxt(&listaTest, argv[CARAVANA_TXT]) != TODO_OK)
        printf("Error cargarTableroDesdeTxt\n");

    //menuPrincipal();

    mostrarTablero(&listaTest);

    vaciarListaCircularD(&listaTest);
    return 0;
}
