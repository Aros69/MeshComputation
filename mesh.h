#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <QMap>
#include <QtAlgorithms>
#include <fstream>
#include <sstream>

#include "utils.h"

class Iterator_on_vertices;
class Circulator_on_vertices;
class Iterator_on_faces;
class Circulator_on_faces;

class Mesh
{
private:
    QVector<Vertex> vertexTab;
    QVector<Face> faceTab;
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

    // TrinagleSplit
    void triangleSplit(int faceIndex, Point newV);

    double getFaceArea(int index);
    double getFaceArea(Vertex& v1,Vertex& v2, Vertex& v3);
    //Get Cot of the angle (v2 v1 v3)
    double getCot(Vertex& v1,Vertex& v2, Vertex& v3);
    //InFace test
    bool isInFace(int index,const Vertex& v);
    void printFaces();

    //Mesh Modification methods
    void flip(int index1, int index2){
      Face f1 = getFace(index1);
      Face f2 = getFace(index2);
      //Get all the data that needs to persist
    }

};

#include "iterators.h"
#include "segmentMemory.h"
#endif // MESH_H
