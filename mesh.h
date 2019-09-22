#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <QMap>
#include <QtAlgorithms>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

// TO MODIFY
// Advice => You should create a new file for each module but not necessarily for each class
class Point
{
    double _x;
    double _y;
    double _z;

public:
    Point() : _x(), _y(), _z() {}
    Point(double x_, double y_, double z_) : _x(x_), _y(y_), _z(z_) {}
    // get
    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }
};

class Vertex
{
    Point point;
    int faceIndex;

public:
    Vertex() : point() {}
    Vertex(double x_, double y_, double z_) : point(x_, y_, z_) {}
    double x() const { return point.x(); }
    double y() const { return point.y(); }
    double z() const { return point.z(); }
    void setFaceIndex(int i) { faceIndex = i; }
    int getFaceIndex() { return faceIndex; }
    bool equals(const Vertex& m) const
    {
        if (m.x() == this->x() && m.y() == this->y() && m.z() == this->z())
        {
            return true;
        }
        return false;
    }
};

struct Vector
{
    double x;
    double y;
    double z;
    Vector(): x(0), y(0), z(0) {}
    Vector(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

    //Init a vector going FROM p1 TO p2
    Vector(const Point& p1,const Point& p2) : x(p2.x() - p1.x()), y(p2.y() - p1.y()), z(p2.z() - p1.z()){}
    //Init a vector going FROM v1 TO v2
    Vector(const Vertex& v1,const Vertex& v2) : x(v2.x() - v1.x()), y(v2.y() - v1.y()), z(v2.z() - v1.z()){}

};

double dot(const Vector& v1,const Vector& v2);
Vector cross(const Vector& v1,const Vector& v2);
double norm(const Vector& v);
Vector normalize(const Vector& v);
double getCos(const Vector& v1,const Vector& v2);
double getSin(const Vector& v1,const Vector& v2);

class Face
{

    int verticesIndex[3];
    int neibFace[3];

public:
    Face() : Face(0, 0, 0) {}
    Face(int a, int b, int c)
    {
        verticesIndex[0] = a;
        verticesIndex[1] = b;
        verticesIndex[2] = c;
        neibFace[0] = -1;
        neibFace[1] = -1;
        neibFace[2] = -1;
    }

    void setNeibFace(int neib1, int neib2, int neib3)
    {
        neibFace[0] = neib1;
        neibFace[1] = neib2;
        neibFace[2] = neib3;
    }

    void setNeibFace(int neib, int index)
    {
        neibFace[index] = neib;
    }

    int getNeibFace(int index) const { return neibFace[index]; }

    int global2localIndex(int globalIndex)
    {
        for (int i = 0; i < 3; i++)
        {
            if (verticesIndex[i] == globalIndex)
            {
                return i;
            }
        }
        std::cout << "vertex not part of this triangle\n";
        std::cout << "indexes are ["<< verticesIndex [0] <<"] ["<< verticesIndex[1] <<"] and ["<< verticesIndex[2]<<"\n";
        std::cout << " Provided was " << globalIndex << "\n";
        return -1;
    }

    int getVertex(int index) const
    {
        return verticesIndex[index];
    }

    void print(int ID){
        std::cout << "Face ["<< ID <<"] : \n";
        std::cout << "Vertices Index :\n 1 :\t ["<< verticesIndex [0] <<"]\t 2 : ["<< verticesIndex[1]
                  <<"]\t 3 :["<< verticesIndex[2]<<"]\n";
        std::cout << "Near Faces Index :\n 1 :\t ["<< neibFace[0] <<"]\t 2 : ["<< neibFace[1]
                  <<"]\t 3 :["<< neibFace[2]<<"]\n";
    }

    int &operator[](int x)
    {
        return verticesIndex[x];
    }
};
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
    Vector colorA = Vector(0.2,0.25,0.0);
    Vector colorB = Vector(0.0,1.0,0.0);
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

    //Laplacian Functions
    void computeLaplacian();
    void threadedLaplacian();
    void minMaxLaplacian();
    void clampLamplacian(int clamp);


    double getFaceArea(int index);
    double getFaceArea(Vertex& v1,Vertex& v2, Vertex& v3);

    //Get Cot of the angle (v2 v1 v3)
    double getCot(Vertex& v1,Vertex& v2, Vertex& v3);
    void printFaces();

};

#include "iterators.h"
#include "segmentMemory.h"
#endif // MESH_H
