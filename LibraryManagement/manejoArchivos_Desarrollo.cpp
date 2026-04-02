/*
    Archivo que se encarga de la creacion de los archivos txt usando las funciones de
    ofstream

*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <list>
#include "manejoArchivos.h"
#include "validacionesEntradas.h"

using namespace std;
extern manejoArchivos archivos;
validaciones entradasUsuario;

//funcion para crear y escribir el archivo catalogo.txt
void manejoArchivos::escribirContenido(list <Libro> &Libros){
    ofstream archivo("catalogo.txt",ios::app);  // Abre el archivo
    if (!archivo.is_open()) { // validacion minima de creacion
        cout << "Error al abrir el archivo.\n";
        return;
    }
        //estructura de escritura, sus elementos y formato
        const Libro &libro = Libros.back();
        archivo << "ID: " << libro.ID << ", "
                << "Titulo del libro: " << libro.Titulo << ", "
                << "Autor: " << libro.Autor << ", "
                << "Copias disponibles: " << libro.Copias << "\n";
                cout << "Libro: "<< libro.Titulo << " (ID: " << libro.ID << ") registrado con exito\n";

    archivo.close();
}

// funcion para crear y escribir los prestamos
void manejoArchivos::escribirContenidoPrestamos(list <Prestamos> &prestamos){
    ofstream archivo("Prestamos.txt",ios::app);  // Abre el archivo
    if (!archivo.is_open()) { // validacion minima de creacion
        cout << "Error al abrir el archivo.\n";
        return;
    }

        //estructura de escritura, sus elementos y formato
        const Prestamos &listaPrestamo = prestamos.back();
        archivo << "ID estudiante: " << listaPrestamo.IDEstudiante << ", "
                << "ID del libro: " << listaPrestamo.IDLibro << ", "
                << "Nombre estudiante: " << listaPrestamo.nombreEstudiante << ", "
                << "Fecha: " << listaPrestamo.fecha << "\n";
                cout << "Prestamo Registrado: " <<"\n"<< "Alumno ID: "<<listaPrestamo.IDEstudiante << "\n"<< "Libro ID: " << listaPrestamo.IDLibro << "\n"<< "Fecha: " << listaPrestamo.fecha << "\n";


    archivo.close();
}
