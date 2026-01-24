#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <fstream>
#include <vector>
#include "dialogmodificar.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tblMostrar->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblMostrar->setSelectionMode(QAbstractItemView::SingleSelection);
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
void MainWindow::actualizarArchivo(int idObjetivo, string nuevoNombre, int nuevaCant, double nuevoPrecio) {
    ifstream archivoLectura("inventario.txt");
    vector<string> lineas;
    string id, nom, cant, prec;

    // Leemos todo el archivo y actualizamos la línea coincidente en memoria
    if (archivoLectura.is_open()) {
        while (archivoLectura >> id >> nom >> cant >> prec) {
            if (stoi(id) == idObjetivo) {
                // Esta es la línea a modificar, guardamos los nuevos datos
                lineas.push_back(id + " " + nuevoNombre + " " + to_string(nuevaCant) + " " + QString::number(nuevoPrecio, 'f', 2).toStdString());
            } else {
                // Mantenemos los datos originales
                lineas.push_back(id + " " + nom + " " + cant + " " + prec);
            }
        }
        archivoLectura.close();
    }

    // Reescribimos el archivo con los datos actualizados
    ofstream archivoEscritura("inventario.txt", ios::trunc); // ios::trunc borra el contenido anterior
    if (archivoEscritura.is_open()) {
        for (const string &linea : lineas) {
            archivoEscritura << linea << endl;
        }
        archivoEscritura.close();
    }
}

// En mainwindow.cpp

void MainWindow::on_btnModificar_clicked()
{
    // 1. VALIDACIÓN ESTRICTA: Verificamos si la lista de items seleccionados está vacía
    if (ui->tblMostrar->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Selección Requerida", "Por favor, elija el producto a modificar de la lista.");
        return; // Detenemos la función aquí si no hay selección
    }

    int fila = ui->tblMostrar->currentRow();

    // Doble verificación de seguridad (por si acaso fila sea inválida)
    if (fila < 0) {
        return;
    }

    // 2. Obtener datos actuales de la fila seleccionada
    // (Asegúrate de que las columnas coincidan con tu tabla: 0=ID, 1=Nombre, 2=Cantidad, 3=Precio)
    QString idStr = ui->tblMostrar->item(fila, 0)->text();
    QString nombreActual = ui->tblMostrar->item(fila, 1)->text();
    QString cantActual = ui->tblMostrar->item(fila, 2)->text();
    QString precioActual = ui->tblMostrar->item(fila, 3)->text();

    // 3. Abrir la ventana emergente
    DialogModificar ventanaEdicion(this);
    ventanaEdicion.setDatos(nombreActual, cantActual, precioActual);

    // 4. Si el usuario guarda los cambios
    if (ventanaEdicion.exec() == QDialog::Accepted) {
        QString nuevoNombre = ventanaEdicion.getNombre();
        QString nuevaCantStr = ventanaEdicion.getCantidad();
        QString nuevoPrecioStr = ventanaEdicion.getPrecio();

        // Actualizamos el archivo .txt
        actualizarArchivo(idStr.toInt(), nuevoNombre.toStdString(), nuevaCantStr.toInt(), nuevoPrecioStr.toDouble());

        // Actualizamos la tabla visualmente
        ui->tblMostrar->item(fila, 1)->setText(nuevoNombre);
        ui->tblMostrar->item(fila, 2)->setText(nuevaCantStr);
        ui->tblMostrar->item(fila, 3)->setText(nuevoPrecioStr);

        QMessageBox::information(this, "Éxito", "Producto modificado correctamente.");

        // Quitamos la selección para obligar a seleccionar de nuevo si quiere modificar otro
        ui->tblMostrar->clearSelection();
    }
}

void MainWindow::on_btnEliminar_clicked()
{

}
//correcion






