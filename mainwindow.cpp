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
    connect(ui->randFlip,SIGNAL(released()),this, SLOT(randomFlip()));
    connect(ui->randomFH,SIGNAL(released()),this, SLOT(randomFHighlight()));
    connect(ui->unmark,SIGNAL(released()),this, SLOT(unmarkAll()));
}
void MainWindow::onWireframe(){
    //std::cout <<"WireFrame Display Mode\n";
    ui->widget->setDisplayWireframe();
}
void MainWindow::onPlain(){
    //std::cout <<"Plain Display Mode" << std::endl;
    ui->widget->setDisplayPlain();
}
void MainWindow::randomFlip(){
    //std::cout << "Random Flip" << std::endl;
    ui->widget->randomFlip();
}
void MainWindow::randomFHighlight(){
    // std::cout << "Random Highlight" << std::endl;
    ui->widget->randomFHighlight();
}
void MainWindow::unmarkAll(){
    // std::cout << "Remove Highlights" << std::endl;
    ui->widget->unmarkAll();
}
MainWindow::~MainWindow()
{
    delete ui;
}

