#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->setupUi(this);

    // Configuración de la tabla para la ferretería
    // Asegúrate de que tu QTableWidget se llame 'tableWidget' en el editor
    ui->tableWidget->setColumnCount(4);
    QStringList cabeceras;
    cabeceras << "ID" << "Nombre" << "Stock" << "Estado";
    ui->tableWidget->setHorizontalHeaderLabels(cabeceras);

    // Hace que las columnas ocupen todo el ancho disponible
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_btnRegistrar_clicked()
{
 QMessageBox::information(this, "Inventario", "Abriendo formulario de registro...");
}
void MainWindow::on_btnSalir_clicked()
{
    close();
}
