#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus(); // ui->mygl points to the myl window of the ui (where the geom is shown) 

    // Connect the gui to update the focus such that you dont have to click mygl in order to see updates
    connect(ui->mygl, SIGNAL(sig_setFocus()), this, SLOT(slot_setFocus()));

    // Connect MyGL's signal that contains the root of the skeleton that adds the root node to the tree widget
    connect(ui->mygl, SIGNAL(sig_sendSkeletonRoot(QTreeWidgetItem*)), this, SLOT(slot_addRoottoTreeWidget(QTreeWidgetItem*)));

    // Connect the obj button to import a .obj file
    // (sender, signal, receiver, slot)
    connect(ui->loadObjButton, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_loadObj()));

    // Connect the json button to import a .json file
    connect(ui->loadJSONButton, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_loadJson()));

    // Connect to the gui the functiuons for selecing faces, verts, and halfEdges, and joints
    connect(ui->vertsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), ui->mygl, SLOT(slot_selectVertex(QListWidgetItem*)));
    connect(ui->facesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), ui->mygl, SLOT(slot_selectFace(QListWidgetItem*)));
    connect(ui->halfEdgesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), ui->mygl, SLOT(slot_selectHE(QListWidgetItem*)));
    connect(ui->jointTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), ui->mygl, SLOT(slot_selectJoint(QTreeWidgetItem*)));

    // Connect the selected faces, verts, and halfEdges to the gui list widget
    connect(ui->mygl, SIGNAL(sig_sendMesh(Mesh*)), this, SLOT(slot_addToListWidget(Mesh*)));

    // Connect the gui split edge button to the split edge function (slot)
    connect(ui->splitEdge, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_splitHE()));

    // Connect the gui traingulate button to the triangulate slot
    connect(ui->triangulate, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_triangulate()));

    // Connect the gui spin boxes to the slot_modAttributes
    connect(ui->vertPosXSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_modVertPosX(double)));
    connect(ui->vertPosYSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_modVertPosY(double)));
    connect(ui->vertPosZSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_modVertPosZ(double)));
    connect(ui->faceRedSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_modFaceRed(double)));
    connect(ui->faceGreenSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_modFaceGreen(double)));
    connect(ui->faceBlueSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_modFaceBlue(double)));
    connect(ui->jointPosXSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_modJointPosX(double)));
    connect(ui->jointPosYSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_modJointPosY(double)));
    connect(ui->jointPosZSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_modJointPosZ(double)));
    connect(ui->jointRotXpos, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_modJointRotXPos()));
    connect(ui->jointRotYpos, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_modJointRotYPos()));
    connect(ui->jointRotZpos, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_modJointRotZPos()));
    connect(ui->jointRotXneg, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_modJointRotXNeg()));
    connect(ui->jointRotYneg, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_modJointRotYNeg()));
    connect(ui->jointRotZneg, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_modJointRotZNeg()));

    // Connect the gui subdivide button to the subdivide slot
    connect(ui->subdivide, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_subdivide()));

    // Connect the gui skinning button the the skinning slot
    connect(ui->skinningButton, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_skinning()));
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

    // Ask why this breaks the code
    // ui->facesListWidget->clear();
    // ui->vertsListWidget->clear();
    // ui->halfEdgesListWidget->clear();

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

void MainWindow::slot_addRoottoTreeWidget(QTreeWidgetItem* i)
{
    ui->jointTree->addTopLevelItem(i);
}

void MainWindow::slot_setFocus()
{
    ui->mygl->setFocus();
}
