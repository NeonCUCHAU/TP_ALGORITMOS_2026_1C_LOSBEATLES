#include "Tipos.h"
#include "Tablero.h"

#define CONFIG_TXT 1
#define CARAVANA_TXT 2

int main(int argc, char* argv[]) {
Config configuracionTest;

LeerConfig(argv[CONFIG_TXT], &configuracionTest);
cargarTableroATxt(configuracionTest, argv[CARAVANA_TXT]);
return 0;
}
