/*
Archivo que se encarga de manejar el menu prencipal del programa
invocando las funciones de las opciones
*/

#include <iostream>
#include <windows.h>
#include "menuPrincipal.h"
#include "mostrarOpciones.h"
#include "subMenus.h"

using namespace std;

//definicion de los objetos de cada header necesario para la invocacion de funciones
subMenus submenu;
mostrarOpciones mostrarMenu;
int opciones;
bool salir = false;
std::string respuesta = "";

//funcion principal del menu
void menuPrincipal::menu(){
do{

  try{ // try..catch sobre el menu para asegurar la correcta digitacion de opciones
    mostrarMenu.mostrarOpcionesMenu(); // llamado de lafuncion que muestra las opciones
    cout << "Seleccione una opcion: ";
    cin >> opciones;
    if(cin.fail()){
        cin.clear();
        cin.ignore(1000,'\n');
        throw "Opcion no valida intente de nuevo!";
    }
    if(opciones < 1 || opciones > 6){
        throw "Usted solo puede seleccionar una opcion del 1 al 6";
    }

    switch(opciones){

    case 1:
        submenu.subMenuRegistro(); // llamado de la funcion para registrar un libro
        do{
        cout << "Desea volver al menu principal? S/N: ";
        cin >> respuesta;
        if(respuesta == "N"){submenu.subMenuRegistro();
        }
        }while(respuesta != "S");
        break;
    case 2:
        submenu.MostrarLibros(); // llamado de la funcion para ver catalogo.txt
        while (true) {
        if (GetAsyncKeyState(VK_RSHIFT) & 0xA1) { // mostrar el resultado hasta que se presione R SHIFT para volver al menu
            break;
        }
            Sleep(100); // sleep para no recargar el procesador del equipo
        }
         break;
    case 3:
        submenu.subMenuPrestamos();//llamado de funcion para registrar un prestamo
        break;
    case 4:
        submenu.devolverLibro();// llamado de la funcion para devolver un libro
        break;
    case 5:
        submenu.historialPrestamos(); // llamado de la funcion para ver el historial de prestamos
         while (true) {
        if (GetAsyncKeyState(VK_RSHIFT) & 0xA1) { // mostrar el resultado hasta que se presione R SHIFT para volver al menu
                break;
        }
            Sleep(100); // sleep para no recargar el procesador del equipo
        }
        break;
    case 6:
        salir = true; // opcion para salir del programa
        break;
    }
}catch(const char* mensaje){
    cerr << mensaje;
}}while(!salir);}
