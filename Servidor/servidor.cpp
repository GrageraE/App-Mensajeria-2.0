#include "servidor.h"

#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

Servidor::Servidor(int _puerto, const QString& _nombre, QObject* _parent)
    : QObject(_parent), port(_puerto), name(_nombre)
{
    this->server = new QWebSocketServer(this->name, QWebSocketServer::NonSecureMode, this);
    if(this->server->listen(QHostAddress::Any, this->port))
    {
        qDebug() <<" Server listening on port " << this->port;
        connect(this->server, &QWebSocketServer::newConnection, this, &Servidor::nuevoUsuario);
    }
}

Servidor::~Servidor()
{
    // Dealojamos a todos los usuarios
    // TODO: Comunicar a los usuarios

    QMapIterator<QWebSocket*, QString> i(this->listaUsuarios);
    while (i.hasNext()) {
        i.next();
        i.key()->deleteLater();
    }
    delete this->server;
}

void Servidor::nuevoUsuario()
{
    QWebSocket* usuario = this->server->nextPendingConnection();

    qDebug() <<" New user connected. Origin = " << usuario->origin() <<" Peer name = " <<usuario->peerName();

    connect(usuario, &QWebSocket::textMessageReceived, this, &Servidor::mensajeRecibido);
    connect(usuario, &QWebSocket::disconnected, this, &Servidor::desconectado);

    this->listaUsuarios[usuario] = ".";
    // TODO: Comunicar a los usurios
}

void Servidor::mensajeRecibido(QString message)
{
    QWebSocket* usuarioRemitente = qobject_cast<QWebSocket*>(sender());
    QString nombreUsuario = this->listaUsuarios.value(usuarioRemitente);
    /*
     * TODO:
     * - Enviar mensaje al resto de usuarios
     * - Configurar el nombre de un usuario conectado
    */

    emit mostrarMensaje({message, nombreUsuario});
}

void Servidor::desconectado()
{
    qDebug() <<" User disconnected";

    QWebSocket* usuario = qobject_cast<QWebSocket*>(sender());

    if(usuario)
    {
        this->listaUsuarios.remove(usuario);
        usuario->deleteLater();
    }
    // TODO: Comunicar a los usuarios
}
