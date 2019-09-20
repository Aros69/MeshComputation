#include<mesh.h>

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
