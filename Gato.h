#ifndef GATO_H
#define GATO_H

#include "Animal.h"

// ===================================================================
// FASE 4 - Gato hereda publicamente de Animal.
// Agrega color de pelaje y si vive en interior o exterior.
// ===================================================================

class Gato : public Animal {
private:
    std::string colorPelaje;
    bool esDeInterior;     // true = vive dentro de la casa, false = exterior

public:
    Gato();
    Gato(int id, const std::string& nombre, int edad, const std::string& estadoSalud,
         const std::string& colorPelaje, bool esDeInterior);
    Gato(const Gato& otro);
    ~Gato() override;

    std::string getColorPelaje() const;
    bool getEsDeInterior() const;

    // --- Redefiniciones polimorficas ---
    void mostrarInfo() const override;
    std::string tipo() const override;
    Animal* clonar() const override;
};

#endif //GATO_H
