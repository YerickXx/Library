#ifndef MANEJOARCHIVOS_H_INCLUDED
#define MANEJOARCHIVOS_H_INCLUDED
#include <list>

using namespace std;

struct Libro {
    int ID;
    string Titulo;
    string Autor;
    int Copias;
};

struct Prestamos {
    int IDEstudiante;
    int IDLibro;
    string nombreEstudiante;
    string fecha;
};


class manejoArchivos{
public:
    void crearPrestamo();
    void escribirContenido(list <Libro> &Libros);
    void escribirContenidoPrestamos(list <Prestamos> &prestamos);
};

extern manejoArchivos archivos;

#endif // MANEJOARCHIVOS_H_INCLUDED
