#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <iostream>
// TO MODIFY
// Advice => You should create a new file for each module but not necessarily for each class
class Point
{
    double _x;
    double _y;
    double _z;

public:
    Point():_x(),_y(),_z() {}
    Point(float x_, float y_, float z_):_x(x_),_y(y_),_z(z_) {}
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
        Vertex():point(){}
        Vertex(float x_, float y_, float z_):point(x_,y_,z_){}
        double x() const { return point.x(); }
        double y() const { return point.y(); }
        double z() const { return point.z(); }
        void setFaceIndex(int i){
            faceIndex = i;
        }
};

class Face{

    int verticesIndex[3];
    int neibFace[3];
    public:
        Face(){}
        Face(int a, int b, int c){
            verticesIndex[0] = a;
            verticesIndex[1] = b;
            verticesIndex[2] = c;
        }

        void setNeibFace(int neib1, int neib2, int neib3){
            neibFace = {neib1,neib2,neib3};
        }
        int& operator[] (int x) {
          return verticesIndex[x];
        }
 
};

//** TO MODIFY
class Mesh
{
    QVector<Vertex> vertexTab;
    QVector<Face> faceTab;
    
public:
    Mesh();
    void setMesh(QVector<Vertex>,QVector<Face>);
    void drawMesh();
    void drawMeshWireFrame();
};

#endif // MESH_H
