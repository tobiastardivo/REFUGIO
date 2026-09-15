#ifndef EXCEPCIONES_H
#define EXCEPCIONES_H

#include <exception>   // std::exception: clase base de todas las excepciones estandar
#include <string>

// ===================================================================
// FASE 5 - Excepciones personalizadas
// Todas heredan de std::exception, asi que ademas de capturarlas una
// por una se las puede atrapar de forma generica con
//     catch (const std::exception& e)
// Siempre se capturan por REFERENCIA CONSTANTE (const&) para evitar el
// "slicing": si se capturaran por valor se perderia la parte derivada.
// ===================================================================

// Clase base propia del sistema: guarda el mensaje y lo devuelve en what()
class RefugioException : public std::exception {
protected:
    std::string mensaje;                       // texto explicativo del error
public:
    explicit RefugioException(const std::string& msg) : mensaje(msg) {}
    // what() es virtual en std::exception, aca la redefinimos (override)
    const char* what() const noexcept override { return mensaje.c_str(); }
};

// Se lanza al intentar registrar un ID que ya existe
class IdRepetidoException : public RefugioException {
public:
    explicit IdRepetidoException(int id)
        : RefugioException("El ID " + std::to_string(id) + " ya esta registrado en el sistema.") {}
};

// Se lanza cuando se accede a una posicion fuera de rango (operador [])
class IndiceInvalidoException : public RefugioException {
public:
    IndiceInvalidoException(int indice, int cantidad)
        : RefugioException("Indice invalido: " + std::to_string(indice) +
                           ". La coleccion tiene " + std::to_string(cantidad) + " elemento(s).") {}
};

// Se lanza cuando se quiere adoptar un animal que no esta DISPONIBLE
class AnimalNoDisponibleException : public RefugioException {
public:
    AnimalNoDisponibleException(int id, const std::string& estado)
        : RefugioException("El animal con ID " + std::to_string(id) +
                           " no esta disponible (estado actual: " + estado + ").") {}
};

// Se lanza cuando no se encuentra un animal, un adoptante o una solicitud
class NoEncontradoException : public RefugioException {
public:
    NoEncontradoException(const std::string& que, int id)
        : RefugioException("No existe " + que + " con ID " + std::to_string(id) + ".") {}
};

// Se lanza ante datos cargados incorrectamente (edad negativa, nombre vacio, etc.)
class DatoInvalidoException : public RefugioException {
public:
    explicit DatoInvalidoException(const std::string& detalle)
        : RefugioException("Dato invalido: " + detalle) {}
};

#endif //EXCEPCIONES_H
