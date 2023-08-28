#include "ventanalistausuarios.h"
#include "ui_ventanalistausuarios.h"

#include <QCloseEvent>

ventanaListaUsuarios::ventanaListaUsuarios(const QMap<QString, QWebSocket*>& _lista, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ventanaListaUsuarios)
{
    ui->setupUi(this);
    ui->tablaUsuarios->setColumnCount(2);
    ui->tablaUsuarios->setHorizontalHeaderLabels(QStringList({"Nombre", "IP"}));

    int i = 0;
    for(auto it = _lista.cbegin(); it != _lista.cend(); ++it)
    {
        ui->tablaUsuarios->insertRow(i);
        auto* item = new QTableWidgetItem(it.key());
        ui->tablaUsuarios->setItem(i, 0, item);
        item = new QTableWidgetItem(it.value()->peerAddress().toString());
        ui->tablaUsuarios->setItem(i, 1, item);
        ++i;
    }
    ui->tablaUsuarios->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

ventanaListaUsuarios::~ventanaListaUsuarios()
{
    delete ui;
}

void ventanaListaUsuarios::on_pushButton_clicked()
{
    close();
}

/*
 * Esta funcion se usa cuando se conecta un nuevo usuario mientras esta la ventana abierta
*/
void ventanaListaUsuarios::nuevoUsuarioConectado(QString _nombre, QString _ip)
{
    int row = this->ui->tablaUsuarios->rowCount();
    this->ui->tablaUsuarios->insertRow(row);
    auto* item = new QTableWidgetItem(_nombre);
    this->ui->tablaUsuarios->setItem(row, 0, item);
    item = new QTableWidgetItem(_ip);
    this->ui->tablaUsuarios->setItem(row, 1, item);
    ui->tablaUsuarios->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void ventanaListaUsuarios::usuarioDesconectado(QString _nombre)
{
    // Encontramos el elemento con el nombre de usuario dado
    auto elementoL = this->ui->tablaUsuarios->findItems(_nombre, Qt::MatchExactly);
    if(elementoL.size() != 1)
        qDebug() <<" Existen varios usuarios con el mismo nombre (ERROR) - Eliminando varias entradas...";

    for(auto* i : elementoL)
    {
        this->ui->tablaUsuarios->removeRow(this->ui->tablaUsuarios->row(i));
        //delete i; -- This crashes
    }
    ui->tablaUsuarios->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

/*
 * Informamos a la ventana principal que se va a cerrar esta
*/
void ventanaListaUsuarios::closeEvent(QCloseEvent* event)
{
    emit cerrarVentana();
    event->accept();
}
