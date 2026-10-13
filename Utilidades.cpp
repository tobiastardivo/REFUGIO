#include "Utilidades.h"
#include <sstream>
#include <cctype>
#include <cstdlib>

using namespace std;

int leerEntero(const string& mensaje, int minimo, int maximo) {
    string entrada;
    int valor;
    while (true) {
        cout << mensaje;
        if (!getline(cin, entrada)) {
            cin.clear();
            cout << endl;
            return minimo;
        }

        stringstream ss(entrada);
        char sobrante;

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
    cin.get();
    cin.clear();
}

void limpiarPantalla() {
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
