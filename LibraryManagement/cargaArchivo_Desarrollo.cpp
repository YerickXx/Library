/*
Archivo que se dedica a carcar los txt a una struct para el manejo de mostrar informacion
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include "cargaArchivo.h"

using namespace std;
cargarArchivo cargar;

//funcion para mostrar el catalogo (opcion 2)
void cargarArchivo::cargaDeArchivo() {
    struct Libro {
        string id;
        string titulo;
        string autor;
        int disponibles;
    };

    ifstream archivo("catalogo.txt");
    if (!archivo.is_open()) {
        cout << "NO SE PUDO ABRIR EL ARCHIVO" << endl;
        return;
    }

    vector<Libro> libros;
    string letters;
    int contador = 0;

    while (getline(archivo, letters)) { // lectura linea por linea del archivo
        if (letters.empty()) continue;

        // asegurarnos que contenga al menos las etiquetas mínimas
        if (letters.find("ID:") == string::npos ||
            letters.find("Titulo del libro:") == string::npos ||
            letters.find("Autor:") == string::npos ||
            letters.find("Copias disponibles:") == string::npos) {
            // línea con formato inesperado: saltarla
            continue;
        }

        Libro lib;

        //busquedas y seleccion de elementos dentro del archivo usando: offset, substrings y cantidad de caracteres.

        // ID
        size_t pos = letters.find("ID: ");
        size_t pos2 = letters.find(", Titulo del libro: ");
        if (pos == string::npos || pos2 == string::npos) continue; // formato raro
        lib.id = letters.substr(pos + 4, pos2 - (pos + 4));

        // geteo de Título
        pos = pos2;
        pos2 = letters.find(", Autor: ", pos);
        if (pos2 == string::npos) continue;
        lib.titulo = letters.substr(pos + string(", Titulo del libro: ").length(), pos2 - (pos + string(", Titulo del libro: ").length()));

        // geteo de Autor
        pos = pos2;
        pos2 = letters.find(", Copias disponibles: ", pos);
        if (pos2 == string::npos) continue;
        lib.autor = letters.substr(pos + string(", Autor: ").length(), pos2 - (pos + string(", Autor: ").length()));

        // geteo de Copias disponibles: localizar sólo los dígitos
        pos = pos2;
        size_t start = pos + string(", Copias disponibles: ").length();

        // asegurarse start dentro del string
        if (start >= letters.size()) {
            lib.disponibles = 0;
        } else {
            // buscar el primer carácter digit empezando en start
            while (start < letters.size() && !isdigit(static_cast<unsigned char>(letters[start]))) ++start;

            size_t end = start;
            while (end < letters.size() && isdigit(static_cast<unsigned char>(letters[end]))) ++end;

            int disponibles_val = 0;
            if (start < end) {
                string numStr = letters.substr(start, end - start);
                try {
                    disponibles_val = stoi(numStr);
                } catch (...) {
                    disponibles_val = 0;
                }
            } else {
                // no se encontraron dígitos, asignar 0
                disponibles_val = 0;
            }
            lib.disponibles = disponibles_val;
        }

        libros.push_back(lib);
        ++contador;
    }

    archivo.close();

    // Imprimir tabla
    cout << string(80, '-') << endl;
    cout << left << setw(8) << "ID"
         << "| " << setw(20) << "Titulo"
         << "| " << setw(30) << "Autor"
         << "| " << setw(12) << "Copias Disponibles" << endl;
    cout << string(7, '-') << "+" << string(20, '-') << "+"
         << string(30, '-') << "+" << string(12, '-') << endl;

    for (const auto& libro : libros) {
        cout << left << setw(8) << libro.id
             << "| " << setw(20) << libro.titulo
             << "| " << setw(30) << libro.autor
             << "| " << setw(12) << libro.disponibles << endl;
    }
    cout << "\nTotal de registros: " << contador << endl;
}

//funcion para reducir las copias del catalogo en caso de prestamos
bool cargarArchivo::reducirCopiasDisponibles(string idLibro) {
    struct Libro {
        string id;
        string titulo;
        string autor;
        int disponibles;
    };

    ifstream archivo("catalogo.txt");
    if (!archivo.is_open()) {
        cout << "Error al abrir catalogo.txt\n";
        return false;
    }

    vector<Libro> libros;
    string linea;

    // === Cargar todos los libros ===
    while (getline(archivo, linea)) {
        if (!linea.empty() && linea.find("ID: ") != string::npos) {
            Libro lib;

            //busqueda de elementos del txt usando: offset, substrings y cantidad de caracteres
            // ID
            size_t pos = linea.find("ID: ");
            size_t pos2 = linea.find(", Titulo del libro: ");
            lib.id = linea.substr(pos + 4, pos2 - (pos + 4));

            // Título
            pos = pos2;
            pos2 = linea.find(", Autor: ");
            lib.titulo = linea.substr(pos + 20, pos2 - (pos + 20));

            // Autor
            pos = pos2;
            pos2 = linea.find(", Copias disponibles:");
            lib.autor = linea.substr(pos + 9, pos2 - (pos + 9));

            // Copias disponibles
            string temp = linea.substr(pos2 + 21);
            temp.erase(remove_if(temp.begin(), temp.end(), ::isspace), temp.end()); // limpia espacios

            try {
                lib.disponibles = stoi(temp);
            } catch (...) {
                cout << "Error: valor no numérico al leer 'Copias disponibles' → '" << temp << "'" << endl;
                lib.disponibles = 0;
            }

            libros.push_back(lib);
        }
    }
    archivo.close();

    // Buscar y reducir copias
    bool encontrado = false;
    for (auto &libro : libros) {
        if (libro.id == idLibro) {
            encontrado = true;
            if (libro.disponibles > 0) {
                libro.disponibles--;
                cout << "Copias disponibles ahora: " << libro.disponibles << endl;
            } else {
                cout << "No hay copias disponibles de este libro\n";
                return false;
            }
            break;
        }
    }

    if (!encontrado) {
        cout << "Libro no encontrado en el catálogo\n";
        return false;
    }

    //Guardar cambios
    ofstream archivoSalida("catalogo.txt");
    if (!archivoSalida.is_open()) {
        cout << "Error al guardar cambios en catalogo.txt\n";
        return false;
    }

    for (const auto &libro : libros) {
        archivoSalida << "ID: " << libro.id
                      << ", Titulo del libro: " << libro.titulo
                      << ", Autor: " << libro.autor
                      << ", Copias disponibles: " << libro.disponibles << endl;
    }

    archivoSalida.close();
    return true;
}

//funcion para el historial de prestamos (opcion 5)
void cargarArchivo::verPrestamos() {
    struct Registro {
        string id_estudiante;
        string id_libro;
        string nombre_estudiante;
        string fecha;
    };

    ifstream archivo("Prestamos.txt");
    if (!archivo.is_open()) {
        cout << "NO SE PUDO ABRIR EL ARCHIVO" << endl;
        return;
    }

    vector<Registro> registros;
    string letters;
    int contador = 0;

    while (getline(archivo, letters)) { // lectura linea por linea de Prestamos.txt
        if (letters.empty()) continue;

        // Validar que la línea tenga el formato esperado utilizando el npos
        if (letters.find("ID estudiante:") == string::npos ||
            letters.find("ID del libro:") == string::npos ||
            letters.find("Nombre estudiante:") == string::npos ||
            letters.find("Fecha:") == string::npos) {
            continue;
        }

        Registro reg;

        //busqueda de los elementos del txt usando: offset, substrings y cantidad de caracteres del elemento

        // ID estudiante
        size_t pos = letters.find("ID estudiante: ");
        size_t pos2 = letters.find(", ID del libro: ");
        if (pos == string::npos || pos2 == string::npos) continue;
        reg.id_estudiante = letters.substr(pos + string("ID estudiante: ").length(), pos2 - (pos + string("ID estudiante: ").length()));

        // ID del libro
        pos = pos2;
        pos2 = letters.find(", Nombre estudiante: ", pos);
        if (pos2 == string::npos) continue;
        reg.id_libro = letters.substr(pos + string(", ID del libro: ").length(), pos2 - (pos + string(", ID del libro: ").length()));

        // Nombre del estudiante
        pos = pos2;
        pos2 = letters.find(", Fecha: ", pos);
        if (pos2 == string::npos) continue;
        reg.nombre_estudiante = letters.substr(pos + string(", Nombre estudiante: ").length(), pos2 - (pos + string(", Nombre estudiante: ").length()));

        // Fecha
        pos = pos2;
        size_t start = pos + string(", Fecha: ").length();
        if (start >= letters.size()) {
            reg.fecha = "Desconocida";
        } else {
            reg.fecha = letters.substr(start);
        }

        registros.push_back(reg);
        ++contador;
    }

    archivo.close();

    // Imprimir tabla de préstamos
    cout << string(90, '-') << endl;
    cout << left << setw(15) << "ID Estudiante"
         << "| " << setw(15) << "ID Libro"
         << "| " << setw(30) << "Nombre Estudiante"
         << "| " << setw(15) << "Fecha" << endl;
    cout << string(15, '-') << "+" << string(15, '-') << "+"
         << string(30, '-') << "+" << string(15, '-') << endl;

    for (const auto& reg : registros) {
        cout << left << setw(15) << reg.id_estudiante
             << "| " << setw(15) << reg.id_libro
             << "| " << setw(30) << reg.nombre_estudiante
             << "| " << setw(15) << reg.fecha << endl;
    }

    cout << "\nTotal de registros: " << contador << endl;
}
