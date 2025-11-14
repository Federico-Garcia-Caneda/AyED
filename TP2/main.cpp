#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "listas.hpp"
#include "rwstring.hpp"
using namespace std;

//ESTRUCTURAS DE DATOS
struct Registros {
    string accion; //nombre de la accion
    int plazo; //codigo entre 0 y 3
    string bolsa; //nombre de la bolsa donde se negocio la operacion
    float precio; //precio de cada accion
    int cantidad; //cantidad de acciones en esta operacion (cantidad negativa = compra, cantidad postiva = venta)
};

const int lacc = 12;
const int lbolsa = 14;

struct Nivel3 {
    string operacion;
    string accion;
    int cantidad = 0;
};


struct Nivel2 {
    string bolsa; //clave
    float dintotal = 0;
    float resultado = 0;
    Nodo<Nivel3>* pnv3 = nullptr;
};

struct Nivel1 {
    int plazo; //clave
    int cantventas = 0;
    int cantcompras = 0;
    Nodo<Nivel2>* pnv2 = nullptr;
};

//CRITERIOS
int criterioplazoasc(Nivel1 a, Nivel1 b){ //punto 1
    return a.plazo - b.plazo;
}

int criteriobolsaasc (Nivel2 a, Nivel2 b){ //punto 2
    return a.bolsa.compare(b.bolsa);
}

//FUNCION ESPECIFICA
string nombreplazo(int a) {
    switch (a) {
        case 0: return "CI";
        case 1: return "24hs";
        case 2: return "48hs";
        case 3: return "72hs";
        default: return "Desconocido";
    }
}

//SOBRECARGA DE OPERADORES Y FUNCIONES READSTRING Y WRITESTRING
ostream& operator << (ostream& os, const Nivel1& a){
    os << "Plazo: " << nombreplazo(a.plazo) << "\tCantidad de ventas: " << a.cantventas << "\tCantidad de compras: " << a.cantcompras << endl;
    return os; 
}

ostream& operator << (ostream& os, const Nivel2& a){
    os << "\nBolsa: " << a.bolsa << "\tDinero total: " << a.dintotal << "\tResultado: " << a.resultado << endl;
    return os;
}

ostream& operator << (ostream& os, const Nivel3& a){
    string operacion;
    if (a.cantidad > 0){
        operacion = "Vta";
    }
    if (a.cantidad < 0){
        operacion = "Cpra";
    }
    os << "Operacion: " << operacion << "\t\tAccion: " << a.accion << "\t\tCantidad de acciones: " << abs(a.cantidad);
    return os;
}

fstream& operator >> (fstream& fs, Registros& a){
    a.accion = readstring (fs, lacc);
    fs.read(reinterpret_cast<char*>(&a.plazo), sizeof(int));
    a.bolsa = readstring (fs, lbolsa);
    fs.read(reinterpret_cast<char*> (&a.precio), sizeof(float));
    fs.read(reinterpret_cast<char*> (&a.cantidad), sizeof(int));
    return fs;
}

int main(){
fstream archilec;
archilec.open("Datos.bin", ios::binary | ios::in);
if (!archilec){
    cout << "No se pudo ingresar al archivo";
    return -1;
}

Nodo<Nivel1>* listaclaveplazo = nullptr;

Nodo<Nivel1>* pnodo = nullptr;
Nodo<Nivel2>* pnodo2 = nullptr;

Registros reg;
Nivel1 estrucnv1;
Nivel2 estrucnv2;
Nivel3 estrucnv3;

while(archilec >> reg){
    estrucnv1.plazo = reg.plazo;
    
    estrucnv2.bolsa = reg.bolsa;

    estrucnv3.accion = reg.accion;
    estrucnv3.cantidad = reg.cantidad;

    pnodo = insertar_unico(estrucnv1, listaclaveplazo, criterioplazoasc); //punto 1
    if (reg.cantidad > 0){
        pnodo->dato.cantventas++;
    }
    if (reg.cantidad < 0){
        pnodo->dato.cantcompras++;
    }
    
    pnodo2 = insertar_unico(estrucnv2, pnodo->dato.pnv2, criteriobolsaasc); //punto 2
    pnodo2->dato.dintotal += (reg.precio * (float)abs(reg.cantidad));
    pnodo2->dato.resultado += (reg.precio * (float)reg.cantidad);

    agregar(pnodo2->dato.pnv3, estrucnv3);
}

archilec.close();

pnodo = listaclaveplazo;
while (pnodo != nullptr){
    cout << "\n----------------------------------------------------- \n\n";
    cout << pnodo->dato << endl;
    pnodo2 = pnodo->dato.pnv2;
    while (pnodo2 != nullptr){
        cout << pnodo2->dato << "\n";

        Nodo<Nivel3>* pnodo3 = pnodo2->dato.pnv3;
        while (pnodo3 != nullptr){
            cout << pnodo3->dato << "\n";
            pnodo3 = pnodo3->sig;
        }
        pnodo2 = pnodo2->sig;
    }
    pnodo = pnodo->sig;
}
    return 0;
}