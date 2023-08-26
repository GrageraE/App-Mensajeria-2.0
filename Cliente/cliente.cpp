#include "cliente.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QtWebSockets/QWebSocket>

Cliente::Cliente(QString _servidor,  QString _usuario, QObject* _parent)
    : QObject(_parent), usuario(_usuario), servidor(_servidor)
{
    qDebug() <<" Conectando a " << this->servidor <<" con nombre: " <<this->usuario;
    this->socket.open(QUrl(this->servidor));
    connect(&this->socket, &QWebSocket::connected, this, &Cliente::conectado);
    connect(&this->socket, &QWebSocket::disconnected, this, &Cliente::desconectado);
}

Cliente::~Cliente()
{
    // Mandamos el mensaje de desconexion
    QJsonObject mensaje;
    mensaje[TIPO_STR] = DESCONEXION_STR;
    mensaje[USUARIO_STR] = this->usuario;
    QJsonDocument doc(mensaje);
    this->socket.sendTextMessage(doc.toJson());

    this->socket.close();
    //this->socket.deleteLater();
}

void Cliente::conectado()
{
    qDebug() <<" Conectados";
    connect(&this->socket, &QWebSocket::textMessageReceived, this, &Cliente::mensajeRecibido);

    QJsonObject mensajeConexion;
    mensajeConexion[TIPO_STR] = CONEXION_STR;
    mensajeConexion[USUARIO_STR] = this->usuario;
    QJsonDocument doc(mensajeConexion);
    this->socket.sendTextMessage(doc.toJson());
}

void Cliente::desconectado()
{
    qDebug() <<" Desconectados"; // TODO: Manejar motivos de desconexion (baneo, perdida de conexion, etc)
    this->socket.close();
}

void Cliente::mensajeRecibido(QString _msg)
{
    // TODO: Parsear mensaje
    auto doc = QJsonDocument::fromJson(_msg.toUtf8());
    if(doc.isNull())
    {
        qDebug() <<" [CLIENTE - mensajeRecibido(): Problema al parsear el caparazon";
        return;
    }
    auto caparazon = doc.object();
    if(caparazon[TIPO_STR] == MENSAJE_STR)
    {
        // Hemos recibido un mensaje
        Mensaje msj;
        msj.usuario = caparazon[USUARIO_STR].toString();
        msj.contenido = caparazon[CONTENIDO_STR].toString();
        emit mandarMensaje(msj);
    }
    else if(caparazon[TIPO_STR] == CONEXION_STR)
    {
        // Se ha conectado un nuevo usuario
        emit mandarConexion(caparazon[USUARIO_STR].toString());
    }
    else if(caparazon[TIPO_STR] == DESCONEXION_STR)
    {
        // Se ha desconectado un usuario
        emit mandarDesconexion(caparazon[USUARIO_STR].toString());
    }
}

void Cliente::enviarMensaje(QString _msg)
{
    QJsonObject caparazonMensaje;
    caparazonMensaje[TIPO_STR] = MENSAJE_STR;
    caparazonMensaje[USUARIO_STR] = this->usuario;
    caparazonMensaje[CONTENIDO_STR] = _msg;

    QJsonDocument doc(caparazonMensaje);
    this->socket.sendTextMessage(doc.toJson());
}
