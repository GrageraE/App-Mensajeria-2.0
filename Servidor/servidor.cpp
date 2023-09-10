#include "servidor.h"

#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

Servidor::Servidor(int _puerto, const QString& _nombre, bool seguro, const QString& passwd, QObject* _parent)
    : QObject(_parent), port(_puerto), name(_nombre), sec(seguro), passwd(passwd), banList()
{
    if(this->sec)
    {
        this->server = new QWebSocketServer(this->name, QWebSocketServer::SecureMode, this);
        QSslConfiguration conf;
        QFile certFile("./server.cert");
        QFile keyFile("./server.key");
        if(!certFile.open(QIODevice::ReadOnly))
        {
            qDebug() <<"No se puede abrir ./server.key";
        }
        if(!keyFile.open(QIODevice::ReadOnly))
        {
            qDebug() <<" No se puede abrir ./server.cert";
        }
        QSslCertificate cert(&certFile, QSsl::Pem);
        QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, "localhost");
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        conf.setLocalCertificate(cert);
        conf.setPrivateKey(key);
        certFile.close();
        keyFile.close();

        this->server->setSslConfiguration(conf);
    }
    else
    {
        this->server = new QWebSocketServer(this->name, QWebSocketServer::NonSecureMode, this);
    }

    if(this->server->listen(QHostAddress::Any, this->port))
    {
        qDebug() <<" Server listening on port " << this->port;
        connect(this->server, &QWebSocketServer::newConnection, this, &Servidor::nuevoUsuario);
        connect(this->server, &QWebSocketServer::serverError, this, &Servidor::errorSetup);
        if(seguro)
            connect(this->server, &QWebSocketServer::sslErrors, this, &Servidor::errorSsl);
    }
    // Cargamos la lista de ban
    QFile fileBan("./ban.txt");
    if(fileBan.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&fileBan);
        while(!stream.atEnd())
        {
            QString userBanned = stream.readLine();
            this->banList.push_back(userBanned);
            qDebug() <<" Cargando usuario baneado: " <<userBanned;
        }
        fileBan.close();
    }
    else qDebug() <<" No se puede cargar la lista de baneos.";
}

Servidor::~Servidor()
{
    QJsonObject msg_json;
    msg_json[TIPO_STR] = DESCONEXION_STR;
    msg_json[USUARIO_STR] = "";
    msg_json[CONTENIDO_STR] = "Servidor cerrado";
    QString msg = QJsonDocument(msg_json).toJson();

    for(auto i = this->listaUsuarios.cbegin(); i != this->listaUsuarios.cend(); ++i)
    {
        i.value()->sendTextMessage(msg);
        // Dealojamos a al usuario
        disconnect(i.value(), &QWebSocket::textMessageReceived, this, &Servidor::mensajeRecibido);
        disconnect(i.value(), &QWebSocket::disconnected, this, &Servidor::desconectado);
        i.value()->close();
        i.value()->deleteLater();
    }
    // Guardamos la lista de baneos
    QFile file("./ban.txt");
    if(file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream stream(&file);
        for(const auto& ip : this->banList)
        {
            stream << ip <<"\n";
        }
        file.close();
    }
    else qDebug() <<" No se puede guardar la lista de baneos";

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

    auto doc = QJsonDocument::fromJson(message.toUtf8());
    if(doc.isNull())
    {
        qDebug() <<" Problema al parsear el mensaje";
    }
    auto caparazon = doc.object();
    if(caparazon[TIPO_STR] == CONEXION_STR)
    {
        // Se ha conectado un usuario: revisamos su nombre y lo vinculamos a la lista
        if(this->listaUsuarios.contains(caparazon[USUARIO_STR].toString()))
        {
            // Nombre duplicado
            QJsonObject mensaje;
            mensaje[TIPO_STR] = DESCONEXION_STR;
            mensaje[USUARIO_STR] = "";
            mensaje[CONTENIDO_STR] = "Nombre duplicado";
            socketEmisor->sendTextMessage(QJsonDocument(mensaje).toJson());
            return;     // Mensaje privado
        }
        if(this->banList.contains(socketEmisor->peerAddress().toString()))
        {
            // Usuario baneado
            QJsonObject mensaje;
            mensaje[TIPO_STR] = DESCONEXION_STR;
            mensaje[USUARIO_STR] = "";
            mensaje[CONTENIDO_STR] = "Usuario baneado";
            socketEmisor->sendTextMessage(QJsonDocument(mensaje).toJson());
            return;
        }
        if(this->sec && !this->passwd.isEmpty())
        {
            // Se requiere passwd
            if(caparazon[CONTENIDO_STR].toString() != this->passwd)
            {
                if(this->contadores[socketEmisor] > 2)
                {
                    // Numero max de intentos superado -> Baneo
                    QJsonObject expulsion;
                    expulsion[TIPO_STR] = DESCONEXION_STR;
                    expulsion[USUARIO_STR] = "";
                    expulsion[CONTENIDO_STR] = "Baneado por 3 intentos fallidos";
                    socketEmisor->sendTextMessage(QJsonDocument(expulsion).toJson());

                    this->banList.push_back(socketEmisor->peerAddress().toString());
                    return;
                }
                ++this->contadores[socketEmisor];
                QJsonObject msj;
                msj[TIPO_STR] = CONEXION_STR;
                msj[USUARIO_STR] = "";
                socketEmisor->sendTextMessage(QJsonDocument(msj).toJson());
                return;
            }
        }
        this->contadores.remove(socketEmisor);
        this->listaUsuarios[caparazon[USUARIO_STR].toString()] = socketEmisor;
        emit mostrarNuevoUsuario(caparazon[USUARIO_STR].toString());
    }
    // Esto se verifica si el usuario esta en la lista
    if(this->listaUsuarios.value(caparazon[USUARIO_STR].toString()) == socketEmisor)
    {
        if(caparazon[TIPO_STR] == DESCONEXION_STR)
        {
            // Se ha desconectado un usuario
            disconnect(socketEmisor, &QWebSocket::textMessageReceived, this, &Servidor::mensajeRecibido);
            disconnect(socketEmisor, &QWebSocket::disconnected, this, &Servidor::desconectado);
            socketEmisor->deleteLater();
            this->listaUsuarios.remove(caparazon[USUARIO_STR].toString());
            emit mostrarUsuarioDesconectado(caparazon[USUARIO_STR].toString());
        }
        else if(caparazon[TIPO_STR] == MENSAJE_STR)
        {
            // Mensaje recibido
            Mensaje msj;
            msj.usuario = caparazon[USUARIO_STR].toString();
            msj.contenido = caparazon[CONTENIDO_STR].toString();
            emit mostrarMensaje(msj);
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
}

void Servidor::desconectado()
{
    qDebug() <<" Usuario desconectado inesperadamente";

    QWebSocket* usuario = qobject_cast<QWebSocket*>(sender());
    this->listaUsuarios.remove(this->listaUsuarios.key(usuario));
    this->contadores.remove(usuario);
    usuario->deleteLater();
    emit mostrarUsuarioDesconectado(this->listaUsuarios.key(usuario));
}

void Servidor::expulsar(QString nombreUsuario, bool _ban)
{
    QJsonObject msg;
    msg[TIPO_STR] = DESCONEXION_STR;
    msg[USUARIO_STR] = "";
    msg[CONTENIDO_STR] = "Expulsado";
    auto* socket = this->listaUsuarios.value(nombreUsuario, nullptr);
    if(!socket)
    {
        qDebug() <<" No se encuentra el socket del usuario a expulsar. Nombre: " <<nombreUsuario;
        return;
    }
    socket->sendTextMessage(QJsonDocument(msg).toJson());

    if(_ban)
    {
        // La lista se guardara a la salida
        this->banList.push_back(this->listaUsuarios.value(nombreUsuario)->peerAddress().toString());
    }
}

void Servidor::perdonar(QString _ip)
{
    qDebug() <<" Perdonando a IP: " << _ip;
    this->banList.removeAll(_ip);
}

QMap<QString, QWebSocket*> Servidor::getLista()
{
    return this->listaUsuarios;
}

QStringList Servidor::getBaneados()
{
    return this->banList;
}

void Servidor::errorSetup(QWebSocketProtocol::CloseCode e)
{
    qDebug() <<" Error en el arranque del servidor: " <<e;
}

void Servidor::errorSsl(const QList<QSslError>& _errores)
{
    for(const auto& error : _errores)
    {
        qDebug() <<" Error SSL: " <<error.errorString();
    }
}
