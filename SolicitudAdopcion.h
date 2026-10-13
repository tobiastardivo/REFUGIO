#ifndef SOLICITUDADOPCION_H
#define SOLICITUDADOPCION_H

#include <string>

enum class EstadoSolicitud { PENDIENTE, CONFIRMADA, CANCELADA };

std::string estadoSolicitudATexto(EstadoSolicitud estado);

class SolicitudAdopcion {
private:
    int id;
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

    void confirmar();
    void cancelar();
    bool estaPendiente() const;

    void mostrarInfo() const;
    bool operator==(const SolicitudAdopcion& otra) const;
};

#endif
