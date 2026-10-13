#ifndef MENUCONSOLA_H
#define MENUCONSOLA_H

#include <string>
#include "Refugio.h"

class MenuConsola {
private:
    Refugio refugio;

    enum class Filtro { TODOS, DISPONIBLES, NO_DISPONIBLES };

    void mostrarMenu() const;

    int elegirAnimal(const std::string& titulo, Filtro filtro) const;
    int elegirAdoptante(const std::string& titulo) const;
    int elegirSolicitudPendiente(const std::string& titulo) const;

    static bool pasaElFiltro(const Animal& animal, Filtro filtro);
    std::string nombreDeAdoptante(int idAdoptante) const;
    void mostrarSolicitud(const SolicitudAdopcion& solicitud, int numeroDeLista) const;

    void ejecutarOpcion(int opcion);

    void registrarPerro();
    void registrarGato();
    void registrarAdoptante();
    void verFichaDeAnimal() const;
    void crearSolicitud();
    void confirmarSolicitud();
    void cancelarSolicitud();
    void devolverAnimal();
    void verAdoptantesYSolicitudes() const;
    void pruebasDeIntegridad();

public:
    explicit MenuConsola(const std::string& nombreRefugio = "Refugio Huellitas");
    ~MenuConsola();

    void ejecutar();

    static void informarMemoriaLiberada();
};

#endif
