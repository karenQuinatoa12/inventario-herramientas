#include "login.h"
#include "ui_login.h"
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
    QString qUser = ui->txtUser->text();
    QString qPass = ui->txtPass->text();


    for(const QChar &c : qUser) {
        if(!c.isLetter()) {
            QMessageBox::warning(this, "Usuario Inválido", "El usuario solo puede contener letras y no debe tener espacios.");
            return;
        }
    }

    if(qPass.contains(" ")) {
        QMessageBox::warning(this, "Contraseña Inválida", "La contraseña no puede contener espacios.");
        return;
    }

    string user = qUser.toStdString();
    string pass = qPass.toStdString();


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
        QMessageBox::warning(this, "Usuario Duplicado", "Este nombre de usuario ya está registrado.");
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
        QMessageBox::critical(this, "Error de Sistema", "No existe base de datos. Registra un usuario primero.");
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
