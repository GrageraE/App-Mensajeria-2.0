#include "cliente.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QtNetwork/QSslCertificate>
#include <QtWebSockets/QWebSocket>
#include <QMessageBox>
#include <QFile>

Cliente::Cliente(const QString& _servidor, int _puerto, const QString& _usuario, bool seguro, QObject* _parent)
    : QObject(_parent), usuario(_usuario), servidor(_servidor), puerto(_puerto), conectados(false)
{
    qDebug() <<" Conectando a " << this->servidor <<" con nombre: " <<this->usuario;
    this->socket = new QWebSocket;
    if(seguro)
    {
        QFile certFile("./server.cert");
        if(!certFile.open(QIODevice::ReadOnly))
            qDebug() <<" No se puede abrir server.cert";

        QSslConfiguration conf;
        QSslCertificate cert(&certFile, QSsl::Pem);
        conf.addCaCertificate(cert);
        certFile.close();
        this->socket->setSslConfiguration(conf);
        this->socket->open(QUrl("wss://" + this->servidor + ":" + QString::number(this->puerto)));
    }
    else
        this->socket->open(QUrl("ws://" + this->servidor + ":" + QString::number(this->puerto)));
    connect(this->socket, &QWebSocket::connected, this, &Cliente::conectado);
    connect(this->socket, &QWebSocket::disconnected, this, &Cliente::desconectado);
    if(seguro)
        connect(this->socket, &QWebSocket::sslErrors, this, &Cliente::errorSsl);
}

Cliente::~Cliente()
{
    disconnect(this->socket, &QWebSocket::disconnected, this, &Cliente::desconectado);
    // Mandamos el mensaje de desconexion
    QJsonObject mensaje;
    mensaje[TIPO_STR] = DESCONEXION_STR;
    mensaje[USUARIO_STR] = this->usuario;
    QJsonDocument doc(mensaje);
    this->socket->sendTextMessage(doc.toJson());

    if(this->socket->isValid())
    {
        this->socket->close();
        this->socket->deleteLater();
    }
}

void Cliente::conectado()
{
    qDebug() <<" Conectados";
    this->conectados = true;
    connect(this->socket, &QWebSocket::textMessageReceived, this, &Cliente::mensajeRecibido);

    QJsonObject mensajeConexion;
    mensajeConexion[TIPO_STR] = CONEXION_STR;
    mensajeConexion[USUARIO_STR] = this->usuario;
    QJsonDocument doc(mensajeConexion);
    this->socket->sendTextMessage(doc.toJson());
}

void Cliente::desconectado()
{
    qDebug() <<" Desconectados forzosamente - Inesperado";
    emit mandarDesconexionServidor("Desconocido");
}

void Cliente::mensajeRecibido(QString _msg)
{
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
        // Se ha desconectado el servidor
        if(caparazon[USUARIO_STR].toString().isEmpty())
        {
            emit mandarDesconexionServidor(caparazon[CONTENIDO_STR].toString());
            return;
        }
        // Se ha desconectado un usuario
        emit mandarDesconexion(caparazon[USUARIO_STR].toString());
    }
    else if(caparazon[TIPO_STR] == LISTA_STR)
    {
        if(caparazon[USUARIO_STR] == "")
        {
            QStringList lista;
            QJsonObject listaJson = caparazon[CONTENIDO_STR].toObject();
            for(auto it = listaJson.begin(); it != listaJson.end(); ++it)
            {
                lista.push_back(it.value().toString());
            }
            emit mandarLista(lista);
        }
    }
    else
    {
        qDebug() <<" Tipo no reconocido";
    }
}

void Cliente::enviarMensaje(QString _msg)
{
    if(this->conectados)
    {
        QJsonObject caparazonMensaje;
        caparazonMensaje[TIPO_STR] = MENSAJE_STR;
        caparazonMensaje[USUARIO_STR] = this->usuario;
        caparazonMensaje[CONTENIDO_STR] = _msg;

        QJsonDocument doc(caparazonMensaje);
        this->socket->sendTextMessage(doc.toJson());
    }
}

void Cliente::pedirListaUsuarios()
{
    QJsonObject mensaje;
    mensaje[TIPO_STR] = LISTA_STR;
    mensaje[USUARIO_STR] = this->usuario;
    QJsonDocument doc(mensaje);
    this->socket->sendTextMessage(doc.toJson());
}

void Cliente::errorSsl(const QList<QSslError>& _errores)
{
    for(const auto& error : _errores)
    {
        qDebug() <<" Error SSL: " <<error.errorString();
    }
    // Ignoraremos errores SSl (utilizamos certificados básicos)
    // TODO: Poder configurar esto (mas/menos restrictivo)
    this->socket->ignoreSslErrors(_errores);
}
