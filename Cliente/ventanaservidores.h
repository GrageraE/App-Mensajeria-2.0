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
        QString puerto;

        bool operator== (const DatosServidor& other) const
        {
            return (this->ip == other.ip && this->nombreUsuario == other.nombreUsuario && this->puerto == other.puerto);
        }
    };

    explicit ventanaServidores(QWidget *parent = nullptr);
    ~ventanaServidores();

    DatosServidor getResultado();

    void agregarDatos(const DatosServidor& _datos);

private slots:
    void on_pushButton_clicked();

    void on_botonConectar_clicked();

    void on_botonEliminar_clicked();

private:
    Ui::ventanaServidores *ui;
    DatosServidor resultado;
    QList<DatosServidor> listaServidores;
};

#endif // VENTANASERVIDORES_H
