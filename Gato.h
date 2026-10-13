#ifndef GATO_H
#define GATO_H

#include "Animal.h"

class Gato : public Animal {
private:
    std::string colorPelaje;
    bool esDeInterior;

public:
    Gato();
    Gato(int id, const std::string& nombre, int edad, const std::string& estadoSalud,
         const std::string& colorPelaje, bool esDeInterior);
    Gato(const Gato& otro);
    ~Gato() override;

    std::string getColorPelaje() const;
    bool getEsDeInterior() const;

    void mostrarInfo() const override;
    std::string tipo() const override;
    Animal* clonar() const override;
};

#endif
