#include "ventanaservidores.h"
#include "ui_ventanaservidores.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

ventanaServidores::ventanaServidores(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ventanaServidores),
    resultado({"", "", 0})
{
    ui->setupUi(this);
    this->setWindowTitle("Lista de Servidores");
    ui->tablaServidores->setColumnCount(3);
    ui->tablaServidores->setHorizontalHeaderLabels({"Dirección", "Puerto", "Usuario"});

    QFile listaFile("./servidores.txt");
    if(listaFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&listaFile);
        auto doc = QJsonDocument::fromJson(stream.readAll().toUtf8());
        auto listaJson = doc.object();
        int i = 0;
        for(auto it = listaJson.begin(); it != listaJson.end(); ++it)
        {
            auto entradaServidor = it.value().toObject();
            ui->tablaServidores->insertRow(i);
            auto* item = new QTableWidgetItem(entradaServidor["Servidor"].toString());
            ui->tablaServidores->setItem(i, 0, item);
            item = new QTableWidgetItem(entradaServidor["Puerto"].toString());
            ui->tablaServidores->setItem(i, 1, item);
            item = new QTableWidgetItem(entradaServidor["Usuario"].toString());
            ui->tablaServidores->setItem(i, 2, item);
            this->listaServidores.push_back({entradaServidor["Usuario"].toString(), entradaServidor["Servidor"].toString(), entradaServidor["Puerto"].toString()});
        }
        listaFile.close();
    }
    else
        qDebug() <<" No se puede abrir servidores.txt";
    ui->tablaServidores->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tablaServidores->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

ventanaServidores::~ventanaServidores()
{
    QFile listaFile("./servidores.txt");
    if(listaFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        int i = 0;
        QJsonObject json;
        for(const auto& server : this->listaServidores)
        {
            QJsonObject entradaJson;
            entradaJson["Servidor"] = server.ip;
            entradaJson["Puerto"] = server.puerto;
            entradaJson["Usuario"] = server.nombreUsuario;
            json[QString::number(i)] = entradaJson;
            ++i;
        }
        listaFile.write(QJsonDocument(json).toJson());
    }
    else
        qDebug() <<" No se puede abrir para escritura servidores.txt";

    delete ui;
}

void ventanaServidores::on_pushButton_clicked()
{
    reject();
}

ventanaServidores::DatosServidor ventanaServidores::getResultado()
{
    return this->resultado;
}

void ventanaServidores::on_botonConectar_clicked()
{
    auto serverList = this->ui->tablaServidores->selectedItems();
    if(serverList.size() != 1)
    {
        qDebug() <<" Hay mas de un elemento seleccionado";
        return;
    }
    auto* item = serverList.at(0);
    switch(this->ui->tablaServidores->column(item))
    {
    case 0: // Direccion
    {
        this->resultado.ip = item->text();
        this->resultado.puerto = this->ui->tablaServidores->item(item->row(), 1)->text();
        this->resultado.nombreUsuario = this->ui->tablaServidores->item(item->row(), 2)->text();
    }
    break;
    case 1: // Puerto
    {
        this->resultado.ip = this->ui->tablaServidores->item(item->row(), 0)->text();
        this->resultado.puerto = item->text();
        this->resultado.nombreUsuario = this->ui->tablaServidores->item(item->row(), 2)->text();
    }
    break;
    case 2: // Usuario
    {
        this->resultado.ip = this->ui->tablaServidores->item(item->row(), 0)->text();
        this->resultado.puerto = this->ui->tablaServidores->item(item->row(), 1)->text();
        this->resultado.nombreUsuario = item->text();
    }
    break;
    default: qDebug() <<" Error en la toma de datos";
    }

    accept();
}

void ventanaServidores::on_botonEliminar_clicked()
{
    auto serverList = this->ui->tablaServidores->selectedItems();
    if(serverList.size() != 1)
    {
        qDebug() <<" Hay mas de un elemento seleccionado";
        return;
    }
    auto* item = serverList.at(0);
    DatosServidor datos;
    switch(this->ui->tablaServidores->column(item))
    {
    case 0: // Direccion
    {
        datos.ip = item->text();
        datos.puerto = this->ui->tablaServidores->item(item->row(), 1)->text();
        datos.nombreUsuario = this->ui->tablaServidores->item(item->row(), 2)->text();
    }
    break;
    case 1: // Puerto
    {
        datos.ip = this->ui->tablaServidores->item(item->row(), 0)->text();
        datos.puerto = item->text();
        datos.nombreUsuario = this->ui->tablaServidores->item(item->row(), 2)->text();
    }
    break;
    case 2: // Usuario
    {
        datos.ip = this->ui->tablaServidores->item(item->row(), 0)->text();
        datos.puerto = this->ui->tablaServidores->item(item->row(), 1)->text();
        datos.nombreUsuario = item->text();
    }
    break;
    default:
    {
        qDebug() <<" Error en la toma de datos";
        return;
    }
    }
    this->ui->tablaServidores->removeRow(item->row());
    this->listaServidores.removeAll(datos);
}

void ventanaServidores::agregarDatos(const ventanaServidores::DatosServidor& _datos)
{
    this->listaServidores.push_back(_datos);
}

