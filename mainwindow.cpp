#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "scanner.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MedLexer - Proyecto 1 - USAC");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 1. BOTÓN: CARGAR ARCHIVO (.med)
void MainWindow::on_btnCargar_clicked()
{
    QString nombre = QFileDialog::getOpenFileName(this, "Seleccionar archivo MED", "", "Archivos MED (*.med);;Todos los archivos (*.*)");

    if (!nombre.isEmpty()) {
        QFile archivo(nombre);
        if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&archivo);
            QString contenido = in.readAll();
            ui->plainTextEdit->setPlainText(contenido);
            archivo.close();

            // --- PROCESAMIENTO ---
            Scanner scanner;
            this->tokensActuales = scanner.analizar(contenido);
            this->listaErrores = scanner.getErrores();
            this->listaPacientesGlobal = scanner.getPacientes();
            this->listaMedicosGlobal = scanner.getMedicos();
            // ---------------------

            QMessageBox::information(this, "Éxito", "Archivo analizado y listas llenas.");
        }
    }
}

// 2. BOTÓN: ANALIZAR (Scanner)
void MainWindow::on_pushButton_clicked()
{
    QString textoEntrada = ui->plainTextEdit->toPlainText();

    if(textoEntrada.isEmpty()){
        QMessageBox::warning(this, "Aviso", "El cuadro de texto está vacío.");
        return;
    }
    this->tokensActuales.clear();
    this->listaErrores.clear();

    Scanner miScanner;
    this->tokensActuales = miScanner.analizar(textoEntrada);

    this->listaErrores = miScanner.getErrores();

    for (int k = 0; k < this->listaErrores.size(); k++) {
        this->listaErrores[k].no = k + 1;
    }

    if (!this->listaErrores.isEmpty()) {
        QMessageBox::critical(this, "Análisis finalizado",
                              "Se detectaron " + QString::number(listaErrores.size()) + " error(es) léxico(s).\n"
                                                                                        "Presiona 'Reporte de Errores' para ver el detalle.");
    } else {
        QMessageBox::information(this, "Análisis finalizado",
                                 "✔ Análisis léxico exitoso. No se encontraron errores.");
    }

    if(!this->listaErrores.isEmpty()) {
        QMessageBox::critical(this, "Análisis finalizado",
                              "Se detectaron " + QString::number(listaErrores.size()) + " errores.");
    } else {
        QMessageBox::information(this, "Análisis finalizado",
                                 "Análisis léxico exitoso.");
    }
}


// 4. BOTÓN: ESTADÍSTICAS GENERALES (REPORTE 4)
void MainWindow::on_btnEstaditicasG_clicked()
{
    if(this->tokensActuales.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Primero analizá el archivo.");
        return;
    }

    struct CitaAux {
        QString medico, fecha, hora;
    };
    struct DatosEsp {
        int numMedicos = 0;
        int numCitas = 0;
        QSet<QString> pacientesUnicos;
    };

    QString nombreHospital = "Hospital General";
    int totalPacientes = 0, totalMedicos = 0, totalCitas = 0, conflictos = 0;
    double sumaEdades = 0;
    QSet<QString> pacientesConDiag;
    QMap<QString, int> conteoMedicamentos;
    QMap<QString, int> citasPorMedico;
    QMap<QString, QString> especialidadDeMedico;
    QList<CitaAux> listaParaConflictos;
    QMap<QString, DatosEsp> statsPorEspecialidad;

    bool enPac = false, enMed = false, enCit = false, enDiag = false;

    for(int i = 0; i < tokensActuales.size(); i++) {
        QString val = tokensActuales[i].lexema.remove("\"").toLower();

        if(val == "hospital" && (i + 1) < tokensActuales.size()) {
            if(tokensActuales[i+1].lexema != "{") {
                nombreHospital = tokensActuales[i+1].lexema.remove("\"");
            }
        }

        if(val == "pacientes") { enPac = true; continue; }
        if(val == "medicos") { enMed = true; continue; }
        if(val == "citas") { enCit = true; continue; }
        if(val == "diagnosticos") { enDiag = true; continue; }

        if(enPac && val == "paciente") {
            totalPacientes++;
            for(int j = i; j < i+10 && j < tokensActuales.size(); j++) {
                if(tokensActuales[j].lexema.toLower() == "edad") {
                    sumaEdades += tokensActuales[j+2].lexema.toInt();
                }
            }
        }
        else if(enMed && val == "medico") {
            totalMedicos++;
            QString nombre = tokensActuales[i+2].lexema.remove("\"");
            QString esp = "GENERAL";
            for(int j = i; j < i+15 && j < tokensActuales.size(); j++) {
                if(tokensActuales[j].lexema.toLower() == "especialidad") {
                    esp = tokensActuales[j+2].lexema.remove("\"");
                }
            }
            especialidadDeMedico[nombre] = esp;
            statsPorEspecialidad[esp].numMedicos++;
        }

        else if(enCit && val == "cita") {
            totalCitas++;
            QString pac = tokensActuales[i+2].lexema.remove("\"");
            QString med = tokensActuales[i+4].lexema.remove("\"");
            QString f, h;
            for(int j = i; j < i+15 && j < tokensActuales.size(); j++) {
                if(tokensActuales[j].lexema.toLower() == "fecha") f = tokensActuales[j+2].lexema.remove("\"");
                if(tokensActuales[j].lexema.toLower() == "hora") h = tokensActuales[j+2].lexema.remove("\"");
            }

            citasPorMedico[med]++;
            QString esp = especialidadDeMedico.value(med, "GENERAL");
            statsPorEspecialidad[esp].numCitas++;
            statsPorEspecialidad[esp].pacientesUnicos.insert(pac);

            for(const auto &existente : listaParaConflictos) {
                if(existente.medico == med && existente.fecha == f && existente.hora == h) {
                    conflictos++;
                    break;
                }
            }
            listaParaConflictos.append({med, f, h});
        }
        else if(enDiag && val == "diagnostico") {
            QString pac = tokensActuales[i+2].lexema.remove("\"");
            pacientesConDiag.insert(pac);
            for(int j = i; j < i+15 && j < tokensActuales.size(); j++) {
                if(tokensActuales[j].lexema.toLower() == "medicamento") {
                    conteoMedicamentos[tokensActuales[j+2].lexema.remove("\"")]++;
                }
            }
        }

        if(tokensActuales[i].lexema == "}") { enPac=enMed=enCit=enDiag=false; }
    }

    double promedioEdad = (totalPacientes > 0) ? ((double)sumaEdades / totalPacientes) : 0;
    int pacActivos = pacientesConDiag.size();
    double porcActivos = (totalPacientes > 0) ? ((double)pacActivos / totalPacientes) * 100 : 0;

    QString medTop = "N/A"; int maxMed = 0;
    for(auto it = conteoMedicamentos.begin(); it != conteoMedicamentos.end(); ++it) {
        if(it.value() > maxMed) { maxMed = it.value(); medTop = it.key(); }
    }

    QString medCarga = "N/A", espCarga = "N/A"; int maxCitas = 0;
    for(auto it = citasPorMedico.begin(); it != citasPorMedico.end(); ++it) {
        if(it.value() > maxCitas) {
            maxCitas = it.value();
            medCarga = it.key();
            espCarga = especialidadDeMedico.value(medCarga);
        }
    }

    QString html = "<html><head><meta charset='UTF-8'><style>";
    html += "body { font-family: 'Segoe UI', sans-serif; background-color: #f4f7f6; padding: 30px; }";
    html += "h2 { color: #1e3d33; border-bottom: 2px solid #1e3d33; padding-bottom: 10px; }";
    html += "table { width: 100%; border-collapse: collapse; margin-bottom: 40px; background: white; }";
    html += "th { background-color: #d1d9e6; color: #444; padding: 12px; text-align: left; border: 1px solid #b8c5d6; }";
    html += "td { padding: 12px; border: 1px solid #b8c5d6; }";
    html += ".val { font-weight: bold; color: #2c3e50; }";
    html += ".err { color: #c62828; font-weight: bold; }";
    html += ".bar-bg { width: 150px; background: #e0e0e0; height: 18px; border: 1px solid #ccc; display: inline-block; margin-right: 10px; vertical-align: middle; }";
    html += ".bar-fill { height: 100%; }";
    html += "</style></head><body>";

    html += "<h2>Sección A: Indicadores Clave</h2>";
    html += "<table><tr><th>Indicador</th><th>Valor</th></tr>";
    html += "<tr><td>Nombre del hospital</td><td class='val'>" + nombreHospital + "</td></tr>";
    html += "<tr><td>Total de pacientes registrados</td><td class='val'>" + QString::number(totalPacientes) + "</td></tr>";
    html += "<tr><td>Total de médicos activos</td><td class='val'>" + QString::number(totalMedicos) + "</td></tr>";
    html += "<tr><td>Total de citas programadas</td><td class='val'>" + QString::number(totalCitas) + "</td></tr>";
    html += "<tr><td>Citas con conflicto de horario</td><td class='err'>" + QString::number(conflictos) + " </td></tr>";
    html += "<tr><td>Pacientes con diagnóstico activo</td><td class='val'>" + QString::number(pacActivos) + " de " + QString::number(totalPacientes) + " (" + QString::number(porcActivos, 'f', 1) + "%)</td></tr>";
    html += "<tr><td>Medicamento más prescrito</td><td class='val'>" + medTop + " (" + QString::number(maxMed) + " pacientes)</td></tr>";
    html += "<tr><td>Especialidad con mayor carga</td><td class='val'>" + espCarga + " — " + medCarga + " (" + QString::number(maxCitas) + " citas)</td></tr>";
    html += "<tr><td>Promedio de edad de los pacientes</td><td class='val'>" + QString::number(promedioEdad, 'f', 1) + " años</td></tr></table>";

    html += "<h2>Sección B: Distribución por Especialidad</h2>";
    html += "<table><thead><tr><th>Especialidad</th><th>Médicos</th><th>Citas</th><th>Pacientes</th><th>Barra de ocupación</th></tr></thead><tbody>";

    for(auto it = statsPorEspecialidad.begin(); it != statsPorEspecialidad.end(); ++it) {
        double porc = (totalCitas > 0) ? ((double)it.value().numCitas / totalCitas) * 100 : 0;
        QString color = (porc > 80) ? "#c62828" : "#1a4e7a";
        html += "<tr><td><b>" + it.key() + "</b></td>";
        html += "<td>" + QString::number(it.value().numMedicos) + "</td>";
        html += "<td>" + QString::number(it.value().numCitas) + "</td>";
        html += "<td>" + QString::number(it.value().pacientesUnicos.size()) + "</td>";
        html += "<td><div class='bar-bg'><div class='bar-fill' style='width:"+QString::number(porc)+"%; background:"+color+";'></div></div>";
        html += "<span style='font-weight:bold; color:"+color+";'>" + QString::number(porc, 'f', 0) + "%</span></td></tr>";
    }

    html += "</tbody></table></body></html>";

    QFile f("Reporte4_Estadistico.html");
    if(f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f); out << html; f.close();
        QDesktopServices::openUrl(QUrl::fromLocalFile(f.fileName()));
    }
}


void MainWindow::on_btnReporteTokens_clicked()
{
    {
        if(this->tokensActuales.isEmpty()) {
            QMessageBox::warning(this, "Aviso", "Primero debés cargar un archivo y darle al botón 'Analizar'.");
            return;
        }

        QString html = "<html><head><meta charset='UTF-8'><title>Reporte de Tokens</title>";
        html += "<style>table{width:100%; border-collapse:collapse;} th,td{border:1px solid black; padding:8px; text-align:left;} th{background-color:#2c3e50; color:white;}</style></head><body>";
        html += "<h1>Listado de Tokens Encontrados</h1>";
        html += "<table><tr><th>#</th><th>Lexema</th><th>Línea</th></tr>";

        for(int i = 0; i < this->tokensActuales.size(); i++) {
            html += "<tr>";
            html += "<td>" + QString::number(i + 1) + "</td>";
            html += "<td>" + this->tokensActuales[i].lexema + "</td>";
            html += "<td>" + QString::number(this->tokensActuales[i].linea) + "</td>";
            html += "</tr>";
        }

        html += "</table></body></html>";

        QFile archivo("Reporte_Tokens.html");
        if(archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&archivo);
            out << html;
            archivo.close();
            QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(archivo).absoluteFilePath()));
        } else {
            QMessageBox::critical(this, "Error", "No se pudo crear el archivo HTML.");
        }
    }
}


void MainWindow::on_ReporteHistorialPacientes_clicked()
{
    if(this->tokensActuales.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Primero analizá el archivo.");
        return;
    }

    struct InfoDiag { QString cond, med, dos; };
    QMap<QString, InfoDiag> mapaDiagnosticos;
    QMap<QString, int> contadorDiags;

    bool enDiag = false;
    for(int i = 0; i < tokensActuales.size(); i++) {
        QString val = tokensActuales[i].lexema.remove("\"").toLower();
        if(val == "diagnosticos") enDiag = true;

        if(enDiag && val == "diagnostico") {
            QString pac = tokensActuales[i+2].lexema.remove("\"");

            contadorDiags[pac]++;

            InfoDiag d;
            for(int j = i; j < i + 25 && j < tokensActuales.size(); j++) {
                QString k = tokensActuales[j].lexema.remove("\"").toLower();
                if(k == "condicion") d.cond = tokensActuales[j+2].lexema.remove("\"");
                if(k == "medicamento") d.med = tokensActuales[j+2].lexema.remove("\"");
                if(k == "dosis") d.dos = tokensActuales[j+2].lexema.remove("\"");
            }
            mapaDiagnosticos[pac] = d;
        }
        if(enDiag && tokensActuales[i].lexema == "}") enDiag = false;
    }

    QString html = "<html><head><meta charset='UTF-8'><style>";
    html += "body { font-family: sans-serif; background-color: #f4f7f6; padding: 20px; }";
    html += "table { width: 100%; border-collapse: collapse; background: white; }";
    html += "th { background-color: #2c3e50; color: white; padding: 12px; text-align: left; }";
    html += "td { border: 1px solid #ddd; padding: 10px; }";
    html += ".status { padding: 5px 10px; border-radius: 4px; color: white; font-weight: bold; display: inline-block; }";
    html += ".activo { background-color: #27ae60; }";   // Verde
    html += ".sindiag { background-color: #e67e22; }";  // Naranja
    html += ".critico { background-color: #c0392b; }";  // Rojo
    html += "</style></head><body><h1>Reporte 1: Historial de Pacientes</h1>";
    html += "<table><thead><tr><th>Paciente</th><th>Edad</th><th>Sangre</th><th>Habitación</th><th>Diagnóstico</th><th>Medicamento</th><th>Estado</th></tr></thead><tbody>";

    bool enPac = false;
    for(int i = 0; i < tokensActuales.size(); i++) {
        QString val = tokensActuales[i].lexema.remove("\"").toLower();
        if(val == "pacientes") enPac = true;

        if(enPac && val == "paciente") {
            QString nom = tokensActuales[i+2].lexema.remove("\"");
            QString ed = "N/A", san = "N/A", hab = "N/A";

            for(int j = i; j < i + 15 && j < tokensActuales.size(); j++) {
                QString k = tokensActuales[j].lexema.remove("\"").toLower();
                if(k == "edad") ed = tokensActuales[j+2].lexema;
                if(k == "tipo_sangre") san = tokensActuales[j+2].lexema.remove("\"");
                if(k == "habitacion") hab = tokensActuales[j+2].lexema;
            }

            QString diag = "Sin registro", med = "N/A", est = "SIN DIAG.", clase = "sindiag";

            if(mapaDiagnosticos.contains(nom)) {
                diag = mapaDiagnosticos[nom].cond;
                med = mapaDiagnosticos[nom].med + " (" + mapaDiagnosticos[nom].dos + ")";

                bool marcadoUrgente = diag.toUpper().contains("URGENTE");
                bool multiplesDiags = (contadorDiags[nom] > 1);

                if(marcadoUrgente || multiplesDiags) {
                    est = "CRÍTICO";
                    clase = "critico";
                } else {
                    est = "ACTIVO";
                    clase = "activo";
                }
            }

            html += "<tr><td>" + nom + "</td><td>" + ed + "</td><td>" + san + "</td><td>" + hab + "</td>";
            html += "<td>" + diag + "</td><td>" + med + "</td>";
            html += "<td><span class='status " + clase + "'>" + est + "</span></td></tr>";
        }
        if(enPac && tokensActuales[i].lexema == "}") enPac = false;
    }

    html += "</tbody></table></body></html>";

    QFile f("Reporte1_Historial.html");
    if(f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f); out << html; f.close();
        QDesktopServices::openUrl(QUrl::fromLocalFile(f.fileName()));
    }
}

void MainWindow::on_btnCargaMedicos_clicked()
{
    if(this->tokensActuales.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Primero analizá el archivo.");
        return;
    }

    struct EstadisticasMedico {
        QString nombre;
        QString codigo;
        QString especialidad;
        int totalCitas = 0;
        QSet<QString> pacientesUnicos;
    };
    QMap<QString, EstadisticasMedico> mapaCarga;

    bool enMed = false;
    for(int i = 0; i < tokensActuales.size(); i++) {
        QString val = tokensActuales[i].lexema.remove("\"").toLower();
        if(val == "medicos") enMed = true;
        if(enMed && val == "medico") {
            QString nombreMed = tokensActuales[i+2].lexema.remove("\"");
            QString cod = "N/A", esp = "N/A";

            for(int j = i; j < i + 15 && j < tokensActuales.size(); j++) {
                QString k = tokensActuales[j].lexema.remove("\"").toLower();
                if(k == "codigo") cod = tokensActuales[j+2].lexema.remove("\"");
                if(k == "especialidad") esp = tokensActuales[j+2].lexema.remove("\"");
            }
            mapaCarga[nombreMed].nombre = nombreMed;
            mapaCarga[nombreMed].codigo = cod;
            mapaCarga[nombreMed].especialidad = esp;
        }
        if(enMed && tokensActuales[i].lexema == "}") enMed = false;
    }

    bool enCitas = false;
    for(int i = 0; i < tokensActuales.size(); i++) {
        QString val = tokensActuales[i].lexema.remove("\"").toLower();
        if(val == "citas") enCitas = true;
        if(enCitas && val == "cita") {
            QString pacNom = tokensActuales[i+2].lexema.remove("\"");
            QString medNom = "";
            if(tokensActuales[i+3].lexema.toLower() == "con") {
                medNom = tokensActuales[i+4].lexema.remove("\"");
            }

            if(mapaCarga.contains(medNom)) {
                mapaCarga[medNom].totalCitas++;
                mapaCarga[medNom].pacientesUnicos.insert(pacNom);
            }
        }
        if(enCitas && tokensActuales[i].lexema == "}") enCitas = false;
    }

    QString html = "<html><head><meta charset='UTF-8'><style>";
    html += "body { font-family: sans-serif; background-color: #f4f7f6; padding: 20px; }";
    html += "table { width: 100%; border-collapse: collapse; background: white; }";
    html += "th { background-color: #1e3d33; color: white; padding: 12px; text-align: left; border: 1px solid #ccc; }"; // Color verde oscuro similar a la imagen
    html += "td { border: 1px solid #ddd; padding: 10px; }";
    html += ".carga { padding: 5px 10px; border-radius: 4px; color: white; font-weight: bold; text-align: center; display: block; }";
    html += ".baja { background-color: #1a4e7a; } .normal { background-color: #2e7d32; }";
    html += ".alta { background-color: #ef6c00; } .saturada { background-color: #c62828; }";
    html += "</style></head><body><h1>Reporte 2: Carga de Médicos</h1>";

    html += "<table><thead><tr><th>Médico</th><th>Código</th><th>Especialidad</th><th>Citas Prog.</th><th>Pacientes</th><th>Nivel de Carga</th></tr></thead><tbody>";

    for(auto it = mapaCarga.begin(); it != mapaCarga.end(); ++it) {
        EstadisticasMedico m = it.value();
        int citas = m.totalCitas;

        QString nivel = "BAJA", clase = "baja";
        if(citas >= 4 && citas <= 6) { nivel = "NORMAL"; clase = "normal"; }
        else if(citas >= 7 && citas <= 8) { nivel = "ALTA"; clase = "alta"; }
        else if(citas >= 9) { nivel = "SATURADA"; clase = "saturada"; }

        html += "<tr>";
        html += "<td><b>" + m.nombre + "</b></td>";
        html += "<td>" + m.codigo + "</td>";
        html += "<td>" + m.especialidad + "</td>";
        html += "<td>" + QString::number(citas) + "</td>";
        html += "<td>" + QString::number(m.pacientesUnicos.size()) + "</td>";
        html += "<td><span class='carga " + clase + "'>" + nivel + "</span></td>";
        html += "</tr>";
    }

    html += "</tbody></table></body></html>";

    QFile f("Reporte2_CargaMedicos.html");
    if(f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f); out << html; f.close();
        QDesktopServices::openUrl(QUrl::fromLocalFile(f.fileName()));
    }
}



void MainWindow::on_btnAgendaCitas_clicked()
{
    if(this->tokensActuales.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Primero analizá el archivo.");
        return;
    }

    QMap<QString, QString> mapaEspecialidades;
    bool enMed = false;
    for(int i = 0; i < tokensActuales.size(); i++) {
        QString val = tokensActuales[i].lexema.remove("\"").toLower();
        if(val == "medicos") enMed = true;
        if(enMed && val == "medico") {
            QString nombreMed = tokensActuales[i+2].lexema.remove("\"");
            for(int j = i; j < i + 15 && j < tokensActuales.size(); j++) {
                if(tokensActuales[j].lexema.toLower() == "especialidad") {
                    mapaEspecialidades[nombreMed] = tokensActuales[j+2].lexema.remove("\"");
                }
            }
        }
        if(enMed && tokensActuales[i].lexema == "}") enMed = false;
    }

    struct Cita {
        QString fecha, hora, paciente, medico, especialidad, estado, clase;
    };
    QList<Cita> listaAgenda;

    bool enCitas = false;
    for(int i = 0; i < tokensActuales.size(); i++) {
        QString val = tokensActuales[i].lexema.remove("\"").toLower();
        if(val == "citas") enCitas = true;
        if(enCitas && val == "cita") {
            Cita c;
            c.paciente = tokensActuales[i+2].lexema.remove("\"");
            if(tokensActuales[i+3].lexema.toLower() == "con") {
                c.medico = tokensActuales[i+4].lexema.remove("\"");
            }

            for(int j = i; j < i + 20 && j < tokensActuales.size(); j++) {
                QString k = tokensActuales[j].lexema.remove("\"").toLower();
                if(k == "fecha") c.fecha = tokensActuales[j+2].lexema.remove("\"");
                if(k == "hora") c.hora = tokensActuales[j+2].lexema.remove("\"");
            }
            c.especialidad = mapaEspecialidades.value(c.medico, "N/A");

            c.estado = "CONFIRMADA";
            c.clase = "confirmada";

            if(c.fecha > "2025-04-10") {
                c.estado = "PENDIENTE";
                c.clase = "pendiente";
            }

            for(const Cita &existente : listaAgenda) {
                if(existente.medico == c.medico && existente.fecha == c.fecha && existente.hora == c.hora) {
                    c.estado = "CONFLICTO";
                    c.clase = "conflicto";
                    break;
                }
            }
            listaAgenda.append(c);
        }
        if(enCitas && tokensActuales[i].lexema == "}") enCitas = false;
    }

    QString html = "<html><head><meta charset='UTF-8'><style>";
    html += "body { font-family: sans-serif; background-color: #f4f7f6; padding: 20px; }";
    html += "table { width: 100%; border-collapse: collapse; background: white; }";
    html += "th { background-color: #1e3d33; color: white; padding: 12px; border: 1px solid #ccc; }";
    html += "td { border: 1px solid #ddd; padding: 10px; }";
    html += ".status { padding: 8px; border-radius: 4px; color: white; font-weight: bold; text-align: center; display: block; }";
    html += ".confirmada { background-color: #2e7d32; }"; // Verde
    html += ".conflicto { background-color: #c62828; }";  // Rojo
    html += ".pendiente { background-color: #ef6c00; }";  // Naranja
    html += ".fila-conflicto { background-color: #ffebee; }"; // Fondo rosado suave para la fila
    html += "</style></head><body><h1>Reporte 3: Agenda de Citas</h1>";
    html += "<table><thead><tr><th>Fecha</th><th>Hora</th><th>Paciente</th><th>Médico</th><th>Especialidad</th><th>Estado</th></tr></thead><tbody>";

    for(const Cita &c : listaAgenda) {
        QString filaEstilo = (c.clase == "conflicto") ? " class='fila-conflicto'" : "";
        html += "<tr" + filaEstilo + ">";
        html += "<td>" + c.fecha + "</td><td>" + c.hora + "</td>";
        html += "<td>" + c.paciente + "</td><td>" + c.medico + "</td>";
        html += "<td>" + c.especialidad + "</td>";
        html += "<td><span class='status " + c.clase + "'>" + c.estado + "</span></td></tr>";
    }

    html += "</tbody></table></body></html>";

    QFile f("Reporte3_Agenda.html");
    if(f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f); out << html; f.close();
        QDesktopServices::openUrl(QUrl::fromLocalFile(f.fileName()));
    }
}


void MainWindow::on_btnLimpiar_clicked()
{
    ui->plainTextEdit->clear();
    this->tokensActuales.clear();
    QMessageBox::information(this, "Limpiar", "Pantalla y datos limpios.");
}


void MainWindow::on_btnReporteErrores_clicked()
{
    if(this->listaErrores.isEmpty()) {
        QMessageBox::information(this, "Reporte de Errores", "¡Felicidades! No se encontraron errores léxicos en el archivo.");
        return;
    }

    QString html = "<html><head><meta charset='UTF-8'><style>";
    html += "body { font-family: sans-serif; background-color: #f4f7f6; padding: 20px; }";
    html += "table { width: 100%; border-collapse: collapse; background: white; }";
    html += "th { background-color: #1e3d33; color: white; padding: 12px; border: 1px solid #ddd; }";
    html += "td { padding: 10px; border: 1px solid #ddd; text-align: center; font-size: 14px; }";
    html += ".desc { text-align: left; }";
    html += ".error-cell { background-color: #c62828; color: white; font-weight: bold; }";
    html += ".critico-cell { background-color: #b71c1c; color: #ffcdd2; font-weight: bold; }";
    html += "</style></head><body>";

    html += "<h2>Reporte de Errores Léxicos</h2>";
    html += "<table><thead><tr>";
    html += "<th>No.</th><th>Lexema</th><th>Tipo de Error</th><th>Descripción</th><th>Línea</th><th>Columna</th><th>Gravedad</th>";
    html += "</tr></thead><tbody>";

    for(const auto &e : listaErrores) {
        QString claseGravedad = (e.gravedad == "CRÍTICO") ? "critico-cell" : "error-cell";

        html += "<tr>";
        html += "<td>" + QString::number(e.no) + "</td>";
        html += "<td>" + e.lexema + "</td>";
        html += "<td>" + e.tipo + "</td>";
        html += "<td class='desc'>" + e.descripcion + "</td>";
        html += "<td>" + QString::number(e.linea) + "</td>";
        html += "<td>" + QString::number(e.columna) + "</td>";
        html += "<td class='" + claseGravedad + "'>" + e.gravedad + "</td>";
        html += "</tr>";
    }

    html += "</tbody></table></body></html>";

    QFile f("Reporte_Errores.html");
    if(f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f);
        out << html;
        f.close();
        QDesktopServices::openUrl(QUrl::fromLocalFile(f.fileName()));
    }
}

void MainWindow::on_btnJerarquia_clicked()
{

}


void MainWindow::on_btnGenerarReporteGrap_clicked()
{

}

