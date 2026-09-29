// ===================================================================
// Perro.cpp - Implementacion de la clase derivada Perro.
//
// Perro es una de las dos clases concretas que heredan de Animal.
// Aporta los atributos propios (raza y tamanio) y da cuerpo a las tres
// funciones virtuales puras que Animal deja sin implementar:
// mostrarInfo(), tipo() y clonar().
// ===================================================================

#include "Perro.h"
#include <iostream>

using namespace std;

// --- Constructor por defecto ---
// Primero corre Animal() (datos base neutros + un animal vivo mas en el
// contador estatico) y despues se inicializa lo propio del perro.
Perro::Perro() : Animal(), raza("Mestizo"), tamanio("Mediano") {}

// --- Constructor parametrizado ---
// Los datos comunes viajan al constructor de Animal por la lista de
// inicializacion; el perro solo se ocupa de raza y tamanio.
Perro::Perro(int id, const string& nombre, int edad, const string& estadoSalud,
             const string& raza, const string& tamanio)
    : Animal(id, nombre, edad, estadoSalud), raza(raza), tamanio(tamanio) {}

// --- Constructor de copia ---
// Invoca explicitamente el constructor de copia de Animal. Si no se lo
// invocara, se usaria Animal() por defecto y se perderian los datos base.
Perro::Perro(const Perro& otro)
    : Animal(otro), raza(otro.raza), tamanio(otro.tamanio) {}

// --- Destructor ---
// No libera memoria porque Perro no usa punteros propios, pero se declara
// para dejar explicito el orden ~Perro() -> ~Animal().
Perro::~Perro() {}

// --- Getters ---
// Devuelven una copia del atributo; son const porque solo leen el objeto.
string Perro::getRaza() const    { return raza; }
string Perro::getTamanio() const { return tamanio; }

// --- mostrarInfo(): redefinicion polimorfica ---
// Imprime la etiqueta [PERRO], reutiliza mostrarDatosBase() de Animal para
// los campos comunes y agrega los propios. Al ser virtual, llamarla a
// traves de un Animal* ejecuta esta version (ligadura dinamica).
void Perro::mostrarInfo() const {
    cout << "[PERRO] ";
    mostrarDatosBase();                      // metodo protegido de Animal
    cout << " | Raza: " << raza
         << " | Tamanio: " << tamanio << endl;
}

// --- tipo(): devuelve el nombre del tipo concreto ---
// Sirve para saber que es un Animal* sin recurrir a casteos.
// La usa Refugio::mostrarEstadisticas() para contar perros y gatos.
string Perro::tipo() const { return "Perro"; }

// --- clonar(): pieza clave de la COPIA PROFUNDA ---
// Permite copiar el catalogo sin saber el tipo concreto de cada elemento:
//   1) new Perro(*this) usa el constructor de copia y reserva memoria nueva;
//   2) se copia aparte el estado de adopcion, porque el constructor de
//      Animal siempre deja el estado en DISPONIBLE.
// El que llama se queda con la propiedad del puntero devuelto.
Animal* Perro::clonar() const {
    Perro* copia = new Perro(*this);         // objeto nuevo e independiente
    copia->setEstado(this->estado);          // conserva el estado de adopcion
    return copia;
}
