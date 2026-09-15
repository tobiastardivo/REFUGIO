#include "Gato.h"
#include <iostream>

using namespace std;

Gato::Gato() : Animal(), colorPelaje("Sin datos"), esDeInterior(true) {}

Gato::Gato(int id, const string& nombre, int edad, const string& estadoSalud,
           const string& colorPelaje, bool esDeInterior)
    : Animal(id, nombre, edad, estadoSalud),
      colorPelaje(colorPelaje), esDeInterior(esDeInterior) {}

// Constructor de copia: delega la parte comun en el de Animal
Gato::Gato(const Gato& otro)
    : Animal(otro), colorPelaje(otro.colorPelaje), esDeInterior(otro.esDeInterior) {}

Gato::~Gato() {}

string Gato::getColorPelaje() const { return colorPelaje; }
bool Gato::getEsDeInterior() const  { return esDeInterior; }

void Gato::mostrarInfo() const {
    cout << "[GATO]  ";
    mostrarDatosBase();
    cout << " | Pelaje: " << colorPelaje
         << " | Vive: " << (esDeInterior ? "Interior" : "Exterior") << endl;
}

string Gato::tipo() const { return "Gato"; }

Animal* Gato::clonar() const {
    Gato* copia = new Gato(*this);
    copia->setEstado(this->estado);
    return copia;
}
