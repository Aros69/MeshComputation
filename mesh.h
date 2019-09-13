#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <QMap>
#include <iostream>
#include <fstream>
#include <sstream>

// TO MODIFY
// Advice => You should create a new file for each module but not necessarily for each class
class Point
{
    float _x;
    float _y;
    float _z;

public:
    Point() : _x(), _y(), _z() {}
    Point(float x_, float y_, float z_) : _x(x_), _y(y_), _z(z_) {}
    // get
    float x() const { return _x; }
    float y() const { return _y; }
    float z() const { return _z; }
};

class Vertex
{
    Point point;
    int faceIndex;

public:
    Vertex() : point() {}
    Vertex(float x_, float y_, float z_) : point(x_, y_, z_) {}
    float x() const { return point.x(); }
    float y() const { return point.y(); }
    float z() const { return point.z(); }
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
        return -1;
    }
    int getVertex(int index)
    {
        return verticesIndex[index];
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

//** TO MODIFY
class Mesh
{
private:
    QVector<Vertex> vertexTab;
    QVector<Face> faceTab;
    QVector<Point> Laplacien;

public:
    Mesh();
    void setMesh(QVector<Vertex>, QVector<Face>);
    void drawMesh();
    void drawMeshWireFrame();

    void meshWithFile(std::string filePath);
    void defineNeighbourFaces();

    Vertex *getVertex(int index) { return &vertexTab[index]; }
    Face *getFace(int index) { return &faceTab[index]; }

    int getVertexID(const Vertex &m)
    {
        for (int i = 0; i < vertexTab.size(); i++)
        {

            if (m.equals(vertexTab[i]))
            {
                return i;
            }
        }
        std::cout << "Invalid ID";
        return -1;
    }

    Iterator_on_faces f_begin();
    Iterator_on_faces f_pend();
    Iterator_on_vertices v_begin();
    Iterator_on_vertices v_pend();
    //Todo
    Circulator_on_faces incident_f(Vertex &v);
    Circulator_on_vertices adjacent_v(Vertex &v);


};

#include "iterators.h"
#include "segmentMemory.h"
#endif // MESH_H