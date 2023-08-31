#ifndef VENTANASERVIDORES_H
#define VENTANASERVIDORES_H

#include <QDialog>

namespace Ui {
class ventanaServidores;
}

class ventanaServidores : public QDialog
{
    Q_OBJECT

public:

    struct DatosServidor
    {
        QString nombreUsuario;
        QString ip;
        quint32 puerto;
    };

    explicit ventanaServidores(QWidget *parent = nullptr);
    ~ventanaServidores();

    DatosServidor getResultado();

private slots:
    void on_pushButton_clicked();

    void on_botonConectar_clicked();

private:
    Ui::ventanaServidores *ui;
    DatosServidor resultado;
};

#endif // VENTANASERVIDORES_H
