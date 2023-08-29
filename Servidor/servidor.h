#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <QObject>

#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

#include "../common/mensaje.h"

class Servidor : public QObject
{
    Q_OBJECT
public:
    Servidor(int _puerto, const QString& _nombre, QObject* _parent);
    ~Servidor();

    QMap<QString, QWebSocket*> getLista();

    void expulsar(QString nombreUsuario);

signals:
    void mostrarMensaje(Mensaje msj);

    void mostrarNuevoUsuario(QString _nombre);

    void mostrarUsuarioDesconectado(QString _nombre);

private slots:
    void nuevoUsuario();

    void mensajeRecibido(QString message);

    void desconectado();

private:
    int port;
    QString name;
    QWebSocketServer* server;

    QMap<QString, QWebSocket*> listaUsuarios;

    const QString TIPO_STR = "TIPO";
    const QString LISTA_STR = "LISTA";
    const QString USUARIO_STR = "USUARIO";
    const QString CONTENIDO_STR = "CONTENIDO";
    const QString MENSAJE_STR = "MENSAJE";
    const QString CONEXION_STR = "CONEXION";
    const QString DESCONEXION_STR = "DESCONEXION";
};

#endif // SERVIDOR_H
