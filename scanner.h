#ifndef SCANNER_H
#define SCANNER_H

#include <QString>
#include <QList>

enum TipoToken {
    HOSPITAL, PACIENTES, MEDICOS, CITAS, DIAGNOSTICOS,
    PACIENTE_ELEMENTO, MEDICO_ELEMENTO, CITA_ELEMENTO, DIAGNOSTICO_ELEMENTO,
    ID_MEDICO, CADENA, NUMERO,
    LLAVE_A, LLAVE_C, CORCHETE_A, CORCHETE_C, DOS_PUNTOS, COMA,
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

class Scanner {
public:
    Scanner();
    QList<Token> analizar(QString entrada);
};

#endif