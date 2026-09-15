#ifndef MENUCONSOLA_H
#define MENUCONSOLA_H

#include <string>
#include "Refugio.h"

// ===================================================================
// MenuConsola: toda la interfaz de consola del sistema.
//
// main() no tiene logica: solo crea este objeto y lo pone a funcionar.
// Aca viven el bucle del menu, el despacho de cada opcion, el manejo
// de las excepciones y las funciones que piden los datos por teclado.
//
// COMPOSICION (Fase 4): el MenuConsola "tiene" un Refugio. Cuando se
// destruye el menu se destruye el refugio, y con el toda la memoria
// dinamica del catalogo.
// ===================================================================

class MenuConsola {
private:
    Refugio refugio;                 // composicion: el menu es duenio del refugio

    // --- Presentacion ---
    void mostrarMenu() const;

    // --- Despacho: manda a ejecutar la opcion elegida ---
    void ejecutarOpcion(int opcion);

    // --- Una funcion por cada opcion del menu ---
    void registrarPerro();
    void registrarGato();
    void registrarAdoptante();
    void buscarAnimalPorId() const;
    void crearSolicitud();
    void confirmarSolicitud();
    void cancelarSolicitud();
    void devolverAnimal();
    void verAdoptantesYSolicitudes() const;
    void pruebasDeIntegridad();

public:
    explicit MenuConsola(const std::string& nombreRefugio = "Refugio Huellitas");
    ~MenuConsola();

    // Bucle principal: se repite hasta que el usuario elige la opcion 0.
    void ejecutar();

    // Informe final de memoria. Es ESTATICO porque se llama despues de que
    // el menu y el refugio ya fueron destruidos: no hay objeto al que
    // pedirselo, y lo que se muestra es el contador estatico de Animal.
    static void informarMemoriaLiberada();
};

#endif //MENUCONSOLA_H
