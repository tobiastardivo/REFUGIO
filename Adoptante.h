#ifndef ADOPTANTE_H
#define ADOPTANTE_H

#include <string>

// ===================================================================
// FASE 2 - Adoptante: TAD simple, con todos sus atributos privados.
// Se guarda POR VALOR dentro de una Coleccion<Adoptante> (Fase 5),
// por eso define constructor de copia y operador de asignacion.
// ===================================================================

class Adoptante {
private:
    int id;
    std::string nombre;
    std::string contacto;        // telefono o mail
    std::string tipoVivienda;    // "Casa con patio", "Departamento", etc.

public:
    Adoptante();                                                     // por defecto
    Adoptante(int id, const std::string& nombre,
              const std::string& contacto, const std::string& tipoVivienda);
    Adoptante(const Adoptante& otro);                                // de copia
    Adoptante& operator=(const Adoptante& otro);                     // asignacion
    ~Adoptante();

    int getId() const;
    std::string getNombre() const;
    std::string getContacto() const;
    std::string getTipoVivienda() const;

    void mostrarInfo() const;                    // la usa la funcion generica de Utilidades.h
    bool operator==(const Adoptante& otro) const; // compara por ID
};

#endif //ADOPTANTE_H
