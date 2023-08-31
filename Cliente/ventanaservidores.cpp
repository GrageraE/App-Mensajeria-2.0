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
        // TODO: Escribir el archivo
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
    // TODO: Obtener el servidor seleccionado y enviarlo
    accept();
}

