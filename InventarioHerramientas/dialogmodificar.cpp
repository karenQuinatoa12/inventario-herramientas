#include "dialogmodificar.h"
#include "ui_dialogmodificar.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>

DialogModificar::DialogModificar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogModificar)
{
    ui->setupUi(this);
    setWindowTitle("Modificar Producto");
    QRegularExpression rx("^[a-zA-ZáéíóúÁÉÍÓÚñÑ\\s]*$");

    // Creamos el validador basado en esa regla
    QValidator *validadorNombre = new QRegularExpressionValidator(rx, this);

    // Aplicamos el validador al campo de texto del nombre
    ui->txtNombreMod->setValidator(validadorNombre);
    QRegularExpression rxPrecio("^\\d{1,8}(\\.\\d{0,2})?$");
    QValidator *valPrecio = new QRegularExpressionValidator(rxPrecio, this);
    ui->txtPrecioMod->setValidator(valPrecio);

    // --- 3. (OPCIONAL) VALIDACIÓN CANTIDAD (Solo enteros) ---
    // Esto evita que pongan decimales o letras en la cantidad
    QRegularExpression rxCantidad("^\\d{1,5}$"); // Máximo 5 dígitos enteros
    QValidator *valCantidad = new QRegularExpressionValidator(rxCantidad, this);
    ui->txtCantidadMod->setValidator(valCantidad);
}

DialogModificar::~DialogModificar()
{
    delete ui;
}

void DialogModificar::setDatos(QString nombre, QString cantidad, QString precio) {
    // Asumiendo que en tu .ui creaste estos campos con estos nombres
    ui->txtNombreMod->setText(nombre);
    ui->txtCantidadMod->setText(cantidad);
    ui->txtPrecioMod->setText(precio);
}

QString DialogModificar::getNombre() const {
    return ui->txtNombreMod->text();
}

QString DialogModificar::getCantidad() const {
    return ui->txtCantidadMod->text();
}

QString DialogModificar::getPrecio() const {
    return ui->txtPrecioMod->text();
}

void DialogModificar::on_btnCancelar_clicked() {
    reject(); // Cierra la ventana sin guardar
}

void DialogModificar::on_btnGuardar_clicked() {
    QString nombre = ui->txtNombreMod->text();
    QString cantStr = ui->txtCantidadMod->text();
    QString precStr = ui->txtPrecioMod->text();

    // 1. Validación de campos vacíos
    if(nombre.isEmpty() || cantStr.isEmpty() || precStr.isEmpty()) {
        QMessageBox::warning(this, "Error de Entrada", "Todos los campos son obligatorios.");
        return;
    }

    // 2. Validación de tipos de datos (Números)
    bool okCant, okPrec;
    int cantidad = cantStr.toInt(&okCant);
    double precio = precStr.toDouble(&okPrec);

    if(!okCant) {
        QMessageBox::warning(this, "Error de Entrada", "La cantidad debe ser un número entero válido.");
        ui->txtCantidadMod->setFocus();
        return;
    }

    if(!okPrec) {
        QMessageBox::warning(this, "Error de Entrada", "El precio debe ser un número decimal válido.");
        ui->txtPrecioMod->setFocus();
        return;
    }

    // 3. Validación de valores lógicos
    if(cantidad < 0) {
        QMessageBox::warning(this, "Error de Entrada", "La cantidad no puede ser negativa.");
        return;
    }
    if(precio <= 0) {
        QMessageBox::warning(this, "Error de Entrada", "El precio debe ser mayor a 0.");
        return;
    }

    // Si todo está bien, aceptamos el diálogo
    accept();
}
