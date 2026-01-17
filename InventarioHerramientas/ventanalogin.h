#ifndef VENTANALOGIN_H
#define VENTANALOGIN_H

#include <QDialog>

namespace Ui {
class VentanaLogin;
}

class VentanaLogin : public QDialog
{
    Q_OBJECT

public:
    explicit VentanaLogin(QWidget *parent = nullptr);
    ~VentanaLogin();

private:
    Ui::VentanaLogin *ui;
private slots:
    void on_btnRegistrar_clicked();
    void on_btnIngresar_clicked();
};

#endif // VENTANALOGIN_H
