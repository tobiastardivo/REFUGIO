// ===================================================================
// Adoptante.cpp - Implementacion del TAD Adoptante.
//
// Es la clase mas simple del sistema: cuatro atributos privados y sus
// operaciones. Lo importante es que los objetos Adoptante se guardan
// POR VALOR dentro de una Coleccion<Adoptante>, y esa coleccion los
// copia y los asigna al redimensionarse; por eso la clase define su
// constructor de copia y su operador de asignacion en vez de confiar
// en los que genera el compilador.
// ===================================================================

#include "Adoptante.h"
#include <iostream>

using namespace std;

// --- Constructor por defecto ---
// Hace falta porque Coleccion<T> reserva memoria con  new T[capacidad] ,
// y esa instruccion construye todos los elementos del bloque llamando al
// constructor sin parametros. Deja el adoptante con datos neutros hasta
// que se le asigne uno real.
Adoptante::Adoptante()
    : id(0), nombre("Sin nombre"), contacto("Sin datos"), tipoVivienda("Sin datos") {}

// --- Constructor parametrizado ---
// Carga los cuatro datos por lista de inicializacion. Recibe los strings
// por referencia constante para no copiarlos dos veces (una al pasar el
// argumento y otra al guardarlo).
Adoptante::Adoptante(int id, const string& nombre,
                     const string& contacto, const string& tipoVivienda)
    : id(id), nombre(nombre), contacto(contacto), tipoVivienda(tipoVivienda) {}

// --- Constructor de copia ---
// Copia campo por campo. Como no hay punteros de por medio, la copia es
// naturalmente independiente del original: modificar una no toca la otra.
Adoptante::Adoptante(const Adoptante& otro)
    : id(otro.id), nombre(otro.nombre),
      contacto(otro.contacto), tipoVivienda(otro.tipoVivienda) {}

// --- Operador de asignacion ---
// Se usa cada vez que la coleccion mueve elementos de un bloque a otro.
// El  if (this != &otro)  protege contra la autoasignacion (a = a): sin
// ese control, en clases que liberan memoria antes de copiar se estaria
// destruyendo el origen antes de leerlo. Devuelve *this por referencia
// para poder encadenar  a = b = c .
Adoptante& Adoptante::operator=(const Adoptante& otro) {
    if (this != &otro) {
        id = otro.id;
        nombre = otro.nombre;
        contacto = otro.contacto;
        tipoVivienda = otro.tipoVivienda;
    }
    return *this;
}

// --- Destructor ---
// Vacio: los cuatro atributos se destruyen solos (las std::string liberan
// su propia memoria). Se declara para dejar completa la Regla de los Tres.
Adoptante::~Adoptante() {}

// --- Getters ---
// Unica via de lectura de los atributos, que son privados. Son const y
// devuelven copia, asi nadie modifica el objeto desde afuera.
int Adoptante::getId() const               { return id; }
string Adoptante::getNombre() const        { return nombre; }
string Adoptante::getContacto() const      { return contacto; }
string Adoptante::getTipoVivienda() const  { return tipoVivienda; }

// --- mostrarInfo() ---
// Imprime el adoptante en una linea. El nombre no es casual: la funcion
// generica mostrarColeccion<T>() de Utilidades.h llama a mostrarInfo()
// sobre cualquier T, asi que alcanza con que la clase tenga este metodo
// para que pueda listarse con el mismo codigo que las solicitudes.
void Adoptante::mostrarInfo() const {
    cout << "ID: " << id
         << " | Nombre: " << nombre
         << " | Contacto: " << contacto
         << " | Vivienda: " << tipoVivienda << endl;
}

// --- operator== ---
// Compara por ID, que es la clave: dos personas pueden llamarse igual,
// pero el numero de adoptante es unico. Lo usa el control de altas
// repetidas del Refugio.
bool Adoptante::operator==(const Adoptante& otro) const { return id == otro.id; }
