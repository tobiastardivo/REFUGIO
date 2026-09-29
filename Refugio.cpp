// ===================================================================
// Refugio.cpp - Implementacion de la clase que administra el sistema.
//
// El Refugio es el "cerebro" del TP: guarda el catalogo de animales, la
// coleccion de adoptantes y la de solicitudes, y concentra las reglas
// del negocio (que ID no se puede repetir, que animal se puede adoptar,
// como cambia el estado de una solicitud). No sabe nada de consola: si
// algo sale mal lanza una excepcion y deja que la interfaz decida como
// mostrarla.
//
// El archivo esta dividido en cinco secciones, en este orden:
//   1) Constructores, destructor y Regla de los Tres
//   2) Auxiliares privados
//   3) Altas
//   4) Consultas e informes
//   5) Operadores (Fase 3) y proceso de adopcion
// ===================================================================

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

// --- Constructor ---
// Reserva el arreglo inicial de 4 punteros y deja el refugio vacio.
// Las dos colecciones se construyen solas (adoptantes() y solicitudes()
// llaman a su constructor por defecto) y el numerador de solicitudes
// arranca en 1 para que la primera sea la "Solicitud N 1".
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

// --- Operador de asignacion (tercer miembro de la Regla de los Tres) ---
// Hace lo mismo que el constructor de copia, con una diferencia: el objeto
// de la izquierda YA EXISTE y ya tiene memoria reservada, asi que primero
// hay que liberarla. Los cuatro pasos estan numerados abajo.
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

// --- liberarCatalogo(): la unica funcion que libera el catalogo ---
// Son dos liberaciones distintas y el orden importa:
//   1) delete de cada animal -> destruye los OBJETOS Perro/Gato;
//   2) delete[] del arreglo  -> destruye el bloque de PUNTEROS.
// Si se hiciera al reves se perderian las direcciones antes de poder
// liberar los objetos, y esa memoria quedaria inaccesible para siempre.
// Los punteros se ponen en nullptr despues de cada delete para que no
// queden "colgados" apuntando a memoria que ya no es nuestra.
// La usan el destructor y el operador de asignacion: al centralizarla,
// la logica de liberacion esta escrita una sola vez.
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

// Nombre del refugio: lo muestra el encabezado del menu y lo cambia la
// prueba de copia profunda para distinguir el original de la copia.
string Refugio::getNombre() const { return nombre; }
void Refugio::setNombre(const string& nuevoNombre) { nombre = nuevoNombre; }

// ===================================================================
// AUXILIARES PRIVADOS
// ===================================================================

// --- redimensionarCatalogo(): agranda el arreglo cuando se llena ---
// Un arreglo dinamico no se puede "estirar": hay que pedir uno nuevo,
// mudar el contenido y devolver el viejo. Lo que se muda son los
// PUNTEROS, no los animales: los objetos no se tocan, siguen exactamente
// donde estaban y con las mismas direcciones. Por eso al final se hace
// delete[] del arreglo viejo pero NINGUN delete de animal.
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

// --- Las tres busquedas por ID ---
// Hacen una busqueda LINEAL: recorren de principio a fin comparando el ID
// hasta encontrarlo. Son privadas porque devuelven una POSICION interna,
// un dato que no tiene sentido fuera de la clase (si el catalogo se
// reordena, ese numero deja de significar lo mismo).
// Devuelven -1 cuando no hay coincidencia, y es el metodo publico que las
// llama el que decide que hacer con eso: casi siempre, lanzar una
// NoEncontradoException con un mensaje claro para el usuario.
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

// --- agregarAnimal(): alta de un animal en el catalogo ---
// Recibe un puntero YA creado con new (desde el menu) y se queda con la
// propiedad de esa memoria: a partir de aca, el unico que puede hacerle
// delete es el Refugio.
// Ese contrato obliga a tener cuidado en los caminos de error: si el alta
// no prospera porque el ID esta repetido, el objeto no queda guardado en
// ningun lado, asi que hay que liberarlo ANTES de lanzar la excepcion o
// se perderia esa memoria para siempre. Por eso el ID se copia a una
// variable antes del delete: despues de liberarlo ya no se puede leer.
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

// --- registrarAdoptante(): alta de una persona ---
// Mucho mas simple que el alta de animales porque no hay memoria dinamica
// de por medio: la coleccion guarda una COPIA del adoptante (lo recibe por
// referencia constante y agregar() lo copia adentro), asi que no hay
// ninguna propiedad que traspasar ni nada que liberar si el alta falla.
void Refugio::registrarAdoptante(const Adoptante& nuevo) {
    if (buscarIndiceAdoptante(nuevo.getId()) != -1)
        throw IdRepetidoException(nuevo.getId());
    adoptantes.agregar(nuevo);           // se guarda una copia dentro de la coleccion
}

// ===================================================================
// CONSULTAS
// ===================================================================

// --- Contadores ---
// Los usa el encabezado del menu para mostrar el resumen de una linea.
int Refugio::getCantidadAnimales() const    { return cantAnimales; }
int Refugio::getCantidadAdoptantes() const  { return adoptantes.cantidad(); }
int Refugio::getCantidadSolicitudes() const { return solicitudes.cantidad(); }

// --- Acceso de solo lectura a las dos colecciones ---
// Devuelven REFERENCIA CONSTANTE, no copia, por dos razones: copiar una
// coleccion entera solo para listarla seria un desperdicio, y el  const
// garantiza que el menu pueda leerlas pero no modificarlas por su cuenta
// (para eso estan los metodos del Refugio, que validan las reglas).
// Se usan para pasarselas a la funcion generica mostrarColeccion<T>().
const Coleccion<Adoptante>& Refugio::getAdoptantes() const { return adoptantes; }
const Coleccion<SolicitudAdopcion>& Refugio::getSolicitudes() const { return solicitudes; }

// --- contarDisponibles(): cuantos animales se pueden adoptar hoy ---
// Recorre el catalogo y cuenta los que estan en DISPONIBLE. No se guarda
// como atributo a proposito: un contador aparte habria que actualizarlo en
// cada alta, adopcion, cancelacion y devolucion, y bastaria olvidarse en
// un solo lugar para que quedara mintiendo. Calcularlo es mas confiable.
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

// --- mostrarHistorialAdopciones(): listado de adopciones concretadas ---
// Filtra las solicitudes que quedaron en CONFIRMADA y, para cada una,
// cruza el ID del animal contra el catalogo para poder mostrar tambien
// de que animal se trata. El  if (idx != -1)  cubre el caso de que el
// animal ya no este: la solicitud sigue siendo parte del historial y se
// muestra igual, pero sin el renglon del animal.
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

// --- mostrarEstadisticas(): informe general del refugio ---
// Se resuelve en tres tramos:
//   1) una pasada por el catalogo contando por tipo (con tipo(), que es
//      polimorfica) y por estado de adopcion;
//   2) una pasada por las solicitudes contando por estado del tramite;
//   3) la impresion, que ademas muestra los MIEMBROS ESTATICOS.
// Ese ultimo bloque es el que sirve para la defensa del TP: Animal::
// animalesVivos tiene que coincidir con la cantidad de animales cargados,
// y los dos contadores de Coleccion demuestran que cada instanciacion del
// template tiene su propio estatico independiente.
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

// --- crearSolicitud(): arranca un tramite de adopcion ---
// Valida en cascada y recien despues toca los datos, para que un pedido
// invalido no deje el sistema a medio modificar:
//   1) que el adoptante exista;
//   2) que el animal exista (lo resuelve el operator(), que ya lanza);
//   3) que el animal este DISPONIBLE (aca se usa el operador unario !).
// Si pasa las tres, crea la solicitud PENDIENTE, avanza el numerador y
// deja el animal EN_PROCESO, o sea reservado: asi dos personas no pueden
// iniciar un tramite por el mismo animal.
// Devuelve el numero de solicitud para que el menu pueda ofrecer
// confirmarla en el acto.
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

// --- confirmarSolicitud(): cierra el tramite con la adopcion hecha ---
// Exige que la solicitud exista y que siga PENDIENTE: una ya confirmada
// o cancelada es un estado final y no se puede volver a tocar (de ahi el
// mensaje "ya fue CONFIRMADA/CANCELADA").
// Deja los dos objetos en su estado final: la solicitud en CONFIRMADA y
// el animal en ADOPTADO. Se busca el animal ANTES de cambiar nada, para
// que si ese animal no existiera la excepcion salte con la solicitud
// todavia intacta.
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

// --- cancelarSolicitud(): da de baja un tramite abierto ---
// Mismas validaciones que confirmar (existir y seguir PENDIENTE), pero el
// desenlace es el opuesto: la solicitud queda CANCELADA y el animal, que
// estaba reservado, vuelve a DISPONIBLE para que otra persona lo pueda
// solicitar. Si no se liberara el animal quedaria bloqueado para siempre.
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

// --- devolverAnimal(): el animal vuelve al refugio ---
// Cubre el caso real de una adopcion que no funciono, y tambien el de una
// reserva que hay que deshacer. Se entra por el animal y no por la
// solicitud, porque quien devuelve trae al animal, no un numero de tramite.
// Rechaza a los que ya figuran DISPONIBLE: no tiene sentido devolver algo
// que nunca salio. Antes de liberarlo cancela cualquier solicitud que
// siguiera pendiente por ese animal, asi no queda un tramite abierto
// apuntando a un animal que ya volvio a estar libre.
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
// DATOS DE PRUEBA
// ===================================================================

// --- cargarDatosDePrueba(): deja el sistema con datos para mostrar ---
// La llama el constructor de MenuConsola, asi el programa arranca con
// algo cargado y no hay que dar de alta todo a mano en cada demostracion.
// Los datos estan elegidos para que se pueda probar el circuito completo
// sin cargar nada: hay perros y gatos (para ver el polimorfismo), un
// animal en tratamiento y dos adoptantes con tipos de vivienda distintos.
// Los IDs de los animales van de 1 en adelante y los de los adoptantes
// desde 100, para que en la pantalla nunca se confunda uno con otro.
void Refugio::cargarDatosDePrueba() {
    agregarAnimal(new Perro(1, "Rocky", 3, "Sano", "Labrador", "Grande"));
    agregarAnimal(new Perro(2, "Luna", 1, "Vacunada", "Caniche", "Pequenio"));
    agregarAnimal(new Gato(3, "Michi", 2, "Sano", "Naranja", true));
    agregarAnimal(new Gato(4, "Nube", 5, "En tratamiento", "Blanco", false));

    registrarAdoptante(Adoptante(100, "Ana Perez", "351-1234567", "Casa con patio"));
    registrarAdoptante(Adoptante(101, "Juan Gomez", "juan@mail.com", "Departamento"));
}
