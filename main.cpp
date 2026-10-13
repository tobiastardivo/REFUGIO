#include "MenuConsola.h"

int main() {
    {
        MenuConsola sistema("Refugio Huellitas");
        sistema.ejecutar();
    }

    MenuConsola::informarMemoriaLiberada();
    return 0;
}
