#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , servidor(nullptr)
{
    ui->setupUi(this);
    ui->Logs->setReadOnly(true);
    ui->Parar->setEnabled(false);
}

MainWindow::~MainWindow()
{
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
    int puerto = puertoStr.toInt();

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
    this->ui->Logs->appendPlainText("<SISTEMA> Servidor parado");
    qDebug() <<" Servidor terminado";
    this->ui->Iniciar->setEnabled(true);
    this->ui->Parar->setEnabled(false);
}

void MainWindow::mostrarMensaje(Mensaje _mensaje)
{
    this->ui->Logs->appendPlainText("<" + _mensaje.usuario + ">" + " " + _mensaje.contenido);
}

void MainWindow::mostrarNuevoUsuario(QString _nombre)   // TODO: Conectar señal
{
    this->ui->Logs->appendPlainText("Se ha conectado: " + _nombre);
}

void MainWindow::mostrarUsuarioDesconectado(QString _nombre)
{
    this->ui->Logs->appendPlainText("Se ha desconectado: " + _nombre);
}

void MainWindow::on_botonLimpiar_clicked()
{
    this->ui->Logs->clear();
}

