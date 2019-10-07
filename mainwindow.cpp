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
    connect(ui->nextFace,SIGNAL(released()),this,SLOT(circulate()));
    connect(ui->naiveInsert,SIGNAL(released()),this,SLOT(naiveInsert()));
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
void MainWindow::circulate(){
    int axis = std::stoi(ui->axisIndex->text().toLocal8Bit().constData());
    std::cout << "Circulating around : "<< axis << std::endl;
    ui->widget->circulate(axis);
}
void MainWindow::naiveInsert(){
    float x = std::stof(ui->xInsert->text().toLocal8Bit().constData());
    float y = std::stof(ui->yInsert->text().toLocal8Bit().constData());
    float z = std::stof(ui->zInsert->text().toLocal8Bit().constData());
    std::cout << "Inserting vertex V("<< x <<","<< y <<","<< z <<") "<< std::endl;
    ui->widget->naiveInsert(x,y,z);
}

MainWindow::~MainWindow()
{
    delete ui;
}

