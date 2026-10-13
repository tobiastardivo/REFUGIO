#include "Refugio.h"
#include "Perro.h"
#include "Gato.h"
#include "Excepciones.h"
#include "Utilidades.h"
#include <iostream>

using namespace std;

Refugio::Refugio(const string& nombre)
    : nombre(nombre), catalogo(new Animal*[4]), cantAnimales(0), capacidad(4),
      adoptantes(), solicitudes(), proximoIdSolicitud(1),
      proximoIdAnimal(1), proximoIdAdoptante(100) {
    for (int i = 0; i < capacidad; i++)
        catalogo[i] = nullptr;
}

Refugio::Refugio(const Refugio& otro)
    : nombre(otro.nombre), catalogo(new Animal*[otro.capacidad]),
      cantAnimales(otro.cantAnimales), capacidad(otro.capacidad),
      adoptantes(otro.adoptantes),
      solicitudes(otro.solicitudes),
      proximoIdSolicitud(otro.proximoIdSolicitud),
      proximoIdAnimal(otro.proximoIdAnimal),
      proximoIdAdoptante(otro.proximoIdAdoptante) {
    for (int i = 0; i < capacidad; i++)
        catalogo[i] = nullptr;
    for (int i = 0; i < cantAnimales; i++)
        catalogo[i] = otro.catalogo[i]->clonar();
}

Refugio& Refugio::operator=(const Refugio& otro) {
    if (this != &otro) {
        liberarCatalogo();

        nombre = otro.nombre;
        capacidad = otro.capacidad;
        cantAnimales = otro.cantAnimales;
        proximoIdSolicitud = otro.proximoIdSolicitud;
        proximoIdAnimal = otro.proximoIdAnimal;
        proximoIdAdoptante = otro.proximoIdAdoptante;

        catalogo = new Animal*[capacidad];
        for (int i = 0; i < capacidad; i++)
            catalogo[i] = nullptr;
        for (int i = 0; i < cantAnimales; i++)
            catalogo[i] = otro.catalogo[i]->clonar();

        adoptantes = otro.adoptantes;
        solicitudes = otro.solicitudes;
    }
    return *this;
}

void Refugio::liberarCatalogo() {
    if (catalogo != nullptr) {
        for (int i = 0; i < cantAnimales; i++) {
            delete catalogo[i];
            catalogo[i] = nullptr;
        }
        delete[] catalogo;
        catalogo = nullptr;
    }
    cantAnimales = 0;
    capacidad = 0;
}

Refugio::~Refugio() {
    liberarCatalogo();
}

string Refugio::getNombre() const { return nombre; }
void Refugio::setNombre(const string& nuevoNombre) { nombre = nuevoNombre; }

void Refugio::redimensionarCatalogo(int nuevaCapacidad) {
    Animal** nuevo = new Animal*[nuevaCapacidad];
    for (int i = 0; i < nuevaCapacidad; i++)
        nuevo[i] = nullptr;
    for (int i = 0; i < cantAnimales; i++)
        nuevo[i] = catalogo[i];

    delete[] catalogo;
    catalogo = nuevo;
    capacidad = nuevaCapacidad;
}

int Refugio::buscarIndiceAnimal(int id) const {
    for (int i = 0; i < cantAnimales; i++)
        if (catalogo[i]->getId() == id) return i;
    return -1;
}

int Refugio::buscarIndiceAdoptante(int id) const {
    for (int i = 0; i < adoptantes.cantidad(); i++)
        if (adoptantes[i].getId() == id) return i;
    return -1;
}

int Refugio::buscarIndiceSolicitud(int id) const {
    for (int i = 0; i < solicitudes.cantidad(); i++)
        if (solicitudes[i].getId() == id) return i;
    return -1;
}

void Refugio::agregarAnimal(Animal* nuevo) {
    if (nuevo == nullptr)
        throw DatoInvalidoException("se intento agregar un animal nulo.");

    if (buscarIndiceAnimal(nuevo->getId()) != -1) {
        int idRepetido = nuevo->getId();
        delete nuevo;
        throw IdRepetidoException(idRepetido);
    }

    if (cantAnimales == capacidad)
        redimensionarCatalogo(capacidad * 2);

    catalogo[cantAnimales] = nuevo;
    cantAnimales++;

    if (nuevo->getId() >= proximoIdAnimal)
        proximoIdAnimal = nuevo->getId() + 1;
}

void Refugio::registrarAdoptante(const Adoptante& nuevo) {
    if (buscarIndiceAdoptante(nuevo.getId()) != -1)
        throw IdRepetidoException(nuevo.getId());
    adoptantes.agregar(nuevo);

    if (nuevo.getId() >= proximoIdAdoptante)
        proximoIdAdoptante = nuevo.getId() + 1;
}

int Refugio::nuevoIdAnimal() const    { return proximoIdAnimal; }
int Refugio::nuevoIdAdoptante() const { return proximoIdAdoptante; }

int Refugio::getCantidadAnimales() const    { return cantAnimales; }
int Refugio::getCantidadAdoptantes() const  { return adoptantes.cantidad(); }
int Refugio::getCantidadSolicitudes() const { return solicitudes.cantidad(); }

const Coleccion<Adoptante>& Refugio::getAdoptantes() const { return adoptantes; }
const Coleccion<SolicitudAdopcion>& Refugio::getSolicitudes() const { return solicitudes; }

int Refugio::contarDisponibles() const {
    int cont = 0;
    for (int i = 0; i < cantAnimales; i++)
        if (catalogo[i]->estaDisponible()) cont++;
    return cont;
}

void Refugio::listarAnimales(bool soloDisponibles) const {
    if (cantAnimales == 0) {
        cout << "(El refugio no tiene animales cargados)" << endl;
        return;
    }
    int mostrados = 0;
    for (int i = 0; i < cantAnimales; i++) {
        if (soloDisponibles && !catalogo[i]->estaDisponible())
            continue;
        cout << "  ";
        catalogo[i]->mostrarInfo();
        mostrados++;
    }
    if (mostrados == 0)
        cout << "(No hay animales que cumplan el filtro)" << endl;
    else
        cout << endl << "Total listado: " << mostrados << " animal(es)." << endl;
}

void Refugio::listarConAritmeticaDePunteros() const {
    if (cantAnimales == 0) {
        cout << "(El refugio no tiene animales cargados)" << endl;
        return;
    }
    cout << "Recorrido del catalogo usando aritmetica de punteros:" << endl;
    for (Animal** p = catalogo; p < catalogo + cantAnimales; p++) {
        cout << "  [dir. relativa +" << (p - catalogo) << "] ";
        (*p)->mostrarInfo();
    }
}

void Refugio::mostrarHistorialAdopciones() const {
    cout << "--- Historial de adopciones concretadas ---" << endl;
    int cont = 0;
    for (int i = 0; i < solicitudes.cantidad(); i++) {
        if (solicitudes[i].getEstado() == EstadoSolicitud::CONFIRMADA) {
            cout << "  Solicitud N " << solicitudes[i].getId() << " | ";

            int idxAdoptante = buscarIndiceAdoptante(solicitudes[i].getIdAdoptante());
            if (idxAdoptante != -1)
                cout << adoptantes[idxAdoptante].getNombre();
            else
                cout << "(adoptante dado de baja)";

            int idxAnimal = buscarIndiceAnimal(solicitudes[i].getIdAnimal());
            if (idxAnimal != -1)
                cout << " adopto a " << catalogo[idxAnimal]->getNombre()
                     << " (" << catalogo[idxAnimal]->tipo() << ")";
            else
                cout << " adopto a (animal dado de baja)";

            cout << endl;
            cont++;
        }
    }
    if (cont == 0) cout << "  (Todavia no hay adopciones confirmadas)" << endl;
}

void Refugio::mostrarEstadisticas() const {
    int perros = 0, gatos = 0, disp = 0, proceso = 0, adoptados = 0;
    for (int i = 0; i < cantAnimales; i++) {
        if (catalogo[i]->tipo() == "Perro") perros++; else gatos++;
        switch (catalogo[i]->getEstado()) {
            case EstadoAdopcion::DISPONIBLE: disp++; break;
            case EstadoAdopcion::EN_PROCESO: proceso++; break;
            case EstadoAdopcion::ADOPTADO:   adoptados++; break;
        }
    }
    int pend = 0, conf = 0, canc = 0;
    for (int i = 0; i < solicitudes.cantidad(); i++) {
        switch (solicitudes[i].getEstado()) {
            case EstadoSolicitud::PENDIENTE:  pend++; break;
            case EstadoSolicitud::CONFIRMADA: conf++; break;
            case EstadoSolicitud::CANCELADA:  canc++; break;
        }
    }

    cout << "Refugio: " << nombre << endl;
    linea('-');
    cout << "Animales: " << cantAnimales
         << "  (Perros: " << perros << " | Gatos: " << gatos << ")" << endl;
    cout << "  Disponibles: " << disp
         << " | En proceso: " << proceso
         << " | Adoptados: " << adoptados << endl;
    cout << "Adoptantes registrados: " << adoptantes.cantidad() << endl;
    cout << "Solicitudes: " << solicitudes.cantidad()
         << "  (Pendientes: " << pend
         << " | Confirmadas: " << conf
         << " | Canceladas: " << canc << ")" << endl;
    linea('-');

    cout << "[Miembros estaticos]" << endl;
    cout << "  Animales vivos en memoria (Animal::animalesVivos): "
         << Animal::animalesVivos << endl;
    cout << "  Coleccion<Adoptante>::coleccionesCreadas ......... : "
         << Coleccion<Adoptante>::coleccionesCreadas << endl;
    cout << "  Coleccion<SolicitudAdopcion>::coleccionesCreadas . : "
         << Coleccion<SolicitudAdopcion>::coleccionesCreadas << endl;
    cout << "  (Son contadores INDEPENDIENTES: el compilador genera una" << endl;
    cout << "   clase distinta por cada tipo T, cada una con su estatico)" << endl;
}

Animal& Refugio::operator[](int indice) const {
    if (indice < 0 || indice >= cantAnimales)
        throw IndiceInvalidoException(indice, cantAnimales);
    return *catalogo[indice];
}

Animal* Refugio::operator()(int id) const {
    int idx = buscarIndiceAnimal(id);
    if (idx == -1)
        throw NoEncontradoException("un animal", id);
    return catalogo[idx];
}

int Refugio::crearSolicitud(int idAdoptante, int idAnimal) {
    if (buscarIndiceAdoptante(idAdoptante) == -1)
        throw NoEncontradoException("un adoptante", idAdoptante);

    Animal* animal = (*this)(idAnimal);

    if (!(*animal))
        throw AnimalNoDisponibleException(idAnimal, estadoATexto(animal->getEstado()));

    SolicitudAdopcion nueva(proximoIdSolicitud, idAdoptante, idAnimal);
    solicitudes.agregar(nueva);
    proximoIdSolicitud++;

    animal->setEstado(EstadoAdopcion::EN_PROCESO);
    return nueva.getId();
}

void Refugio::confirmarSolicitud(int idSolicitud) {
    int idx = buscarIndiceSolicitud(idSolicitud);
    if (idx == -1)
        throw NoEncontradoException("una solicitud", idSolicitud);

    if (!solicitudes[idx].estaPendiente())
        throw DatoInvalidoException("la solicitud N " + to_string(idSolicitud) +
                                    " ya fue " +
                                    estadoSolicitudATexto(solicitudes[idx].getEstado()) + ".");

    Animal* animal = (*this)(solicitudes[idx].getIdAnimal());
    solicitudes[idx].confirmar();
    animal->setEstado(EstadoAdopcion::ADOPTADO);
}

void Refugio::cancelarSolicitud(int idSolicitud) {
    int idx = buscarIndiceSolicitud(idSolicitud);
    if (idx == -1)
        throw NoEncontradoException("una solicitud", idSolicitud);

    if (!solicitudes[idx].estaPendiente())
        throw DatoInvalidoException("la solicitud N " + to_string(idSolicitud) +
                                    " ya fue " +
                                    estadoSolicitudATexto(solicitudes[idx].getEstado()) + ".");

    Animal* animal = (*this)(solicitudes[idx].getIdAnimal());
    solicitudes[idx].cancelar();

    animal->setEstado(EstadoAdopcion::DISPONIBLE);
}

void Refugio::devolverAnimal(int idAnimal) {
    Animal* animal = (*this)(idAnimal);
    if (animal->estaDisponible())
        throw DatoInvalidoException("el animal con ID " + to_string(idAnimal) +
                                    " ya figura como DISPONIBLE.");

    for (int i = 0; i < solicitudes.cantidad(); i++)
        if (solicitudes[i].getIdAnimal() == idAnimal && solicitudes[i].estaPendiente())
            solicitudes[i].cancelar();

    animal->setEstado(EstadoAdopcion::DISPONIBLE);
}

void Refugio::cargarDatosDePrueba() {
    agregarAnimal(new Perro(1, "Rocky", 3, "Sano", "Labrador", "Grande"));
    agregarAnimal(new Perro(2, "Luna", 1, "Vacunada", "Caniche", "Pequenio"));
    agregarAnimal(new Gato(3, "Michi", 2, "Sano", "Naranja", true));
    agregarAnimal(new Gato(4, "Nube", 5, "En tratamiento", "Blanco", false));

    registrarAdoptante(Adoptante(100, "Ana Perez", "351-1234567", "Casa con patio"));
    registrarAdoptante(Adoptante(101, "Juan Gomez", "juan@mail.com", "Departamento"));
}
