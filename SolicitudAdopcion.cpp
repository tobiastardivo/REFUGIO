#include "SolicitudAdopcion.h"
#include <iostream>

using namespace std;

string estadoSolicitudATexto(EstadoSolicitud estado) {
    switch (estado) {
        case EstadoSolicitud::PENDIENTE: return "PENDIENTE";
        case EstadoSolicitud::CONFIRMADA: return "CONFIRMADA";
        case EstadoSolicitud::CANCELADA: return "CANCELADA";
        default: return "DESCONOCIDO";
    }
}

SolicitudAdopcion::SolicitudAdopcion()
    : id(0), idAdoptante(0), idAnimal(0), estado(EstadoSolicitud::PENDIENTE) {}

SolicitudAdopcion::SolicitudAdopcion(int id, int idAdoptante, int idAnimal)
    : id(id), idAdoptante(idAdoptante), idAnimal(idAnimal),
      estado(EstadoSolicitud::PENDIENTE) {}

SolicitudAdopcion::SolicitudAdopcion(const SolicitudAdopcion& otra)
    : id(otra.id), idAdoptante(otra.idAdoptante),
      idAnimal(otra.idAnimal), estado(otra.estado) {}

SolicitudAdopcion& SolicitudAdopcion::operator=(const SolicitudAdopcion& otra) {
    if (this != &otra) {
        id = otra.id;
        idAdoptante = otra.idAdoptante;
        idAnimal = otra.idAnimal;
        estado = otra.estado;
    }
    return *this;
}

SolicitudAdopcion::~SolicitudAdopcion() {}

int SolicitudAdopcion::getId() const             { return id; }
int SolicitudAdopcion::getIdAdoptante() const    { return idAdoptante; }
int SolicitudAdopcion::getIdAnimal() const       { return idAnimal; }
EstadoSolicitud SolicitudAdopcion::getEstado() const { return estado; }

void SolicitudAdopcion::confirmar() { estado = EstadoSolicitud::CONFIRMADA; }
void SolicitudAdopcion::cancelar()  { estado = EstadoSolicitud::CANCELADA; }

bool SolicitudAdopcion::estaPendiente() const { return estado == EstadoSolicitud::PENDIENTE; }

void SolicitudAdopcion::mostrarInfo() const {
    cout << "Solicitud N " << id
         << " | Adoptante ID: " << idAdoptante
         << " | Animal ID: " << idAnimal
         << " | Estado: " << estadoSolicitudATexto(estado) << endl;
}

bool SolicitudAdopcion::operator==(const SolicitudAdopcion& otra) const {
    return id == otra.id;
}
