#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include <QFileDialog> // for the file selection button
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus(); // ui->mygl points to the myl window of the ui (where the geom is shown)

    // Connect the obj button to import a .obj file
//    connect(ui->pushButton, SIGNAL(clicked()),
//            ui->mygl->m_mesh, SLOT());


//    connect(ui->mygl,
//            // Signal name
//            SIGNAL(on_pushButton_clicked()),
//            // Widget with the slot that receives the signal
//            this,
//            // Slot name
//            SLOT(slot_addRootToTreeWidget(QTreeWidgetItem*)));
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

void MainWindow::on_pushButton_clicked()
{
    // https://www.youtube.com/watch?v=tKdfpA74HYY&ab_channel=ProgrammingKnowledge
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open OBJ File"), // title of the file dialog (basically the window that pops up)
                "/home/nicholas/Documents/upenn/CIS560/homework", // Default file to open
                "OBJ Files (*.obj)" // filter for obj files
                );
    QMessageBox::information(this, tr("Selected File Name"), filename);
}

