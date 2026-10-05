#ifndef REFUGIO_H
#define REFUGIO_H

#include <string>
#include "Animal.h"
#include "Adoptante.h"
#include "SolicitudAdopcion.h"
#include "Coleccion.h"

// ===================================================================
// Refugio: clase que administra todo el sistema.
//
// COMPOSICION (Fase 4): el Refugio "tiene" un catalogo de animales,
// una coleccion de adoptantes y una de solicitudes. Cuando muere el
// Refugio mueren tambien esos objetos.
//
// MEMORIA DINAMICA (Fase 2):
//   - catalogo es un arreglo dinamico de PUNTEROS a Animal (Animal**).
//     Se usan punteros para poder guardar Perros y Gatos mezclados y
//     recorrerlos polimorficamente.
//   - Quien reserva: agregarAnimal() recibe un Animal* creado con new
//     en el menu; a partir de ahi la memoria PERTENECE al Refugio.
//   - Quien libera: el destructor ~Refugio() hace delete de cada animal
//     y despues delete[] del arreglo de punteros. Nadie mas debe hacerlo.
//   - Los adoptantes y las solicitudes se guardan por valor dentro de
//     Coleccion<T>, que administra su propia memoria (ver Coleccion.h).
// ===================================================================

class Refugio {
private:
    std::string nombre;

    Animal** catalogo;        // arreglo dinamico de punteros a Animal
    int cantAnimales;         // animales cargados
    int capacidad;            // lugares reservados en el arreglo

    Coleccion<Adoptante> adoptantes;             // instanciacion 1 del template
    Coleccion<SolicitudAdopcion> solicitudes;    // instanciacion 2 del template

    // --- Numeradores automaticos ---
    // Guardan cual es el proximo numero libre de cada cosa. Existen para
    // que el usuario NUNCA tenga que inventar ni recordar un ID: los IDs
    // siguen estando (los necesitan el operator(), el operator== y las
    // solicitudes), pero los pone el sistema.
    int proximoIdSolicitud;   // numerador automatico de solicitudes
    int proximoIdAnimal;      // numerador automatico de animales
    int proximoIdAdoptante;   // numerador automatico de adoptantes

    // --- Auxiliares privados ---
    void redimensionarCatalogo(int nuevaCapacidad);
    int buscarIndiceAnimal(int id) const;        // devuelve -1 si no existe
    int buscarIndiceAdoptante(int id) const;     // devuelve -1 si no existe
    int buscarIndiceSolicitud(int id) const;     // devuelve -1 si no existe
    void liberarCatalogo();                      // libera animales + arreglo

public:
    // --- Constructores y destructor (Regla de los Tres) ---
    explicit Refugio(const std::string& nombre = "Refugio Huellitas");
    Refugio(const Refugio& otro);                // COPIA PROFUNDA (clona cada animal)
    Refugio& operator=(const Refugio& otro);     // asignacion con copia profunda
    ~Refugio();

    std::string getNombre() const;                    // lo muestra el encabezado del menu
    void setNombre(const std::string& nuevoNombre);   // lo usa la prueba de copia profunda

    // --- Altas ---
    // agregarAnimal recibe un puntero ya creado con new y se queda con la
    // propiedad de esa memoria. Si el ID esta repetido lanza excepcion y
    // libera el objeto para no perder memoria.
    void agregarAnimal(Animal* nuevo);
    void registrarAdoptante(const Adoptante& nuevo);

    // Le dan al menu el proximo numero libre para que arme el objeto sin
    // preguntarselo a nadie. Son const: solo informan, el contador lo
    // avanzan agregarAnimal() y registrarAdoptante() cuando el alta sale bien.
    int nuevoIdAnimal() const;
    int nuevoIdAdoptante() const;

    // --- Consultas ---
    // Los contadores alimentan el resumen que el menu muestra en cada vuelta.
    int getCantidadAnimales() const;
    int getCantidadAdoptantes() const;
    int getCantidadSolicitudes() const;
    int contarDisponibles() const;             // se recalcula, no se guarda

    // Devuelven REFERENCIA CONSTANTE: el menu las puede leer y listar,
    // pero no modificarlas por fuera de las reglas del Refugio.
    const Coleccion<Adoptante>& getAdoptantes() const;
    const Coleccion<SolicitudAdopcion>& getSolicitudes() const;

    // --- Informes por pantalla ---
    void listarAnimales(bool soloDisponibles) const;   // recorrido polimorfico
    void listarConAritmeticaDePunteros() const;        // Fase 2: recorrido con punteros
    void mostrarHistorialAdopciones() const;           // solo las solicitudes CONFIRMADAS
    void mostrarEstadisticas() const;                  // totales + miembros estaticos

    // --- Fase 3: sobrecarga de operadores ---
    Animal& operator[](int indice) const;   // acceso POR POSICION (lanza IndiceInvalido)
    Animal* operator()(int id) const;       // busqueda POR ID (lanza NoEncontrado)

    // --- Proceso de adopcion ---
    // Las cuatro validan y lanzan excepcion si la operacion no corresponde;
    // ninguna imprime nada: los mensajes son cosa de la interfaz.
    //   PENDIENTE --confirmar--> CONFIRMADA  (el animal queda ADOPTADO)
    //   PENDIENTE --cancelar---> CANCELADA   (el animal vuelve DISPONIBLE)
    int crearSolicitud(int idAdoptante, int idAnimal);  // devuelve el N de solicitud
    void confirmarSolicitud(int idSolicitud);           // cierra la adopcion
    void cancelarSolicitud(int idSolicitud);            // libera al animal reservado
    void devolverAnimal(int idAnimal);                  // el animal regresa al refugio

    // --- Datos de prueba ---
    // La llama el constructor de MenuConsola para que el sistema arranque
    // con animales y adoptantes cargados.
    void cargarDatosDePrueba();
};

#endif //REFUGIO_H
