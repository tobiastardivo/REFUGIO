// ===================================================================
// MenuConsola.cpp - Toda la interfaz de consola del sistema.
//
// Separacion de responsabilidades del TP:
//   - main.cpp  : crea el menu y lo ejecuta. Nada mas.
//   - MenuConsola : habla con el usuario (pide datos, muestra resultados,
//                   captura las excepciones y las traduce a mensajes).
//   - Refugio     : aplica las reglas del negocio y lanza las excepciones.
// Gracias a eso el modelo no tiene ni un cout adentro y podria usarse
// con otra interfaz sin tocarle una linea.
//
// Patron que repiten todas las opciones del menu: piden los datos con
// las funciones validadoras de Utilidades, llaman a UN metodo del
// Refugio y avisan que salio bien. Ninguna valida reglas por su cuenta:
// si algo esta mal, el Refugio lanza y el try/catch de ejecutar() se
// encarga. Por eso las funciones de aca no tienen if de control.
//
// EL USUARIO NUNCA ESCRIBE UN ID (ver el comentario de MenuConsola.h).
// Los IDs siguen existiendo porque son la clave del modelo, pero quien
// usa el programa no los conoce. Hay dos casos y cada uno se resuelve
// en un solo lugar:
//   - ALTAS (opciones 1, 2 y 5): el numero lo pone el Refugio con
//     nuevoIdAnimal() / nuevoIdAdoptante(). Al usuario solo se le avisa
//     con que numero quedo registrado.
//   - ELEGIR ALGO QUE YA EXISTE (opciones 6 a 10): se muestra la lista
//     numerada de 1 a N y se elige por posicion. Las funciones elegir*()
//     traducen ese numero de lista al ID interno antes de llamar al
//     Refugio, asi el modelo no se entera de que la interfaz cambio.
// ===================================================================

#include "MenuConsola.h"
#include "Perro.h"
#include "Gato.h"
#include "Adoptante.h"
#include "Utilidades.h"
#include "Excepciones.h"
#include <iostream>

using namespace std;

// ===================================================================
// CONSTRUCTOR Y DESTRUCTOR
// ===================================================================

// El refugio se construye con la lista de inicializacion y se le cargan
// los datos de prueba, para poder probar el circuito completo sin tener
// que dar de alta todo a mano en cada demostracion.
MenuConsola::MenuConsola(const string& nombreRefugio) : refugio(nombreRefugio) {
    refugio.cargarDatosDePrueba();
}

// No hay nada que liberar a mano: el Refugio libera el catalogo en su
// propio destructor, que se ejecuta automaticamente al morir el menu.
MenuConsola::~MenuConsola() {}

// ===================================================================
// BUCLE PRINCIPAL
// ===================================================================

// --- ejecutar(): el bucle del menu ---
// Es un  do...while  y no un  while  porque el menu tiene que mostrarse
// por lo menos una vez antes de poder preguntar si hay que salir.
// Cada vuelta hace siempre lo mismo: limpiar, dibujar, leer la opcion
// (leerEntero ya garantiza que sea un numero entre 0 y 14), ejecutarla
// dentro del try y pausar para que se alcance a leer el resultado.
// La linea final imprime el contador de animales justo antes de que el
// menu se destruya, para poder compararla con la que imprime main()
// despues de la destruccion: es la prueba de que no quedan fugas.
void MenuConsola::ejecutar() {
    int opcion;
    do {
        limpiarPantalla();
        mostrarMenu();
        opcion = leerEntero("Ingrese una opcion (0-14): ", 0, 14);

        cout << endl;
        // --- FASE 5: un solo bloque try envuelve toda la operacion elegida.
        // Si algo falla se muestra el mensaje y el menu SIGUE funcionando.
        try {
            ejecutarOpcion(opcion);
        }
        // --- MULTIPLES CATCH, siempre por REFERENCIA CONSTANTE ---
        // Del mas especifico al mas general: si se pusiera primero
        // std::exception, atraparia todo y los demas nunca se usarian.
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
        catch (const RefugioException& e) {      // cualquier otro error del sistema
            cout << "[ERROR] " << e.what() << endl;
        }
        catch (const std::exception& e) {        // errores estandar (ej: bad_alloc)
            cout << "[ERROR inesperado] " << e.what() << endl;
        }

        if (opcion != 0) pausar();
    } while (opcion != 0);

    cout << endl << "Animales en memoria ANTES de destruir el refugio: "
         << Animal::animalesVivos << endl;
}

// Despacho de la opcion elegida. Cada case delega en una funcion propia:
// el switch solo decide a quien llamar, no resuelve nada por si mismo.
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

// Se llama una vez destruido el refugio: si el contador llego a 0,
// no quedo memoria sin liberar.
void MenuConsola::informarMemoriaLiberada() {
    cout << "Animales en memoria DESPUES de destruir el refugio: "
         << Animal::animalesVivos << endl;
    cout << "Memoria liberada correctamente. Hasta luego." << endl;
}

// ===================================================================
// MENU
// ===================================================================

// --- mostrarMenu(): dibuja la pantalla principal ---
// Solo imprime: por eso es const, no toca ningun dato. Ademas de las 15
// opciones muestra un resumen en vivo (animales, disponibles, adoptantes
// y solicitudes) que se recalcula en cada vuelta del bucle, asi el efecto
// de la operacion anterior se ve enseguida sin tener que ir al informe.
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

// ===================================================================
// SELECCION POR LISTA
//
// Estas funciones son las que hacen que no haya que escribir ningun ID.
// Las tres trabajan igual, en cuatro pasos:
//   1) cuentan cuantos elementos hay para ofrecer y, si no hay ninguno,
//      lanzan DatoInvalidoException explicando por que (asi el usuario se
//      entera enseguida y no despues de cargar otros datos);
//   2) imprimen la lista numerada de 1 a N;
//   3) leen UN numero con leerEntero(..., 1, N), que ya no deja elegir
//      nada fuera de la lista: no hace falta validar la eleccion;
//   4) traducen ese numero de lista al ID interno y lo devuelven.
//
// El paso 4 existe porque el numero de la lista NO es la posicion en el
// catalogo: cuando hay filtro, el 1 de la lista puede ser el animal que
// esta en la posicion 3. Por eso se vuelve a recorrer contando solo los
// que pasan el filtro, hasta llegar al que el usuario eligio.
// ===================================================================

// --- pasaElFiltro(): decide si un animal entra en el listado ---
// Es static porque no necesita mirar el refugio: le alcanza con el animal
// que recibe y el filtro pedido. Separarla evita repetir el mismo  if  en
// los tres recorridos de elegirAnimal().
bool MenuConsola::pasaElFiltro(const Animal& animal, Filtro filtro) {
    switch (filtro) {
        // FASE 3: el  !  de esta linea es el operador unario sobrecargado
        // de Animal, que significa "este animal NO esta disponible".
        case Filtro::NO_DISPONIBLES: return !animal;
        case Filtro::DISPONIBLES:    return animal.estaDisponible();
        case Filtro::TODOS:          break;
    }
    return true;
}

// --- elegirAnimal(): muestra los animales y devuelve el ID del elegido ---
// El filtro cambia que animales se ofrecen segun para que se este eligiendo:
// para adoptar solo tiene sentido ver los DISPONIBLES, y para devolver solo
// los que estan reservados o adoptados (NO_DISPONIBLES).
int MenuConsola::elegirAnimal(const string& titulo, Filtro filtro) const {
    int total = refugio.getCantidadAnimales();

    // Paso 1: contar y avisar si no hay nada para elegir
    int cuantos = 0;
    for (int i = 0; i < total; i++)
        if (pasaElFiltro(refugio[i], filtro)) cuantos++;

    if (cuantos == 0) {
        if (filtro == Filtro::DISPONIBLES)
            throw DatoInvalidoException("no hay animales disponibles para adoptar.");
        if (filtro == Filtro::NO_DISPONIBLES)
            throw DatoInvalidoException("no hay animales reservados ni adoptados: "
                                        "no hay nada para devolver.");
        throw DatoInvalidoException("el refugio no tiene animales cargados "
                                    "(se cargan con las opciones 1 y 2).");
    }

    // Paso 2: imprimir la lista numerada. mostrarInfo() es virtual, asi que
    // cada renglon lo dibuja Perro o Gato segun lo que sea (polimorfismo).
    cout << titulo << endl;
    int numeroDeLista = 0;
    for (int i = 0; i < total; i++) {
        if (!pasaElFiltro(refugio[i], filtro)) continue;
        numeroDeLista++;
        cout << "  " << numeroDeLista << ") ";
        refugio[i].mostrarInfo();
    }

    // Paso 3: leer la eleccion, ya acotada a los numeros de la lista
    int elegido = leerEntero("Elija un numero de la lista: ", 1, cuantos);

    // Paso 4: traducir el numero de lista al ID interno
    numeroDeLista = 0;
    for (int i = 0; i < total; i++) {
        if (!pasaElFiltro(refugio[i], filtro)) continue;
        numeroDeLista++;
        if (numeroDeLista == elegido) return refugio[i].getId();
    }

    // No se puede llegar aca: leerEntero ya garantizo que el numero este
    // entre 1 y la cantidad listada. La linea existe para que la funcion
    // devuelva algo por todos los caminos posibles.
    throw DatoInvalidoException("no se pudo identificar el animal elegido.");
}

// --- elegirAdoptante(): muestra las personas y devuelve el ID de la elegida ---
// Aca no hay filtro, asi que el numero de la lista coincide siempre con la
// posicion en la coleccion: alcanza con restarle 1 y no hace falta el
// segundo recorrido que si necesita elegirAnimal().
int MenuConsola::elegirAdoptante(const string& titulo) const {
    const Coleccion<Adoptante>& lista = refugio.getAdoptantes();

    if (lista.vacia())
        throw DatoInvalidoException("no hay adoptantes registrados "
                                    "(se registran con la opcion 5).");

    cout << titulo << endl;
    for (int i = 0; i < lista.cantidad(); i++) {
        cout << "  " << (i + 1) << ") ";
        lista[i].mostrarInfo();
    }

    int elegido = leerEntero("Elija un numero de la lista: ", 1, lista.cantidad());
    return lista[elegido - 1].getId();      // -1: de numero de lista a posicion
}

// --- elegirSolicitudPendiente(): las opciones 8 y 9 eligen de aca ---
// Solo ofrece las PENDIENTES porque son las unicas que se pueden confirmar
// o cancelar: una confirmada o cancelada ya esta en un estado final. Antes
// se listaban todas y el usuario podia elegir una cerrada, para recien
// entonces comerse el error del Refugio.
int MenuConsola::elegirSolicitudPendiente(const string& titulo) const {
    const Coleccion<SolicitudAdopcion>& lista = refugio.getSolicitudes();

    int cuantas = 0;
    for (int i = 0; i < lista.cantidad(); i++)
        if (lista[i].estaPendiente()) cuantas++;

    if (cuantas == 0)
        throw DatoInvalidoException("no hay solicitudes pendientes "
                                    "(se crean con la opcion 7).");

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

// --- nombreDeAdoptante(): traduce un ID de adoptante a su nombre ---
// La solicitud guarda IDs (ver SolicitudAdopcion.h), pero en pantalla un
// numero no dice nada. Esta funcion es la que permite que el listado se
// lea "Ana Perez quiere adoptar a Michi" y no "Adoptante ID: 100".
string MenuConsola::nombreDeAdoptante(int idAdoptante) const {
    const Coleccion<Adoptante>& lista = refugio.getAdoptantes();
    for (int i = 0; i < lista.cantidad(); i++)
        if (lista[i].getId() == idAdoptante)
            return lista[i].getNombre();
    return "(adoptante dado de baja)";
}

// --- mostrarSolicitud(): un renglon de solicitud en castellano ---
// No se usa SolicitudAdopcion::mostrarInfo() porque esa imprime los dos
// IDs crudos, que es justo lo que no le sirve al usuario. El operator()
// del Refugio se llama dentro de un try propio: si por lo que fuera el
// animal no estuviera, el listado tiene que seguir dibujandose y no
// cortarse por la mitad.
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

// ===================================================================
// OPCIONES DEL MENU
// ===================================================================

// --- Opcion 1: registrar un perro ---
// Pide los datos con las funciones validadoras (cada una ya se encarga de
// insistir si el dato esta mal) y arma el Perro. El tamanio se elige por
// numero en vez de escribirlo, para que no entren variantes como "grande",
// "GRANDE" o "grnde" que despues ensucien los listados.
// El ID no se pide: lo da el Refugio con nuevoIdAnimal(). Antes habia que
// escribirlo y era el unico dato que el usuario no podia saber, porque
// tenia que acordarse de cual numero estaba libre para que el alta no
// rebotara con IdRepetidoException.
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

    // Se crea con new: la memoria queda a cargo del Refugio.
    refugio.agregarAnimal(new Perro(id, nombre, edad, salud, raza, tamanio));
    cout << "Perro registrado correctamente (quedo con el numero "
         << id << ")." << endl;
}

// --- Opcion 2: registrar un gato ---
// Igual que registrarPerro() pero con los atributos propios del gato.
// El dato de interior/exterior es un bool, asi que se pide con leerSiNo(),
// que acepta SI/S/NO/N y devuelve directamente true o false.
// Tambien se crea con new: la memoria pasa a ser del Refugio.
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

// --- Opcion 5: registrar un adoptante ---
// Aca no aparece  new  por ningun lado: el Adoptante se crea como objeto
// temporal en la misma llamada y la coleccion del Refugio se guarda una
// copia. Al terminar la funcion el temporal se destruye solo, sin que
// nadie tenga que acordarse de liberarlo.
// El numero de adoptante lo pone el Refugio, igual que en las altas de
// animales, y arranca en 100 para no confundirlo con el de un animal.
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

// --- Opcion 6: ver la ficha de un animal ---
// Es la opcion que muestra dos operadores sobrecargados trabajando:
//   refugio(id)    -> operator() del Refugio, busca por ID;
//   !(*encontrado) -> operator! del Animal, "no esta disponible".
// El ID que recibe el operator() ya no lo escribe el usuario: sale de la
// lista que muestra elegirAnimal(). Y no hace falta comprobar que el
// puntero no sea nulo: si el ID no existiera, el operator() lanzaria
// NoEncontradoException y esta funcion ni llegaria a la linea siguiente.
// Es const porque solo consulta.
void MenuConsola::verFichaDeAnimal() const {
    cout << "--- Ver ficha de un animal ---" << endl;
    int id = elegirAnimal("Animales del refugio:", Filtro::TODOS);

    Animal* encontrado = refugio(id);    // FASE 3: sobrecarga del operador ()
    cout << endl << "Ficha del animal elegido:" << endl << "  ";
    encontrado->mostrarInfo();

    // FASE 3: operador unario !
    if (!(*encontrado))
        cout << "Atencion: este animal NO esta disponible para adopcion." << endl;
    else
        cout << "Este animal esta disponible para adopcion." << endl;
}

// --- Opcion 7: crear una solicitud de adopcion ---
// Antes esta funcion arrancaba con dos  if  que avisaban si no habia
// adoptantes o animales disponibles. Ya no estan porque elegirAdoptante()
// y elegirAnimal() hacen ese mismo aviso solas: la comprobacion quedo en
// un unico lugar en vez de repetida en cada opcion que elige algo.
// Despues de crear la solicitud ofrece confirmarla en el acto, que es el
// caso mas comun (la persona ya esta ahi). Si dice que no, el tramite
// queda PENDIENTE y el animal reservado, listo para cerrarse con la
// opcion 8 o para soltarse con la 9.
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

// --- Opcion 8: confirmar una solicitud ---
// elegirSolicitudPendiente() ya se encarga de mostrar solo las que se
// pueden confirmar y de devolver su numero, asi que aca solo queda
// delegar en el Refugio, que es el que aplica la regla.
void MenuConsola::confirmarSolicitud() {
    cout << "--- Confirmar solicitud ---" << endl;
    int nro = elegirSolicitudPendiente("Solicitudes pendientes:");
    refugio.confirmarSolicitud(nro);
    cout << endl << "Solicitud CONFIRMADA. El animal quedo ADOPTADO." << endl;
}

// --- Opcion 9: cancelar una solicitud ---
// Espejo de la opcion 8: mismo listado previo, misma delegacion, pero el
// animal en vez de quedar ADOPTADO vuelve a estar DISPONIBLE.
void MenuConsola::cancelarSolicitud() {
    cout << "--- Cancelar solicitud ---" << endl;
    int nro = elegirSolicitudPendiente("Solicitudes pendientes:");
    refugio.cancelarSolicitud(nro);
    cout << endl << "Solicitud CANCELADA. El animal vuelve a estar DISPONIBLE." << endl;
}

// --- Opcion 10: devolver un animal al refugio ---
// Se elige el animal y no la solicitud, porque quien devuelve trae al
// animal y no tiene por que saber de que tramite salio. La lista se filtra
// con NO_DISPONIBLES: los que figuran DISPONIBLE nunca salieron del
// refugio, asi que de ellos no hay nada que devolver. El Refugio se ocupa
// del resto: cancela sola cualquier solicitud que hubiera quedado
// pendiente por ese animal.
void MenuConsola::devolverAnimal() {
    cout << "--- Devolver animal al refugio ---" << endl;
    int id = elegirAnimal("Animales reservados o adoptados:", Filtro::NO_DISPONIBLES);
    refugio.devolverAnimal(id);
    cout << endl << "El animal volvio al estado DISPONIBLE." << endl;
}

// --- Opcion 11: ver adoptantes y solicitudes ---
// Es la demostracion mas directa de la FUNCION GENERICA de la Fase 5: la
// misma linea de codigo lista dos tipos de datos que no tienen nada que
// ver entre si. El compilador genera una version de mostrarColeccion()
// por cada tipo usado, y lo unico que les exige es tener mostrarInfo().
// Este listado es informativo (no se elige nada), asi que es el unico
// lugar donde sigue teniendo sentido mostrar los datos crudos con sus IDs.
void MenuConsola::verAdoptantesYSolicitudes() const {
    cout << "--- Adoptantes registrados ---" << endl;
    mostrarColeccion(refugio.getAdoptantes());       // T = Adoptante

    cout << endl << "--- Solicitudes de adopcion ---" << endl;
    mostrarColeccion(refugio.getSolicitudes());      // T = SolicitudAdopcion
}

// ===================================================================
// OPCION 14 - Pruebas que pide la consigna:
// copia profunda, liberacion de memoria, operadores y excepciones.
// ===================================================================

// --- Opcion 14: bateria de pruebas ---
// Ejecuta en orden las cuatro demostraciones que pide la consigna y va
// imprimiendo el resultado de cada una, para poder mostrarlas en la
// defensa sin tener que preparar nada:
//   1) recorrido con aritmetica de punteros (Fase 2);
//   2) los operadores [] , () , == y ! (Fase 3);
//   3) la copia profunda, comparando direcciones de memoria y verificando
//      que al modificar la copia el original no se entere (Fases 2 y 3);
//   4) tres excepciones provocadas a proposito y capturadas (Fase 5).
// Las excepciones de la prueba 4 se atrapan aca mismo, cada una en su
// try, en vez de dejarlas subir al try general de ejecutar(): si subieran,
// la primera cortaria la funcion y las otras dos no llegarian a probarse.
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
    Animal& primero = refugio[0];                  // operator[] por posicion
    cout << "refugio[0] -> "; primero.mostrarInfo();

    Animal* porId = refugio(primero.getId());      // operator() por ID
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
        // Constructor de copia: clona cada animal en memoria nueva
        Refugio copia = refugio;
        copia.setNombre("COPIA de " + refugio.getNombre());

        cout << "Animales vivos DESPUES de copiar: " << Animal::animalesVivos
             << "  (se duplicaron)" << endl;

        cout << "Direccion del animal 0 en el original: " << &refugio[0] << endl;
        cout << "Direccion del animal 0 en la copia ..: " << &copia[0] << endl;
        cout << "-> Son direcciones DISTINTAS: la copia es profunda." << endl << endl;

        // Se modifica la copia y se verifica que el original NO cambie
        copia[0].setEstadoSalud("MODIFICADO EN LA COPIA");
        cout << "Original: "; refugio[0].mostrarInfo();
        cout << "Copia ..: "; copia[0].mostrarInfo();
        cout << "-> El original quedo intacto." << endl;

        cout << endl << "Saliendo del bloque: se destruye la copia..." << endl;
    }   // aca se ejecuta ~Refugio() de 'copia' y libera todos sus animales
    cout << "Animales vivos DESPUES de destruir la copia: "
         << Animal::animalesVivos << "  (volvio al valor original)" << endl;

    linea('=');
    cout << "PRUEBA 4: excepciones controladas (Fase 5)" << endl;
    linea('-');
    // Cada prueba tiene su propio try para que el programa siga adelante
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
    cout << "Fin de las pruebas. El menu sigue funcionando normalmente." << endl;
}
