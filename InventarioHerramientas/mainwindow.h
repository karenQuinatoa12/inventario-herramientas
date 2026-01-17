#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

using namespace std;

struct Herramientas{
    int id;
    string nombre;
    int cantidad;
    string estado;

};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void on_btnAgregar_clicked();
    void on_btnModificar_clicked();
    void on_btnEliminar_clicked();
    void on_btnLimpiar_clicked();
    void on_btnRegistrar_clicked();
    void on_btnSalir_clicked();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
