#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>
#include <iostream>
#include "Coleccion.h"

int leerEntero(const std::string& mensaje, int minimo, int maximo);
std::string leerTexto(const std::string& mensaje);
bool leerSiNo(const std::string& mensaje);
std::string aMayusculas(const std::string& texto);

void pausar();
void limpiarPantalla();
void linea(char caracter = '=', int largo = 60);

template <typename T>
void mostrarColeccion(const Coleccion<T>& coleccion) {
    if (coleccion.vacia()) {
        std::cout << "(No hay registros cargados)" << std::endl;
        return;
    }
    for (int i = 0; i < coleccion.cantidad(); i++) {
        std::cout << "  " << (i + 1) << ") ";
        coleccion[i].mostrarInfo();
    }
}

#endif
