#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_2,   SIGNAL(released()),     this, SLOT(onWireframe()));
    connect(ui->pushButton_3,   SIGNAL(released()),     this, SLOT(onPlain()));
    connect(ui->randFlip,       SIGNAL(released()),     this, SLOT(randomFlip()));
    connect(ui->flip,           SIGNAL(released()),     this, SLOT(flip()));
    connect(ui->highLightFace,  SIGNAL(released()),     this, SLOT(highLightFace()));
    connect(ui->randomFH,       SIGNAL(released()),     this, SLOT(randomFHighlight()));
    connect(ui->unmark,         SIGNAL(released()),     this, SLOT(unmarkAll()));
    connect(ui->nextFace,       SIGNAL(released()),     this, SLOT(circulate()));
    connect(ui->naiveInsert,    SIGNAL(released()),     this, SLOT(naiveInsert()));
    connect(ui->computeVoronoi, SIGNAL(released()),     this, SLOT(computeVoronoi()));
    connect(ui->showVoronoi,    SIGNAL(toggled(bool)),  this, SLOT(toggleVoronoi()));
    connect(ui->Delaunize,      SIGNAL(released()),     this, SLOT(delaunize()));
    connect(ui->delaunayInsert, SIGNAL(released()),     this, SLOT(delaunayInsert()));
    connect(ui->mergeVertices,  SIGNAL(released()),     this, SLOT(mergeVertices()));
    connect(ui->simplifyMesh,   SIGNAL(released()),     this, SLOT(simplify()));
}
void MainWindow::onWireframe(){
    //std::cout <<"WireFrame Display Mode\n";
    ui->widget->setDisplayWireframe();
}

void MainWindow::onPlain(){
    //std::cout <<"Plain Display Mode" << std::endl;
    ui->widget->setDisplayPlain();
}

void MainWindow::flip()
{
    if(ui->facetoFlip1->text() != 0 && ui->facetoFlip2->text() != 0)
    {
        int faceIndex1 = std::stoi(ui->facetoFlip1->text().toLocal8Bit().constData());
        int faceIndex2 = std::stoi(ui->facetoFlip2->text().toLocal8Bit().constData());
        std::cout << "flipping faces : "<< faceIndex1 << " and "<< faceIndex2 << std::endl;
        ui->widget->flip(faceIndex1,faceIndex2);
    }
}

void MainWindow::randomFlip(){
    //std::cout << "Random Flip" << std::endl;
    ui->widget->randomFlip();
}

void MainWindow::highLightFace()
{
    if(ui->faceHLID->text() != 0)
    {
        int faceIndex = std::stoi(ui->faceHLID->text().toLocal8Bit().constData());
        std::cout << "Highlighting face : "<< faceIndex<< std::endl;
        ui->widget->highlightFace(faceIndex);
    }
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
    if(ui->axisIndex->text() != 0)
    {
        int axis = std::stoi(ui->axisIndex->text().toLocal8Bit().constData());
        std::cout << "Circulating around : "<< axis << std::endl;
        ui->widget->circulate(axis);
    }
}

void MainWindow::naiveInsert(){
    if(ui->xInsert->text().size() != 0 || ui->yInsert->text().size() != 0 || ui->zInsert->text().size() != 0)
    {
        /*float x = std::stof(ui->xInsert->text().toLocal8Bit().constData());
        float y = std::stof(ui->yInsert->text().toLocal8Bit().constData());
        float z = std::stof(ui->zInsert->text().toLocal8Bit().constData());*/
        float x = ui->xInsert->text().toFloat();
        float y = ui->yInsert->text().toFloat();
        float z = ui->zInsert->text().toFloat();
        // TODO Changer la conversion pour vraiment obtenir des flottants
        std::cout << "Inserting vertex V("<< x <<","<< y <<","<< z <<") "<< std::endl;
        ui->widget->naiveInsert(x,y,z);
    }
}

void MainWindow::toggleVoronoi()
{
    //std::cout <<"Toggle voronoi display" << std::endl;
    ui->widget->toggleVoronoi();
}
void MainWindow::computeVoronoi()
{
    //std::cout <<"Computing Voronoi mesh..." << std::endl;
    ui->widget->computeVoronoi();
}

void MainWindow::delaunize()
{
    ui->widget->delaunize();
}

void MainWindow::delaunayInsert(){
    if(ui->xInsert->text().size() != 0 || ui->yInsert->text().size() != 0 || ui->zInsert->text().size() != 0)
    {
        /*float x = std::stof(ui->xInsert->text().toLocal8Bit().constData());
        float y = std::stof(ui->yInsert->text().toLocal8Bit().constData());
        float z = std::stof(ui->zInsert->text().toLocal8Bit().constData());*/
        float x = ui->xInsert->text().toFloat();
        float y = ui->yInsert->text().toFloat();
        float z = ui->zInsert->text().toFloat();
        // TODO Changer la conversion pour vraiment obtenir des flottants
        std::cout << "Inserting vertex V("<< x <<","<< y <<","<< z <<") "<< std::endl;
        ui->widget->delaunayInsert(x,y,z);
    }
}

void MainWindow::mergeVertices()
{
    if(ui->fusionFaceID->text() != 0 && ui->fusionVertexID->text() != 0)
    {
        float f = ui->fusionFaceID->text().toFloat();
        float v = ui->fusionVertexID->text().toFloat();
        ui->widget->merge(f, v);
    }
}

void MainWindow::splitVertex()
{
    if(ui->splitVertexID->text() != 0)
    {

    }
}

void MainWindow::simplify()
{
    ui->widget->simplify();
}

MainWindow::~MainWindow()
{
    delete ui;
}

