// ===================================================================
// Animal.cpp - Implementacion de la clase abstracta Animal.
//
// Animal es el TAD base de la jerarquia: concentra todo lo que Perro y
// Gato tienen en comun (id, nombre, edad, salud y estado de adopcion) y
// deja sin implementar las tres funciones que cada tipo resuelve a su
// manera. Aca tambien vive el contador estatico animalesVivos, que es la
// herramienta con la que el sistema demuestra que no pierde memoria.
// ===================================================================

#include "Animal.h"
#include <iostream>

using namespace std;

// Definicion del miembro estatico. Va una sola vez y FUERA de la clase,
// porque la variable no pertenece a ningun objeto sino a la clase entera:
// existe aunque no haya un solo animal creado. Arranca en 0, sube en cada
// constructor y baja en el destructor.
int Animal::animalesVivos = 0;

// --- estadoATexto(): funcion libre auxiliar ---
// Traduce el enum EstadoAdopcion a texto imprimible. Se hizo funcion libre
// (y no metodo) porque no necesita ningun dato del objeto: recibe el enum
// y devuelve su nombre. El caso default nunca deberia darse, pero se deja
// por seguridad y para que el compilador no avise de un switch incompleto.
string estadoATexto(EstadoAdopcion estado) {
    switch (estado) {
        case EstadoAdopcion::DISPONIBLE: return "DISPONIBLE";
        case EstadoAdopcion::EN_PROCESO: return "EN_PROCESO";
        case EstadoAdopcion::ADOPTADO:   return "ADOPTADO";
        default:                         return "DESCONOCIDO";
    }
}

// --- Constructor por defecto ---
// Deja el animal con datos neutros y en estado DISPONIBLE. Igual que los
// otros dos constructores, suma uno a animalesVivos: cada objeto que nace
// queda contado.
Animal::Animal() : id(0), nombre("Sin nombre"), edad(0),
                   estadoSalud("Sin datos"), estado(EstadoAdopcion::DISPONIBLE) {
    animalesVivos++;
}

// --- Constructor parametrizado ---
// Usa lista de inicializacion (los ":" antes de la llave) en vez de asignar
// dentro del cuerpo: asi cada atributo se construye una sola vez, ya con su
// valor final. Todo animal nuevo entra al refugio como DISPONIBLE.
Animal::Animal(int id, const string& nombre, int edad, const string& estadoSalud)
    : id(id), nombre(nombre), edad(edad),
      estadoSalud(estadoSalud), estado(EstadoAdopcion::DISPONIBLE) {
    animalesVivos++;
}

// --- Constructor de copia ---
// Animal no administra memoria dinamica propia (sus strings se copian
// solas), pero se define igual por dos motivos: para que Perro y Gato lo
// invoquen al copiarse, y para que la copia tambien quede registrada en
// el contador animalesVivos.
Animal::Animal(const Animal& otro)
    : id(otro.id), nombre(otro.nombre), edad(otro.edad),
      estadoSalud(otro.estadoSalud), estado(otro.estado) {
    animalesVivos++;
}

// --- Destructor virtual ---
// Descuenta el animal del contador. Es virtual para que al hacer  delete
// sobre un Animal* que en realidad apunta a un Perro se ejecute primero
// ~Perro() y despues ~Animal(). Sin virtual habria comportamiento
// indefinido y perdida de memoria: es el error clasico de la herencia.
Animal::~Animal() {
    animalesVivos--;
}

// --- Getters ---
// Dan acceso de solo lectura a los atributos, que son protected. Son const
// porque no modifican el objeto, y devuelven copia para que nadie pueda
// alterar el atributo original desde afuera (encapsulamiento).
int Animal::getId() const                    { return id; }
string Animal::getNombre() const             { return nombre; }
int Animal::getEdad() const                  { return edad; }
string Animal::getEstadoSalud() const        { return estadoSalud; }
EstadoAdopcion Animal::getEstado() const     { return estado; }

// --- Setters ---
// Solo se exponen los dos atributos que cambian durante la vida del animal:
// el estado de adopcion (lo mueve el circuito de solicitudes) y el estado
// de salud. El id, el nombre y la edad no tienen setter a proposito: son
// datos de alta y no deberian poder modificarse despues.
void Animal::setEstado(EstadoAdopcion n)     { estado = n; }
void Animal::setEstadoSalud(const string& n) { estadoSalud = n; }

// Atajo de lectura que se usa en todo el sistema para filtrar y validar.
bool Animal::estaDisponible() const          { return estado == EstadoAdopcion::DISPONIBLE; }

// --- mostrarDatosBase() ---
// Imprime los campos comunes SIN salto de linea final, a proposito: es un
// auxiliar protected que Perro::mostrarInfo() y Gato::mostrarInfo() llaman
// en el medio de su propia salida para despues agregar lo suyo y recien
// ahi cerrar el renglon. Evita repetir el mismo cout en las dos clases.
void Animal::mostrarDatosBase() const {
    cout << "ID: " << id
         << " | Nombre: " << nombre
         << " | Edad: " << edad << " anio(s)"
         << " | Salud: " << estadoSalud
         << " | Estado: " << estadoATexto(estado);
}

// ===================================================================
// FASE 3 - SOBRECARGA DE OPERADORES
// ===================================================================

// operator== : dos animales son "el mismo" si comparten el ID, porque el
// ID es la clave del sistema (puede haber dos perros llamados Rocky, pero
// no dos con el mismo numero).
bool Animal::operator==(const Animal& otro) const { return id == otro.id; }

// operator!= : se define en terminos de == para que las dos comparaciones
// no puedan quedar nunca desincronizadas.
bool Animal::operator!=(const Animal& otro) const { return !(*this == otro); }

// operator! (unario): se lee como "este animal NO esta disponible".
// Devuelve true cuando el animal esta EN_PROCESO o ADOPTADO, que son
// justamente los casos en los que hay que frenar una adopcion.
// Uso:  if (!(*animal)) cout << "No se puede iniciar la adopcion";
bool Animal::operator!() const { return estado != EstadoAdopcion::DISPONIBLE; }
