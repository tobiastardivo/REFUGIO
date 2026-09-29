// ===================================================================
// SolicitudAdopcion.cpp - Implementacion del TAD SolicitudAdopcion.
//
// Una solicitud es el nexo entre un adoptante y un animal: registra
// quien quiere adoptar a quien y en que punto del tramite esta.
// Guarda los IDs y no los objetos, para no duplicar informacion ni
// quedarse con punteros que apunten a animales ya liberados.
//
// El ciclo de vida del estado es siempre el mismo:
//     PENDIENTE --confirmar()--> CONFIRMADA   (final)
//     PENDIENTE --cancelar()---> CANCELADA    (final)
// ===================================================================

#include "SolicitudAdopcion.h"
#include <iostream>

using namespace std;

// --- estadoSolicitudATexto(): funcion libre auxiliar ---
// Traduce el enum EstadoSolicitud a texto imprimible. Es funcion libre
// porque no necesita ningun dato del objeto: recibe el enum y devuelve su
// nombre. El default no deberia darse nunca, pero se deja por seguridad.
string estadoSolicitudATexto(EstadoSolicitud estado) {
    switch (estado) {
        case EstadoSolicitud::PENDIENTE: return "PENDIENTE";
        case EstadoSolicitud::CONFIRMADA: return "CONFIRMADA";
        case EstadoSolicitud::CANCELADA: return "CANCELADA";
        default: return "DESCONOCIDO";
    }
}

// --- Constructor por defecto ---
// Necesario porque Coleccion<SolicitudAdopcion> reserva su bloque con
// new T[capacidad] , y esa instruccion construye cada elemento con el
// constructor sin parametros. Deja una solicitud "vacia" (todos los IDs
// en 0) que despues se pisa con una real.
SolicitudAdopcion::SolicitudAdopcion()
    : id(0), idAdoptante(0), idAnimal(0), estado(EstadoSolicitud::PENDIENTE) {}

// --- Constructor parametrizado ---
// El estado no se recibe por parametro a proposito: toda solicitud nueva
// nace PENDIENTE, y la unica forma de moverla es con confirmar() o
// cancelar(). Asi no se puede crear una solicitud ya confirmada por error.
SolicitudAdopcion::SolicitudAdopcion(int id, int idAdoptante, int idAnimal)
    : id(id), idAdoptante(idAdoptante), idAnimal(idAnimal),
      estado(EstadoSolicitud::PENDIENTE) {}

// --- Constructor de copia ---
// Copia los cuatro campos, estado incluido. Como son todos tipos simples,
// la copia queda naturalmente independiente del original.
SolicitudAdopcion::SolicitudAdopcion(const SolicitudAdopcion& otra)
    : id(otra.id), idAdoptante(otra.idAdoptante),
      idAnimal(otra.idAnimal), estado(otra.estado) {}

// --- Operador de asignacion ---
// Lo usa Coleccion<T> al copiar elementos de un bloque a otro cuando se
// redimensiona. El  if (this != &otra)  evita el problema de la
// autoasignacion (a = a) y devolver *this permite encadenar a = b = c.
SolicitudAdopcion& SolicitudAdopcion::operator=(const SolicitudAdopcion& otra) {
    if (this != &otra) {
        id = otra.id;
        idAdoptante = otra.idAdoptante;
        idAnimal = otra.idAnimal;
        estado = otra.estado;
    }
    return *this;
}

// --- Destructor ---
// Vacio: la clase solo tiene enteros y un enum, no hay memoria que liberar.
// Se declara para dejar completa la Regla de los Tres.
SolicitudAdopcion::~SolicitudAdopcion() {}

// --- Getters ---
// Lectura de los atributos privados. Los dos IDs son la forma de llegar
// al adoptante y al animal: el Refugio los busca en sus colecciones.
int SolicitudAdopcion::getId() const             { return id; }
int SolicitudAdopcion::getIdAdoptante() const    { return idAdoptante; }
int SolicitudAdopcion::getIdAnimal() const       { return idAnimal; }
EstadoSolicitud SolicitudAdopcion::getEstado() const { return estado; }

// --- confirmar() / cancelar() ---
// Las dos unicas transiciones de estado posibles. No validan nada: quien
// se encarga de comprobar que la solicitud todavia este PENDIENTE (y de
// mover el estado del animal en consecuencia) es el Refugio, que es el
// que conoce las reglas del negocio. La solicitud solo guarda su dato.
void SolicitudAdopcion::confirmar() { estado = EstadoSolicitud::CONFIRMADA; }
void SolicitudAdopcion::cancelar()  { estado = EstadoSolicitud::CANCELADA; }

// Atajo de lectura: true solo mientras el tramite siga abierto.
bool SolicitudAdopcion::estaPendiente() const { return estado == EstadoSolicitud::PENDIENTE; }

// --- mostrarInfo() ---
// Imprime la solicitud en una linea. Tiene el mismo nombre que el metodo
// de Adoptante justamente para que la funcion generica mostrarColeccion<T>()
// pueda listar las dos colecciones con un unico codigo.
void SolicitudAdopcion::mostrarInfo() const {
    cout << "Solicitud N " << id
         << " | Adoptante ID: " << idAdoptante
         << " | Animal ID: " << idAnimal
         << " | Estado: " << estadoSolicitudATexto(estado) << endl;
}

// --- operator== ---
// Compara por numero de solicitud, que es la clave: dos tramites distintos
// pueden involucrar al mismo adoptante y al mismo animal, pero nunca
// comparten numero.
bool SolicitudAdopcion::operator==(const SolicitudAdopcion& otra) const {
    return id == otra.id;
}
