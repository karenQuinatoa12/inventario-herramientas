#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <fstream>
#include <vector>
#include <QRegularExpressionValidator>
#include "dialogmodificar.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tblMostrar->setStyleSheet("color: black; background-color: white;");
    ui->txtNombre->setStyleSheet("background-color: white; color: black;");
    ui->txtCantidad->setStyleSheet("background-color: white; color: black;");
    ui->txtPrecio->setStyleSheet("background-color: white; color: black;");
    ui->txtBuscar->setStyleSheet("background-color: white; color: black;");
    ui->tblMostrar->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblMostrar->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tblMostrar->setStyleSheet(
        "QTableWidget { "
        "  background-color: white; "
        "  color: black; "
        "  gridline-color: #d3d3d3; " // Color de las líneas de la cuadrícula
        "  selection-background-color: #ff9800; " // Fondo naranja al seleccionar
        "  selection-color: white; " // Texto blanco al seleccionar
        "}"
        "QTableWidget::item:selected { "
        "  background-color: #ff9800; " // Refuerza el naranja sólido
        "  color: white; "
        "}"
        );
    ui->tblMostrar->verticalHeader()->setVisible(false);
    ui->centralwidget->setStyleSheet("#centralwidget { border-image: url(:/fondo.png) 0 0 0 0 stretch stretch; }");

    // mainwindow.cpp
    ui->centralwidget->setStyleSheet("#centralwidget { border-image: url(:/fondo.png) 0 0 0 0 stretch stretch; }");
    int ultimoID = 0;
    ifstream archivo("inventario.txt");
    if (archivo.is_open()) {
        string id, nom, cant, prec_str; // Cambiamos el nombre para no confundirnos
        while(archivo >> id >> nom >> cant >> prec_str) {
            int idInt = stoi(id);
            if (idInt > ultimoID) ultimoID = idInt;

            int fila = ui->tblMostrar->rowCount();
            ui->tblMostrar->insertRow(fila);

            // Convertimos el precio de texto a número para darle formato de 2 decimales
            double precioLeido = stod(prec_str);

            // Columnas 0, 1 y 2 (ID, Nombre, Cantidad)
            ui->tblMostrar->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(id)));
            ui->tblMostrar->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(nom)));
            ui->tblMostrar->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(cant)));

            // Columna 3 (Precio) con el formato de 2 decimales
            QTableWidgetItem *itPrecio = new QTableWidgetItem(QString::number(precioLeido, 'f', 2));
            itPrecio->setForeground(Qt::black);
            ui->tblMostrar->setItem(fila, 3, itPrecio);

            // Asegúrate de ponerle color negro a los otros items también si lo necesitas
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

    bool yaExiste = false;
    for (int i = 0; i < ui->tblMostrar->rowCount(); ++i) {
        if (ui->tblMostrar->item(i, 1)->text().toLower() == nombre.toLower()) {
            yaExiste = true;
            break;
        }
    }

    if (yaExiste) {
        QMessageBox::warning(this, "Ferretería", "Este producto ya está registrado en el inventario.");
        return;
    }


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

        QTableWidgetItem *itAgregar0 = new QTableWidgetItem(QString::number(nuevoID));
        itAgregar0->setForeground(Qt::black);
        ui->tblMostrar->setItem(fila, 0, itAgregar0);

        QTableWidgetItem *itAgregar1 = new QTableWidgetItem(nombre);
        itAgregar1->setForeground(Qt::black);
        ui->tblMostrar->setItem(fila, 1, itAgregar1);

        QTableWidgetItem *itAgregar2 = new QTableWidgetItem(QString::number(cantidad));
        itAgregar2->setForeground(Qt::black);
        ui->tblMostrar->setItem(fila, 2, itAgregar2);


        QTableWidgetItem *itAgregar3 = new QTableWidgetItem(QString::number(precio, 'f', 2));
        itAgregar3->setForeground(Qt::black);
        ui->tblMostrar->setItem(fila, 3, itAgregar3);

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


        ui->tblMostrar->clearSelection();
    }
}

void MainWindow::on_btnEliminar_clicked()
{
    if (ui->tblMostrar->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Eliminar", "Seleccione un producto para borrar.");
        return;
    }

    int filaAEliminar = ui->tblMostrar->currentRow();

    if (QMessageBox::question(this, "Confirmar", "¿Eliminar este producto y reorganizar IDs?") == QMessageBox::Yes) {

        // 1. Eliminamos de la tabla visual
        ui->tblMostrar->removeRow(filaAEliminar);

        // 2. REORGANIZACIÓN: Recorremos TODA la tabla para reasignar IDs dinámicamente
        // Esto hace que si borras el 2, el que era 3 ahora se escriba como 2 en el archivo
        ifstream leer("inventario.txt");
        vector<string> nombres, cantidades, precios;
        string id_viejo, n, c, p;

        // Leemos todos los datos excepto el que eliminamos
        int contadorFila = 0;
        while(leer >> id_viejo >> n >> c >> p) {
            if (contadorFila != filaAEliminar) {
                nombres.push_back(n);
                cantidades.push_back(c);
                precios.push_back(p);
            }
            contadorFila++;
        }
        leer.close();

        // 3. Reescribimos el archivo con NUEVOS IDs SECUENCIALES
        ofstream escribir("inventario.txt", ios::trunc);
        for(int i = 0; i < nombres.size(); i++) {
            // El nuevo ID es simplemente la posición (i + 1)
            escribir << (i + 1) << " " << nombres[i] << " " << cantidades[i] << " " << precios[i] << endl;
        }
        escribir.close();

        // 4. Actualizamos la tabla visualmente para que los IDs coincidan con el archivo
        for(int i = 0; i < ui->tblMostrar->rowCount(); i++) {
            ui->tblMostrar->item(i, 0)->setText(QString::number(i + 1));
        }

        // 5. Actualizamos el próximo ID disponible en el formulario
        ui->txtID->setText(QString::number(ui->tblMostrar->rowCount() + 1));

        QMessageBox::information(this, "Éxito", "Producto eliminado y numeración reiniciada.");
    }
}

//correcion





