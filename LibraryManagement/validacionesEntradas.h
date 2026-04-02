#ifndef VALIDACIONESENTRADAS_H_INCLUDED
#define VALIDACIONESENTRADAS_H_INCLUDED

class validaciones{
public:
    void validacionID(int &n);
    void validacionIDEstudiante(int &n);
    void validacionCantidades(int &n);
    void validacionTexto(std::string &n);
    bool idExiste(std::string& id);
    std::string fecha();
    bool estado();
    bool idEstudianteExiste(std::string& id);
};
extern validaciones entradasUsuario;

#endif // VALIDACIONESENTRADAS_H_INCLUDED
