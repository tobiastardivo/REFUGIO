#ifndef PERRO_H
#define PERRO_H

#include "Animal.h"

class Perro : public Animal {
private:
    std::string raza;
    std::string tamanio;

public:
    Perro();
    Perro(int id, const std::string& nombre, int edad, const std::string& estadoSalud,
          const std::string& raza, const std::string& tamanio);
    Perro(const Perro& otro);
    ~Perro() override;

    std::string getRaza() const;
    std::string getTamanio() const;

    void mostrarInfo() const override;
    std::string tipo() const override;
    Animal* clonar() const override;
};

#endif
