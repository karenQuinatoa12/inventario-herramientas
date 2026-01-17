#include "ventanalogin.h"
#include "ui_ventanalogin.h"
#include <fstream>
#include <QMessageBox>

using namespace std;

VentanaLogin::VentanaLogin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VentanaLogin)
{
    ui->setupUi(this);
}

VentanaLogin::~VentanaLogin()
{
    delete ui;
}

void VentanaLogin::on_btnRegistrar_clicked()
{
    string user = ui->txtUser->text().toStdString();
    string pass = ui->txtPass->text().toStdString();

    if(user.empty() || pass.empty()) {
        QMessageBox::warning(this, "Error", "Debes llenar ambos campos");
        return;
    }

    ofstream archivo("usuarios.txt", ios::app);
    if(archivo.is_open()) {
        archivo << user << " " << pass << endl;
        archivo.close();
        QMessageBox::information(this, "Éxito", "¡Usuario registrado correctamente!");
    }
}

void VentanaLogin::on_btnIngresar_clicked()
{
    string userIn = ui->txtUser->text().toStdString();
    string passIn = ui->txtPass->text().toStdString();

    ifstream archivo("usuarios.txt");
    string u, p;
    bool encontrado = false;

    while(archivo >> u >> p) {
        if(u == userIn && p == passIn) {
            encontrado = true;
            break;
        }
    }
    archivo.close();

    if(encontrado) {
        accept(); // Cierra el login con éxito para abrir el menú principal
    } else {
        QMessageBox::critical(this, "Error", "Usuario o contraseña incorrectos");
    }
}
