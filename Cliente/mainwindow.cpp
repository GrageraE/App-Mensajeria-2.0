#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "ventanaservidores.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client(nullptr)
    , ventana(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("Cliente");
    ui->Logs->setReadOnly(true);
    ui->botonDesconectar->setEnabled(false);
    ui->botonEnviar->setEnabled(false);
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
    QString puertoStr = this->ui->inputPuerto->text();
    QString servidor = this->ui->inputServidor->text();
    bool seguro = this->ui->checkSeguro->isChecked();
    bool statusPort = true;
    int puerto = puertoStr.toInt(&statusPort);
    if(nombreUsuario.isEmpty() || servidor.isEmpty() || !statusPort)
    {
        QMessageBox::critical(this, "Error", "El nombre o el servidor estan vacios; o bien el puerto no es un numero");
        return;
    }
    this->client = new Cliente(servidor, puerto, nombreUsuario, seguro, this);
    connect(this->client, &Cliente::mandarConexion, this, &MainWindow::usuarioConectado);
    connect(this->client, &Cliente::mandarDesconexion, this, &MainWindow::usuarioDesconectado);
    connect(this->client, &Cliente::mandarDesconexionServidor, this, &MainWindow::servidorDesconectado);
    connect(this->client, &Cliente::mandarMensaje, this, &MainWindow::mensaje);

    this->ui->botonConectar->setEnabled(false);
    this->ui->botonDesconectar->setEnabled(true);
    this->ui->botonEnviar->setEnabled(true);
    // Para que funcione la tecla enter
    connect(this->ui->inputMensaje, &QLineEdit::returnPressed, this, &MainWindow::enterEnviar);
}

void MainWindow::on_botonDesconectar_clicked()
{
    disconnect(this->client, &Cliente::mandarConexion, this, &MainWindow::usuarioConectado);
    disconnect(this->client, &Cliente::mandarDesconexion, this, &MainWindow::usuarioDesconectado);
    disconnect(this->client, &Cliente::mandarDesconexionServidor, this, &MainWindow::servidorDesconectado);
    disconnect(this->client, &Cliente::mandarMensaje, this, &MainWindow::mensaje);
    delete this->client;

    this->client = nullptr;
    this->ui->botonConectar->setEnabled(true);
    this->ui->botonDesconectar->setEnabled(false);
    this->ui->botonEnviar->setEnabled(false);
    disconnect(this->ui->inputMensaje, &QLineEdit::returnPressed, this, &MainWindow::enterEnviar);
    if(this->ventana)
        this->ventana->close();
}

void MainWindow::usuarioConectado(QString _usuario)
{
    this->ui->Logs->appendPlainText("Se ha conectado: " + _usuario);
    if(this->ventana)
    {
        this->ventana->nuevoUsuarioConectado(_usuario);
    }
}

void MainWindow::usuarioDesconectado(QString _usuario)
{
    this->ui->Logs->appendPlainText("Se ha desconectado: " + _usuario);
    if(this->ventana)
    {
        this->ventana->usuarioDesconectado(_usuario);
    }
}

void MainWindow::servidorDesconectado(QString _motivo)
{
    this->ui->Logs->appendPlainText("Se ha desconectado el servidor. Motivo: " + _motivo);
    this->on_botonDesconectar_clicked();
}

void MainWindow::mensaje(Mensaje _mensaje)
{
    this->ui->Logs->appendPlainText("<" + _mensaje.usuario + ">: " + _mensaje.contenido);
}

void MainWindow::on_botonEnviar_clicked()
{
    QString mensaje = this->ui->inputMensaje->text();
    this->ui->inputMensaje->clear();
    if(mensaje.isEmpty()) return;

    this->client->enviarMensaje(mensaje);
}

void MainWindow::enterEnviar()
{
    this->on_botonEnviar_clicked();
}

void MainWindow::on_actionLista_de_usuarios_triggered()
{
    // Pedimos la lista de usuarios al servidor
    if(!this->client)
    {
        QMessageBox::critical(this, "Error", "No se ha conectado a ningún servidor");
        return;
    }
    connect(this->client, &Cliente::mandarLista, this, &MainWindow::listaRecibida);
    this->client->pedirListaUsuarios();
}

void MainWindow::listaRecibida(QStringList _lista)
{
    // Hemos recibido la lista, abrimos la ventana
    disconnect(this->client, &Cliente::mandarLista, this, &MainWindow::listaRecibida);
    this->ventana = new ventanaListaUsuarios(_lista, this);
    this->ventana->setModal(false);
    connect(this->ventana, &ventanaListaUsuarios::cerrarVentana, this, &MainWindow::cierreListaUsuario);
    this->ventana->show();
}

void MainWindow::cierreListaUsuario()
{
    disconnect(this->ventana, &ventanaListaUsuarios::cerrarVentana, this, &MainWindow::cierreListaUsuario);
    this->ventana = nullptr;
}

void MainWindow::on_actionLista_triggered() // Lista de servidores
{
    if(this->client)
    {
        QMessageBox::critical(this, "Error", "Desconéctate para abrir la lista de servidores");
            return;
    }
    ventanaServidores v(this);
    v.setModal(true);
    if(v.exec() == QDialog::Accepted)
    {
        auto result = v.getResultado();
        this->ui->inputNombre->setText(result.nombreUsuario);
        this->ui->inputServidor->setText(result.ip);
        this->ui->inputPuerto->setText(result.puerto);
        if(result.seguro) this->ui->checkSeguro->setCheckState(Qt::Checked);
        else this->ui->checkSeguro->setCheckState(Qt::Unchecked);
    }
}


void MainWindow::on_actionA_adir_triggered()
{
    if(!this->client)
    {
        QMessageBox::critical(this, "Error", "Conéctate a un servidor para guardar sus datos");
        return;
    }
    ventanaServidores v;
    v.agregarDatos({this->ui->inputNombre->text(), this->ui->inputServidor->text(),
                    this->ui->inputPuerto->text(), this->ui->checkSeguro->isChecked()});
}

