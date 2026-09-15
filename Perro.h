#ifndef PERRO_H
#define PERRO_H

#include "Animal.h"

// ===================================================================
// FASE 4 - Perro hereda publicamente de Animal.
// Agrega los atributos propios (raza y tamanio) y redefine (override)
// las funciones virtuales puras de la clase base.
// ===================================================================

class Perro : public Animal {
private:
    std::string raza;
    std::string tamanio;   // "Pequenio", "Mediano", "Grande"

public:
    // Constructores: delegan en los de Animal la parte comun
    Perro();
    Perro(int id, const std::string& nombre, int edad, const std::string& estadoSalud,
          const std::string& raza, const std::string& tamanio);
    Perro(const Perro& otro);      // de copia: llama al de Animal y copia lo propio
    ~Perro() override;             // se ejecuta antes de ~Animal()

    std::string getRaza() const;
    std::string getTamanio() const;

    // --- Redefiniciones polimorficas ---
    void mostrarInfo() const override;   // ligadura dinamica: se resuelve en ejecucion
    std::string tipo() const override;
    Animal* clonar() const override;     // devuelve un Perro nuevo en memoria dinamica
};

#endif //PERRO_H
