#ifndef SCANNER_H
#define SCANNER_H

#include <QString>
#include <QList>

enum Tipo {
    HOSPITAL, PACIENTES, MEDICOS,
    LLAVE_A, LLAVE_C, DOS_PUNTOS, COMA,
    CADENA, NUMERO, ID_MEDICO, ERROR, FIN
};

struct Token {
    Tipo tipo;
    QString lexema;
    int linea;
};

class Scanner {
public:
    Scanner();
    QList<Token> analizar(QString entrada);
};

#endif