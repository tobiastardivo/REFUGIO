#include "Animal.h"
#include <iostream>

using namespace std;

int Animal::animalesVivos = 0;

string estadoATexto(EstadoAdopcion estado) {
    switch (estado) {
        case EstadoAdopcion::DISPONIBLE: return "DISPONIBLE";
        case EstadoAdopcion::EN_PROCESO: return "EN_PROCESO";
        case EstadoAdopcion::ADOPTADO:   return "ADOPTADO";
        default:                         return "DESCONOCIDO";
    }
}

Animal::Animal() : id(0), nombre("Sin nombre"), edad(0),
                   estadoSalud("Sin datos"), estado(EstadoAdopcion::DISPONIBLE) {
    animalesVivos++;
}

Animal::Animal(int id, const string& nombre, int edad, const string& estadoSalud)
    : id(id), nombre(nombre), edad(edad),
      estadoSalud(estadoSalud), estado(EstadoAdopcion::DISPONIBLE) {
    animalesVivos++;
}

Animal::Animal(const Animal& otro)
    : id(otro.id), nombre(otro.nombre), edad(otro.edad),
      estadoSalud(otro.estadoSalud), estado(otro.estado) {
    animalesVivos++;
}

Animal::~Animal() {
    animalesVivos--;
}

int Animal::getId() const                    { return id; }
string Animal::getNombre() const             { return nombre; }
int Animal::getEdad() const                  { return edad; }
string Animal::getEstadoSalud() const        { return estadoSalud; }
EstadoAdopcion Animal::getEstado() const     { return estado; }

void Animal::setEstado(EstadoAdopcion n)     { estado = n; }
void Animal::setEstadoSalud(const string& n) { estadoSalud = n; }

bool Animal::estaDisponible() const          { return estado == EstadoAdopcion::DISPONIBLE; }

void Animal::mostrarDatosBase() const {
    cout << "ID: " << id
         << " | Nombre: " << nombre
         << " | Edad: " << edad << " anio(s)"
         << " | Salud: " << estadoSalud
         << " | Estado: " << estadoATexto(estado);
}

bool Animal::operator==(const Animal& otro) const { return id == otro.id; }

bool Animal::operator!=(const Animal& otro) const { return !(*this == otro); }

bool Animal::operator!() const { return estado != EstadoAdopcion::DISPONIBLE; }
