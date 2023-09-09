#include "ventanapasswd.h"
#include "ui_ventanapasswd.h"

ventanaPasswd::ventanaPasswd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ventanaPasswd)
{
    ui->setupUi(this);
    ui->inputPasswd->setEchoMode(QLineEdit::Password);
    this->setWindowTitle("Se necesita autenticación...");
}

ventanaPasswd::~ventanaPasswd()
{
    delete ui;
}

void ventanaPasswd::on_pushButton_2_clicked()
{
    reject();
}

void ventanaPasswd::on_pushButton_clicked()
{
    this->res = this->ui->inputPasswd->text();
    if(!this->res.isEmpty())
        accept();
}

QString ventanaPasswd::getResultado()
{
    return this->res;
}
