#include "scanner.h"

Scanner::Scanner() {}

QList<Token> Scanner::analizar(QString entrada) {
    QList<Token> lista;
    int estado = 0;
    QString lexema = "";
    int linea = 1;

    for(int i = 0; i < entrada.length(); i++) {
        QChar c = entrada.at(i);

        switch(estado) {
        case 0:
            if(c.isSpace()) {
                if(c == '\n') linea++;
            } else if(c.isLetter()) {
                estado = 1;
                lexema += c;
            } else if(c.isDigit()) {
                estado = 2;
                lexema += c;
            } else if(c == '{') {
                lista.append({LLAVE_A, "{", linea});
            } else if(c == '}') {
                lista.append({LLAVE_C, "}", linea});
            } else if(c == ':') {
                lista.append({DOS_PUNTOS, ":", linea});
            } else if(c == ',') {
                lista.append({COMA, ",", linea});
            } else if(c == '"') {
                estado = 3;
            } else {
                lista.append({ERROR, QString(c), linea});
            }
            break;

        case 1: // PALABRAS
            if(c.isLetterOrNumber() || c == '_') {
                lexema += c;
            } else {
                QString aux = lexema.toLower();
                if(aux == "hospital") lista.append({HOSPITAL, lexema, linea});
                else if(aux == "pacientes") lista.append({PACIENTES, lexema, linea});
                else if(aux == "medicos") lista.append({MEDICOS, lexema, linea});
                else lista.append({ID_MEDICO, lexema, linea});

                estado = 0;
                lexema = "";
                i--;
            }
            break;

        case 2: // NÚMEROS
            if(c.isDigit()) {
                lexema += c;
            } else {
                lista.append({NUMERO, lexema, linea});
                estado = 0;
                lexema = "";
                i--;
            }
            break;

        case 3: // CADENAS
            if(c == '"') {
                lista.append({CADENA, lexema, linea});
                lexema = "";
                estado = 0;
            } else {
                lexema += c;
            }
            break;
        }
    }
    return lista;
}