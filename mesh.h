#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <QHash>
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
    Point():_x(),_y(),_z() {}
    Point(float x_, float y_, float z_):_x(x_),_y(y_),_z(z_) {}
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
        Vertex():point(){}
        Vertex(float x_, float y_, float z_):point(x_,y_,z_){}
        float x() const { return point.x(); }
        float y() const { return point.y(); }
        float z() const { return point.z(); }
        void setFaceIndex(int i){faceIndex = i;}
        int getFaceIndex(){return faceIndex;}
};

class Face{

    int verticesIndex[3];
    int neibFace[3];
    public:
        Face() : Face(0,0,0){}
        Face(int a, int b, int c){
            verticesIndex[0] = a;
            verticesIndex[1] = b;
            verticesIndex[2] = c;
            neibFace[0] = -1;
            neibFace[1] = -1;
            neibFace[2] = -1;
        }

        void setNeibFace(int neib1, int neib2, int neib3){
            neibFace[0] = neib1;
            neibFace[1] = neib2;
            neibFace[2] = neib3;
        }

        void setNeibFace(int neib, int index){
            neibFace[index] = neib;
        }

        int getNeibFace(int index) const {return neibFace[index];}

        int& operator[] (int x) {
          return verticesIndex[x];
        }
 
};

//** TO MODIFY
class Mesh
{
private:
    QVector<Vertex> vertexTab;
    QVector<Face> faceTab;
    
public:
    Mesh();
    void setMesh(QVector<Vertex>,QVector<Face>);
    void drawMesh();
    void drawMeshWireFrame();

    void meshWithFile(std::string filePath);
    void defineNeighbourFaces();

    class SegmentMemory{
        /*class SegmentMemoryData{
        public:

        };*/
    public:
        //QHash<std::pair<int, int>, SegmentMemoryData> hashMap;
        int vertexIndex1;
        int vertexIndex2;
        int faceIndex;
        int vertexInFaceIndex;

        SegmentMemory(int vI1, int vI2, int fI, int vIFI) : vertexIndex1(vI1),
            vertexIndex2(vI2), faceIndex(fI), vertexInFaceIndex(vIFI){}
    };
};

#endif // MESH_H
