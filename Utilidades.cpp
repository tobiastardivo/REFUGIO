// ===================================================================
// Utilidades.cpp - Funciones auxiliares de entrada/salida por consola.
//
// Todo lo que sea "pedirle un dato al usuario y validarlo" vive aca, y
// no dentro de las clases del modelo. La idea es que Refugio, Animal y
// las demas no sepan que existe un teclado: se pueden usar igual desde
// una interfaz grafica o desde un archivo.
//
// Criterio de validacion: los errores de tipeo NO lanzan excepcion. Un
// dato mal escrito es algo esperable de la interfaz, asi que se avisa y
// se vuelve a pedir. Las excepciones quedan reservadas para los errores
// del modelo (ID repetido, animal no disponible, indice fuera de rango).
//
// Todas las lecturas usan getline() y nunca  cin >> , porque  cin >>
// deja el ENTER en el buffer y la lectura siguiente se saltea sola.
// ===================================================================

#include "Utilidades.h"
#include <sstream>   // stringstream: para convertir texto a numero con control
#include <cctype>    // toupper()
#include <cstdlib>   // system()

using namespace std;

// --- leerEntero(): pide un entero dentro de un rango ---
// Repite el pedido hasta que el dato sea valido. Funciona asi:
//   1) lee la linea COMPLETA como texto;
//   2) la mete en un stringstream e intenta sacar un entero;
//   3) vuelve a leer del stream para comprobar que no haya quedado nada
//      atras: ese segundo intento es el que rechaza entradas como "12abc"
//      (si sobra algo, la lectura tiene exito y por eso se considera error);
//   4) verifica el rango.
// Si la entrada se cierra (Ctrl+Z en Windows, o al correr el programa con
// un archivo de pruebas redirigido) devuelve el minimo, que en el menu es
// la opcion 0: asi el programa termina ordenado en vez de quedarse en un
// bucle infinito leyendo el fin de archivo una y otra vez.
int leerEntero(const string& mensaje, int minimo, int maximo) {
    string entrada;
    int valor;
    while (true) {
        cout << mensaje;
        if (!getline(cin, entrada)) {        // fin de la entrada
            cin.clear();
            cout << endl;
            return minimo;
        }

        stringstream ss(entrada);
        char sobrante;
        // Falla si no hay numero, o si despues del numero quedo basura ("12abc")
        if (!(ss >> valor) || (ss >> sobrante)) {
            cout << "  Error: debe ingresar un numero entero." << endl;
            continue;
        }
        if (valor < minimo || valor > maximo) {
            cout << "  Error: el valor debe estar entre " << minimo
                 << " y " << maximo << "." << endl;
            continue;
        }
        return valor;
    }
}

// --- leerTexto(): pide una linea de texto no vacia ---
// Acepta espacios (por eso getline y no  cin >> , que cortaria en el
// primer espacio y dejaria el resto del nombre en el buffer). Si la
// entrada se cierra devuelve "(sin datos)" para no cortar el programa.
string leerTexto(const string& mensaje) {
    string entrada;
    while (true) {
        cout << mensaje;
        if (!getline(cin, entrada)) {
            cin.clear();
            cout << endl;
            return "(sin datos)";
        }
        if (entrada.empty()) {
            cout << "  Error: no puede quedar vacio." << endl;
            continue;
        }
        return entrada;
    }
}

// --- aMayusculas(): devuelve una copia del texto en mayusculas ---
// Trabaja sobre una copia para no modificar el original (el parametro
// entra por referencia constante). El doble casteo no es capricho:
// toupper() espera un int con el valor de un unsigned char, y pasarle un
// char con signo (una letra acentuada, por ejemplo) es comportamiento
// indefinido; por eso se convierte primero a unsigned char y el
// resultado se vuelve a llevar a char.
string aMayusculas(const string& texto) {
    string copia = texto;
    for (size_t i = 0; i < copia.size(); i++)
        copia[i] = static_cast<char>(toupper(static_cast<unsigned char>(copia[i])));
    return copia;
}

// --- leerSiNo(): pregunta de si o no ---
// Normaliza la respuesta con aMayusculas() para aceptar "si", "Si", "SI"
// o "s" sin escribir cuatro comparaciones distintas. Cualquier otra cosa
// vuelve a preguntar. Ante el fin de la entrada asume NO, que siempre es
// la opcion conservadora (no confirma nada por las dudas).
bool leerSiNo(const string& mensaje) {
    string entrada;
    while (true) {
        cout << mensaje;
        if (!getline(cin, entrada)) {
            cin.clear();
            cout << endl;
            return false;
        }
        string r = aMayusculas(entrada);
        if (r == "SI" || r == "S") return true;
        if (r == "NO" || r == "N") return false;
        cout << "  Error: responda SI o NO." << endl;
    }
}

// --- pausar(): frena la pantalla hasta que el usuario apriete ENTER ---
// Sin esto el menu se limpiaria enseguida y no habria tiempo de leer el
// resultado de la operacion. El cin.clear() de atras deja el flujo en
// condiciones por si la entrada ya estaba cerrada.
void pausar() {
    cout << endl << "Presione ENTER para continuar...";
    cin.get();          // espera el ENTER (el buffer quedo limpio por usar getline)
    cin.clear();
}

// --- limpiarPantalla(): borra la consola antes de redibujar el menu ---
// system() es la solucion mas simple, pero depende del sistema operativo,
// asi que se elige el comando en tiempo de COMPILACION con #ifdef: en
// Windows "cls" y en Linux/Mac "clear". Al ser directivas del
// preprocesador, en el ejecutable final queda una sola de las dos lineas.
void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// --- linea(): dibuja un separador horizontal ---
// El caracter y el largo tienen valor por defecto declarado en el .h
// ('=' y 60), asi que se puede llamar como  linea()  o como  linea('-') .
// Los valores por defecto van SOLO en la declaracion del header: repetirlos
// aca seria un error de compilacion.
void linea(char caracter, int largo) {
    for (int i = 0; i < largo; i++) cout << caracter;
    cout << endl;
}
