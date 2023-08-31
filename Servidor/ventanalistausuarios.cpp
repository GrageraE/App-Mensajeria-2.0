#include "ventanalistausuarios.h"
#include "ui_ventanalistausuarios.h"

#include <QCloseEvent>

ventanaListaUsuarios::ventanaListaUsuarios(const QMap<QString, QWebSocket*>& _lista, const QStringList& _baneados, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ventanaListaUsuarios)
{
    ui->setupUi(this);
    this->setWindowTitle("Lista de Usuarios");
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
    ui->tablaUsuarios->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tablaUsuarios->setEditTriggers(QAbstractItemView::NoEditTriggers);

    i = 0;
    for(const auto& ip : _baneados)
    {
        auto* item = new QListWidgetItem(ip);
        ui->listaBaneados->insertItem(i, item);
        ++i;
    }
    ui->listaBaneados->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->listaBaneados->setEditTriggers(QAbstractItemView::NoEditTriggers);
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

void ventanaListaUsuarios::on_botonExpulsar_clicked()
{
    auto usuarios = this->ui->tablaUsuarios->selectedItems();
    for(auto* item : usuarios)
    {
        qDebug() <<" [EXPULSAR] Extraido de la primera columna: " <<item->text();
        auto nombre = item->text();
        if(item->column() == 1)
        {
            // Extraemos el nombre de la tabla
            auto itemNombre = this->ui->tablaUsuarios->item(item->row(), 0);
            nombre = itemNombre->text();
            qDebug() <<" [EXPULSAR] Extraido de la operacion: " <<nombre;
        }
        emit expulsarUsuario(nombre);
    }
}

void ventanaListaUsuarios::on_botonBanear_clicked()
{
    auto usuarios = this->ui->tablaUsuarios->selectedItems();
    for(auto* item : usuarios)
    {
        qDebug() <<" [EXPULSAR] Extraido de la primera columna: " <<item->text();
        auto nombre = item->text();
        if(item->column() == 1)
        {
            // Extraemos el nombre de la tabla
            auto itemNombre = this->ui->tablaUsuarios->item(item->row(), 0);
            nombre = itemNombre->text();
            qDebug() <<" [EXPULSAR] Extraido de la operacion: " <<nombre;
        }
        emit expulsarUsuario(nombre, true);
        // Insertamos en la lista
        auto* listItem = new QListWidgetItem(this->ui->tablaUsuarios->item(item->row(), 1)->text());
        qDebug() <<" [EXPULSAR] Extraido de la segunda columna: " <<listItem->text();
        this->ui->listaBaneados->insertItem(this->ui->listaBaneados->count() - 1, listItem->text());
    }
    this->ui->listaBaneados->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

/*
 * Segunda pestaña
*/
void ventanaListaUsuarios::on_pushButton_2_clicked()
{
    close();
}

void ventanaListaUsuarios::on_pushButton_3_clicked()     // Boton Eliminar
{
    // Eliminamos de la lista
    auto usuarios = this->ui->listaBaneados->selectedItems();
    for(auto* item : usuarios)
    {
        emit perdonarUsuario(item->text());
        delete item;
    }
}
