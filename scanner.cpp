#include "scanner.h"

Scanner::Scanner() {}

QList<ErrorLexico> Scanner::getErrores() {
    return this->errores;
}

void Scanner::registrarError(QString lex, QString tipo, QString desc, int l, int c, QString gravedad) {
    ErrorLexico e;
    e.no      = errores.size() + 1;
    e.lexema  = lex;
    e.tipo    = tipo;
    e.descripcion = desc;
    e.linea   = l;
    e.columna = c;
    e.gravedad = gravedad;
    errores.append(e);
}

static const QStringList ESPECIALIDADES_VALIDAS = {
    "CARDIOLOGIA", "NEUROLOGIA", "PEDIATRIA",
    "CIRUGIA", "MEDICINA_GENERAL", "ONCOLOGIA"
};
static const QStringList DOSIS_VALIDAS = {
    "DIARIA", "CADA_8_HORAS", "CADA_12_HORAS", "SEMANAL"
};
static const QStringList KEYWORDS_VALIDAS = {
    "HOSPITAL", "PACIENTES", "MEDICOS", "CITAS", "DIAGNOSTICOS"
};

QList<Token> Scanner::analizar(QString entrada) {
    QList<Token> lista;
    this->errores.clear();

    int     estado  = 0;
    QString lexema  = "";
    int     linea   = 1;
    int     columna = 1;
    int     colInicioStr = 1;

    entrada += " ";

    for (int i = 0; i < (int)entrada.length(); i++) {
        QChar c = entrada.at(i);

        switch (estado) {

        case 0:
            if (c.isSpace()) {
                if (c == '\n') { linea++; columna = 1; }
                else           { columna++; }
            }
            else if (c.isLetter() || c.isDigit()) {
                estado  = 1;
                lexema += c;
                columna++;
            }
            else if (c == '"') {
                estado       = 3;
                lexema       = "";
                colInicioStr = columna;
                columna++;
            }
            else if (c == '{') { lista.append({LLAVE_A,      "{", linea, columna++}); }
            else if (c == '}') { lista.append({LLAVE_C,      "}", linea, columna++}); }
            else if (c == '[') { lista.append({CORCHETE_A,   "[", linea, columna++}); }
            else if (c == ']') { lista.append({CORCHETE_C,   "]", linea, columna++}); }
            else if (c == ':') { lista.append({DOS_PUNTOS,   ":", linea, columna++}); }
            else if (c == ',') { lista.append({COMA,         ",", linea, columna++}); }
            else if (c == ';') { lista.append({PUNTO_COMA,   ";", linea, columna++}); }
            else {
                registrarError(QString(c), "Carácter ilegal",
                               "El carácter '" + QString(c) + "' no pertenece al lenguaje.",
                               linea, columna++, "ERROR");
            }
            break;

        case 1:
            if (c.isLetterOrNumber() || c == '_' || c == '-' || c == ':' || c == '.') {
                lexema += c;
                columna++;
            } else {
                QString aux    = lexema.toUpper();
                int     colReal = columna - (int)lexema.length();

                if      (aux == "HOSPITAL")     lista.append({HOSPITAL,    lexema, linea, colReal});
                else if (aux == "PACIENTES")    lista.append({PACIENTES,   lexema, linea, colReal});
                else if (aux == "MEDICOS")      lista.append({MEDICOS,     lexema, linea, colReal});
                else if (aux == "CITAS")        lista.append({CITAS,       lexema, linea, colReal});
                else if (aux == "DIAGNOSTICOS") lista.append({DIAGNOSTICOS,lexema, linea, colReal});

                else if (lexema.at(0).isDigit()) lista.append({NUMERO, lexema, linea, colReal});

                else if (lexema == lexema.toUpper() && lexema.at(0).isLetter()) {
                    if (ESPECIALIDADES_VALIDAS.contains(lexema)) {
                        lista.append({ID_MEDICO, lexema, linea, colReal});
                    }
                    else if (DOSIS_VALIDAS.contains(lexema)) {
                        lista.append({ID_MEDICO, lexema, linea, colReal});
                    }
                    else {
                        registrarError(
                            lexema,
                            "Especialidad no reconocida",
                            "'" + lexema + "' no es una especialidad válida. "
                                           "Use: CARDIOLOGIA, NEUROLOGIA, PEDIATRIA, CIRUGIA, "
                                           "MEDICINA_GENERAL u ONCOLOGIA.",
                            linea, colReal, "ERROR"
                            );
                        lista.append({ERROR, lexema, linea, colReal});
                    }
                }
                else {
                    lista.append({ID_MEDICO, lexema, linea, colReal});
                }

                estado = 0;
                lexema = "";
            }
            break;

        case 3:
            if (c == '"') {
                QString val   = lexema.trimmed();
                QString valUP = val.toUpper();


                if ((val.contains('+') || val.contains('-')) &&
                    val.length() >= 2 && val.length() <= 3)
                {
                    QStringList sangres = {"O+","O-","A+","A-","B+","B-","AB+","AB-"};
                    if (!sangres.contains(valUP)) {
                        registrarError(
                            "\"" + lexema + "\"",
                            "Tipo de sangre inválido",
                            "El valor '" + val + "' no es un tipo de sangre reconocido. "
                                                 "Valores válidos: A+, A-, B+, B-, O+, O-, AB+, AB-.",
                            linea, colInicioStr, "ERROR"
                            );
                    }
                }
                else if (val.contains('-') && val.length() > 3 && val.length() < 10
                         && val.split("-").size() != 3)
                {
                    registrarError(
                        "\"" + lexema + "\"",
                        "Tipo de sangre inválido",
                        "Formato incorrecto. Valores válidos: A+, A-, B+, B-, O+, O-, AB+, AB-.",
                        linea, colInicioStr, "ERROR"
                        );
                }

                if (val.contains("-") && val.split("-").size() == 3) {
                    QStringList p = val.split("-");
                    bool okMes, okDia;
                    int mes = p[1].toInt(&okMes);
                    int dia = p[2].toInt(&okDia);

                    if (!okMes || mes < 1 || mes > 12) {
                        registrarError(
                            "\"" + lexema + "\"",
                            "Fecha con mes inválido",
                            "El mes '" + p[1] + "' está fuera del rango permitido (01-12). "
                                                "Formato esperado: AAAA-MM-DD.",
                            linea, colInicioStr, "ERROR"
                            );
                    } else if (!okDia || dia < 1 || dia > 31) {
                        registrarError(
                            "\"" + lexema + "\"",
                            "Fecha con día inválido",
                            "El día '" + p[2] + "' está fuera del rango permitido (01-31). "
                                                "Formato esperado: AAAA-MM-DD.",
                            linea, colInicioStr, "ERROR"
                            );
                    }
                }

                if (val.contains(":") && val.split(":").size() == 2) {
                    QStringList p = val.split(":");
                    bool okHr, okMin;
                    int hr  = p[0].toInt(&okHr);
                    int min = p[1].toInt(&okMin);

                    if (!okHr || !okMin || hr < 0 || hr > 23 || min < 0 || min > 59) {
                        registrarError(
                            "\"" + lexema + "\"",
                            "Hora fuera de rango",
                            "La hora '" + val + "' está fuera del rango permitido (00:00-23:59). "
                                                "Formato esperado: HH:MM en 24 horas.",
                            linea, colInicioStr, "ERROR"
                            );
                    }
                }

                lista.append({CADENA, lexema, linea, colInicioStr});
                lexema = "";
                estado = 0;
                columna++;

            } else if (c == '\n' || i == (int)entrada.length() - 1) {
                registrarError(
                    "\"" + lexema,
                    "Cadena sin cerrar",
                    "Se encontró el inicio de una cadena (comilla doble) "
                    "pero no se detectó el cierre antes del fin de la línea.",
                    linea, colInicioStr, "CRÍTICO"
                    );
                estado = 0;
                lexema = "";
                if (c == '\n') { linea++; columna = 1; }

            } else {
                lexema += c;
                columna++;
            }
            break;
        }
    }

    return lista;
}