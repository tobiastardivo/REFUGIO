#include "Utilidades.h"
#include <sstream>
#include <cctype>
#include <cstdlib>   // system()

using namespace std;

// Lee una linea completa y la convierte a entero.
// Si el usuario escribe letras o un numero fuera de rango, vuelve a pedirlo.
// (No lanza excepcion: es un error esperable de la interfaz, no del modelo.)
//
// Si la entrada se cierra (Ctrl+Z en Windows, o cuando se ejecuta el programa
// con un archivo de pruebas redirigido) devuelve el valor minimo para que el
// menu termine de forma ordenada en vez de quedarse en un bucle infinito.
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

// Lee texto no vacio
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

string aMayusculas(const string& texto) {
    string copia = texto;
    for (size_t i = 0; i < copia.size(); i++)
        copia[i] = static_cast<char>(toupper(static_cast<unsigned char>(copia[i])));
    return copia;
}

// Devuelve true solo si el usuario responde SI o S (en mayusculas o minusculas)
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

void pausar() {
    cout << endl << "Presione ENTER para continuar...";
    cin.get();          // espera el ENTER (el buffer quedo limpio por usar getline)
    cin.clear();
}

void limpiarPantalla() {
    // system() es simple pero depende del sistema operativo:
    // en Windows se usa "cls" y en Linux/Mac "clear".
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void linea(char caracter, int largo) {
    for (int i = 0; i < largo; i++) cout << caracter;
    cout << endl;
}
