#ifndef CARGAARCHIVO_H_INCLUDED
#define CARGAARCHIVO_H_INCLUDED

class cargarArchivo{
public:
    void cargaDeArchivo();
    bool reducirCopiasDisponibles(std::string idLibro);
    void verPrestamos();
};

extern cargarArchivo cargar;

#endif // CARGAARCHIVO_H_INCLUDED
