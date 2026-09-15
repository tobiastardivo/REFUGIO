#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>
#include <iostream>
#include "Coleccion.h"

// ===================================================================
// Funciones auxiliares de entrada/salida por consola (Fase 1)
// y la FUNCION GENERICA pedida en la Fase 5.
// ===================================================================

// --- Lectura validada desde teclado ---
// Leen una linea completa y la validan; si el dato esta mal, vuelven a pedirlo.
int leerEntero(const std::string& mensaje, int minimo, int maximo);
std::string leerTexto(const std::string& mensaje);
bool leerSiNo(const std::string& mensaje);       // acepta SI/S/NO/N en mayus o minus
std::string aMayusculas(const std::string& texto);

// --- Utilidades de pantalla ---
void pausar();
void limpiarPantalla();
void linea(char caracter = '=', int largo = 60);

// ===================================================================
// FASE 5 - FUNCION GENERICA (template)
// Recorre cualquier Coleccion<T> y muestra sus elementos, sin importar
// de que tipo sean, siempre que T tenga un metodo mostrarInfo().
// En el TP se la usa con DOS TIPOS DISTINTOS:
//     mostrarColeccion(adoptantes);   -> Coleccion<Adoptante>
//     mostrarColeccion(solicitudes);  -> Coleccion<SolicitudAdopcion>
// Va en el .h por el mismo motivo que la clase generica: el compilador
// necesita el codigo a la vista para instanciarla con cada tipo.
// ===================================================================
template <typename T>
void mostrarColeccion(const Coleccion<T>& coleccion) {
    if (coleccion.vacia()) {
        std::cout << "(No hay registros cargados)" << std::endl;
        return;
    }
    for (int i = 0; i < coleccion.cantidad(); i++) {
        std::cout << "  " << (i + 1) << ") ";
        coleccion[i].mostrarInfo();     // usa el operator[] const de Coleccion<T>
    }
}

#endif //UTILIDADES_H
