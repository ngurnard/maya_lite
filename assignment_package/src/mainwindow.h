#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTreeWidgetItem>
#include "mesh.h"


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();

    void on_actionCamera_Controls_triggered();

    void slot_addToListWidget(Mesh*);
    void slot_addRoottoTreeWidget(QTreeWidgetItem*);

    void slot_setFocus();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
