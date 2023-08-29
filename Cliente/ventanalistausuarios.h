#ifndef VENTANALISTAUSUARIOS_H
#define VENTANALISTAUSUARIOS_H

#include <QDialog>
#include <QtWebSockets/QWebSocket>

namespace Ui {
class ventanaListaUsuarios;
}

class ventanaListaUsuarios : public QDialog
{
    Q_OBJECT

public:
    explicit ventanaListaUsuarios(const QStringList& _lista, QWidget *parent = nullptr);
    ~ventanaListaUsuarios();

    void nuevoUsuarioConectado(QString _nombre);

    void usuarioDesconectado(QString _nombre);

signals:
    void cerrarVentana();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ventanaListaUsuarios *ui;
    void closeEvent(QCloseEvent* event);
};

#endif // VENTANALISTAUSUARIOS_H
