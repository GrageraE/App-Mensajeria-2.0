#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "servidor.h"
#include "../common/mensaje.h"
#include "ventanalistausuarios.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSalir_triggered();

    void on_Iniciar_clicked();

    void on_Parar_clicked();

    void mostrarMensaje(Mensaje _mensaje);

    void mostrarNuevoUsuario(QString _nombre);

    void mostrarUsuarioDesconectado(QString _nombre);

    void on_botonLimpiar_clicked();

    void on_actionLista_Usuarios_triggered();

    void cierreListaUsuario();

private:
    Ui::MainWindow *ui;

    Servidor* servidor;
    ventanaListaUsuarios* ventana;
};
#endif // MAINWINDOW_H
