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

class Iterator_on_vertices : public std::iterator<std::input_iterator_tag, Face>
{
    int index;
    Mesh *m;

public:
    Iterator_on_vertices(int x, Mesh *mesh) : index(x), m(mesh) {}
    Iterator_on_vertices &operator++()
    {
        index++;
        return *this;
    }
    Vertex *operator*() { return m->getVertex(index); }
};

class Circulator_on_vertices : public std::iterator<std::input_iterator_tag, Face>
{
    int currentFaceIndex;
    int axisVertexIndex;
    int currentAdjacentVertex;
    Mesh *m;

public:
    Circulator_on_vertices(int faceIndex, int axisIndex, Mesh *mesh) : currentFaceIndex(faceIndex),
                                                                       axisVertexIndex(axisIndex), m(mesh)
    {
        int axisLocalIndex = m->getFace(currentFaceIndex)->global2localIndex(axisVertexIndex);
        currentAdjacentVertex = m->getFace(currentFaceIndex)->getVertex((axisLocalIndex + 1) % 3);
    }
    Circulator_on_vertices(int axisIndex, Mesh *mesh) : currentFaceIndex(mesh->getVertex(axisIndex)->getFaceIndex()),
                                                                       axisVertexIndex(axisIndex), m(mesh)
    {
        int axisLocalIndex = m->getFace(currentFaceIndex)->global2localIndex(axisVertexIndex);
        currentAdjacentVertex = m->getFace(currentFaceIndex)->getVertex((axisLocalIndex + 1) % 3);
    }

    Circulator_on_vertices &operator++()
    {
        int axisLocalIndex = m->getFace(currentFaceIndex)->global2localIndex(axisVertexIndex);
        currentFaceIndex = m->getFace(currentFaceIndex)->getNeibFace((axisLocalIndex + 1) % 3);
        return *this;
    }

    Vertex *operator*() { return m->getVertex(currentFaceIndex); }
};

class Iterator_on_faces : public std::iterator<std::input_iterator_tag, Face>
{
    int index;
    Mesh *m;

public:
    Iterator_on_faces(int x, Mesh *mesh) : index(x), m(mesh) {}
    //Todo Make the face iteration trigonometric
    Iterator_on_faces &operator++()
    {
        index++;
        return *this;
    }

    Face *operator*() { return m->getFace(index); }
};

class Circulator_on_faces : public std::iterator<std::input_iterator_tag, Face>
{
    int currentFaceIndex;
    int axisVertexIndex;
    Mesh *m;

public:
    Circulator_on_faces(int faceIndex, int axisIndex, Mesh *mesh) : currentFaceIndex(faceIndex),
                                                                    axisVertexIndex(axisIndex), m(mesh) {}

    Circulator_on_faces(int axisIndex, Mesh *mesh) : axisVertexIndex(axisIndex), m(mesh)
    {
        currentFaceIndex = m->getVertex(axisIndex)->getFaceIndex();
    }

    Circulator_on_faces &operator++()
    {
        int axisLocalIndex = m->getFace(currentFaceIndex)->global2localIndex(axisVertexIndex);
        currentFaceIndex = m->getFace(currentFaceIndex)->getNeibFace((axisLocalIndex + 1) % 3);
        return *this;
    }
    Face *operator*() { return m->getFace(currentFaceIndex); }
};

class SegmentMemory{
    class SegmentMemoryKey{
    public:
        int vertexIndex1;
        int vertexIndex2;
        SegmentMemoryKey(int index1, int index2)
            : vertexIndex1(index1), vertexIndex2(index2){}

        bool operator==(const SegmentMemoryKey& cmp) const{
            return ((this->vertexIndex1==cmp.vertexIndex2&&this->vertexIndex2==cmp.vertexIndex1)
                    ||(this->vertexIndex1==cmp.vertexIndex2&&this->vertexIndex2==cmp.vertexIndex2));
        }

        bool operator<(const SegmentMemoryKey& cmp) const{
            return ((this->vertexIndex1<cmp.vertexIndex2&&this->vertexIndex2<cmp.vertexIndex1)
                    ||(this->vertexIndex1<cmp.vertexIndex2&&this->vertexIndex2<cmp.vertexIndex2));
        }

    };

    class SegmentMemoryData{
    public:
        int faceIndex;
        int vertexInFaceIndex;
        SegmentMemoryData(int fIndex, int vertexIndex)
            : faceIndex(fIndex), vertexInFaceIndex(vertexIndex){}
    };
public:
    QMap<SegmentMemoryKey, SegmentMemoryData> hashMap;

    void addSegment(int vI1, int vI2, int fI, int vIFI){
        hashMap.insert(SegmentMemoryKey(vI1, vI2), SegmentMemoryData(fI, vIFI));
    }

    void deleteSegment(const int cmp[2]){
        SegmentMemoryKey toDelete(cmp[0], cmp[1]);
        QMap<SegmentMemoryKey, SegmentMemoryData>::iterator it = hashMap.find(toDelete);
        hashMap.erase(it);
    }

    bool contain(const int cmp[2]){
        return hashMap.contains(SegmentMemoryKey(cmp[0], cmp[1]));
    }

    int faceIndex(const int cmp[2]){
        return (hashMap.find(SegmentMemoryKey(cmp[0], cmp[1]))).value().faceIndex;
    }

    int vertexInFaceIndex(const int cmp[2]){
        return (hashMap.find(SegmentMemoryKey(cmp[0], cmp[1]))).value().vertexInFaceIndex;
    }
};
#endif // MESH_H
