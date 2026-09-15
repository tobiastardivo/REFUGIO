#include "Perro.h"
#include <iostream>

using namespace std;

// Constructor por defecto: primero corre Animal(), despues lo propio de Perro
Perro::Perro() : Animal(), raza("Mestizo"), tamanio("Mediano") {}

// Constructor parametrizado: pasa los datos comunes al constructor de Animal
Perro::Perro(int id, const string& nombre, int edad, const string& estadoSalud,
             const string& raza, const string& tamanio)
    : Animal(id, nombre, edad, estadoSalud), raza(raza), tamanio(tamanio) {}

// Constructor de copia: invoca explicitamente el constructor de copia de Animal.
// Si no se lo invocara, se usaria Animal() por defecto y se perderian los datos base.
Perro::Perro(const Perro& otro)
    : Animal(otro), raza(otro.raza), tamanio(otro.tamanio) {}

// Destructor: no libera memoria porque Perro no usa punteros propios,
// pero se declara para dejar explicito el orden ~Perro() -> ~Animal().
Perro::~Perro() {}

string Perro::getRaza() const    { return raza; }
string Perro::getTamanio() const { return tamanio; }

// Muestra los datos comunes (heredados) + los especificos de Perro
void Perro::mostrarInfo() const {
    cout << "[PERRO] ";
    mostrarDatosBase();                      // metodo protegido de Animal
    cout << " | Raza: " << raza
         << " | Tamanio: " << tamanio << endl;
}

string Perro::tipo() const { return "Perro"; }

// Fase 3/4: clonar() permite copiar el catalogo sin saber el tipo concreto.
// Cada objeto sabe crear una copia exacta de si mismo -> COPIA PROFUNDA.
Animal* Perro::clonar() const {
    Perro* copia = new Perro(*this);         // usa el constructor de copia de Perro
    copia->setEstado(this->estado);          // conserva tambien el estado de adopcion
    return copia;
}
