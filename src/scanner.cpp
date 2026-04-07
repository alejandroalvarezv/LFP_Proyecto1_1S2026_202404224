#include "scanner.h"
#include <string>
#include <cctype>
#include <vector>
#include <stdexcept>



static std::string strToUpper(const std::string& s) {
    std::string r = s;
    for (size_t i = 0; i < r.size(); i++)
        r.at(i) = static_cast<char>(std::toupper(static_cast<unsigned char>(r.at(i))));
    return r;
}

static std::string strToLower(const std::string& s) {
    std::string r = s;
    for (size_t i = 0; i < r.size(); i++)
        r.at(i) = static_cast<char>(std::tolower(static_cast<unsigned char>(r.at(i))));
    return r;
}

static std::string strTrim(const std::string& s) {
    size_t ini = s.find_first_not_of(" \t\r\n");
    if (ini == std::string::npos) return "";
    size_t fin = s.find_last_not_of(" \t\r\n");
    return s.substr(ini, fin - ini + 1);
}

static std::vector<std::string> strSplit(const std::string& s, char delim) {
    std::vector<std::string> partes;
    size_t inicio = 0;
    size_t pos    = s.find(delim);
    while (pos != std::string::npos) {
        partes.push_back(s.substr(inicio, pos - inicio));
        inicio = pos + 1;
        pos    = s.find(delim, inicio);
    }
    partes.push_back(s.substr(inicio));
    return partes;
}

static bool strContains(const std::string& s, char c) {
    return s.find(c) != std::string::npos;
}

static int strToInt(const std::string& s, bool& ok) {
    if (s.empty()) { ok = false; return 0; }
    try {
        size_t pos = 0;
        int val = std::stoi(s, &pos);
        ok = (pos == s.size());
        return ok ? val : 0;
    } catch (...) { ok = false; return 0; }
}

static bool tieneDigito(const std::string& s) {
    for (size_t i = 0; i < s.size(); i++)
        if (std::isdigit(static_cast<unsigned char>(s.at(i)))) return true;
    return false;
}

static std::string strRemoveChar(const std::string& s, char c) {
    std::string r;
    for (size_t i = 0; i < s.size(); i++)
        if (s.at(i) != c) r += s.at(i);
    return r;
}


static const std::vector<std::string> ESPECIALIDADES_VALIDAS = {
    "CARDIOLOGIA", "NEUROLOGIA", "PEDIATRIA",
    "CIRUGIA", "MEDICINA_GENERAL", "ONCOLOGIA"
};
static const std::vector<std::string> DOSIS_VALIDAS = {
    "DIARIA", "CADA_8_HORAS", "CADA_12_HORAS", "SEMANAL"
};
static const std::vector<std::string> SANGRES_VALIDAS = {
    "O+","O-","A+","A-","B+","B-","AB+","AB-"
};

static bool listaContiene(const std::vector<std::string>& lista, const std::string& val) {
    for (size_t i = 0; i < lista.size(); i++)
        if (lista.at(i) == val) return true;
    return false;
}


Scanner::Scanner() {}

QList<ErrorLexico> Scanner::getErrores()   { return this->errores; }
QList<Paciente>    Scanner::getPacientes() { return this->listaPacientes; }
QList<Medico>      Scanner::getMedicos()   { return this->listaMedicos; }

void Scanner::registrarError(QString lex, QString tipo, QString desc,
                             int l, int c, QString gravedad) {
    ErrorLexico e;
    e.no          = errores.size() + 1;
    e.lexema      = lex;
    e.tipo        = tipo;
    e.descripcion = desc;
    e.linea       = l;
    e.columna     = c;
    e.gravedad    = gravedad;
    errores.append(e);
}

// Wrapper: registrar error usando std::string — convierte a QString al final
static void regErr(Scanner* sc, const std::string& lex, const std::string& tipo,
                   const std::string& desc, int l, int c, const std::string& grav) {
    sc->registrarError(QString::fromStdString(lex),
                       QString::fromStdString(tipo),
                       QString::fromStdString(desc),
                       l, c,
                       QString::fromStdString(grav));
}


QList<Token> Scanner::analizar(QString entradaQt) {
    QList<Token> lista;
    this->errores.clear();

    std::string entrada = entradaQt.toStdString();
    entrada += ' ';

    int         estado       = 0;
    std::string lexema;
    int         linea        = 1;
    int         columna      = 1;
    int         colInicioStr = 1;

    const int N = static_cast<int>(entrada.size());

    auto emitir = [&](TipoToken tipo, const std::string& lex, int lin, int col) {
        lista.append({ tipo, QString::fromStdString(lex), lin, col });
    };

    for (int i = 0; i < N; i++) {
        char c = entrada.at(i);

        switch (estado) {

        case 0:
            if (c == '\n') {
                linea++; columna = 1;
            }
            else if (c == ' ' || c == '\t' || c == '\r') {
                columna++;
            }
            else if (std::isalpha(static_cast<unsigned char>(c)) ||
                     std::isdigit(static_cast<unsigned char>(c))) {
                estado   = 1;
                lexema  += c;
                columna++;
            }
            else if (c == '"') {
                estado       = 3;
                lexema       = "";
                colInicioStr = columna;
                columna++;
            }
            else if (c == '{') { emitir(LLAVE_A,    "{", linea, columna++); }
            else if (c == '}') { emitir(LLAVE_C,    "}", linea, columna++); }
            else if (c == '[') { emitir(CORCHETE_A, "[", linea, columna++); }
            else if (c == ']') { emitir(CORCHETE_C, "]", linea, columna++); }
            else if (c == ':') { emitir(DOS_PUNTOS,  ":", linea, columna++); }
            else if (c == ',') { emitir(COMA,        ",", linea, columna++); }
            else if (c == ';') { emitir(PUNTO_COMA,  ";", linea, columna++); }
            else {
                std::string ch(1, c);
                regErr(this, ch, "Carácter ilegal",
                       "El carácter '" + ch + "' no es reconocido.",
                       linea, columna++, "ERROR");
            }
            break;

        case 1:
            if (std::isalnum(static_cast<unsigned char>(c)) ||
                c == '_' || c == '-' || c == '.') {
                lexema += c;
                columna++;
            }
            else if (c == ':' &&
                     std::isdigit(static_cast<unsigned char>(lexema.at(0))) &&
                     lexema.size() <= 2)
            {
                int k = i + 1;
                std::string mins;
                // Leer dígitos de los minutos con at()
                while (k < N && std::isdigit(static_cast<unsigned char>(entrada.at(k)))) {
                    mins += entrada.at(k);
                    k++;
                }
                if (mins.size() == 2) {
                    std::string horaCompleta = lexema + ":" + mins;
                    int colReal = columna - static_cast<int>(lexema.size());
                    bool okHr, okMin;
                    int hr  = strToInt(lexema, okHr);
                    int min = strToInt(mins,   okMin);
                    if (!okHr || !okMin || hr < 0 || hr > 23 || min < 0 || min > 59) {
                        regErr(this, horaCompleta, "Hora fuera de rango",
                               "La hora '" + horaCompleta + "' está fuera del rango "
                                                            "permitido (00:00-23:59). Formato esperado: HH:MM.",
                               linea, colReal, "ERROR");
                    }
                    emitir(NUMERO, horaCompleta, linea, colReal);
                    columna += 1 + static_cast<int>(mins.size());
                    i = k - 1;
                } else {
                    int colReal = columna - static_cast<int>(lexema.size());
                    emitir(NUMERO, lexema, linea, colReal);
                    i--; // re-evaluar ':'
                }
                estado = 0;
                lexema = "";
            }
            else {
                std::string aux     = strToUpper(lexema);
                int         colReal = columna - static_cast<int>(lexema.size());

                // Palabras reservadas — comparación con std::string::operator==
                if      (aux == "HOSPITAL")     emitir(HOSPITAL,     lexema, linea, colReal);
                else if (aux == "PACIENTES")    emitir(PACIENTES,    lexema, linea, colReal);
                else if (aux == "MEDICOS")      emitir(MEDICOS,      lexema, linea, colReal);
                else if (aux == "CITAS")        emitir(CITAS,        lexema, linea, colReal);
                else if (aux == "DIAGNOSTICOS") emitir(DIAGNOSTICOS, lexema, linea, colReal);

                else if (std::isdigit(static_cast<unsigned char>(lexema.at(0)))) {
                    if (strContains(lexema, '-')) {
                        std::vector<std::string> p = strSplit(lexema, '-');
                        if (p.size() == 3) {
                            bool okMes, okDia;
                            int mes = strToInt(p.at(1), okMes);
                            int dia = strToInt(p.at(2), okDia);
                            if (!okMes || mes < 1 || mes > 12) {
                                regErr(this, lexema, "Fecha con mes inválido",
                                       "El mes '" + p.at(1) + "' está fuera del rango (01-12). "
                                                              "Formato esperado: AAAA-MM-DD.",
                                       linea, colReal, "ERROR");
                            } else if (!okDia || dia < 1 || dia > 31) {
                                regErr(this, lexema, "Fecha con día inválido",
                                       "El día '" + p.at(2) + "' está fuera del rango (01-31). "
                                                              "Formato esperado: AAAA-MM-DD.",
                                       linea, colReal, "ERROR");
                            }
                        }
                    }
                    emitir(NUMERO, lexema, linea, colReal);
                }

                else {
                    if (listaContiene(ESPECIALIDADES_VALIDAS, aux) ||
                        listaContiene(DOSIS_VALIDAS, aux)) {
                        emitir(ID_MEDICO, lexema, linea, colReal);
                    }
                    else if (std::isupper(static_cast<unsigned char>(lexema.at(0)))) {
                        bool esValida = listaContiene(ESPECIALIDADES_VALIDAS, aux) ||
                                        listaContiene(DOSIS_VALIDAS, aux);
                        if (!esValida) {
                            regErr(this, lexema, "Especialidad no reconocida",
                                   "'" + lexema + "' no es una especialidad válida. "
                                                  "Use: CARDIOLOGIA, NEUROLOGIA, PEDIATRIA, CIRUGIA, "
                                                  "MEDICINA_GENERAL u ONCOLOGIA.",
                                   linea, colReal, "ERROR");
                            emitir(ERROR, lexema, linea, colReal);
                        } else {
                            emitir(ID_MEDICO, lexema, linea, colReal);
                        }
                    }
                    else {
                        emitir(ID_MEDICO, lexema, linea, colReal);
                    }
                }

                estado = 0;
                lexema = "";
                i--;
            }
            break;

        case 3:
            if (c == '"') {
                std::string val   = strTrim(lexema);
                std::string valUP = strToUpper(val);

                if ((strContains(val, '+') || strContains(val, '-')) &&
                    val.size() >= 2 && val.size() <= 4)
                {
                    bool esFecha = strContains(val, '-') && strSplit(val, '-').size() == 3;
                    if (!esFecha) {
                        if (!listaContiene(SANGRES_VALIDAS, valUP)) {
                            regErr(this, "\"" + lexema + "\"", "Tipo de sangre inválido",
                                   "El valor '" + val + "' no es un tipo de sangre reconocido. "
                                                        "Valores válidos: A+, A-, B+, B-, O+, O-, AB+, AB-.",
                                   linea, colInicioStr, "ERROR");
                        }
                    }
                }
                else if (strContains(val, '-') && val.size() > 3 && val.size() < 10 &&
                         strSplit(val, '-').size() != 3 && !tieneDigito(val))
                {
                    regErr(this, "\"" + lexema + "\"", "Tipo de sangre inválido",
                           "Formato incorrecto. Valores válidos: A+, A-, B+, B-, O+, O-, AB+, AB-.",
                           linea, colInicioStr, "ERROR");
                }

                if (strContains(val, '-')) {
                    std::vector<std::string> p = strSplit(val, '-');
                    if (p.size() == 3) {
                        bool okMes, okDia;
                        int mes = strToInt(p.at(1), okMes);
                        int dia = strToInt(p.at(2), okDia);
                        if (!okMes || mes < 1 || mes > 12) {
                            regErr(this, "\"" + lexema + "\"", "Fecha con mes inválido",
                                   "El mes '" + p.at(1) + "' está fuera del rango (01-12). "
                                                          "Formato esperado: AAAA-MM-DD.",
                                   linea, colInicioStr, "ERROR");
                        } else if (!okDia || dia < 1 || dia > 31) {
                            regErr(this, "\"" + lexema + "\"", "Fecha con día inválido",
                                   "El día '" + p.at(2) + "' está fuera del rango (01-31). "
                                                          "Formato esperado: AAAA-MM-DD.",
                                   linea, colInicioStr, "ERROR");
                        }
                    }
                }

                if (strContains(val, ':')) {
                    std::vector<std::string> partes = strSplit(val, ':');
                    if (partes.size() == 2 &&
                        partes.at(0).size() >= 1 && partes.at(0).size() <= 2 &&
                        partes.at(1).size() == 2 &&
                        tieneDigito(partes.at(0)) && tieneDigito(partes.at(1)))
                    {
                        bool okHr, okMin;
                        int hr  = strToInt(partes.at(0), okHr);
                        int min = strToInt(partes.at(1), okMin);
                        if (!okHr || !okMin || hr < 0 || hr > 23 || min < 0 || min > 59) {
                            regErr(this, "\"" + lexema + "\"", "Hora fuera de rango",
                                   "La hora '" + val + "' está fuera del rango (00:00-23:59). "
                                                       "Formato esperado: HH:MM.",
                                   linea, colInicioStr, "ERROR");
                        }
                    }
                }

                emitir(CADENA, lexema, linea, colInicioStr);
                lexema = "";
                estado = 0;
                columna++;

            } else if (c == '\n' || i == N - 1) {
                regErr(this, "\"" + lexema, "Cadena sin cerrar",
                       "Se encontró el inicio de una cadena (comilla doble) "
                       "pero no se detectó el cierre antes del fin de la línea.",
                       linea, colInicioStr, "CRÍTICO");
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


    this->listaPacientes.clear();
    this->listaMedicos.clear();

    auto qLex = [&](int idx) -> std::string {
        return lista.at(idx).lexema.toStdString();
    };
    auto lexLower = [&](int idx) -> std::string {
        return strToLower(lista.at(idx).lexema.toStdString());
    };
    auto qStrip = [](const std::string& s) -> std::string {
        return strRemoveChar(strTrim(s), '"');
    };

    for (int j = 0; j < lista.size(); j++) {
        std::string lex = lexLower(j);

        if (lex == "paciente" && j+2 < lista.size() && qLex(j+1) == ":") {
            Paciente p;
            p.nombre = QString::fromStdString(qStrip(qLex(j+2)));
            for (int k = j+2; k < lista.size() && qLex(k) != "]"; k++) {
                std::string campo = lexLower(k);
                if (campo == "edad"        && k+2 < lista.size())
                    p.edad = lista.at(k+2).lexema.toInt();
                if (campo == "tipo_sangre" && k+2 < lista.size())
                    p.sangre = QString::fromStdString(qStrip(qLex(k+2)));
                if (campo == "habitacion"  && k+2 < lista.size())
                    p.habitacion = lista.at(k+2).lexema.toInt();
            }
            this->listaPacientes.append(p);
        }
        else if (lex == "medico" && j+2 < lista.size() && qLex(j+1) == ":") {
            Medico m;
            m.nombre = QString::fromStdString(qStrip(qLex(j+2)));
            for (int k = j+2; k < lista.size() && qLex(k) != "]"; k++) {
                std::string campo = lexLower(k);
                if (campo == "especialidad" && k+2 < lista.size())
                    m.especialidad = lista.at(k+2).lexema;
                if (campo == "codigo" && k+2 < lista.size())
                    m.codigo = QString::fromStdString(qStrip(qLex(k+2)));
            }
            this->listaMedicos.append(m);
        }
    }

    for (int j = 0; j < lista.size(); j++) {
        std::string lex = lexLower(j);
        if (lex.find("diag") != std::string::npos &&
            j+2 < lista.size() && qLex(j+1) == ":")
        {
            std::string nombrePac = qStrip(qLex(j+2));
            for (int pi = 0; pi < this->listaPacientes.size(); pi++) {
                if (this->listaPacientes[pi].nombre.toStdString() == nombrePac) {
                    for (int k = j+2; k < lista.size() && qLex(k) != "]"; k++) {
                        std::string campo = lexLower(k);
                        if (campo.find("condic") != std::string::npos && k+2 < lista.size()) {
                            this->listaPacientes[pi].condicion =
                                QString::fromStdString(qStrip(qLex(k+2)));
                            this->listaPacientes[pi].estado = "CON DIAG.";
                        }
                        if (campo == "medicamento" && k+2 < lista.size()) {
                            this->listaPacientes[pi].medicamento =
                                QString::fromStdString(qStrip(qLex(k+2)));
                        }
                    }
                    break;
                }
            }
        }
    }

    return lista;
}