#ifndef SOLICITUDADOPCION_H
#define SOLICITUDADOPCION_H

#include <string>

// ===================================================================
// FASE 2 / FASE 4 (composicion)
// SolicitudAdopcion relaciona un adoptante con un animal y registra
// en que estado esta el tramite. Guarda los IDs (no los objetos) para
// no duplicar informacion ni quedarse con punteros colgados.
// ===================================================================

enum class EstadoSolicitud { PENDIENTE, CONFIRMADA, CANCELADA };

std::string estadoSolicitudATexto(EstadoSolicitud estado);

class SolicitudAdopcion {
private:
    int id;             // numero de solicitud
    int idAdoptante;
    int idAnimal;
    EstadoSolicitud estado;

public:
    SolicitudAdopcion();
    SolicitudAdopcion(int id, int idAdoptante, int idAnimal);
    SolicitudAdopcion(const SolicitudAdopcion& otra);
    SolicitudAdopcion& operator=(const SolicitudAdopcion& otra);
    ~SolicitudAdopcion();

    int getId() const;
    int getIdAdoptante() const;
    int getIdAnimal() const;
    EstadoSolicitud getEstado() const;

    void confirmar();     // PENDIENTE -> CONFIRMADA
    void cancelar();      // PENDIENTE -> CANCELADA
    bool estaPendiente() const;

    void mostrarInfo() const;                             // usada por la funcion generica
    bool operator==(const SolicitudAdopcion& otra) const; // compara por numero de solicitud
};

#endif //SOLICITUDADOPCION_H
