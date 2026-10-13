#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>

enum class EstadoAdopcion { DISPONIBLE, EN_PROCESO, ADOPTADO };

std::string estadoATexto(EstadoAdopcion estado);

class Animal {
protected:
    int id;
    std::string nombre;
    int edad;
    std::string estadoSalud;
    EstadoAdopcion estado;

    void mostrarDatosBase() const;

public:
    static int animalesVivos;

    Animal();
    Animal(int id, const std::string& nombre, int edad,
           const std::string& estadoSalud);
    Animal(const Animal& otro);
    virtual ~Animal();

    int getId() const;
    std::string getNombre() const;
    int getEdad() const;
    std::string getEstadoSalud() const;
    EstadoAdopcion getEstado() const;
    void setEstado(EstadoAdopcion nuevoEstado);
    void setEstadoSalud(const std::string& nuevoEstadoSalud);
    bool estaDisponible() const;

    virtual void mostrarInfo() const = 0;
    virtual std::string tipo() const = 0;
    virtual Animal* clonar() const = 0;

    bool operator==(const Animal& otro) const;
    bool operator!=(const Animal& otro) const;
    bool operator!() const;
};

#endif
