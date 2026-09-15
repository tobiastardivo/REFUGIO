#ifndef COLECCION_H
#define COLECCION_H

#include "Excepciones.h"

// ===================================================================
// FASE 5 - Clase generica (template) Coleccion<T>
//
// Guarda los elementos POR VALOR en un arreglo dinamico que crece solo.
// En este TP se instancia con dos tipos distintos:
//     Coleccion<Adoptante>          (dentro de Refugio)
//     Coleccion<SolicitudAdopcion>  (dentro de Refugio)
//
// Como es una plantilla, la implementacion va entera en el .h:
// el compilador necesita ver el codigo para generar cada instanciacion.
//
// MIEMBRO ESTATICO: 'coleccionesCreadas' NO es un contador global unico.
// El compilador genera una clase distinta por cada tipo T, y cada una
// tiene su propia copia del estatico. Es decir:
//     Coleccion<Adoptante>::coleccionesCreadas
//     Coleccion<SolicitudAdopcion>::coleccionesCreadas
// son dos variables independientes que se cuentan por separado.
// ===================================================================

template <typename T>
class Coleccion {
private:
    T* elementos;     // arreglo dinamico de elementos (new[] / delete[])
    int cant;         // cuantos elementos hay cargados
    int capacidad;    // cuantos entran sin volver a pedir memoria

    // Pide un bloque mas grande, copia lo viejo y libera el bloque anterior
    void redimensionar(int nuevaCapacidad) {
        T* nuevo = new T[nuevaCapacidad];             // 1) reservo el bloque nuevo
        for (int i = 0; i < cant; i++)
            nuevo[i] = elementos[i];                  // 2) copio elemento por elemento
        delete[] elementos;                           // 3) libero el bloque viejo
        elementos = nuevo;                            // 4) apunto al nuevo
        capacidad = nuevaCapacidad;
    }

public:
    static int coleccionesCreadas;   // se declara aca y se define abajo

    // --- Constructor ---
    explicit Coleccion(int capacidadInicial = 4)
        : elementos(new T[capacidadInicial]), cant(0), capacidad(capacidadInicial) {
        coleccionesCreadas++;
    }

    // --- Constructor de copia: COPIA PROFUNDA ---
    // Reserva su propio bloque de memoria en vez de compartir el puntero.
    Coleccion(const Coleccion<T>& otra)
        : elementos(new T[otra.capacidad]), cant(otra.cant), capacidad(otra.capacidad) {
        for (int i = 0; i < cant; i++)
            elementos[i] = otra.elementos[i];
        coleccionesCreadas++;
    }

    // --- Operador de asignacion (Regla de los Tres) ---
    Coleccion<T>& operator=(const Coleccion<T>& otra) {
        if (this != &otra) {                 // control de autoasignacion
            delete[] elementos;              // libero lo mio
            capacidad = otra.capacidad;
            cant = otra.cant;
            elementos = new T[capacidad];    // reservo memoria nueva
            for (int i = 0; i < cant; i++)
                elementos[i] = otra.elementos[i];
        }
        return *this;
    }

    // --- Destructor: libera el arreglo dinamico ---
    ~Coleccion() {
        delete[] elementos;
        elementos = nullptr;
    }

    // Agrega al final; si no hay lugar duplica la capacidad
    void agregar(const T& elemento) {
        if (cant == capacidad)
            redimensionar(capacidad * 2);
        elementos[cant] = elemento;
        cant++;
    }

    int cantidad() const { return cant; }
    bool vacia() const   { return cant == 0; }

    // Acceso por posicion con control de rango -> lanza excepcion (Fase 5)
    T& operator[](int indice) {
        if (indice < 0 || indice >= cant)
            throw IndiceInvalidoException(indice, cant);
        return elementos[indice];
    }

    // Version const: permite leer elementos de una coleccion constante
    const T& operator[](int indice) const {
        if (indice < 0 || indice >= cant)
            throw IndiceInvalidoException(indice, cant);
        return elementos[indice];
    }
};

// Definicion del miembro estatico de la plantilla (una copia por cada tipo T)
template <typename T>
int Coleccion<T>::coleccionesCreadas = 0;

#endif //COLECCION_H
