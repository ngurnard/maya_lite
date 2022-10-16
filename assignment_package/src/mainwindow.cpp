#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus(); // ui->mygl points to the myl window of the ui (where the geom is shown)

    // Connect the obj button to import a .obj file
    // (sender, signal, receiver, slot)
    connect(ui->loadObjButton, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_loadObj()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}
