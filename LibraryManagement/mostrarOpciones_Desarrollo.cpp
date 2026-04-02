/*
Funcion que se encarga de mostrar las opciones correspondientes al menu
*/

#include <iostream>
#include "mostrarOpciones.h"

using namespace std;

void mostrarOpciones::mostrarOpcionesMenu(){

cout << "=========================\n";
cout << "SISTEMA DE BIBLIOTECA C++\n";
cout << "=========================\n";

cout << "1. Ingresar registro al catalogo\n";
cout << "2. Mostrar catalogo\n";
cout << "3. Registrar nuevo prestamo\n";
cout << "4. Devolver libro\n";
cout << "5. Ver historial de prestamos\n";
cout <<"6. salir\n";
}
