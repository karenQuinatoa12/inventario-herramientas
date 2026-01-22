#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <fstream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // mainwindow.cpp
    ui->centralwidget->setStyleSheet("#centralwidget { border-image: url(:/fondo.png) 0 0 0 0 stretch stretch; }");
    int ultimoID = 0;

    ifstream archivo("inventario.txt");
    if (archivo.is_open()) {
        string id, nom, cant, prec;
        while(archivo >> id >> nom >> cant >> prec) {
            int idInt = stoi(id);
            if (idInt > ultimoID) ultimoID = idInt;

            int fila = ui->tblMostrar->rowCount();
            ui->tblMostrar->insertRow(fila);
            ui->tblMostrar->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(id)));
            ui->tblMostrar->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(nom)));
            ui->tblMostrar->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(cant)));
            ui->tblMostrar->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(prec)));
        }
        archivo.close();
    }


    ui->txtID->setReadOnly(true);
    ui->txtID->setText(QString::number(ultimoID + 1));


    ui->tblMostrar->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_txtBuscar_textChanged(const QString &arg1){

    if (arg1.isEmpty()) {
        for (int i = 0; i < ui->tblMostrar->rowCount(); ++i) {
            ui->tblMostrar->setRowHidden(i, false);
        }
        return;
    }


    QString filtro = arg1.toLower();


    int columnaABuscar = (ui->cbxBuscar->currentText() == "ID") ? 0 : 1;


    for (int i = 0; i < ui->tblMostrar->rowCount(); ++i) {
        QTableWidgetItem *item = ui->tblMostrar->item(i, columnaABuscar);
        if (item) {

            bool coincide = item->text().toLower().contains(filtro);


            ui->tblMostrar->setRowHidden(i, !coincide);
        }
    }
}

void MainWindow::on_btnLimpiar_clicked(){
    ui->txtNombre->clear();
    ui->txtCantidad->clear();
    ui->txtPrecio->clear();
    ui->txtBuscar->clear();
}

void MainWindow::on_btnSalir_clicked()
{
    QMessageBox::StandardButton respuesta;
    respuesta = QMessageBox::question(this, "Cerrar Sesión", "¿Estás seguro de que quieres volver al Login?",
                                      QMessageBox::Yes | QMessageBox::No);

    if (respuesta == QMessageBox::Yes) {
        qApp->exit(1000);
    }
}

void MainWindow::on_btnAgregar_clicked()
{

    int nuevoID = 1;
    int totalFilas = ui->tblMostrar->rowCount();

    if (totalFilas > 0) {

        nuevoID = ui->tblMostrar->item(totalFilas - 1, 0)->text().toInt() + 1;
    }


    QString nombre = ui->txtNombre->text();
    QString cantidadStr = ui->txtCantidad->text();
    QString precioStr = ui->txtPrecio->text();


    if(nombre.isEmpty() || cantidadStr.isEmpty() || precioStr.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debe llenar Nombre, Cantidad y Precio.");
        return;
    }

    bool okCant, okPrec;
    int cantidad = cantidadStr.toInt(&okCant);
    double precio = precioStr.toDouble(&okPrec);

    if(!okCant || !okPrec || cantidad < 0 || precio <= 0) {
        QMessageBox::critical(this, "Error", "Verifique que Cantidad y Precio sean valores válidos.");
        return;
    }


    ofstream archivo("inventario.txt", ios::app);
    if(archivo.is_open()) {
        archivo << nuevoID << " " << nombre.toStdString() << " " << cantidad << " " << precio << endl;
        archivo.close();

        int fila = ui->tblMostrar->rowCount();
        ui->tblMostrar->insertRow(fila);
        ui->tblMostrar->setItem(fila, 0, new QTableWidgetItem(QString::number(nuevoID)));
        ui->tblMostrar->setItem(fila, 1, new QTableWidgetItem(nombre));
        ui->tblMostrar->setItem(fila, 2, new QTableWidgetItem(QString::number(cantidad)));
        ui->tblMostrar->setItem(fila, 3, new QTableWidgetItem(QString::number(precio)));


        ui->txtID->setText(QString::number(nuevoID + 1));


        ui->txtNombre->clear();
        ui->txtCantidad->clear();
        ui->txtPrecio->clear();
        ui->txtNombre->setFocus();
    }
}

void MainWindow::on_btnModificar_clicked()
{

}

void MainWindow::on_btnEliminar_clicked()
{

}
//correcion






