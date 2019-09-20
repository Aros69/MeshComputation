#include<mesh.h>

// VERTEX ITERATOR ==========================================================================================
class Iterator_on_vertices : public std::iterator<std::input_iterator_tag, Face>
{
    int index;
    Mesh *m;

public:
    Iterator_on_vertices():index(-1),m(nullptr){}
    Iterator_on_vertices(int x, Mesh *mesh) : index(x), m(mesh){}
    Iterator_on_vertices &operator++()
    {
        index++;
        return *this;
    }

    friend bool operator!=(Iterator_on_vertices v1,Iterator_on_vertices v2 );

    Vertex& operator*() { return m->getVertex(index); }
};

inline bool operator!=(Iterator_on_vertices v1,Iterator_on_vertices v2 )
{
    return (v1.index != v2.index);
}

// VERTEX CIRCULATOR ==========================================================================================


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
        std::cout <<"Circulator Init1\n";
        int axisLocalIndex = m->getFace(currentFaceIndex).global2localIndex(axisVertexIndex);
        currentAdjacentVertex = m->getFace(currentFaceIndex).getVertex((axisLocalIndex + 1) % 3);
    }
    Circulator_on_vertices(int axisIndex, Mesh *mesh) : currentFaceIndex(mesh->getVertex(axisIndex).getFaceIndex()),
                                                                       axisVertexIndex(axisIndex), m(mesh)
    {
        std::cout <<"Circulator Init2\n";
        std::cout <<"Axis Index is ["<< axisIndex<<"]\nCurrentFace Index is ["<<currentFaceIndex<<"]\n";
        int axisLocalIndex = m->getFace(currentFaceIndex).global2localIndex(axisVertexIndex);
        currentAdjacentVertex = m->getFace(currentFaceIndex).getVertex((axisLocalIndex + 1) % 3);
    }

    Circulator_on_vertices &operator++()
    {
        int axisLocalIndex = m->getFace(currentFaceIndex).global2localIndex(axisVertexIndex);
        currentFaceIndex = m->getFace(currentFaceIndex).getNeibFace((axisLocalIndex + 1) % 3);
        return *this;

    }

    Vertex& operator*() { return m->getVertex(currentFaceIndex); }
};


// FACE ITERATOR ==========================================================================================
class Iterator_on_faces : public std::iterator<std::input_iterator_tag, Face>
{
    int index;
    Mesh *m;

public:
    Iterator_on_faces():index(-1),m(nullptr){}
    Iterator_on_faces(int x, Mesh *mesh) : index(x), m(mesh) {}
    //Todo Make the face iteration trigonometric
    Iterator_on_faces &operator++()
    {
        index++;
        return *this;
    }
    Iterator_on_faces &operator++(int)
    {
        index++;
        return *this;
    }
    friend bool operator!=(Iterator_on_faces f1,Iterator_on_faces f2);

    Face& operator*() { return m->getFace(index); }
};
inline bool operator!=(Iterator_on_faces f1,Iterator_on_faces f2){
    return f1.index != f2.index;
}

// FACE CIRCULATOR ==========================================================================================

class Circulator_on_faces : public std::iterator<std::input_iterator_tag, Face>
{
    int currentFaceIndex;
    int axisVertexIndex;
    Mesh *m;

public:
    Circulator_on_faces():currentFaceIndex(-1),axisVertexIndex(-1){}
    Circulator_on_faces(int faceIndex, int axisIndex, Mesh *mesh) : currentFaceIndex(faceIndex),
                                                                    axisVertexIndex(axisIndex), m(mesh) {}

    Circulator_on_faces(int axisIndex, Mesh *mesh) : axisVertexIndex(axisIndex), m(mesh)
    {
        currentFaceIndex = m->getVertex(axisIndex).getFaceIndex();
        std::cout <<"Circulator Init2\n";
        std::cout <<"Axis Index is ["<< axisIndex<<"]\nCurrentFace Index is ["<<currentFaceIndex<<"]\n";
    }

    Circulator_on_faces &operator++()
    {
        int axisLocalIndex = m->getFace(currentFaceIndex).global2localIndex(axisVertexIndex);
        currentFaceIndex = m->getFace(currentFaceIndex).getNeibFace((axisLocalIndex + 1) % 3);
        return *this;
    }

    Circulator_on_faces &operator++(int)
    {
        int axisLocalIndex = m->getFace(currentFaceIndex).global2localIndex(axisVertexIndex);
        currentFaceIndex = m->getFace(currentFaceIndex).getNeibFace((axisLocalIndex + 1) % 3);
        return *this;
    }

    friend bool operator!=(Circulator_on_faces v1,Circulator_on_faces v2);
    Face& operator*() { return m->getFace(currentFaceIndex); }
};

inline bool operator!=(Circulator_on_faces v1,Circulator_on_faces v2 )
{
    return (v1.currentFaceIndex != v2.currentFaceIndex)
            | (v1.axisVertexIndex != v2.axisVertexIndex);
}

