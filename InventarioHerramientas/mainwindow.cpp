#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
