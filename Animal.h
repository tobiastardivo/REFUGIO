#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>

// ===================================================================
// FASE 4 - Animal es una CLASE ABSTRACTA (el TAD base del sistema).
// No se pueden crear objetos Animal: solo Perro o Gato.
// Tiene destructor virtual y funciones virtuales puras (= 0).
// ===================================================================

// Estados posibles de adopcion de un animal
enum class EstadoAdopcion { DISPONIBLE, EN_PROCESO, ADOPTADO };

// Funcion libre auxiliar: convierte el enum en texto legible
std::string estadoATexto(EstadoAdopcion estado);

class Animal {
protected:                       // protected: lo ven Perro y Gato, no el exterior
    int id;                      // identificador unico del animal
    std::string nombre;
    int edad;                    // en anios
    std::string estadoSalud;     // ej: "Sano", "En tratamiento"
    EstadoAdopcion estado;       // DISPONIBLE / EN_PROCESO / ADOPTADO

    // Auxiliar para que Perro y Gato reutilicen la impresion de los datos comunes
    void mostrarDatosBase() const;

public:
    static int animalesVivos;    // miembro estatico: cuantos animales hay en memoria

    // --- Constructores y destructor (Fase 2) ---
    Animal();                                                    // por defecto
    Animal(int id, const std::string& nombre, int edad,
           const std::string& estadoSalud);                      // parametrizado
    Animal(const Animal& otro);                                  // de copia
    virtual ~Animal();                                           // virtual: clave en herencia

    // --- Getters y setters (encapsulamiento) ---
    int getId() const;
    std::string getNombre() const;
    int getEdad() const;
    std::string getEstadoSalud() const;
    EstadoAdopcion getEstado() const;
    void setEstado(EstadoAdopcion nuevoEstado);
    void setEstadoSalud(const std::string& nuevoEstadoSalud);
    bool estaDisponible() const;

    // --- Fase 4: interfaz polimorfica ---
    virtual void mostrarInfo() const = 0;      // virtual pura -> vuelve abstracta a la clase
    virtual std::string tipo() const = 0;      // devuelve "Perro" o "Gato"
    virtual Animal* clonar() const = 0;        // necesaria para la COPIA PROFUNDA del catalogo

    // --- Fase 3: sobrecarga de operadores ---
    bool operator==(const Animal& otro) const; // compara animales por ID
    bool operator!=(const Animal& otro) const; // complemento de ==
    bool operator!() const;                    // unario: true si el animal NO esta disponible
};

#endif //ANIMAL_H
