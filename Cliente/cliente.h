#ifndef CLIENTE_H
#define CLIENTE_H

#include <QObject>
#include <QtWebSockets/QWebSocket>

#include "mensaje.h"

class Cliente : public QObject
{
    Q_OBJECT
public:
    Cliente(QString _servidor,  QString _usuario, QObject* _parent);

    ~Cliente();

    void enviarMensaje(QString _msg);

signals:
    void mandarConexion(QString _user);

    void mandarDesconexion(QString _user);

    void mandarMensaje(Mensaje msj);

private slots:
    void conectado();

    void desconectado();

    void mensajeRecibido(QString _msg);

private:
    QWebSocket socket;
    QString usuario;
    QString servidor;

    const QString TIPO_STR = "TIPO";
    const QString USUARIO_STR = "USUARIO";
    const QString CONTENIDO_STR = "CONTENIDO";
    const QString MENSAJE_STR = "MENSAJE";
    const QString CONEXION_STR = "CONEXION";
    const QString DESCONEXION_STR = "DESCONEXION";
};

#endif // CLIENTE_H