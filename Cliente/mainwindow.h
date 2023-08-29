#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "cliente.h"
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

    void on_actionLimpiar_triggered();

    void on_botonConectar_clicked();

    void on_botonDesconectar_clicked();

    void usuarioConectado(QString _ususario);

    void usuarioDesconectado(QString _usuario);

    void mensaje(Mensaje _mensaje);

    void on_botonEnviar_clicked();

    void on_actionLista_de_usuarios_triggered();

    void listaRecibida(QStringList _lista);

    void cierreListaUsuario();

private:
    Ui::MainWindow *ui;
    Cliente* client;
    ventanaListaUsuarios* ventana;

};
#endif // MAINWINDOW_H
