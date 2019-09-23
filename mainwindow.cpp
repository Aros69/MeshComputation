#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_2,SIGNAL(released()),this, SLOT(onWireframe()));
    connect(ui->pushButton_3,SIGNAL(released()),this, SLOT(onPlain()));
}
void MainWindow::onWireframe(){
    //std::cout <<"WireFrame Display Mode\n";
    ui->widget->setDisplayWireframe();
}
void MainWindow::onPlain(){
    //std::cout <<"Plain Display Mode\n";
    ui->widget->setDisplayPlain();
}
MainWindow::~MainWindow()
{
    delete ui;
}
