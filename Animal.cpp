#include "Animal.h"
#include <iostream>

using namespace std;

// Inicializacion del miembro estatico (se hace una sola vez, fuera de la clase)
int Animal::animalesVivos = 0;

// Convierte el enum a texto para poder mostrarlo por pantalla
string estadoATexto(EstadoAdopcion estado) {
    switch (estado) {
        case EstadoAdopcion::DISPONIBLE: return "DISPONIBLE";
        case EstadoAdopcion::EN_PROCESO: return "EN_PROCESO";
        case EstadoAdopcion::ADOPTADO:   return "ADOPTADO";
        default:                         return "DESCONOCIDO";
    }
}

// --- Constructor por defecto ---
Animal::Animal() : id(0), nombre("Sin nombre"), edad(0),
                   estadoSalud("Sin datos"), estado(EstadoAdopcion::DISPONIBLE) {
    animalesVivos++;
}

// --- Constructor parametrizado (usa lista de inicializacion) ---
Animal::Animal(int id, const string& nombre, int edad, const string& estadoSalud)
    : id(id), nombre(nombre), edad(edad),
      estadoSalud(estadoSalud), estado(EstadoAdopcion::DISPONIBLE) {
    animalesVivos++;
}

// --- Constructor de copia ---
// Animal no administra memoria dinamica propia, pero se define igual porque
// Perro y Gato lo invocan cuando se copian a si mismos.
Animal::Animal(const Animal& otro)
    : id(otro.id), nombre(otro.nombre), edad(otro.edad),
      estadoSalud(otro.estadoSalud), estado(otro.estado) {
    animalesVivos++;
}

// --- Destructor virtual ---
// Es virtual para que al hacer  delete  sobre un Animal* que en realidad
// apunta a un Perro se ejecute primero ~Perro() y despues ~Animal().
// Sin virtual habria comportamiento indefinido y perdida de memoria.
Animal::~Animal() {
    animalesVivos--;
}

// --- Getters / setters ---
int Animal::getId() const                    { return id; }
string Animal::getNombre() const             { return nombre; }
int Animal::getEdad() const                  { return edad; }
string Animal::getEstadoSalud() const        { return estadoSalud; }
EstadoAdopcion Animal::getEstado() const     { return estado; }
void Animal::setEstado(EstadoAdopcion n)     { estado = n; }
void Animal::setEstadoSalud(const string& n) { estadoSalud = n; }
bool Animal::estaDisponible() const          { return estado == EstadoAdopcion::DISPONIBLE; }

// Impresion de los datos comunes a todos los animales
void Animal::mostrarDatosBase() const {
    cout << "ID: " << id
         << " | Nombre: " << nombre
         << " | Edad: " << edad << " anio(s)"
         << " | Salud: " << estadoSalud
         << " | Estado: " << estadoATexto(estado);
}

// --- Fase 3: sobrecarga de operadores ---

// Dos animales son "iguales" si comparten el ID (el ID es la clave del sistema)
bool Animal::operator==(const Animal& otro) const { return id == otro.id; }
bool Animal::operator!=(const Animal& otro) const { return !(*this == otro); }

// Operador unario !: se lee como "este animal NO esta disponible".
// Uso:  if (!(*animal)) cout << "No se puede iniciar la adopcion";
bool Animal::operator!() const { return estado != EstadoAdopcion::DISPONIBLE; }
