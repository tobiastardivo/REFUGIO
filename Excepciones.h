#ifndef EXCEPCIONES_H
#define EXCEPCIONES_H

#include <exception>
#include <string>

class RefugioException : public std::exception {
protected:
    std::string mensaje;
public:
    explicit RefugioException(const std::string& msg) : mensaje(msg) {}

    const char* what() const noexcept override { return mensaje.c_str(); }
};

class IdRepetidoException : public RefugioException {
public:
    explicit IdRepetidoException(int id)
        : RefugioException("El ID " + std::to_string(id) + " ya esta registrado en el sistema.") {}
};

class IndiceInvalidoException : public RefugioException {
public:
    IndiceInvalidoException(int indice, int cantidad)
        : RefugioException("Indice invalido: " + std::to_string(indice) +
                           ". La coleccion tiene " + std::to_string(cantidad) + " elemento(s).") {}
};

class AnimalNoDisponibleException : public RefugioException {
public:
    AnimalNoDisponibleException(int id, const std::string& estado)
        : RefugioException("El animal con ID " + std::to_string(id) +
                           " no esta disponible (estado actual: " + estado + ").") {}
};

class NoEncontradoException : public RefugioException {
public:
    NoEncontradoException(const std::string& que, int id)
        : RefugioException("No existe " + que + " con ID " + std::to_string(id) + ".") {}
};

class DatoInvalidoException : public RefugioException {
public:
    explicit DatoInvalidoException(const std::string& detalle)
        : RefugioException("Dato invalido: " + detalle) {}
};

class NadaParaElegirException : public RefugioException {
public:
    explicit NadaParaElegirException(const std::string& aviso)
        : RefugioException(aviso) {}
};

#endif
