#ifndef REFUGIO_H
#define REFUGIO_H

#include <string>
#include "Animal.h"
#include "Adoptante.h"
#include "SolicitudAdopcion.h"
#include "Coleccion.h"

// ===================================================================
// MEMORIA DINAMICA (Fase 2): quien reserva y quien libera.
//   - catalogo es un arreglo dinamico de PUNTEROS a Animal (Animal**),
//     para poder guardar Perros y Gatos mezclados y recorrerlos
//     polimorficamente.
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

    Animal** catalogo;
    int cantAnimales;
    int capacidad;

    Coleccion<Adoptante> adoptantes;
    Coleccion<SolicitudAdopcion> solicitudes;

    int proximoIdSolicitud;
    int proximoIdAnimal;
    int proximoIdAdoptante;

    void redimensionarCatalogo(int nuevaCapacidad);
    int buscarIndiceAnimal(int id) const;
    int buscarIndiceAdoptante(int id) const;
    int buscarIndiceSolicitud(int id) const;
    void liberarCatalogo();

public:
    explicit Refugio(const std::string& nombre = "Refugio Huellitas");
    Refugio(const Refugio& otro);
    Refugio& operator=(const Refugio& otro);
    ~Refugio();

    std::string getNombre() const;
    void setNombre(const std::string& nuevoNombre);

    void agregarAnimal(Animal* nuevo);
    void registrarAdoptante(const Adoptante& nuevo);

    int nuevoIdAnimal() const;
    int nuevoIdAdoptante() const;

    int getCantidadAnimales() const;
    int getCantidadAdoptantes() const;
    int getCantidadSolicitudes() const;
    int contarDisponibles() const;

    const Coleccion<Adoptante>& getAdoptantes() const;
    const Coleccion<SolicitudAdopcion>& getSolicitudes() const;

    void listarAnimales(bool soloDisponibles) const;
    void listarConAritmeticaDePunteros() const;
    void mostrarHistorialAdopciones() const;
    void mostrarEstadisticas() const;

    Animal& operator[](int indice) const;
    Animal* operator()(int id) const;

    int crearSolicitud(int idAdoptante, int idAnimal);
    void confirmarSolicitud(int idSolicitud);
    void cancelarSolicitud(int idSolicitud);
    void devolverAnimal(int idAnimal);

    void cargarDatosDePrueba();
};

#endif
