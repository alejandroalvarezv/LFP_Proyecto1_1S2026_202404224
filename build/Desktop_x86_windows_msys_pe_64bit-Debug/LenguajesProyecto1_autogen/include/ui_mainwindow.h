/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPlainTextEdit *plainTextEdit;
    QPushButton *pushButton;
    QPushButton *btnEstaditicasG;
    QPushButton *btnCargar;
    QPushButton *bntLimpiar;
    QPushButton *btnReporteTokens;
    QPushButton *btnReporteErrores;
    QPushButton *ReporteHistorialPacientes;
    QPushButton *btnCargaMedicos;
    QPushButton *btnAgendaCitas;
    QPushButton *btnGenerarReporteGrap;
    QPushButton *btnJerarquia;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        plainTextEdit = new QPlainTextEdit(centralwidget);
        plainTextEdit->setObjectName("plainTextEdit");
        plainTextEdit->setGeometry(QRect(240, 140, 311, 161));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(70, 220, 90, 29));
        btnEstaditicasG = new QPushButton(centralwidget);
        btnEstaditicasG->setObjectName("btnEstaditicasG");
        btnEstaditicasG->setGeometry(QRect(620, 360, 151, 29));
        btnCargar = new QPushButton(centralwidget);
        btnCargar->setObjectName("btnCargar");
        btnCargar->setGeometry(QRect(70, 130, 111, 29));
        bntLimpiar = new QPushButton(centralwidget);
        bntLimpiar->setObjectName("bntLimpiar");
        bntLimpiar->setGeometry(QRect(60, 270, 121, 29));
        btnReporteTokens = new QPushButton(centralwidget);
        btnReporteTokens->setObjectName("btnReporteTokens");
        btnReporteTokens->setGeometry(QRect(260, 50, 121, 29));
        btnReporteErrores = new QPushButton(centralwidget);
        btnReporteErrores->setObjectName("btnReporteErrores");
        btnReporteErrores->setGeometry(QRect(440, 50, 121, 29));
        ReporteHistorialPacientes = new QPushButton(centralwidget);
        ReporteHistorialPacientes->setObjectName("ReporteHistorialPacientes");
        ReporteHistorialPacientes->setGeometry(QRect(40, 390, 191, 29));
        btnCargaMedicos = new QPushButton(centralwidget);
        btnCargaMedicos->setObjectName("btnCargaMedicos");
        btnCargaMedicos->setGeometry(QRect(280, 380, 131, 29));
        btnAgendaCitas = new QPushButton(centralwidget);
        btnAgendaCitas->setObjectName("btnAgendaCitas");
        btnAgendaCitas->setGeometry(QRect(450, 360, 121, 29));
        btnGenerarReporteGrap = new QPushButton(centralwidget);
        btnGenerarReporteGrap->setObjectName("btnGenerarReporteGrap");
        btnGenerarReporteGrap->setGeometry(QRect(330, 480, 191, 29));
        btnJerarquia = new QPushButton(centralwidget);
        btnJerarquia->setObjectName("btnJerarquia");
        btnJerarquia->setGeometry(QRect(650, 470, 111, 29));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Analizar", nullptr));
        btnEstaditicasG->setText(QCoreApplication::translate("MainWindow", "Estadisticas Generales", nullptr));
        btnCargar->setText(QCoreApplication::translate("MainWindow", "Cargar Archivo", nullptr));
        bntLimpiar->setText(QCoreApplication::translate("MainWindow", "Limpiar Pantalla", nullptr));
        btnReporteTokens->setText(QCoreApplication::translate("MainWindow", "Reporte Tokens", nullptr));
        btnReporteErrores->setText(QCoreApplication::translate("MainWindow", "Reporte errores", nullptr));
        ReporteHistorialPacientes->setText(QCoreApplication::translate("MainWindow", "Reporte Historial Pacientes", nullptr));
        btnCargaMedicos->setText(QCoreApplication::translate("MainWindow", "Carga de Medicos", nullptr));
        btnAgendaCitas->setText(QCoreApplication::translate("MainWindow", "Agenda de Citas", nullptr));
        btnGenerarReporteGrap->setText(QCoreApplication::translate("MainWindow", "Generar Reporte Graphix", nullptr));
        btnJerarquia->setText(QCoreApplication::translate("MainWindow", "Ver Jerarquia", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
