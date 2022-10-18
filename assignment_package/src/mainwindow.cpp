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

    // Connect to the gui the functiuons for selecing faces, verts, and halfEdges
    connect(ui->vertsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), ui->mygl, SLOT(slot_selectVertex(QListWidgetItem*)));
    connect(ui->facesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), ui->mygl, SLOT(slot_selectFace(QListWidgetItem*)));
    connect(ui->halfEdgesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), ui->mygl, SLOT(slot_selectHE(QListWidgetItem*)));

    // Connect the selected faces, verts, and halfEdges to the gui
    connect(ui->mygl, SIGNAL(sig_sendMesh(Mesh*)), this, SLOT(slot_addToListWidget(Mesh*)));
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

void MainWindow::slot_addToListWidget(Mesh *m) {

    ui->facesListWidget->clear();
    ui->vertsListWidget->clear();
    ui->halfEdgesListWidget->clear();

    for (auto &f : m->faces)
    {
        ui->facesListWidget->addItem(f.get());
    }
    for (auto &v : m->vertices)
    {
        ui->vertsListWidget->addItem(v.get());
    }
    for (auto &he : m->halfEdges)
    {
        ui->halfEdgesListWidget->addItem(he.get());
    }

}
