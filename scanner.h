#ifndef SCANNER_H
#define SCANNER_H

#include <QString>
#include <QList>

// Definimos los tipos de tokens que el AFD va a reconocer
enum Tipo {
    HOSPITAL, PACIENTES, MEDICOS,
    LLAVE_A, LLAVE_C, DOS_PUNTOS, COMA,
    CADENA, NUMERO, ID_MEDICO, ERROR, FIN
};

// Estructura para guardar cada palabra o símbolo encontrado
struct Token {
    Tipo tipo;
    QString lexema;
    int linea;
};

class Scanner {
public:
    Scanner();
    // Esta función es la que hará el trabajo pesado
    QList<Token> analizar(QString entrada);
};

#endif