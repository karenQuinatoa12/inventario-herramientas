#ifndef DIALOGMODIFICAR_H
#define DIALOGMODIFICAR_H

#include <QDialog>

namespace Ui {
class DialogModificar;
}

class DialogModificar : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModificar(QWidget *parent = nullptr);
    ~DialogModificar();

    // Métodos para pasar datos entre la ventana principal y esta
    void setDatos(QString nombre, QString cantidad, QString precio);
    QString getNombre() const;
    QString getCantidad() const;
    QString getPrecio() const;

private slots:
    void on_btnGuardar_clicked(); // Botón para confirmar cambios
    void on_btnCancelar_clicked(); // Botón para salir

private:
    Ui::DialogModificar *ui;
};

#endif // DIALOGMODIFICAR_H
