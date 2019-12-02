#include "mesh.h"

void Mesh::updateDebugObj()
{
    vertexDebugTab = QVector<DebugObj>(vertexTab.size());
    faceDebugTab = QVector<DebugObj>(faceTab.size());
}

void Mesh::markFace(int index)
{
    faceDebugTab[index].debug = true;
}


void Mesh::markVertex(int index)
{
    vertexDebugTab[index].debug = true;
}

void Mesh::unMarkAll()
{
    for (int i = 0; i < faceDebugTab.size(); i++)
    {
        faceDebugTab[i].debug = false;
    }
    for (int i = 0; i < vertexDebugTab.size(); i++)
    {
        vertexDebugTab[i].debug = false;
    }
}

void Mesh::printFaces()
{
    Iterator_on_faces itf;
    int i = 0;
    for (itf = f_begin(); itf != f_pend(); itf++)
    {
        (*itf).print(i);
        i++;
    }
}