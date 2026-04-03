#include "scanner.h"

Scanner::Scanner() {}

QList<Token> Scanner::analizar(QString entrada) {
    QList<Token> lista;
    int estado = 0;
    QString lexema = "";
    int linea = 1;
    int columna = 1;

    for(int i = 0; i < entrada.length(); i++) {
        QChar c = entrada.at(i);

        switch(estado) {
        case 0:
            if(c.isSpace()) {
                if(c == '\n') {
                    linea++;
                    columna = 1;
                } else {
                    columna++;
                }
            } else if(c.isLetter()) {
                estado = 1;
                lexema += c;
            } else if(c.isDigit()) {
                estado = 2;
                lexema += c;
            } else if(c == '"') {
                estado = 3;
            } else if(c == '{') {
                lista.append({LLAVE_A, "{", linea, columna++});
            } else if(c == '}') {
                lista.append({LLAVE_C, "}", linea, columna++});
            } else if(c == '[') {
                lista.append({CORCHETE_A, "[", linea, columna++});
            } else if(c == ']') {
                lista.append({CORCHETE_C, "]", linea, columna++});
            } else if(c == ':') {
                lista.append({DOS_PUNTOS, ":", linea, columna++});
            } else if(c == ',') {
                lista.append({COMA, ",", linea, columna++});
            } else {
                lista.append({ERROR, QString(c), linea, columna++});
            }
            break;

        case 1:
            if(c.isLetterOrNumber() || c == '_' || c == '-') {
                lexema += c;
            } else {
                QString aux = lexema.toUpper();
                if(aux == "HOSPITAL") lista.append({HOSPITAL, lexema, linea, columna - lexema.length()});
                else if(aux == "PACIENTES") lista.append({PACIENTES, lexema, linea, columna - lexema.length()});
                else if(aux == "MEDICOS") lista.append({MEDICOS, lexema, linea, columna - lexema.length()});
                else if(aux == "CITAS") lista.append({CITAS, lexema, linea, columna - lexema.length()});
                else if(aux == "DIAGNOSTICOS") lista.append({DIAGNOSTICOS, lexema, linea, columna - lexema.length()});
                else lista.append({ID_MEDICO, lexema, linea, columna - lexema.length()});

                estado = 0;
                lexema = "";
                i--;
            }
            break;

        case 2:
            if(c.isDigit() || c == '-' || c == ':') {
                lexema += c;
            } else {
                lista.append({NUMERO, lexema, linea, columna - lexema.length()});
                estado = 0;
                lexema = "";
                i--;
            }
            break;

        case 3:
            if(c == '"') {
                lista.append({CADENA, lexema, linea, columna - lexema.length()});
                lexema = "";
                estado = 0;
                columna++;
            } else {
                lexema += c;
                if(c == '\n') { linea++; columna = 1; }
                else { columna++; }
            }
            break;
        }

        if(estado != 0 && c != '\n' && c != '"') {
            columna++;
        }
    }
    return lista;
}