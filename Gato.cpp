// ===================================================================
// Gato.cpp - Implementacion de la clase derivada Gato.
//
// Gato es una de las dos clases concretas que heredan de Animal.
// Aporta los atributos propios (color de pelaje y si vive adentro o
// afuera) y da cuerpo a las tres funciones virtuales puras que Animal
// deja sin implementar: mostrarInfo(), tipo() y clonar().
// ===================================================================

#include "Gato.h"
#include <iostream>

using namespace std;

// --- Constructor por defecto ---
// Primero corre Animal() (que pone los datos base en "Sin datos" y suma
// uno al contador de animales vivos) y recien despues se inicializan los
// atributos propios del gato con valores neutros.
Gato::Gato() : Animal(), colorPelaje("Sin datos"), esDeInterior(true) {}

// --- Constructor parametrizado ---
// Los datos comunes (id, nombre, edad, salud) se los pasa al constructor
// de Animal en la lista de inicializacion; el gato solo se ocupa de lo suyo.
Gato::Gato(int id, const string& nombre, int edad, const string& estadoSalud,
           const string& colorPelaje, bool esDeInterior)
    : Animal(id, nombre, edad, estadoSalud),
      colorPelaje(colorPelaje), esDeInterior(esDeInterior) {}

// --- Constructor de copia ---
// Invoca explicitamente Animal(otro). Si no se lo invocara, el compilador
// usaria Animal() por defecto y la copia perderia id, nombre, edad y estado.
Gato::Gato(const Gato& otro)
    : Animal(otro), colorPelaje(otro.colorPelaje), esDeInterior(otro.esDeInterior) {}

// --- Destructor ---
// No libera nada porque Gato no reserva memoria dinamica propia (sus dos
// atributos se destruyen solos). Se declara igual para dejar visible el
// orden en que se ejecutan los destructores: ~Gato() y despues ~Animal().
Gato::~Gato() {}

// --- Getters ---
// Devuelven una copia del atributo. Son const porque solo leen: se pueden
// llamar sobre un Gato constante sin que el compilador proteste.
string Gato::getColorPelaje() const { return colorPelaje; }
bool Gato::getEsDeInterior() const  { return esDeInterior; }

// --- mostrarInfo(): redefinicion polimorfica ---
// Imprime la etiqueta [GATO], reutiliza mostrarDatosBase() de Animal para
// los campos comunes y agrega los propios. Al ser virtual, cuando se la
// llama a traves de un Animal* se ejecuta ESTA version y no otra
// (ligadura dinamica, resuelta en tiempo de ejecucion).
void Gato::mostrarInfo() const {
    cout << "[GATO]  ";
    mostrarDatosBase();                      // metodo protegido heredado de Animal
    cout << " | Pelaje: " << colorPelaje
         << " | Vive: " << (esDeInterior ? "Interior" : "Exterior") << endl;
}

// --- tipo(): devuelve el nombre del tipo concreto ---
// Permite preguntarle a un Animal* que es en realidad sin usar casteos.
// La usa Refugio::mostrarEstadisticas() para contar perros y gatos.
string Gato::tipo() const { return "Gato"; }

// --- clonar(): pieza clave de la COPIA PROFUNDA ---
// El Refugio guarda punteros Animal* y no sabe si cada uno es Perro o Gato.
// En vez de averiguarlo, le pide a cada objeto que se copie a si mismo:
//   1) new Gato(*this) usa el constructor de copia y devuelve memoria nueva;
//   2) se copia tambien el estado de adopcion, que no viaja en la copia
//      porque el constructor de Animal lo deja siempre en DISPONIBLE.
// El que llama se queda con la propiedad del puntero devuelto.
Animal* Gato::clonar() const {
    Gato* copia = new Gato(*this);           // objeto nuevo e independiente
    copia->setEstado(this->estado);          // conserva DISPONIBLE/EN_PROCESO/ADOPTADO
    return copia;
}
