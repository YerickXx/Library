/*
Archivo que desarrolla las distintas opciones del menu desarrollando su logica
y haciendo llamado a archivos secundarios de apoyo como el de validaciones
para asegurar el correcto funcionamiento

*/


#include <iostream>
#include <string>
#include <limits>
#include <list>
#include <fstream>
#include <vector>
#include "subMenus.h"
#include "validacionesEntradas.h"
#include "manejoArchivos.h"
#include "cargaArchivo.h"

using namespace std;

extern validaciones entradasUsuario;
extern manejoArchivos archivos;
extern cargarArchivo cargar;

//decaracion de estructuras
list <Libro> Libros;
list <Prestamos> prestamos;

//declaracion de variables
int ID;
int IDEstudiante;
string Titulo = "";
string nombreAutor = "";
int copiasDisponibles = 0;
string nombreApellido;
bool encontrado = false;
string confirmacionRegistro;

//funcion para registrar un libro
void subMenus::subMenuRegistro(){
    cout << "====================\n";
    cout <<" Registro de libro   \n";
    cout << "====================\n";
 do {// estructura de repeticion sobre la solicitud y validacion del ID del libro
    cout << "Ingrese el ID del libro: \n";
    entradasUsuario.validacionID(ID); // llamado de la funcion de validacion para el ID
    std::string number = std::to_string(ID); // convertir ID a string para pasarselo al idExiste()

    if (entradasUsuario.idExiste(number)) {
        cout << "ID ya existe, ingrese otro.\n";
    } else {
        break; // ID único
    }
} while (true);
    //continuar con solicitud de informcion y a su vez llamado de las funciones de validacion
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');// limpiar el buffer para que no haya interferencia al llamar la siguiente funcion
    cout << "Ingrese el titulo del libro: \n";
    entradasUsuario.validacionTexto(Titulo);
    cout << "Ingrese el nombre del autor: \n";
    entradasUsuario.validacionTexto(nombreAutor);
    cout << "Ingrese la cantidad de copias disponibles: \n";
    entradasUsuario.validacionCantidades(copiasDisponibles);
    Libro nuevoLibro = {ID, Titulo, nombreAutor, copiasDisponibles};
    Libros.push_back(nuevoLibro);
    archivos.escribirContenido(Libros);
}

//funcion para mostrar el catalogo esta se encuentra en cargaArchivos_Desarrollo
void subMenus::MostrarLibros(){
    cargar.cargaDeArchivo();

    cout << "Presione SHIFT para volver al menu de inicio \n";

}

//funcion que se encarga de registrar un prestamo
void subMenus::subMenuPrestamos() {
    //variables locales
    string tituloLibro;
    string letters;
    int disponibles = 0;

    cout << "====================\n";
    cout << "Registro de prestamo\n";
    cout << "====================\n";

    // Validar ID estudiante
    do {
        cout << "Ingrese el ID de estudiante: \n";
        entradasUsuario.validacionIDEstudiante(IDEstudiante);
        std::string numberID = std::to_string(IDEstudiante);
        entradasUsuario.idEstudianteExiste(numberID);
        if (entradasUsuario.idExiste(numberID)) {
            cout << "ID ya existe, ingrese otro.\n";
        } else {
            break;
        }
    } while (true);
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Validar ID libro
    do {
        cout << "Ingrese el ID del libro: \n";
        entradasUsuario.validacionID(ID);
        std::string number = std::to_string(ID);

        if (entradasUsuario.idExiste(number)) {
            ifstream archivo("catalogo.txt");
            if (!archivo.is_open()) {
                cout << "Error al abrir el archivo!\n";
                return;
            }

            while (getline(archivo, letters)) {
                if (letters.find("ID: " + number) != string::npos) {
                    encontrado = true;

                    // Extraer Título del libro
                    size_t posTitulo = letters.find("Titulo del libro:");
                    if (posTitulo != string::npos) {
                        size_t start = posTitulo + 17;
                        size_t end = letters.find(",", start);
                        tituloLibro = letters.substr(start, end - start);
                    }

                    // Extraer copias disponibles
                    size_t posDisp = letters.find("Copias disponibles:");
                    if (posDisp != string::npos) {
                        size_t start = posDisp + 19;
                        while (start < letters.size() && !isdigit(static_cast<unsigned char>(letters[start]))) ++start;
                        size_t end = start;
                        while (end < letters.size() && isdigit(static_cast<unsigned char>(letters[end]))) ++end;
                        disponibles = stoi(letters.substr(start, end - start));
                    }
                    break;
                }
            }
            archivo.close();

            if (encontrado) break;
            else cout << "No se encontró el libro en el archivo.\n";
        } else {
            cout << "El libro con ese ID no existe!\n";
        }
    } while (true);

    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // limpiado del buffer para no tener problemas al invocar futuras funciones

    cout << "Ingrese el nombre y apellido del estudiante: ";
    entradasUsuario.validacionTexto(nombreApellido);

    std::string fechaPrestamo = entradasUsuario.fecha();

    cout << "Se encontraron ejemplares disponibles de este libro: " << tituloLibro
         << " | Disponibles: " << disponibles << "\n"
         << "Desea confirmar el prestamo (S/N): ";

    cin >> confirmacionRegistro;

    if (confirmacionRegistro == "S" || confirmacionRegistro == "s") {
        bool reducido = cargar.reducirCopiasDisponibles(std::to_string(ID));

        Prestamos nuevoPrestamo = {IDEstudiante, ID, nombreApellido, fechaPrestamo};
        prestamos.push_back(nuevoPrestamo);
        archivos.escribirContenidoPrestamos(prestamos);

        if (reducido)
            cout << "Prestamo registrado y copias actualizadas correctamente.\n";
        else
            cout << "Prestamo registrado, pero no se pudo actualizar las copias.\n";
    } else {
        cout << "Registro cancelado.\n";
    }
}

//funcion para devolver el libro
void subMenus::devolverLibro() {
    //variables locales
    string letters;
    string fecha;
    string alumno;
    bool encontrado = false;
    vector<string> prestamosActuales;
    vector<string> catalogoLineas;

    cout << "====================\n";
    cout << "   Devolver Libro   \n";
    cout << "====================\n";

    cout << "Ingrese el ID de estudiante: ";
    while (!(cin >> IDEstudiante) || IDEstudiante <= 0) {
        cout << "ID invalido. Intente nuevamente: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "Ingrese el ID del libro: ";
    while (!(cin >> ID) || ID <= 0) {
        cout << "ID invalido. Intente nuevamente: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Buscar préstamo en Prestamos.txt
    ifstream archivoPrestamos("Prestamos.txt");
    if (!archivoPrestamos.is_open()) {
        cout << "Error al abrir Prestamos.txt\n";
        return;
    }

    while (getline(archivoPrestamos, letters)) {
        prestamosActuales.push_back(letters);

        if (!encontrado &&
            letters.find("ID estudiante: " + std::to_string(IDEstudiante)) != string::npos &&
            letters.find("ID del libro: " + std::to_string(ID)) != string::npos) {

            encontrado = true;

            // Extraer nombre del estudiante
            size_t posNom = letters.find("Nombre estudiante:");
            if (posNom != string::npos) {
                size_t start = posNom + 18;
                size_t end = letters.find(",", start);
                alumno = letters.substr(start, end - start);
            }

            // Extraer fecha
            size_t posFec = letters.find("Fecha:");
            if (posFec != string::npos) {
                size_t start = posFec + 6;
                fecha = letters.substr(start);
            }
        }
    }
    archivoPrestamos.close();

    if (!encontrado) {
        cout << "No se encontro un prestamo con esos datos.\n";
        return;
    }

    cout << "\nPrestamo encontrado:\n";
    cout << "Estudiante: " << alumno << "\n";
    cout << "Fecha de prestamo: " << fecha << "\n";

    char confirm;
    cout << "\nConfirmar devolucion? (S/N): ";
    cin >> confirm;
    if (toupper(confirm) != 'S') {
        cout << "Devolucion cancelada.\n";
        return;
    }

    //  Aumentar copias disponibles en catalogo.txt
    ifstream archivoCatalogo("catalogo.txt");
    if (!archivoCatalogo.is_open()) {
        cout << "Error al abrir catalogo.txt\n";
        return;
    }

    while (getline(archivoCatalogo, letters)) {
        if (letters.find("ID: " + std::to_string(ID)) != string::npos) {
            size_t posDisp = letters.find("Copias disponibles:");
            if (posDisp != string::npos) {
                size_t start = posDisp + 19;
                while (start < letters.size() && (letters[start] == ' ')) start++;
                size_t end = start;
                while (end < letters.size() && isdigit(letters[end])) end++;

                string numeroActual = letters.substr(start, end - start);
                int disponibles = stoi(numeroActual);
                disponibles++;

                // Reemplaza solo el número sin tocar el formato
                letters.replace(start, numeroActual.size(), to_string(disponibles));
            }
        }
        catalogoLineas.push_back(letters);
    }
    archivoCatalogo.close();

    //Reescribir catalogo.txt
    ofstream salidaCatalogo("catalogo.txt", ios::trunc);
    for (auto &linea : catalogoLineas)
        salidaCatalogo << linea << "\n";
    salidaCatalogo.close();

    //  Reescribir Prestamos.txt (eliminando el devuelto)
    ofstream archivoSalida("Prestamos.txt", ios::trunc);
    for (auto &linea : prestamosActuales) {
        if (linea.find("ID estudiante: " + std::to_string(IDEstudiante)) != string::npos &&
            linea.find("ID del libro: " + std::to_string(ID)) != string::npos) {
            continue;
        }
        archivoSalida << linea << "\n";
    }
    archivoSalida.close();

    cout << "\n Libro devuelto con exito \n";
}
   void subMenus::historialPrestamos(){
    cargar.verPrestamos();
    cout << "Presione SHIFT para volver al menu de inicio \n";

   }

