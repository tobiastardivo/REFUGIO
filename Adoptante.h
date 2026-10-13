#ifndef ADOPTANTE_H
#define ADOPTANTE_H

#include <string>

class Adoptante {
private:
    int id;
    std::string nombre;
    std::string contacto;
    std::string tipoVivienda;

public:
    Adoptante();
    Adoptante(int id, const std::string& nombre,
              const std::string& contacto, const std::string& tipoVivienda);
    Adoptante(const Adoptante& otro);
    Adoptante& operator=(const Adoptante& otro);
    ~Adoptante();

    int getId() const;
    std::string getNombre() const;
    std::string getContacto() const;
    std::string getTipoVivienda() const;

    void mostrarInfo() const;
    bool operator==(const Adoptante& otro) const;
};

#endif
