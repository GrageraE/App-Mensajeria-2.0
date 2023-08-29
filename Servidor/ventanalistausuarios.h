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
    explicit ventanaListaUsuarios(const QMap<QString, QWebSocket*>& _lista, QWidget *parent = nullptr);
    ~ventanaListaUsuarios();

    void nuevoUsuarioConectado(QString _nombre, QString _ip);

    void usuarioDesconectado(QString _nombre);

signals:
    void cerrarVentana();

    void expulsarUsuario(QString _usuario, bool _ban = false);

private slots:
    void on_pushButton_clicked();

    void on_botonExpulsar_clicked();

private:
    Ui::ventanaListaUsuarios *ui;
    void closeEvent(QCloseEvent* event);
};

#endif // VENTANALISTAUSUARIOS_H
