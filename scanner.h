#ifndef SCANNER_H
#define SCANNER_H

#include <QString>
#include <QList>

enum TipoToken {
    HOSPITAL, PACIENTES, MEDICOS, CITAS, DIAGNOSTICOS,
    PACIENTE_ELEMENTO, MEDICO_ELEMENTO, CITA_ELEMENTO, DIAGNOSTICO_ELEMENTO,
    ID_MEDICO, CADENA, NUMERO,
    LLAVE_A, LLAVE_C, CORCHETE_A, CORCHETE_C, DOS_PUNTOS, COMA,
    PUNTO_COMA,
    ERROR
};

struct Token {
    TipoToken tipo;
    QString lexema;
    int linea;
    int columna;
};

struct Diagnostico {
    QString paciente;
    QString condicion;
    QString medicamento;
    QString dosis;
};

struct Paciente {
    QString nombre;
    int edad;
    QString sangre;
    int habitacion;
    QString estado = "SIN DIAG.";
    QString condicion = "N/A";
    QString medicamento = "N/A";
};

struct Medico {
    QString codigo;
    QString nombre;
    QString especialidad;
};

struct ErrorLexico {
    int no;
    QString lexema;
    QString tipo;
    QString descripcion;
    int linea;
    int columna;
    QString gravedad;
};

class Scanner {
public:
    Scanner();
    QList<Token> analizar(QString entrada);
    QList<ErrorLexico> getErrores();
    QList<Paciente> getPacientes();
    QList<Medico> getMedicos();

    void registrarError(QString lex, QString tipo, QString desc, int l, int c, QString gravedad);

private:
    QList<ErrorLexico> errores;
    QList<Paciente> listaPacientes;
    QList<Medico> listaMedicos;
};

#endif