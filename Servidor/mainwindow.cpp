#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , servidor(nullptr)
    , ventana(nullptr)
{
    ui->setupUi(this);
    ui->Logs->setReadOnly(true);
    ui->Parar->setEnabled(false);
    this->setWindowTitle("Servidor");
}

MainWindow::~MainWindow()
{
    delete this->ventana;
    delete this->servidor;
    delete ui;
}

void MainWindow::on_actionSalir_triggered()
{
    close();
}

void MainWindow::on_Iniciar_clicked()
{
    QString nombreServidor = ui->nombre->text();
    QString puertoStr = ui->puerto->text();
    if(nombreServidor.isEmpty() || puertoStr.isEmpty())
    {
        QMessageBox::critical(this, "Error", "No se han especificado nombre o puerto del servidor");
        return;
    }
    bool portStatus = true;
    int puerto = puertoStr.toInt(&portStatus);
    if(!portStatus)
    {
        QMessageBox::critical(this, "Error", "El puerto debe ser un valor numérico");
        return;
    }

    servidor = new Servidor(puerto, nombreServidor, this);
    connect(this->servidor, &Servidor::mostrarMensaje, this, &MainWindow::mostrarMensaje);
    connect(this->servidor, &Servidor::mostrarNuevoUsuario, this, &MainWindow::mostrarNuevoUsuario);
    connect(this->servidor, &Servidor::mostrarUsuarioDesconectado, this, &MainWindow::mostrarUsuarioDesconectado);

    this->ui->Logs->appendPlainText("<SISTEMA> Servidor iniciado con puerto " + puertoStr + " y nombre " + nombreServidor);
    this->ui->Iniciar->setEnabled(false);
    this->ui->Parar->setEnabled(true);
}

void MainWindow::on_Parar_clicked()
{
    disconnect(this->servidor, &Servidor::mostrarMensaje, this, &MainWindow::mostrarMensaje);
    disconnect(this->servidor, &Servidor::mostrarNuevoUsuario, this, &MainWindow::mostrarNuevoUsuario);
    disconnect(this->servidor, &Servidor::mostrarUsuarioDesconectado, this, &MainWindow::mostrarUsuarioDesconectado);
    delete this->servidor;
    this->servidor = nullptr;
    this->ui->Logs->appendPlainText("Servidor parado");
    qDebug() <<" Servidor terminado";
    this->ui->Iniciar->setEnabled(true);
    this->ui->Parar->setEnabled(false);
    if(this->ventana)
        this->ventana->close();
}

void MainWindow::mostrarMensaje(Mensaje _mensaje)
{
    this->ui->Logs->appendPlainText("<" + _mensaje.usuario + ">" + " " + _mensaje.contenido);
}

void MainWindow::mostrarNuevoUsuario(QString _nombre)
{
    this->ui->Logs->appendPlainText("Se ha conectado: " + _nombre);
    // Detectamos si la ventana esta abierta
    if(this->ventana)
    {
        auto lista = this->servidor->getLista();
        this->ventana->nuevoUsuarioConectado(_nombre, lista[_nombre]->peerAddress().toString());
    }
}

void MainWindow::mostrarUsuarioDesconectado(QString _nombre)
{
    this->ui->Logs->appendPlainText("Se ha desconectado: " + _nombre);
    if(this->ventana)
    {
        this->ventana->usuarioDesconectado(_nombre);
    }
}

void MainWindow::on_botonLimpiar_clicked()
{
    this->ui->Logs->clear();
}

void MainWindow::on_actionLista_Usuarios_triggered()
{
    if(!this->servidor)
    {
        QMessageBox::critical(this, "Error", "No se ha iniciado el servidor");
        return;
    }
    this->ventana = new ventanaListaUsuarios(this->servidor->getLista(), this->servidor->getBaneados(), this);
    this->ventana->setModal(false);
    connect(this->ventana, &ventanaListaUsuarios::cerrarVentana, this, &MainWindow::cierreListaUsuario);
    connect(this->ventana, &ventanaListaUsuarios::expulsarUsuario, this, &MainWindow::expulsar);
    connect(this->ventana, &ventanaListaUsuarios::perdonarUsuario, this, &MainWindow::perdonar);
    this->ventana->show();
}

void MainWindow::expulsar(QString _usuario, bool _ban)
{
    if(this->servidor)
    {
        this->servidor->expulsar(_usuario, _ban);
    }
}

void MainWindow::perdonar(QString _ip)
{
    this->servidor->perdonar(_ip);
    this->ui->Logs->appendPlainText("Perdonado " + _ip);
}

void MainWindow::cierreListaUsuario()
{
    disconnect(this->ventana, &ventanaListaUsuarios::cerrarVentana, this, &MainWindow::cierreListaUsuario);
    disconnect(this->ventana, &ventanaListaUsuarios::expulsarUsuario, this, &MainWindow::expulsar);
    disconnect(this->ventana, &ventanaListaUsuarios::perdonarUsuario, this, &MainWindow::perdonar);
    this->ventana = nullptr;
}
