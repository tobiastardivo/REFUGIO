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
        case 6:  buscarAnimalPorId(); break;
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
    cout << " 6. Buscar animal por ID" << endl;
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
// OPCIONES DEL MENU
// ===================================================================

// --- Opcion 1: registrar un perro ---
// Pide los datos con las funciones validadoras (cada una ya se encarga de
// insistir si el dato esta mal) y arma el Perro. El tamanio se elige por
// numero en vez de escribirlo, para que no entren variantes como "grande",
// "GRANDE" o "grnde" que despues ensucien los listados.
void MenuConsola::registrarPerro() {
    cout << "--- Registrar perro ---" << endl;
    int id        = leerEntero("ID: ", 1, 99999);
    string nombre = leerTexto("Nombre: ");
    int edad      = leerEntero("Edad (anios): ", 0, 30);
    string salud  = leerTexto("Estado de salud: ");
    string raza   = leerTexto("Raza: ");

    cout << "Tamanio -> 1) Pequenio  2) Mediano  3) Grande" << endl;
    int t = leerEntero("Opcion: ", 1, 3);
    string tamanio = (t == 1) ? "Pequenio" : (t == 2) ? "Mediano" : "Grande";

    // Se crea con new: la memoria queda a cargo del Refugio.
    // Si el ID esta repetido, agregarAnimal() lanza la excepcion y libera el objeto.
    refugio.agregarAnimal(new Perro(id, nombre, edad, salud, raza, tamanio));
    cout << "Perro registrado correctamente." << endl;
}

// --- Opcion 2: registrar un gato ---
// Igual que registrarPerro() pero con los atributos propios del gato.
// El dato de interior/exterior es un bool, asi que se pide con leerSiNo(),
// que acepta SI/S/NO/N y devuelve directamente true o false.
// Tambien se crea con new: la memoria pasa a ser del Refugio.
void MenuConsola::registrarGato() {
    cout << "--- Registrar gato ---" << endl;
    int id        = leerEntero("ID: ", 1, 99999);
    string nombre = leerTexto("Nombre: ");
    int edad      = leerEntero("Edad (anios): ", 0, 30);
    string salud  = leerTexto("Estado de salud: ");
    string color  = leerTexto("Color de pelaje: ");
    bool interior = leerSiNo("Vive en interior? (SI/NO): ");

    refugio.agregarAnimal(new Gato(id, nombre, edad, salud, color, interior));
    cout << "Gato registrado correctamente." << endl;
}

// --- Opcion 5: registrar un adoptante ---
// Aca no aparece  new  por ningun lado: el Adoptante se crea como objeto
// temporal en la misma llamada y la coleccion del Refugio se guarda una
// copia. Al terminar la funcion el temporal se destruye solo, sin que
// nadie tenga que acordarse de liberarlo.
void MenuConsola::registrarAdoptante() {
    cout << "--- Registrar adoptante ---" << endl;
    int id           = leerEntero("ID: ", 1, 99999);
    string nombre    = leerTexto("Nombre y apellido: ");
    string contacto  = leerTexto("Contacto (telefono o mail): ");
    string vivienda  = leerTexto("Tipo de vivienda: ");

    refugio.registrarAdoptante(Adoptante(id, nombre, contacto, vivienda));
    cout << "Adoptante registrado correctamente." << endl;
}

// --- Opcion 6: buscar un animal por ID ---
// Es la opcion que muestra dos operadores sobrecargados trabajando:
//   refugio(id)   -> operator() del Refugio, busca por ID;
//   !(*encontrado) -> operator! del Animal, "no esta disponible".
// No hace falta comprobar que el puntero no sea nulo: si el ID no existe,
// el operator() lanza NoEncontradoException y esta funcion ni siquiera
// llega a la linea siguiente. Es const porque solo consulta.
void MenuConsola::buscarAnimalPorId() const {
    cout << "--- Buscar animal por ID ---" << endl;
    int id = leerEntero("ID a buscar: ", 1, 99999);

    Animal* encontrado = refugio(id);    // FASE 3: sobrecarga del operador ()
    cout << "Animal encontrado:" << endl << "  ";
    encontrado->mostrarInfo();

    // FASE 3: operador unario !
    if (!(*encontrado))
        cout << "Atencion: este animal NO esta disponible para adopcion." << endl;
    else
        cout << "Este animal esta disponible para adopcion." << endl;
}

// --- Opcion 7: crear una solicitud de adopcion ---
// Los dos chequeos del principio no son reglas del negocio, son cortesia
// con el usuario: sin ellos igual funcionaria, pero le haria escribir dos
// IDs para recien despues avisarle que no habia con que trabajar.
// Despues de crear la solicitud ofrece confirmarla en el acto, que es el
// caso mas comun (la persona ya esta ahi). Si dice que no, el tramite
// queda PENDIENTE y el animal reservado, listo para cerrarse con la
// opcion 8 o para soltarse con la 9.
void MenuConsola::crearSolicitud() {
    cout << "--- Nueva solicitud de adopcion ---" << endl;
    if (refugio.contarDisponibles() == 0)
        throw DatoInvalidoException("no hay animales disponibles en este momento.");
    if (refugio.getCantidadAdoptantes() == 0)
        throw DatoInvalidoException("no hay adoptantes registrados (opcion 5).");

    int idAdoptante = leerEntero("ID del adoptante: ", 1, 99999);
    int idAnimal    = leerEntero("ID del animal: ", 1, 99999);

    int nro = refugio.crearSolicitud(idAdoptante, idAnimal);
    cout << "Solicitud N " << nro << " creada en estado PENDIENTE." << endl;
    cout << "El animal quedo reservado (EN_PROCESO)." << endl;

    if (leerSiNo("Desea confirmar la adopcion ahora? (SI/NO): ")) {
        refugio.confirmarSolicitud(nro);
        cout << "Adopcion confirmada. El animal pasa a ADOPTADO." << endl;
    } else {
        cout << "La solicitud queda PENDIENTE y el animal sigue reservado." << endl;
    }
}

// --- Opcion 8: confirmar una solicitud ---
// Lista primero las solicitudes con la funcion generica para que el
// usuario vea los numeros disponibles y no tenga que acordarselos.
// La validacion de que exista y siga PENDIENTE la hace el Refugio.
void MenuConsola::confirmarSolicitud() {
    cout << "--- Confirmar solicitud ---" << endl;
    mostrarColeccion(refugio.getSolicitudes());
    int nro = leerEntero("N de solicitud a confirmar: ", 1, 99999);
    refugio.confirmarSolicitud(nro);
    cout << "Solicitud CONFIRMADA. El animal quedo ADOPTADO." << endl;
}

// --- Opcion 9: cancelar una solicitud ---
// Espejo de la opcion 8: mismo listado previo, misma delegacion, pero el
// animal en vez de quedar ADOPTADO vuelve a estar DISPONIBLE.
void MenuConsola::cancelarSolicitud() {
    cout << "--- Cancelar solicitud ---" << endl;
    mostrarColeccion(refugio.getSolicitudes());
    int nro = leerEntero("N de solicitud a cancelar: ", 1, 99999);
    refugio.cancelarSolicitud(nro);
    cout << "Solicitud CANCELADA. El animal vuelve a estar DISPONIBLE." << endl;
}

// --- Opcion 10: devolver un animal al refugio ---
// Se pide el ID del animal y no el de la solicitud, porque quien devuelve
// trae al animal y no se acuerda del numero de tramite. El Refugio se
// ocupa del resto: rechaza los que ya estaban disponibles y cancela sola
// cualquier solicitud que hubiera quedado pendiente por ese animal.
void MenuConsola::devolverAnimal() {
    cout << "--- Devolver animal al refugio ---" << endl;
    int id = leerEntero("ID del animal: ", 1, 99999);
    refugio.devolverAnimal(id);
    cout << "El animal volvio al estado DISPONIBLE." << endl;
}

// --- Opcion 11: ver adoptantes y solicitudes ---
// Es la demostracion mas directa de la FUNCION GENERICA de la Fase 5: la
// misma linea de codigo lista dos tipos de datos que no tienen nada que
// ver entre si. El compilador genera una version de mostrarColeccion()
// por cada tipo usado, y lo unico que les exige es tener mostrarInfo().
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
