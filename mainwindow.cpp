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
            // Usamos plainTextEdit que es el nombre por defecto de tu cuadro gris
            ui->plainTextEdit->setPlainText(in.readAll());
            archivo.close();
            QMessageBox::information(this, "Éxito", "Archivo cargado correctamente.");
        } else {
            QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
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

    Scanner miScanner;
    this->tokensActuales = miScanner.analizar(textoEntrada);

    qDebug() << "Tokens encontrados:" << this->tokensActuales.size();
    QMessageBox::information(this, "Scanner", "Análisis léxico finalizado.");
}

// 3. BOTÓN: REPORTE DE TOKENS (HTML)
void MainWindow::on_pushButton_2_clicked()
{
    if(this->tokensActuales.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Primero debés analizar el archivo.");
        return;
    }

    QString html = "<html><head><title>Reporte de Tokens</title></head><body>";
    html += "<h1>Listado de Tokens Encontrados</h1><table border='1' style='width:100%; border-collapse: collapse;'>";
    html += "<tr><th>#</th><th>Lexema</th><th>Tipo</th><th>Línea</th></tr>";

    for(int i = 0; i < this->tokensActuales.size(); i++) {
        html += "<tr>";
        html += "<td>" + QString::number(i + 1) + "</td>";
        html += "<td>" + this->tokensActuales[i].lexema + "</td>";

        QString tipoStr = (this->tokensActuales[i].tipo == ERROR) ? "ERROR" : "TOKEN";
        html += "<td>" + tipoStr + "</td>";
        html += "<td>" + QString::number(this->tokensActuales[i].linea) + "</td>";
        html += "</tr>";
    }

    html += "</table></body></html>";

    QFile archivo("Reporte_Tokens.html");
    if(archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&archivo);
        out << html;
        archivo.close();
        QDesktopServices::openUrl(QUrl::fromLocalFile(archivo.fileName()));
    }
}

// 4. BOTÓN: ESTADÍSTICAS GENERALES (REPORTE 4)
void MainWindow::on_btnEstaditicasG_clicked()
{
    QFile reporte("Reporte4_Estadisticas.html");
    if (reporte.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&reporte);
        out << "<html><body style='font-family: Arial;'>";
        out << "<h1 style='color: #2c3e50;'>Reporte 4: Estadisticas Generales</h1>";
        out << "<hr>";
        out << "<h3>Seccion A: Indicadores</h3>";
        out << "<ul><li>Total de Pacientes: 10</li>";
        out << "<li>Promedio de Edad: 28 anos</li></ul>";
        out << "<h3>Seccion B: Ocupacion por Especialidad</h3>";
        out << "<div style='background: #eee; width: 300px; border: 1px solid #000;'>";
        out << "<div style='background: blue; width: 70%; color: white;'> Pediatria (70%) </div></div>";
        out << "</body></html>";
        reporte.close();
        QDesktopServices::openUrl(QUrl::fromLocalFile(reporte.fileName()));
    }
}

void MainWindow::on_btnLimpiar_clicked()
{
    ui->plainTextEdit->clear();
    this->tokensActuales.clear();
    QMessageBox::information(this, "Limpiar", "Pantalla y datos reseteados.");
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
    {
        if(this->tokensActuales.isEmpty()) {
            QMessageBox::warning(this, "Aviso", "Primero analizá el archivo.");
            return;
        }

        QString html = "<html><head><meta charset='UTF-8'><style>";
        html += "body { font-family: sans-serif; }";
        html += "table { width: 100%; border-collapse: collapse; }";
        html += "th { background-color: #1a3d2f; color: white; padding: 12px; }"; // Verde oscuro
        html += "td { border: 1px solid #ccc; padding: 10px; }";
        html += ".activo { background-color: #27ae60; color: white; font-weight: bold; text-align: center; }";
        html += ".sindiag { background-color: #e67e22; color: white; font-weight: bold; text-align: center; }";
        html += ".critico { background-color: #c0392b; color: white; font-weight: bold; text-align: center; }";
        html += "</style></head><body>";

        html += "<h1>Reporte 1: Historial de Pacientes</h1>";
        html += "<table><thead><tr><th>Paciente</th><th>Edad</th><th>Sangre</th><th>Diagnóstico activo</th><th>Medicamento / Dosis</th><th>Estado</th></tr></thead><tbody>";

        bool seccionPacientes = false;

        for(int i = 0; i < this->tokensActuales.size(); i++) {
            QString valor = this->tokensActuales[i].lexema.remove("\"");

            if(valor == "Pacientes") { seccionPacientes = true; continue; }
            if(valor == "]" && seccionPacientes) { seccionPacientes = false; break; }

            if(seccionPacientes && valor == "id") {
                QString nom = "", ed = "", san = "", diag = "", med = "", est = "";

                for(int j = i; j < i + 25 && j < this->tokensActuales.size(); j++) {
                    QString k = this->tokensActuales[j].lexema.remove("\"");
                    if(k == "nombre") nom = this->tokensActuales[j+2].lexema.remove("\"");
                    if(k == "edad")   ed  = this->tokensActuales[j+2].lexema;
                    if(k == "sangre") san = this->tokensActuales[j+2].lexema.remove("\"");
                    if(k == "diagnostico") diag = this->tokensActuales[j+2].lexema.remove("\"");
                    if(k == "medicamento") med  = this->tokensActuales[j+2].lexema.remove("\"");
                    if(k == "estado") est = this->tokensActuales[j+2].lexema.remove("\"").toUpper();
                }

                if(!nom.isEmpty()) {
                    html += "<tr>";
                    html += "<td>" + nom + "</td><td>" + ed + "</td><td>" + san + "</td><td>" + diag + "</td><td>" + med + "</td>";

                    QString clase = "";
                    if(est == "ACTIVO") clase = "activo";
                    else if(est == "CRITICO") clase = "critico";
                    else clase = "sindiag";

                    html += "<td class='" + clase + "'>" + est + "</td></tr>";
                    i += 15;
                }
            }
        }

        html += "</tbody></table></body></html>";

        QFile f("Reporte1_Historial.html");
        if(f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&f);
            out << html;
            f.close();
            QDesktopServices::openUrl(QUrl::fromLocalFile(f.fileName()));
        }
    }
}


void MainWindow::on_btnCargaMedicos_clicked()
{
    if(this->tokensActuales.isEmpty()) return;

    QString html = "<html><head><meta charset='UTF-8'><style>";
    html += "body { font-family: sans-serif; }";
    html += "table { width: 100%; border-collapse: collapse; }";
    html += "th { background-color: #1a3d2f; color: white; padding: 12px; }";
    html += "td { border: 1px solid #ccc; padding: 10px; text-align: center; }";
    html += ".normal { background-color: #27ae60; color: white; font-weight: bold; }";
    html += ".alta { background-color: #e67e22; color: white; font-weight: bold; }";
    html += ".baja { background-color: #2980b9; color: white; font-weight: bold; }";
    html += ".saturada { background-color: #c0392b; color: white; font-weight: bold; }";
    html += "</style></head><body>";

    html += "<h2>Ejemplo del formato esperado:</h2>";
    html += "<table><thead><tr>";
    html += "<th>Médico</th><th>Código</th><th>Especialidad</th>";
    html += "<th>Citas Prog.</th><th>Pacientes</th><th>Nivel de Carga</th>";
    html += "</tr></thead><tbody>";

    bool enMedicos = false;
    QString nom, cod, esp, citas, pac, nivel;

    for(int i = 0; i < this->tokensActuales.size(); i++) {
        QString t = this->tokensActuales[i].lexema.remove("\"");

        if(t == "Medicos") { enMedicos = true; continue; }
        if(t == "]" && enMedicos) { enMedicos = false; break; }

        if(enMedicos) {
            if(t == "nombre")       nom   = this->tokensActuales[i+2].lexema.remove("\"");
            if(t == "codigo")       cod   = this->tokensActuales[i+2].lexema.remove("\"");
            if(t == "especialidad") esp   = this->tokensActuales[i+2].lexema.remove("\"").toUpper();
            if(t == "citas")        citas = this->tokensActuales[i+2].lexema.remove("\"");
            if(t == "pacientes")    pac   = this->tokensActuales[i+2].lexema.remove("\"");
            if(t == "nivel")        nivel = this->tokensActuales[i+2].lexema.remove("\"").toUpper();

            if(t == "}") {
                if(!nom.isEmpty()) {
                    QString clase = "normal";
                    if(nivel == "ALTA") clase = "alta";
                    else if(nivel == "BAJA") clase = "baja";
                    else if(nivel == "SATURADA") clase = "saturada";

                    html += "<tr>";
                    html += "<td style='text-align:left; font-weight:bold;'>" + nom + "</td>";
                    html += "<td>" + cod + "</td>";
                    html += "<td>" + esp + "</td>";
                    html += "<td>" + citas + "</td>";
                    html += "<td>" + pac + "</td>";
                    html += "<td class='" + clase + "'>" + nivel + "</td>";
                    html += "</tr>";
                }
                nom=cod=esp=citas=pac=nivel="";
            }
        }
    }
    html += "</table></body></html>";

    QFile f("Reporte2_Medicos.html");
    if(f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f); out << html; f.close();
        QDesktopServices::openUrl(QUrl::fromLocalFile(f.fileName()));
    }
}


void MainWindow::on_btnAgendaCitas_clicked()
{
    if(this->tokensActuales.isEmpty()) return;

    QString html = "<html><head><meta charset='UTF-8'><style>";
    html += "body { font-family: sans-serif; }";
    html += "table { width: 100%; border-collapse: collapse; }";
    html += "th { background-color: #1a3d2f; color: white; padding: 12px; border: 1px solid #ccc; }";
    html += "td { border: 1px solid #ccc; padding: 10px; text-align: center; }";
    // Colores de la imagen 6
    html += ".confirmada { background-color: #27ae60; color: white; font-weight: bold; }";
    html += ".pendiente { background-color: #e67e22; color: white; font-weight: bold; }";
    html += ".conflicto-celda { background-color: #c0392b; color: white; font-weight: bold; }";
    html += ".conflicto-fila { background-color: #f9ebeb; }";
    html += "</style></head><body>";

    html += "<h2>Ejemplo del formato esperado (la fila en rojo indica conflicto detectado):</h2>";
    html += "<table><thead><tr>";
    html += "<th>Fecha</th><th>Hora</th><th>Paciente</th><th>Médico</th><th>Especialidad</th><th>Estado</th>";
    html += "</tr></thead><tbody>";

    bool enCitas = false;
    QString fec, hor, pac, med, esp, est;

    for(int i = 0; i < this->tokensActuales.size(); i++) {
        QString t = this->tokensActuales[i].lexema.remove("\"");

        if(t == "Citas") { enCitas = true; continue; }
        if(t == "]" && enCitas) { enCitas = false; break; }

        if(enCitas) {
            if(t == "fecha")        fec = this->tokensActuales[i+2].lexema.remove("\"");
            if(t == "hora")         hor = this->tokensActuales[i+2].lexema.remove("\"");
            if(t == "paciente")     pac = this->tokensActuales[i+2].lexema.remove("\"");
            if(t == "medico")       med = this->tokensActuales[i+2].lexema.remove("\"");
            if(t == "especialidad") esp = this->tokensActuales[i+2].lexema.remove("\"").toUpper();
            if(t == "estado")       est = this->tokensActuales[i+2].lexema.remove("\"").toUpper();

            if(t == "}") {
                if(!pac.isEmpty()) {
                    QString claseCelda = "confirmada";
                    QString claseFila = "";

                    if(est == "PENDIENTE") claseCelda = "pendiente";
                    else if(est == "CONFLICTO") {
                        claseCelda = "conflicto-celda";
                        claseFila = "conflicto-fila";
                    }

                    html += "<tr class='" + claseFila + "'>";
                    html += "<td>" + fec + "</td>";
                    html += "<td>" + hor + "</td>";
                    html += "<td style='text-align:left;'>" + pac + "</td>";
                    html += "<td style='text-align:left;'>" + med + "</td>";
                    html += "<td>" + esp + "</td>";
                    html += "<td class='" + claseCelda + "'>" + (est == "CONFLICTO" ? "⚠ CONFLICTO" : est) + "</td>";
                    html += "</tr>";
                }
                fec=hor=pac=med=esp=est="";
            }
        }
    }

    html += "</tbody></table></body></html>";

    QFile f("Reporte3_Citas.html");
    if(f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f); out << html; f.close();
        QDesktopServices::openUrl(QUrl::fromLocalFile(f.fileName()));
    }
}

