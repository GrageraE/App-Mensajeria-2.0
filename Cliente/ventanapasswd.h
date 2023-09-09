#ifndef VENTANAPASSWD_H
#define VENTANAPASSWD_H

#include <QDialog>

namespace Ui {
class ventanaPasswd;
}

class ventanaPasswd : public QDialog
{
    Q_OBJECT

public:
    explicit ventanaPasswd(QWidget *parent = nullptr);
    ~ventanaPasswd();

    QString getResultado();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ventanaPasswd *ui;

    QString res;
};

#endif // VENTANAPASSWD_H
