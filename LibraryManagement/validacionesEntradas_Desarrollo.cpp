/*
Archivo que se encarga del desarrollo de algunas funciones de validacion
*/

#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <iomanip>
#include "validacionesEntradas.h"
#include "manejoArchivos.h"

using namespace std;

//validacion de ID de libro
void validaciones::validacionID(int &n){
    do{
        try{
             cin >> n;
             if(cin.fail()){
                cin.clear();
                cin.ignore(1000,'\n');
                throw "ID no valido intente de nuevo! \n";
             }
             std::string number = std::to_string(n); // convierte a string

            if (number.length() != 6) {
                throw "El ID debe tener exactamente 6 digitos\n";
            }
             break;

        }catch(const char* mensaje){
            cerr << mensaje;
        }

    }while(true);

}

// validacion de cantidades numericas
void validaciones::validacionCantidades(int &n){
     do{
        try{
             cin >> n;
             if(cin.fail()){
                cin.clear();
                cin.ignore(1000,'\n');
                throw "Solo puede ingresar numeros en esta opcion, intente de nuevo! \n";
             }
             if(n > 9){
                throw "No puede registrar mas de 9 copias!, intente de nuevo\n";
             }
             break;

        }catch(const char* mensaje){
            cerr << mensaje;
        }

    }while(true);

}

// validacion de texto
void validaciones::validacionTexto(std::string &n){

do{
        try{
             getline(cin,n);
             if(cin.fail()){
                cin.clear();
                cin.ignore(1000,'\n');
                throw "Debe ingresar un nombre valido \n";
             }
             if(n.empty()){
                throw "El nombre no puede estar vacio, intente de nuevo!\n";
             }
                for (char c : n) {
                if (!isalpha(c) && c != ' ') {
                    throw "Solo puede ingresar letras o texto, intente de nuevo!\n";
                }
            }
             break;

        }catch(const char* mensaje){
            cerr << mensaje;
        }
    }while(true);
}

// validacion de existencia de ID del libro
bool validaciones::idExiste(string& id){
  ifstream archivo("catalogo.txt");
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo para validar ID.\n";
        return false;
    }

    string linea;
    while (getline(archivo, linea)) {
        size_t pos_coma = linea.find(','); // primera coma
        if (pos_coma != string::npos) {
            string idActual = linea.substr(0, pos_coma); // "ID: 123456"

            // quitar la palabra ID:
            size_t pos_dos_puntos = idActual.find(':'); // buscar los 2 puntos de la palabra ID
            if (pos_dos_puntos != string::npos) {
                idActual = idActual.substr(pos_dos_puntos + 2); // +2 para saltar ": " despues de la palabra ID ya que lo que queremos validar solo es el numero
            }

            if (idActual == id) {
                return true; // ID ya existe
            }
        }
    }

    return false; // ID no existe
}

//validacion de existencia de ID del estudiante
bool validaciones::idEstudianteExiste(string& id){
  ifstream archivo("Prestamos.txt");
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo para validar ID.\n";
        return false;
    }

    string linea;
    while (getline(archivo, linea)) {
        size_t pos_coma = linea.find(','); // primera coma
        if (pos_coma != string::npos) {
            string idActual = linea.substr(0, pos_coma); // "ID: 123456"

            // quitar la palabra ID:
            size_t pos_dos_puntos = idActual.find(':'); // buscar los 2 puntos de la palabra ID
            if (pos_dos_puntos != string::npos) {
                idActual = idActual.substr(pos_dos_puntos + 1); // +2 para saltar ": " despues de la palabra ID ya que lo que queremos validar solo es el numero
                idActual.erase(0, idActual.find_first_not_of(" \t"));
            }

            if (idActual == id) {
                return true; // ID ya existe
            }
        }
    }

    return false; // ID no existe
}

//validacion de correcto formato del ID estudiante
void validaciones::validacionIDEstudiante(int &n){
    do{
        try{
             cin >> n;
             if(cin.fail()){
                cin.clear();
                cin.ignore(1000,'\n');
                throw "ID no valido intente de nuevo! \n";
             }
             std::string numberID = std::to_string(n);// convierte a string

            if(numberID.length() != 9){
                throw "El ID de estudiante debe tener 9 digitos!\n";
            }
            if(idEstudianteExiste(numberID)) {
            throw "El ID de estudiante ya existe!\n";
        }
             break;

             //validar existencia del id del estudiante

        }catch(const char* mensaje){
            cerr << mensaje;
        }

    }while(true);

}

// creacion de fecha para prestamos
string validaciones::fecha(){
    time_t t = time(nullptr);// obtiene tiempo actual
    tm* now = localtime(&t);// convierte a tiempo local

    // Formatear fecha DD/MM/YYYY
    string fecha = to_string(now->tm_mday) + "/" +
                   to_string(now->tm_mon + 1) + "/" +
                   to_string(now->tm_year + 1900);

    string fechaUsuario;
    cout << "Fecha (DD/MM/YYYY) [" << fecha << "]: ";
    getline(cin, fechaUsuario);

    // Si el usuario deja vacío, usamos la fecha automática
    if (fechaUsuario.empty()) {
        fechaUsuario = fecha;
    }

    return fechaUsuario;
}

