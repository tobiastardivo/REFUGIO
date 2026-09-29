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

    // --- Destructor (Regla de los Tres) ---
    // delete[] (con corchetes) y no delete, porque lo que se reservo fue
    // un ARREGLO con new T[]: los corchetes son los que hacen que se
    // ejecute el destructor de cada uno de los elementos.
    ~Coleccion() {
        delete[] elementos;
        elementos = nullptr;
    }

    // --- agregar(): suma un elemento al final ---
    // Si el arreglo esta lleno DUPLICA la capacidad en vez de agrandarlo
    // de a uno. Crecer de a uno obligaria a copiar todo en cada alta;
    // duplicando, el costo de las copias se reparte y agregar sale, en
    // promedio, casi gratis. Guarda una COPIA del elemento recibido.
    void agregar(const T& elemento) {
        if (cant == capacidad)
            redimensionar(capacidad * 2);
        elementos[cant] = elemento;
        cant++;
    }

    // Consultas de solo lectura. Devuelven la cantidad de elementos
    // CARGADOS, no la capacidad reservada: los lugares libres del bloque
    // existen en memoria pero todavia no tienen datos validos.
    int cantidad() const { return cant; }
    bool vacia() const   { return cant == 0; }

    // --- operator[]: acceso por posicion CON control de rango ---
    // A diferencia de un arreglo comun de C++, que ante un indice invalido
    // lee memoria ajena sin avisar, aca se verifica y se lanza excepcion
    // (Fase 5): el error se detecta en el momento y con un mensaje claro.
    // Hay DOS versiones. Esta devuelve T& (referencia modificable) y se
    // elige cuando la coleccion no es constante; la de abajo devuelve
    // const T& y es la unica que se puede usar sobre una coleccion const,
    // como la que devuelven getAdoptantes() y getSolicitudes().
    T& operator[](int indice) {
        if (indice < 0 || indice >= cant)
            throw IndiceInvalidoException(indice, cant);
        return elementos[indice];
    }

    // Version const: mismo control de rango, pero solo permite leer.
    // Es la que usa la funcion generica mostrarColeccion<T>().
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
