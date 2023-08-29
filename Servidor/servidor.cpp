#include "servidor.h"

#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>

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
    QJsonObject msg_json;
    msg_json[TIPO_STR] = MENSAJE_STR;
    msg_json[USUARIO_STR] = "*";    // TODO: Cambiar tipo para evitar suplantacion
    msg_json[CONTENIDO_STR] = "Servidor cerrado";
    QString msg = QJsonDocument(msg_json).toJson();

    for(auto i = this->listaUsuarios.cbegin(); i != this->listaUsuarios.cend(); ++i)
    {
        i.value()->sendTextMessage(msg);
        // Dealojamos a al usuario
        i.value()->close();
        i.value()->deleteLater();
    }

    delete this->server;
}

void Servidor::nuevoUsuario()
{
    QWebSocket* usuario = this->server->nextPendingConnection();

    qDebug() <<" New user connected. Origin = " << usuario->origin() <<" Peer name = " <<usuario->peerName();

    connect(usuario, &QWebSocket::textMessageReceived, this, &Servidor::mensajeRecibido);
    connect(usuario, &QWebSocket::disconnected, this, &Servidor::desconectado);
}

void Servidor::mensajeRecibido(QString message)
{
    QWebSocket* socketEmisor = qobject_cast<QWebSocket*>(sender());
    // TODO: Agregar comprobacion de usuario
    auto doc = QJsonDocument::fromJson(message.toUtf8());
    if(doc.isNull())
    {
        qDebug() <<" Problema al parsear el mensaje";
    }
    auto caparazon = doc.object();
    if(caparazon[TIPO_STR] == MENSAJE_STR)
    {
        // Mensaje recibido
        Mensaje msj;
        msj.usuario = caparazon[USUARIO_STR].toString();
        msj.contenido = caparazon[CONTENIDO_STR].toString();
        emit mostrarMensaje(msj);
    }
    else if(caparazon[TIPO_STR] == CONEXION_STR)
    {
        // Se ha conectado un usuario: revisamos su nombre y lo vinculamos a la lista
        if(this->listaUsuarios.contains(caparazon[USUARIO_STR].toString()))
        {
            // Nombre duplicado
            QJsonObject mensaje;
            mensaje[TIPO_STR] = DESCONEXION_STR;
            mensaje[USUARIO_STR] = caparazon[USUARIO_STR];
            mensaje[CONTENIDO_STR] = "Nombre duplicado";
            socketEmisor->sendTextMessage(QJsonDocument(mensaje).toJson());
            return;     // Mensaje privado
        }
        this->listaUsuarios[caparazon[USUARIO_STR].toString()] = socketEmisor;
        emit mostrarNuevoUsuario(caparazon[USUARIO_STR].toString());
    }
    else if(caparazon[TIPO_STR] == DESCONEXION_STR)
    {
        // Se ha desconectado un usuario - Nos llegara un mensaje por la señal del socket
        this->listaUsuarios.remove(caparazon[USUARIO_STR].toString());
        emit mostrarUsuarioDesconectado(caparazon[USUARIO_STR].toString());
    }
    // Reenviamos a todos los usuarios, incluyendo al emisor
    for(auto i = this->listaUsuarios.cbegin(); i != this->listaUsuarios.cend(); ++i)
    {
        qDebug() <<" Enviando a " <<i.key();
        i.value()->sendTextMessage(message);
    }
    if(caparazon[TIPO_STR] == LISTA_STR)
    {
        // Nos han pedido la lista. Se la mandamos al usuario que la ha pedido
        QJsonObject mensaje, listaMensaje;
        mensaje[TIPO_STR] = LISTA_STR;
        mensaje[USUARIO_STR] = "";
        int i = 0;
        for(auto it = this->listaUsuarios.cbegin(); it != this->listaUsuarios.cend(); ++it)
        {
            listaMensaje[QString::number(i)] = it.key();
            ++i;
        }
        mensaje[CONTENIDO_STR] = listaMensaje;
        socketEmisor->sendTextMessage(QJsonDocument(mensaje).toJson());
    }
}

void Servidor::desconectado()
{
    qDebug() <<" User disconnected";

    QWebSocket* usuario = qobject_cast<QWebSocket*>(sender());
    usuario->deleteLater();
}

QMap<QString, QWebSocket*> Servidor::getLista()
{
    return this->listaUsuarios;
}
