#include "Refugio.h"
#include "Perro.h"
#include "Gato.h"
#include "Excepciones.h"
#include "Utilidades.h"
#include <iostream>

using namespace std;

// ===================================================================
// CONSTRUCTORES, DESTRUCTOR Y REGLA DE LOS TRES
// ===================================================================

// Constructor: reserva el arreglo inicial de punteros
Refugio::Refugio(const string& nombre)
    : nombre(nombre), catalogo(new Animal*[4]), cantAnimales(0), capacidad(4),
      adoptantes(), solicitudes(), proximoIdSolicitud(1) {
    // Dejo los punteros en nullptr para que el arreglo no tenga basura
    for (int i = 0; i < capacidad; i++)
        catalogo[i] = nullptr;
}

// --- Constructor de COPIA: copia profunda ---
// No alcanza con copiar el puntero 'catalogo' (eso seria copia superficial:
// dos refugios apuntando al mismo bloque, y al destruirse uno el otro queda
// con punteros colgados y se haria doble delete).
// Por eso se reserva un arreglo nuevo y se CLONA cada animal con clonar(),
// que gracias al polimorfismo devuelve un Perro o un Gato segun corresponda.
Refugio::Refugio(const Refugio& otro)
    : nombre(otro.nombre), catalogo(new Animal*[otro.capacidad]),
      cantAnimales(otro.cantAnimales), capacidad(otro.capacidad),
      adoptantes(otro.adoptantes),        // Coleccion<T> ya hace su copia profunda
      solicitudes(otro.solicitudes),
      proximoIdSolicitud(otro.proximoIdSolicitud) {
    for (int i = 0; i < capacidad; i++)
        catalogo[i] = nullptr;
    for (int i = 0; i < cantAnimales; i++)
        catalogo[i] = otro.catalogo[i]->clonar();   // objeto nuevo e independiente
}

// --- Operador de asignacion ---
Refugio& Refugio::operator=(const Refugio& otro) {
    if (this != &otro) {                 // 1) control de autoasignacion
        liberarCatalogo();               // 2) libero lo que tenia

        nombre = otro.nombre;
        capacidad = otro.capacidad;
        cantAnimales = otro.cantAnimales;
        proximoIdSolicitud = otro.proximoIdSolicitud;

        catalogo = new Animal*[capacidad];          // 3) memoria nueva
        for (int i = 0; i < capacidad; i++)
            catalogo[i] = nullptr;
        for (int i = 0; i < cantAnimales; i++)
            catalogo[i] = otro.catalogo[i]->clonar();

        adoptantes = otro.adoptantes;               // usa el operator= de Coleccion<T>
        solicitudes = otro.solicitudes;
    }
    return *this;                        // 4) permito encadenar a = b = c
}

// Libera cada animal y despues el arreglo de punteros
void Refugio::liberarCatalogo() {
    if (catalogo != nullptr) {
        for (int i = 0; i < cantAnimales; i++) {
            delete catalogo[i];          // llama al destructor virtual (~Perro/~Gato)
            catalogo[i] = nullptr;
        }
        delete[] catalogo;               // libera el arreglo de punteros
        catalogo = nullptr;
    }
    cantAnimales = 0;
    capacidad = 0;
}

// --- Destructor: el Refugio es el duenio de la memoria del catalogo ---
Refugio::~Refugio() {
    liberarCatalogo();
    // adoptantes y solicitudes liberan su memoria en su propio destructor
}

string Refugio::getNombre() const { return nombre; }
void Refugio::setNombre(const string& nuevoNombre) { nombre = nuevoNombre; }

// ===================================================================
// AUXILIARES PRIVADOS
// ===================================================================

// Duplica la capacidad del arreglo de punteros cuando se llena
void Refugio::redimensionarCatalogo(int nuevaCapacidad) {
    Animal** nuevo = new Animal*[nuevaCapacidad];
    for (int i = 0; i < nuevaCapacidad; i++)
        nuevo[i] = nullptr;
    for (int i = 0; i < cantAnimales; i++)
        nuevo[i] = catalogo[i];          // se copian los PUNTEROS, no los objetos:
                                         // los animales siguen siendo los mismos
    delete[] catalogo;                   // libero solo el arreglo viejo (no los animales)
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

// ===================================================================
// ALTAS
// ===================================================================

void Refugio::agregarAnimal(Animal* nuevo) {
    if (nuevo == nullptr)
        throw DatoInvalidoException("se intento agregar un animal nulo.");

    // Control de ID repetido (Fase 5)
    if (buscarIndiceAnimal(nuevo->getId()) != -1) {
        int idRepetido = nuevo->getId();
        delete nuevo;      // importante: si no se guarda, hay que liberarlo aca
        throw IdRepetidoException(idRepetido);
    }

    if (cantAnimales == capacidad)
        redimensionarCatalogo(capacidad * 2);

    catalogo[cantAnimales] = nuevo;      // el Refugio pasa a ser el duenio
    cantAnimales++;
}

void Refugio::registrarAdoptante(const Adoptante& nuevo) {
    if (buscarIndiceAdoptante(nuevo.getId()) != -1)
        throw IdRepetidoException(nuevo.getId());
    adoptantes.agregar(nuevo);           // se guarda una copia dentro de la coleccion
}

// ===================================================================
// CONSULTAS
// ===================================================================

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

// Recorrido POLIMORFICO: el mismo llamado a mostrarInfo() ejecuta la version
// de Perro o la de Gato segun el objeto real al que apunta cada puntero
// (ligadura dinamica gracias a que mostrarInfo() es virtual).
void Refugio::listarAnimales(bool soloDisponibles) const {
    if (cantAnimales == 0) {
        cout << "(El refugio no tiene animales cargados)" << endl;
        return;
    }
    int mostrados = 0;
    for (int i = 0; i < cantAnimales; i++) {
        if (soloDisponibles && !catalogo[i]->estaDisponible())
            continue;                    // salteo los que no estan disponibles
        cout << "  ";
        catalogo[i]->mostrarInfo();
        mostrados++;
    }
    if (mostrados == 0)
        cout << "(No hay animales que cumplan el filtro)" << endl;
    else
        cout << endl << "Total listado: " << mostrados << " animal(es)." << endl;
}

// FASE 2 - Recorrido con ARITMETICA DE PUNTEROS.
// 'p' arranca apuntando al primer elemento del arreglo y avanza de a una
// posicion con p++. La condicion compara punteros: se corta cuando 'p'
// llego al final (catalogo + cantAnimales). *p es el Animal* guardado.
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
            cout << "  ";
            solicitudes[i].mostrarInfo();
            int idx = buscarIndiceAnimal(solicitudes[i].getIdAnimal());
            if (idx != -1)
                cout << "      -> " << catalogo[idx]->tipo()
                     << " " << catalogo[idx]->getNombre() << endl;
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
    // Miembros estaticos: sirven para verificar que no queden fugas de memoria
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

// ===================================================================
// FASE 3 - OPERADORES DE LA CLASE REFUGIO
// ===================================================================

// operator[] : acceso por POSICION dentro del catalogo (0..cantAnimales-1)
Animal& Refugio::operator[](int indice) const {
    if (indice < 0 || indice >= cantAnimales)
        throw IndiceInvalidoException(indice, cantAnimales);
    return *catalogo[indice];            // devuelvo referencia al objeto apuntado
}

// operator() : busqueda por ID. Si no existe lanza NoEncontradoException,
// por eso el puntero devuelto nunca es nullptr.
Animal* Refugio::operator()(int id) const {
    int idx = buscarIndiceAnimal(id);
    if (idx == -1)
        throw NoEncontradoException("un animal", id);
    return catalogo[idx];
}

// ===================================================================
// PROCESO DE ADOPCION
// ===================================================================

int Refugio::crearSolicitud(int idAdoptante, int idAnimal) {
    if (buscarIndiceAdoptante(idAdoptante) == -1)
        throw NoEncontradoException("un adoptante", idAdoptante);

    Animal* animal = (*this)(idAnimal);  // usa el operator() -> lanza si no existe

    // Aca se usa el operador unario ! : "si el animal NO esta disponible"
    if (!(*animal))
        throw AnimalNoDisponibleException(idAnimal, estadoATexto(animal->getEstado()));

    SolicitudAdopcion nueva(proximoIdSolicitud, idAdoptante, idAnimal);
    solicitudes.agregar(nueva);
    proximoIdSolicitud++;

    animal->setEstado(EstadoAdopcion::EN_PROCESO);   // queda reservado
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
    // Al cancelarse el tramite el animal vuelve a estar disponible
    animal->setEstado(EstadoAdopcion::DISPONIBLE);
}

// Devolucion: el animal adoptado (o reservado) vuelve al refugio
void Refugio::devolverAnimal(int idAnimal) {
    Animal* animal = (*this)(idAnimal);
    if (animal->estaDisponible())
        throw DatoInvalidoException("el animal con ID " + to_string(idAnimal) +
                                    " ya figura como DISPONIBLE.");

    // Si tenia una solicitud pendiente, se cancela para no dejarla colgada
    for (int i = 0; i < solicitudes.cantidad(); i++)
        if (solicitudes[i].getIdAnimal() == idAnimal && solicitudes[i].estaPendiente())
            solicitudes[i].cancelar();

    animal->setEstado(EstadoAdopcion::DISPONIBLE);
}

// ===================================================================
// DATOS DE PRUEBA (para no tener que cargar todo a mano en cada demo)
// ===================================================================
void Refugio::cargarDatosDePrueba() {
    agregarAnimal(new Perro(1, "Rocky", 3, "Sano", "Labrador", "Grande"));
    agregarAnimal(new Perro(2, "Luna", 1, "Vacunada", "Caniche", "Pequenio"));
    agregarAnimal(new Gato(3, "Michi", 2, "Sano", "Naranja", true));
    agregarAnimal(new Gato(4, "Nube", 5, "En tratamiento", "Blanco", false));

    registrarAdoptante(Adoptante(100, "Ana Perez", "351-1234567", "Casa con patio"));
    registrarAdoptante(Adoptante(101, "Juan Gomez", "juan@mail.com", "Departamento"));
}
