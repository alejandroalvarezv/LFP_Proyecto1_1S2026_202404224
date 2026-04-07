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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
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
    QPushButton *btnLimpiar;
    QPushButton *btnReporteTokens;
    QPushButton *btnReporteErrores;
    QPushButton *ReporteHistorialPacientes;
    QPushButton *btnCargaMedicos;
    QPushButton *btnAgendaCitas;
    QPushButton *btnGenerarReporteGrap;
    QTableWidget *tablaTokens;
    QTableWidget *tablaErrores;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1338, 602);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        plainTextEdit = new QPlainTextEdit(centralwidget);
        plainTextEdit->setObjectName("plainTextEdit");
        plainTextEdit->setGeometry(QRect(40, 140, 461, 191));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(220, 60, 90, 29));
        btnEstaditicasG = new QPushButton(centralwidget);
        btnEstaditicasG->setObjectName("btnEstaditicasG");
        btnEstaditicasG->setGeometry(QRect(200, 420, 151, 29));
        btnCargar = new QPushButton(centralwidget);
        btnCargar->setObjectName("btnCargar");
        btnCargar->setGeometry(QRect(60, 50, 111, 29));
        btnLimpiar = new QPushButton(centralwidget);
        btnLimpiar->setObjectName("btnLimpiar");
        btnLimpiar->setGeometry(QRect(360, 50, 121, 29));
        btnReporteTokens = new QPushButton(centralwidget);
        btnReporteTokens->setObjectName("btnReporteTokens");
        btnReporteTokens->setGeometry(QRect(740, 20, 121, 29));
        btnReporteErrores = new QPushButton(centralwidget);
        btnReporteErrores->setObjectName("btnReporteErrores");
        btnReporteErrores->setGeometry(QRect(910, 30, 121, 29));
        ReporteHistorialPacientes = new QPushButton(centralwidget);
        ReporteHistorialPacientes->setObjectName("ReporteHistorialPacientes");
        ReporteHistorialPacientes->setGeometry(QRect(20, 360, 191, 29));
        btnCargaMedicos = new QPushButton(centralwidget);
        btnCargaMedicos->setObjectName("btnCargaMedicos");
        btnCargaMedicos->setGeometry(QRect(230, 360, 131, 29));
        btnAgendaCitas = new QPushButton(centralwidget);
        btnAgendaCitas->setObjectName("btnAgendaCitas");
        btnAgendaCitas->setGeometry(QRect(390, 360, 121, 29));
        btnGenerarReporteGrap = new QPushButton(centralwidget);
        btnGenerarReporteGrap->setObjectName("btnGenerarReporteGrap");
        btnGenerarReporteGrap->setGeometry(QRect(180, 490, 191, 29));
        tablaTokens = new QTableWidget(centralwidget);
        if (tablaTokens->columnCount() < 5)
            tablaTokens->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tablaTokens->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tablaTokens->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tablaTokens->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tablaTokens->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tablaTokens->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tablaTokens->setObjectName("tablaTokens");
        tablaTokens->setGeometry(QRect(570, 100, 631, 192));
        tablaErrores = new QTableWidget(centralwidget);
        if (tablaErrores->columnCount() < 6)
            tablaErrores->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tablaErrores->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tablaErrores->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tablaErrores->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tablaErrores->setHorizontalHeaderItem(3, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tablaErrores->setHorizontalHeaderItem(4, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tablaErrores->setHorizontalHeaderItem(5, __qtablewidgetitem10);
        tablaErrores->setObjectName("tablaErrores");
        tablaErrores->setGeometry(QRect(565, 350, 751, 192));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1338, 25));
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
        btnLimpiar->setText(QCoreApplication::translate("MainWindow", "Limpiar Pantalla", nullptr));
        btnReporteTokens->setText(QCoreApplication::translate("MainWindow", "Reporte Tokens", nullptr));
        btnReporteErrores->setText(QCoreApplication::translate("MainWindow", "Reporte errores", nullptr));
        ReporteHistorialPacientes->setText(QCoreApplication::translate("MainWindow", "Reporte Historial Pacientes", nullptr));
        btnCargaMedicos->setText(QCoreApplication::translate("MainWindow", "Carga de Medicos", nullptr));
        btnAgendaCitas->setText(QCoreApplication::translate("MainWindow", "Agenda de Citas", nullptr));
        btnGenerarReporteGrap->setText(QCoreApplication::translate("MainWindow", "Generar Reporte Graphix", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tablaTokens->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "#", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tablaTokens->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Lexema", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tablaTokens->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Tipo ", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tablaTokens->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Linea", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tablaTokens->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Columna", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tablaErrores->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "No.", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tablaErrores->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Lexema", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tablaErrores->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Tipo ", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tablaErrores->horizontalHeaderItem(3);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Descripcion", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tablaErrores->horizontalHeaderItem(4);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Linea", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tablaErrores->horizontalHeaderItem(5);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "Columna", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
