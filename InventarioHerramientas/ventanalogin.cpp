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
    ui->txtUser->setFocus();
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
        QMessageBox::warning(this, "Error", "No puedes registrar un usuario o contraseña vacíos.");
        return;
    }

    ifstream archivoLectura("usuarios.txt");
    string u, p;
    bool existe = false;

    if(archivoLectura.is_open()){
        while(archivoLectura >> u >> p) {
            if(u == user) {
                existe = true;
                break;
            }
        }
        archivoLectura.close();
    }

    if(existe) {
        QMessageBox::warning(this, "Usuario Duplicado", "Este nombre de usuario ya está registrado. Elige otro.");
        ui->txtUser->clear();
        ui->txtPass->clear();
        ui->txtUser->setFocus();

        return;
    }

    ofstream archivoEscritura("usuarios.txt", ios::app);
    if(archivoEscritura.is_open()) {
        archivoEscritura << user << " " << pass << endl;
        archivoEscritura.close();
        QMessageBox::information(this, "Éxito", "¡Usuario registrado correctamente!");

        ui->txtUser->clear();
        ui->txtPass->clear();
    }
}

void VentanaLogin::on_btnIngresar_clicked()
{
    string userIn = ui->txtUser->text().toStdString();
    string passIn = ui->txtPass->text().toStdString();

    if(userIn.empty() || passIn.empty()) {
        QMessageBox::warning(this, "Campos Incompletos", "Por favor, ingresa tus credenciales.");
        return;
    }

    ifstream archivo("usuarios.txt");
    string u, p;
    bool encontrado = false;

    if(!archivo.is_open()) {
        QMessageBox::critical(this, "Error de Sistema", "No existe base de datos de usuarios. Registra uno primero.");
        return;
    }

    while(archivo >> u >> p) {
        if(u == userIn && p == passIn) {
            encontrado = true;
            break;
        }
    }
    archivo.close();

    if(encontrado) {
        accept();
    } else {
        QMessageBox::critical(this, "Acceso Denegado", "Usuario o contraseña incorrectos.");
        ui->txtPass->clear();
        ui->txtPass->setFocus();
    }
}
