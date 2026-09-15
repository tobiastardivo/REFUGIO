#include "Adoptante.h"
#include <iostream>

using namespace std;

Adoptante::Adoptante()
    : id(0), nombre("Sin nombre"), contacto("Sin datos"), tipoVivienda("Sin datos") {}

Adoptante::Adoptante(int id, const string& nombre,
                     const string& contacto, const string& tipoVivienda)
    : id(id), nombre(nombre), contacto(contacto), tipoVivienda(tipoVivienda) {}

// Constructor de copia
Adoptante::Adoptante(const Adoptante& otro)
    : id(otro.id), nombre(otro.nombre),
      contacto(otro.contacto), tipoVivienda(otro.tipoVivienda) {}

// Operador de asignacion: protegido contra la autoasignacion (a = a)
Adoptante& Adoptante::operator=(const Adoptante& otro) {
    if (this != &otro) {
        id = otro.id;
        nombre = otro.nombre;
        contacto = otro.contacto;
        tipoVivienda = otro.tipoVivienda;
    }
    return *this;
}

Adoptante::~Adoptante() {}

int Adoptante::getId() const               { return id; }
string Adoptante::getNombre() const        { return nombre; }
string Adoptante::getContacto() const      { return contacto; }
string Adoptante::getTipoVivienda() const  { return tipoVivienda; }

void Adoptante::mostrarInfo() const {
    cout << "ID: " << id
         << " | Nombre: " << nombre
         << " | Contacto: " << contacto
         << " | Vivienda: " << tipoVivienda << endl;
}

bool Adoptante::operator==(const Adoptante& otro) const { return id == otro.id; }
