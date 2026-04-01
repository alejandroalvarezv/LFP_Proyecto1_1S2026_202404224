#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "scanner.h"    // Importante para usar tu clase de análisis
#include <QDebug>       // Para poder ver los resultados en la consola de abajo

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Aquí puedes poner un título a tu ventana si quieres
    this->setWindowTitle("MedLexer - Proyecto 1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Esta es la función que se activa al darle clic al botón
// NOTA: Si al botón le pusiste otro nombre en el diseño (objectName),
// asegúrate de que el nombre de esta función coincida.
void MainWindow::on_pushButton_clicked()
{
    // 1. Capturamos lo que escribiste en el cuadro de texto
    // "plainTextEdit" es el nombre por defecto que Qt le da al cuadro
    QString textoEntrada = ui->plainTextEdit->toPlainText();

    // 2. Si el cuadro está vacío, no hacemos nada
    if(textoEntrada.isEmpty()){
        qDebug() << "El cuadro de texto está vacío.";
        return;
    }

    // 3. Llamamos a tu clase Scanner para que haga su magia
    Scanner miScanner;
    QList<Token> listaTokens = miScanner.analizar(textoEntrada);

    // 4. Mostramos los resultados en la pestaña "Application Output" (abajo)
    qDebug() << "===============================";
    qDebug() << "INICIANDO ANÁLISIS LÉXICO...";
    qDebug() << "Tokens encontrados:" << listaTokens.size();

    for(int i = 0; i < listaTokens.size(); i++) {
        // Esto te servirá para ver si tu AFD está reconociendo bien las palabras
        qDebug() << "Linea:" << listaTokens[i].linea
                 << " | Lexema:" << listaTokens[i].lexema;
    }

    qDebug() << "ANÁLISIS FINALIZADO.";
    qDebug() << "===============================";
}