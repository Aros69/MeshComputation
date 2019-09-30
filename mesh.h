#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <QMap>
#include <QtAlgorithms>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "utils.h"

class Iterator_on_vertices;
class Circulator_on_vertices;
class Iterator_on_faces;
class Circulator_on_faces;

class Mesh
{
private:
    QVector<Vertex> vertexTab;
    QVector<DebugObj> vertexDebugTab;
    QVector<Face> faceTab;
    QVector<DebugObj> faceDebugTab;
    QVector<Vector> Laplacien;
    bool laplacianDone = false;
    Vector colorA = Vector(0.75,0.75,0.75);
    Vector colorB = Vector(1,0.85,0);
    Vector maxValueLaplacien;
    Vector minValueLaplacien;
    double maxNormLaplacian;
    double minNormLaplacian;

public:
    Mesh();
    ~Mesh();
    void setMesh(QVector<Vertex>, QVector<Face>);
    void drawMesh();
    void drawMeshWireFrame();
    void meshWithFile(std::string filePath);
    void defineNeighbourFaces();

    //Getters
    Vertex& getVertex(int index) { return vertexTab[index]; }
    Face& getFace(int index) { return faceTab[index]; }
    int getVertexID(const Vertex &m);

    Iterator_on_faces f_begin();
    Iterator_on_faces f_pend();
    Iterator_on_vertices v_begin();
    Iterator_on_vertices v_pend();

    Circulator_on_faces incident_f(Vertex &v);
    Circulator_on_vertices adjacent_v(Vertex &v);

    // Laplacian Functions
    //Mesh statistics methods
    void computeLaplacian();
    void minMaxLaplacian();
    void clampLamplacian(int clamp);

    double getFaceArea(int index);
    double getFaceArea(Vertex& v1,Vertex& v2, Vertex& v3);
    //Get Cot of the angle (v2 v1 v3)
    double getCot(Vertex& v1,Vertex& v2, Vertex& v3);
    //InFace test
    bool isInFace(int index,const Vertex& v);

    //Mesh Modification methods
    void flip(int index1, int index2);
    // TriangleSplit
    void triangleSplit(int faceIndex, Point newV);

    // Naive Insertion

    //Debugging Methods ===================================================================
    void updateDebugObj();
    void printFaces();
    void markFace(int index);
    void markVertex(int index);
    void unMarkAll();
    void randomFlip(){
      int index1 = rand()%faceTab.size();
      int index2 = getFace(index1).getNeibFace(rand()%3);
      std::cout<<"size : "<<faceTab.size()<<" Index 1 : " <<index1<<" Index 2 : "<<index2<<std::endl;
      flip(index1,index2);
    }
    void randomFHighlight(){
      markFace(rand()%faceTab.size());
    }
};

#include "iterators.h"
#include "segmentMemory.h"
#endif // MESH_H
