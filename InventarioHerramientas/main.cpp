#include "login.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int codigoSalida = 0;

    do {

        VentanaLogin wLogin;

        if (wLogin.exec() == QDialog::Accepted) {
            MainWindow wMain;
            wMain.show();
            codigoSalida = a.exec();
        } else {
            codigoSalida = 0;
        }
    } while (codigoSalida == 1000);

    return 0;
}
