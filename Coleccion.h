#ifndef COLECCION_H
#define COLECCION_H

#include "Excepciones.h"

// ===================================================================
// MIEMBRO ESTATICO (Fase 5): 'coleccionesCreadas' NO es un contador
// global unico. El compilador genera una clase distinta por cada tipo T
// y cada una tiene su propia copia del estatico, es decir que
//     Coleccion<Adoptante>::coleccionesCreadas
//     Coleccion<SolicitudAdopcion>::coleccionesCreadas
// son dos variables independientes que se cuentan por separado.
// ===================================================================
template <typename T>
class Coleccion {
private:
    T* elementos;
    int cant;
    int capacidad;

    void redimensionar(int nuevaCapacidad) {
        T* nuevo = new T[nuevaCapacidad];
        for (int i = 0; i < cant; i++)
            nuevo[i] = elementos[i];
        delete[] elementos;
        elementos = nuevo;
        capacidad = nuevaCapacidad;
    }

public:
    static int coleccionesCreadas;

    explicit Coleccion(int capacidadInicial = 4)
        : elementos(new T[capacidadInicial]), cant(0), capacidad(capacidadInicial) {
        coleccionesCreadas++;
    }

    Coleccion(const Coleccion<T>& otra)
        : elementos(new T[otra.capacidad]), cant(otra.cant), capacidad(otra.capacidad) {
        for (int i = 0; i < cant; i++)
            elementos[i] = otra.elementos[i];
        coleccionesCreadas++;
    }

    Coleccion<T>& operator=(const Coleccion<T>& otra) {
        if (this != &otra) {
            delete[] elementos;
            capacidad = otra.capacidad;
            cant = otra.cant;
            elementos = new T[capacidad];
            for (int i = 0; i < cant; i++)
                elementos[i] = otra.elementos[i];
        }
        return *this;
    }

    ~Coleccion() {
        delete[] elementos;
        elementos = nullptr;
    }

    void agregar(const T& elemento) {
        if (cant == capacidad)
            redimensionar(capacidad * 2);
        elementos[cant] = elemento;
        cant++;
    }

    int cantidad() const { return cant; }
    bool vacia() const   { return cant == 0; }

    T& operator[](int indice) {
        if (indice < 0 || indice >= cant)
            throw IndiceInvalidoException(indice, cant);
        return elementos[indice];
    }

    const T& operator[](int indice) const {
        if (indice < 0 || indice >= cant)
            throw IndiceInvalidoException(indice, cant);
        return elementos[indice];
    }
};

template <typename T>
int Coleccion<T>::coleccionesCreadas = 0;

#endif
