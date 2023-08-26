#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Logs->setReadOnly(true);
    ui->botonDesconectar->setEnabled(false);
    ui->botonEnviar->setEnabled(false);
    this->client = nullptr;
}

MainWindow::~MainWindow()
{
    delete this->client;
    delete ui;
}

void MainWindow::on_actionSalir_triggered()
{
    close();
}

void MainWindow::on_actionLimpiar_triggered()
{
    this->ui->Logs->clear();
}

void MainWindow::on_botonConectar_clicked()
{
    QString nombreUsuario = this->ui->inputNombre->text();
    QString servidor = this->ui->inputServidor->text();
    if(nombreUsuario.isEmpty() || servidor.isEmpty())
    {
        QMessageBox::critical(this, "Error", "El nombre o el servidor estan vacios");
        return;
    }
    this->client = new Cliente(servidor, nombreUsuario, this);
    connect(this->client, &Cliente::mandarConexion, this, &MainWindow::usuarioConectado);
    connect(this->client, &Cliente::mandarDesconexion, this, &MainWindow::usuarioDesconectado);
    connect(this->client, &Cliente::mandarMensaje, this, &MainWindow::mensaje);

    this->ui->botonConectar->setEnabled(false);
    this->ui->botonDesconectar->setEnabled(true);
    this->ui->botonEnviar->setEnabled(true);
}

void MainWindow::on_botonDesconectar_clicked()
{
    disconnect(this->client, &Cliente::mandarConexion, this, &MainWindow::usuarioConectado);
    disconnect(this->client, &Cliente::mandarDesconexion, this, &MainWindow::usuarioDesconectado);
    disconnect(this->client, &Cliente::mandarMensaje, this, &MainWindow::mensaje);
    delete this->client;

    this->client = nullptr;
    this->ui->botonConectar->setEnabled(true);
    this->ui->botonDesconectar->setEnabled(false);
    this->ui->botonEnviar->setEnabled(false);
}

void MainWindow::usuarioConectado(QString _usuario)
{
    this->ui->Logs->appendPlainText("Se ha conectado: " + _usuario);
}

void MainWindow::usuarioDesconectado(QString _usuario)
{
    this->ui->Logs->appendPlainText("Se ha desconectado: " + _usuario);
}

void MainWindow::mensaje(Mensaje _mensaje)
{
    this->ui->Logs->appendPlainText("<" + _mensaje.usuario + ">: " + _mensaje.contenido);
}


void MainWindow::on_botonEnviar_clicked()
{
    QString mensaje = this->ui->inputMensaje->text();
    if(mensaje.isEmpty()) return;

    this->client->enviarMensaje(mensaje);
}
