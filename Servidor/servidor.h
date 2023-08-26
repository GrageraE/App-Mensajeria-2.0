#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <QObject>

#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

#include "mensaje.h"

class Servidor : public QObject
{
    Q_OBJECT
public:
    Servidor(int _puerto, const QString& _nombre, QObject* _parent);
    ~Servidor();


signals:
    void mostrarMensaje(Mensaje msj);

    //void mostrarNuevoUsuario(QString _nombre);

private slots:
    void nuevoUsuario();

    void mensajeRecibido(QString message);

    void desconectado();

private:
    int port;
    QString name;
    QWebSocketServer* server; // low-level

    QMap<QWebSocket*, QString> listaUsuarios;
};

#endif // SERVIDOR_H
