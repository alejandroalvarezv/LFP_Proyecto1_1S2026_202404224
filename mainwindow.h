#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "scanner.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:

    void on_btnCargar_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_btnEstaditicasG_clicked();
    void on_btnLimpiar_clicked();

    void on_btnReporteTokens_clicked();

    void on_ReporteHistorialPacientes_clicked();

    void on_btnCargaMedicos_clicked();

    void on_btnAgendaCitas_clicked();

private:
    Ui::MainWindow *ui;
    QList<Token> tokensActuales;
};

#endif