#include "MenuConsola.h"
#include "Perro.h"
#include "Gato.h"
#include "Adoptante.h"
#include "Utilidades.h"
#include "Excepciones.h"
#include <iostream>

using namespace std;

MenuConsola::MenuConsola(const string& nombreRefugio) : refugio(nombreRefugio) {
    refugio.cargarDatosDePrueba();
}

MenuConsola::~MenuConsola() {}

void MenuConsola::ejecutar() {
    int opcion;
    do {
        limpiarPantalla();
        mostrarMenu();
        opcion = leerEntero("Ingrese una opcion (0-14): ", 0, 14);

        cout << endl;

        try {
            ejecutarOpcion(opcion);
        }

        catch (const NadaParaElegirException& e) {
            cout << e.what() << endl;
        }
        catch (const IdRepetidoException& e) {
            cout << "[ERROR - ID repetido] " << e.what() << endl;
        }
        catch (const IndiceInvalidoException& e) {
            cout << "[ERROR - Indice] " << e.what() << endl;
        }
        catch (const AnimalNoDisponibleException& e) {
            cout << "[ERROR - Adopcion] " << e.what() << endl;
        }
        catch (const NoEncontradoException& e) {
            cout << "[ERROR - Busqueda] " << e.what() << endl;
        }
        catch (const RefugioException& e) {
            cout << "[ERROR] " << e.what() << endl;
        }
        catch (const std::exception& e) {
            cout << "[ERROR inesperado] " << e.what() << endl;
        }

        if (opcion != 0) pausar();
    } while (opcion != 0);

    cout << endl << "Animales en memoria ANTES de destruir el refugio: "
         << Animal::animalesVivos << endl;
}

void MenuConsola::ejecutarOpcion(int opcion) {
    switch (opcion) {
        case 1:  registrarPerro(); break;
        case 2:  registrarGato(); break;
        case 3:  refugio.listarAnimales(false); break;
        case 4:  refugio.listarAnimales(true); break;
        case 5:  registrarAdoptante(); break;
        case 6:  verFichaDeAnimal(); break;
        case 7:  crearSolicitud(); break;
        case 8:  confirmarSolicitud(); break;
        case 9:  cancelarSolicitud(); break;
        case 10: devolverAnimal(); break;
        case 11: verAdoptantesYSolicitudes(); break;
        case 12: refugio.mostrarHistorialAdopciones(); break;
        case 13: refugio.mostrarEstadisticas(); break;
        case 14: pruebasDeIntegridad(); break;
        case 0:  cout << "Cerrando el sistema..." << endl; break;
        default: cout << "Opcion inexistente." << endl;
    }
}

void MenuConsola::informarMemoriaLiberada() {
    cout << "Animales en memoria DESPUES de destruir el refugio: "
         << Animal::animalesVivos << endl;
    cout << "Memoria liberada correctamente. Hasta luego." << endl;
}

void MenuConsola::mostrarMenu() const {
    linea('=');
    cout << "   SISTEMA DE GESTION DE REFUGIO - " << refugio.getNombre() << endl;
    linea('=');
    cout << " 1. Registrar perro" << endl;
    cout << " 2. Registrar gato" << endl;
    cout << " 3. Listar todos los animales" << endl;
    cout << " 4. Listar animales disponibles" << endl;
    cout << " 5. Registrar adoptante" << endl;
    cout << " 6. Ver ficha de un animal" << endl;
    cout << " 7. Crear solicitud de adopcion" << endl;
    cout << " 8. Confirmar solicitud" << endl;
    cout << " 9. Cancelar solicitud" << endl;
    cout << "10. Devolver animal al refugio" << endl;
    cout << "11. Ver adoptantes y solicitudes" << endl;
    cout << "12. Historial de adopciones" << endl;
    cout << "13. Informes y estadisticas" << endl;
    cout << "14. Pruebas (copia profunda, operadores y excepciones)" << endl;
    cout << " 0. Salir" << endl;
    linea('-');
    cout << "Animales: " << refugio.getCantidadAnimales()
         << " | Disponibles: " << refugio.contarDisponibles()
         << " | Adoptantes: " << refugio.getCantidadAdoptantes()
         << " | Solicitudes: " << refugio.getCantidadSolicitudes() << endl;
    linea('-');
}

bool MenuConsola::pasaElFiltro(const Animal& animal, Filtro filtro) {
    switch (filtro) {
        case Filtro::NO_DISPONIBLES: return !animal;
        case Filtro::DISPONIBLES:    return animal.estaDisponible();
        case Filtro::TODOS:          break;
    }
    return true;
}

int MenuConsola::elegirAnimal(const string& titulo, Filtro filtro) const {
    int total = refugio.getCantidadAnimales();

    int cuantos = 0;
    for (int i = 0; i < total; i++)
        if (pasaElFiltro(refugio[i], filtro)) cuantos++;

    if (cuantos == 0) {
        if (filtro == Filtro::DISPONIBLES)
            throw NadaParaElegirException("No hay animales disponibles para adoptar "
                                          "en este momento.");
        if (filtro == Filtro::NO_DISPONIBLES)
            throw NadaParaElegirException("No hay animales reservados ni adoptados, "
                                          "asi que no hay nada para devolver.");
        throw NadaParaElegirException("El refugio todavia no tiene animales cargados. "
                                      "Se cargan con las opciones 1 y 2.");
    }

    cout << titulo << endl;
    int numeroDeLista = 0;
    for (int i = 0; i < total; i++) {
        if (!pasaElFiltro(refugio[i], filtro)) continue;
        numeroDeLista++;
        cout << "  " << numeroDeLista << ") ";
        refugio[i].mostrarInfo();
    }

    int elegido = leerEntero("Elija un numero de la lista: ", 1, cuantos);

    numeroDeLista = 0;
    for (int i = 0; i < total; i++) {
        if (!pasaElFiltro(refugio[i], filtro)) continue;
        numeroDeLista++;
        if (numeroDeLista == elegido) return refugio[i].getId();
    }

    throw DatoInvalidoException("no se pudo identificar el animal elegido.");
}

int MenuConsola::elegirAdoptante(const string& titulo) const {
    const Coleccion<Adoptante>& lista = refugio.getAdoptantes();

    if (lista.vacia())
        throw NadaParaElegirException("Todavia no hay adoptantes registrados. "
                                      "Se registran con la opcion 5.");

    cout << titulo << endl;
    for (int i = 0; i < lista.cantidad(); i++) {
        cout << "  " << (i + 1) << ") ";
        lista[i].mostrarInfo();
    }

    int elegido = leerEntero("Elija un numero de la lista: ", 1, lista.cantidad());
    return lista[elegido - 1].getId();
}

int MenuConsola::elegirSolicitudPendiente(const string& titulo) const {
    const Coleccion<SolicitudAdopcion>& lista = refugio.getSolicitudes();

    int cuantas = 0;
    for (int i = 0; i < lista.cantidad(); i++)
        if (lista[i].estaPendiente()) cuantas++;

    if (cuantas == 0)
        throw NadaParaElegirException("No hay solicitudes pendientes. "
                                      "Se crean con la opcion 7.");

    cout << titulo << endl;
    int numeroDeLista = 0;
    for (int i = 0; i < lista.cantidad(); i++) {
        if (!lista[i].estaPendiente()) continue;
        numeroDeLista++;
        mostrarSolicitud(lista[i], numeroDeLista);
    }

    int elegida = leerEntero("Elija un numero de la lista: ", 1, cuantas);

    numeroDeLista = 0;
    for (int i = 0; i < lista.cantidad(); i++) {
        if (!lista[i].estaPendiente()) continue;
        numeroDeLista++;
        if (numeroDeLista == elegida) return lista[i].getId();
    }

    throw DatoInvalidoException("no se pudo identificar la solicitud elegida.");
}

string MenuConsola::nombreDeAdoptante(int idAdoptante) const {
    const Coleccion<Adoptante>& lista = refugio.getAdoptantes();
    for (int i = 0; i < lista.cantidad(); i++)
        if (lista[i].getId() == idAdoptante)
            return lista[i].getNombre();
    return "(adoptante dado de baja)";
}

void MenuConsola::mostrarSolicitud(const SolicitudAdopcion& solicitud,
                                   int numeroDeLista) const {
    cout << "  " << numeroDeLista << ") Solicitud N " << solicitud.getId()
         << " | " << nombreDeAdoptante(solicitud.getIdAdoptante())
         << " quiere adoptar a ";

    try {
        Animal* animal = refugio(solicitud.getIdAnimal());
        cout << animal->getNombre() << " (" << animal->tipo() << ")";
    } catch (const NoEncontradoException&) {
        cout << "(animal dado de baja)";
    }

    cout << " | Estado: " << estadoSolicitudATexto(solicitud.getEstado()) << endl;
}

void MenuConsola::registrarPerro() {
    cout << "--- Registrar perro ---" << endl;
    int id        = refugio.nuevoIdAnimal();
    string nombre = leerTexto("Nombre: ");
    int edad      = leerEntero("Edad (anios): ", 0, 30);
    string salud  = leerTexto("Estado de salud: ");
    string raza   = leerTexto("Raza: ");

    cout << "Tamanio -> 1) Pequenio  2) Mediano  3) Grande" << endl;
    int t = leerEntero("Opcion: ", 1, 3);
    string tamanio = (t == 1) ? "Pequenio" : (t == 2) ? "Mediano" : "Grande";

    refugio.agregarAnimal(new Perro(id, nombre, edad, salud, raza, tamanio));
    cout << "Perro registrado correctamente (quedo con el numero "
         << id << ")." << endl;
}

void MenuConsola::registrarGato() {
    cout << "--- Registrar gato ---" << endl;
    int id        = refugio.nuevoIdAnimal();
    string nombre = leerTexto("Nombre: ");
    int edad      = leerEntero("Edad (anios): ", 0, 30);
    string salud  = leerTexto("Estado de salud: ");
    string color  = leerTexto("Color de pelaje: ");
    bool interior = leerSiNo("Vive en interior? (SI/NO): ");

    refugio.agregarAnimal(new Gato(id, nombre, edad, salud, color, interior));
    cout << "Gato registrado correctamente (quedo con el numero "
         << id << ")." << endl;
}

void MenuConsola::registrarAdoptante() {
    cout << "--- Registrar adoptante ---" << endl;
    int id           = refugio.nuevoIdAdoptante();
    string nombre    = leerTexto("Nombre y apellido: ");
    string contacto  = leerTexto("Contacto (telefono o mail): ");
    string vivienda  = leerTexto("Tipo de vivienda: ");

    refugio.registrarAdoptante(Adoptante(id, nombre, contacto, vivienda));
    cout << "Adoptante registrado correctamente (quedo con el numero "
         << id << ")." << endl;
}

void MenuConsola::verFichaDeAnimal() const {
    cout << "--- Ver ficha de un animal ---" << endl;
    int id = elegirAnimal("Animales del refugio:", Filtro::TODOS);

    Animal* encontrado = refugio(id);
    cout << endl << "Ficha del animal elegido:" << endl << "  ";
    encontrado->mostrarInfo();

    if (!(*encontrado))
        cout << "Atencion: este animal NO esta disponible para adopcion." << endl;
    else
        cout << "Este animal esta disponible para adopcion." << endl;
}

void MenuConsola::crearSolicitud() {
    cout << "--- Nueva solicitud de adopcion ---" << endl;
    int idAdoptante = elegirAdoptante("Quien quiere adoptar?");
    cout << endl;
    int idAnimal    = elegirAnimal("A que animal?", Filtro::DISPONIBLES);

    int nro = refugio.crearSolicitud(idAdoptante, idAnimal);
    cout << endl << "Solicitud N " << nro << " creada en estado PENDIENTE." << endl;
    cout << "El animal quedo reservado (EN_PROCESO)." << endl;

    if (leerSiNo("Desea confirmar la adopcion ahora? (SI/NO): ")) {
        refugio.confirmarSolicitud(nro);
        cout << "Adopcion confirmada. El animal pasa a ADOPTADO." << endl;
    } else {
        cout << "La solicitud queda PENDIENTE y el animal sigue reservado." << endl;
    }
}

void MenuConsola::confirmarSolicitud() {
    cout << "--- Confirmar solicitud ---" << endl;
    int nro = elegirSolicitudPendiente("Solicitudes pendientes:");
    refugio.confirmarSolicitud(nro);
    cout << endl << "Solicitud CONFIRMADA. El animal quedo ADOPTADO." << endl;
}

void MenuConsola::cancelarSolicitud() {
    cout << "--- Cancelar solicitud ---" << endl;
    int nro = elegirSolicitudPendiente("Solicitudes pendientes:");
    refugio.cancelarSolicitud(nro);
    cout << endl << "Solicitud CANCELADA. El animal vuelve a estar DISPONIBLE." << endl;
}

void MenuConsola::devolverAnimal() {
    cout << "--- Devolver animal al refugio ---" << endl;
    int id = elegirAnimal("Animales reservados o adoptados:", Filtro::NO_DISPONIBLES);
    refugio.devolverAnimal(id);
    cout << endl << "El animal volvio al estado DISPONIBLE." << endl;
}

void MenuConsola::verAdoptantesYSolicitudes() const {
    cout << "--- Adoptantes registrados ---" << endl;
    mostrarColeccion(refugio.getAdoptantes());

    cout << endl << "--- Solicitudes de adopcion ---" << endl;
    const Coleccion<SolicitudAdopcion>& lista = refugio.getSolicitudes();
    if (lista.vacia()) {
        cout << "(Todavia no hay solicitudes cargadas)" << endl;
        return;
    }
    for (int i = 0; i < lista.cantidad(); i++)
        mostrarSolicitud(lista[i], i + 1);
}

void MenuConsola::pruebasDeIntegridad() {
    if (refugio.getCantidadAnimales() == 0)
        throw DatoInvalidoException("cargue al menos un animal antes de probar.");

    linea('=');
    cout << "PRUEBA 1: recorrido con aritmetica de punteros (Fase 2)" << endl;
    linea('-');
    refugio.listarConAritmeticaDePunteros();

    linea('=');
    cout << "PRUEBA 2: operadores [] , () , == y ! (Fase 3)" << endl;
    linea('-');
    Animal& primero = refugio[0];
    cout << "refugio[0] -> "; primero.mostrarInfo();

    Animal* porId = refugio(primero.getId());
    cout << "refugio(" << primero.getId() << ") -> "; porId->mostrarInfo();

    cout << "refugio[0] == refugio(" << primero.getId() << ") ? "
         << ((primero == *porId) ? "SI (mismo ID)" : "NO") << endl;
    cout << "!refugio[0] (no esta disponible) ? "
         << (!primero ? "SI" : "NO") << endl;

    linea('=');
    cout << "PRUEBA 3: COPIA PROFUNDA y liberacion de memoria (Fases 2 y 3)" << endl;
    linea('-');
    cout << "Animales vivos ANTES de copiar: " << Animal::animalesVivos << endl;
    {
        Refugio copia = refugio;
        copia.setNombre("COPIA de " + refugio.getNombre());

        cout << "Animales vivos DESPUES de copiar: " << Animal::animalesVivos
             << "  (se duplicaron)" << endl;

        cout << "Direccion del animal 0 en el original: " << &refugio[0] << endl;
        cout << "Direccion del animal 0 en la copia ..: " << &copia[0] << endl;
        cout << "-> Son direcciones DISTINTAS: la copia es profunda." << endl << endl;

        copia[0].setEstadoSalud("MODIFICADO EN LA COPIA");
        cout << "Original: "; refugio[0].mostrarInfo();
        cout << "Copia ..: "; copia[0].mostrarInfo();
        cout << "-> El original quedo intacto." << endl;

        cout << endl << "Saliendo del bloque: se destruye la copia..." << endl;
    }
    cout << "Animales vivos DESPUES de destruir la copia: "
         << Animal::animalesVivos << "  (volvio al valor original)" << endl;

    linea('=');
    cout << "PRUEBA 4: excepciones controladas (Fase 5)" << endl;
    linea('-');

    try {
        cout << "a) Acceso a la posicion 999 del catalogo..." << endl;
        refugio[999].mostrarInfo();
    } catch (const IndiceInvalidoException& e) {
        cout << "   Capturada -> " << e.what() << endl;
    }

    try {
        cout << "b) Busqueda de un animal con ID 99999..." << endl;
        refugio(99999);
    } catch (const NoEncontradoException& e) {
        cout << "   Capturada -> " << e.what() << endl;
    }

    try {
        cout << "c) Alta de un animal con un ID ya existente..." << endl;
        refugio.agregarAnimal(new Perro(refugio[0].getId(), "Clon", 1, "Sano", "X", "Mediano"));
    } catch (const IdRepetidoException& e) {
        cout << "   Capturada -> " << e.what() << endl;
    }

    linea('=');
    cout << "PRUEBA 5: la FUNCION GENERICA con dos tipos distintos (Fase 5)" << endl;
    linea('-');

    cout << "mostrarColeccion<Adoptante>:" << endl;
    mostrarColeccion(refugio.getAdoptantes());

    cout << "mostrarColeccion<SolicitudAdopcion>:" << endl;
    mostrarColeccion(refugio.getSolicitudes());
    cout << "-> Una sola funcion, dos instanciaciones generadas por el compilador."
         << endl;

    linea('=');
    cout << "Fin de las pruebas. El menu sigue funcionando normalmente." << endl;
}
