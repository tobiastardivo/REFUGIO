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
// REGLA DE LA INTERFAZ: el usuario nunca escribe un ID.
// Los IDs son internos del modelo (los usan el operator() del Refugio, el
// operator== de las clases y las solicitudes), pero quien usa el programa
// no los conoce ni tiene por que recordarlos. Por eso:
//   - en las altas el numero lo pone el Refugio (nuevoIdAnimal(), etc.);
//   - cuando hay que elegir algo se muestra la lista numerada de 1 a N y
//     se elige por posicion; las funciones elegir*() traducen ese numero
//     de lista al ID interno antes de llamar al Refugio.
//
// COMPOSICION (Fase 4): el MenuConsola "tiene" un Refugio. Cuando se
// destruye el menu se destruye el refugio, y con el toda la memoria
// dinamica del catalogo.
// ===================================================================

class MenuConsola {
private:
    Refugio refugio;                 // composicion: el menu es duenio del refugio

    // Que animales ofrece elegirAnimal() en cada opcion del menu.
    enum class Filtro { TODOS, DISPONIBLES, NO_DISPONIBLES };

    // --- Presentacion ---
    void mostrarMenu() const;        // dibuja las opciones + el resumen de estado

    // --- Seleccion por lista (reemplaza a pedir el ID por teclado) ---
    // Las tres muestran la lista numerada, leen UN numero de esa lista y
    // devuelven el ID interno del elemento elegido. Si no hay nada para
    // elegir lanzan DatoInvalidoException con el motivo, asi el usuario se
    // entera antes de cargar ningun otro dato.
    int elegirAnimal(const std::string& titulo, Filtro filtro) const;
    int elegirAdoptante(const std::string& titulo) const;
    int elegirSolicitudPendiente(const std::string& titulo) const;

    // Auxiliares de esos listados: deciden si un animal entra en el filtro
    // y traducen los IDs guardados en una solicitud a nombres, para que las
    // listas se lean en castellano y no en numeros.
    static bool pasaElFiltro(const Animal& animal, Filtro filtro);
    std::string nombreDeAdoptante(int idAdoptante) const;
    void mostrarSolicitud(const SolicitudAdopcion& solicitud, int numeroDeLista) const;

    // --- Despacho: manda a ejecutar la opcion elegida ---
    void ejecutarOpcion(int opcion); // un switch que solo decide a quien llamar

    // --- Una funcion por cada opcion del menu ---
    // Estan en el mismo orden en que aparecen en pantalla. Las opciones 3,
    // 4, 12 y 13 no figuran aca porque se resuelven con un unico llamado
    // directo al Refugio y no necesitan pedir ningun dato.
    // Son const las que solo consultan y no modifican nada.
    void registrarPerro();                     // 1  - alta de un perro
    void registrarGato();                      // 2  - alta de un gato
    void registrarAdoptante();                 // 5  - alta de una persona
    void verFichaDeAnimal() const;             // 6  - usa los operadores () y !
    void crearSolicitud();                     // 7  - inicia un tramite
    void confirmarSolicitud();                 // 8  - cierra el tramite: ADOPTADO
    void cancelarSolicitud();                  // 9  - da de baja: vuelve DISPONIBLE
    void devolverAnimal();                     // 10 - el animal regresa al refugio
    void verAdoptantesYSolicitudes() const;    // 11 - usa la funcion generica
    void pruebasDeIntegridad();                // 14 - bateria de pruebas del TP

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
