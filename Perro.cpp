#include "Perro.h"
#include <iostream>

using namespace std;

Perro::Perro() : Animal(), raza("Mestizo"), tamanio("Mediano") {}

Perro::Perro(int id, const string& nombre, int edad, const string& estadoSalud,
             const string& raza, const string& tamanio)
    : Animal(id, nombre, edad, estadoSalud), raza(raza), tamanio(tamanio) {}

Perro::Perro(const Perro& otro)
    : Animal(otro), raza(otro.raza), tamanio(otro.tamanio) {}

Perro::~Perro() {}

string Perro::getRaza() const    { return raza; }
string Perro::getTamanio() const { return tamanio; }

void Perro::mostrarInfo() const {
    cout << "[PERRO] ";
    mostrarDatosBase();
    cout << " | Raza: " << raza
         << " | Tamanio: " << tamanio << endl;
}

string Perro::tipo() const { return "Perro"; }

Animal* Perro::clonar() const {
    Perro* copia = new Perro(*this);
    copia->setEstado(this->estado);
    return copia;
}
