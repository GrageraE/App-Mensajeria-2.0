#ifndef CLIENTE_H
#define CLIENTE_H

#include <QObject>
#include <QtWebSockets/QWebSocket>

#include "../common/mensaje.h"

class Cliente : public QObject
{
    Q_OBJECT
public:
    Cliente(const QString& _servidor, int _puerto, const QString& _usuario, QObject* _parent);

    ~Cliente();

    void enviarMensaje(QString _msg);

    void pedirListaUsuarios();

signals:
    void mandarConexion(QString _user);

    void mandarDesconexion(QString _user);

    void mandarMensaje(Mensaje msj);

    void mandarLista(QStringList _lista);

private slots:
    void conectado();

    void desconectado();

    void mensajeRecibido(QString _msg);

private:
    QWebSocket socket;
    QString usuario;
    QString servidor;
    int puerto;
    bool conectados;

    const QString TIPO_STR = "TIPO";
    const QString LISTA_STR = "LISTA";
    const QString USUARIO_STR = "USUARIO";
    const QString CONTENIDO_STR = "CONTENIDO";
    const QString MENSAJE_STR = "MENSAJE";
    const QString CONEXION_STR = "CONEXION";
    const QString DESCONEXION_STR = "DESCONEXION";
};

#endif // CLIENTE_H
