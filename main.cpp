// ===================================================================
// TRABAJO PRACTICO INTEGRADOR - Programacion Orientada a Objetos 2026
// Sistema de Gestion de un Refugio de Animales
//
// main.cpp: punto de entrada del programa, y nada mas.
//
// Aca no hay logica del sistema: main() solo crea el objeto que maneja
// la interfaz de consola y lo pone a funcionar. El menu, el manejo de
// las excepciones y las operaciones viven en MenuConsola; las reglas
// del negocio, en la clase Refugio.
// ===================================================================

#include "MenuConsola.h"

int main() {
    // El menu se crea dentro de un bloque propio: al cerrarse el bloque
    // se destruye, y con el se destruye el Refugio que contiene. Recien
    // ahi tiene sentido preguntar si quedo memoria sin liberar.
    {
        MenuConsola sistema("Refugio Huellitas");
        sistema.ejecutar();
    }

    MenuConsola::informarMemoriaLiberada();
    return 0;
}
