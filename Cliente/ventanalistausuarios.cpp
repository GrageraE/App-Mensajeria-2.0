#include "ventanalistausuarios.h"
#include "ui_ventanalistausuarios.h"

#include <QCloseEvent>

ventanaListaUsuarios::ventanaListaUsuarios(const QStringList& _lista, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ventanaListaUsuarios)
{
    ui->setupUi(this);
    ui->tablaUsuarios->setColumnCount(1);
    ui->tablaUsuarios->setHorizontalHeaderLabels(QStringList({"Nombre"}));

    for(int i = 0; i < _lista.size(); ++i)
    {
        ui->tablaUsuarios->insertRow(i);
        auto* item = new QTableWidgetItem(_lista.at(i));
        ui->tablaUsuarios->setItem(i, 0, item);
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
void ventanaListaUsuarios::nuevoUsuarioConectado(QString _nombre)
{
    int row = this->ui->tablaUsuarios->rowCount();
    this->ui->tablaUsuarios->insertRow(row);
    auto* item = new QTableWidgetItem(_nombre);
    this->ui->tablaUsuarios->setItem(row, 0, item);
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
