#include "mesh.h"

void Mesh::setMesh(QVector<Vertex> vertices, QVector<Face> faces)
{
    vertexTab = vertices;
    faceTab = faces;
    updateDebugObj();
}

void Mesh::meshWithFile(std::string filePath)
{
    //std::cout<<"Begin of creating a mesh with .off file\n";
    std::ifstream file(filePath.c_str());
    if (!file.is_open())
    {
        std::cout << "ERROR : Unable to open file : \"" << filePath << "\"\n";
    }
    else
    {
        int nbVertices, nbFaces, dump, a, b, c;
        double x, y, z;
        std::string temp;
        file >> nbVertices >> nbFaces >> dump;

        vertexTab.clear();
        for (int i = 0; i < nbVertices; ++i)
        {
            file >> x >> y >> z;
            vertexTab.push_back(Vertex(x, y, z));
        }
        faceTab.clear();
        for (int i = 0; i < nbFaces; ++i)
        {
            //TODO use the first element to check if we create triangle or another polygon
            file >> dump >> a >> b >> c;
            vertexTab[a].setFaceIndex(i);
            vertexTab[b].setFaceIndex(i);
            vertexTab[c].setFaceIndex(i);
            faceTab.push_back(Face(a, b, c));
        }
        defineNeighbourFaces();
        file.close();
    }
    //std::cout<<"End of creating a mesh with .off file\n";
    updateDebugObj();
}

void Mesh::defineNeighbourFaces()
{
    SegmentMemory memory;

    for (int i = 0; i < faceTab.size(); ++i)
    {
        int toTest[2]{faceTab[i][0], faceTab[i][1]};
        if (memory.contain(toTest))
        {
            faceTab[i].setNeibFace(memory.faceIndex(toTest), 2);
            faceTab[memory.faceIndex(toTest)]
                .setNeibFace(i, memory.vertexInFaceIndex(toTest));
            memory.deleteSegment(toTest);
        }
        else
        {
            memory.addSegment(faceTab[i][0], faceTab[i][1], i, 2);
        }
        int toTest2[2]{faceTab[i][1], faceTab[i][2]};
        if (memory.contain(toTest2))
        {
            faceTab[i].setNeibFace(memory.faceIndex(toTest2), 0);
            faceTab[memory.faceIndex(toTest2)]
                .setNeibFace(i, memory.vertexInFaceIndex(toTest2));
            memory.deleteSegment(toTest2);
        }
        else
        {
            memory.addSegment(faceTab[i][1], faceTab[i][2], i, 0);
        }
        int toTest3[2]{faceTab[i][0], faceTab[i][2]};
        if (memory.contain(toTest3))
        {
            faceTab[i].setNeibFace(memory.faceIndex(toTest3), 1);
            faceTab[memory.faceIndex(toTest3)]
                .setNeibFace(i, memory.vertexInFaceIndex(toTest3));
            memory.deleteSegment(toTest3);
        }
        else
        {
            memory.addSegment(faceTab[i][0], faceTab[i][2], i, 1);
        }
    }
    //printFacesNeib(faceTab);
    if (!memory.isEmpty())
    {
        // On crée le point infinis coordonées (0,0,INT_MIN)
        Vertex infiniteV(infiniteP);
        infiniteV.setFaceIndex(faceTab.size());
        vertexTab.push_back(infiniteV);
        // Pour tous les segments sans voisin on crée la face avec ce segment et le point infinis.
        for (auto segment : memory.hashMap)
        {
            SegmentMemory::SegmentMemoryKey tempKey = memory.hashMap.key(segment);
            SegmentMemory::SegmentMemoryData tempData = memory.hashMap.find(tempKey).value();
            if (orientation(vertexTab[faceTab[tempData.faceIndex][tempData.vertexInFaceIndex]],
                            vertexTab[tempKey.vertexIndex1], vertexTab[tempKey.vertexIndex2]) > 0)
            {
                faceTab.push_back(Face(vertexTab.size() - 1, tempKey.vertexIndex2, tempKey.vertexIndex1));
            }
            else
            {
                faceTab.push_back(Face(vertexTab.size() - 1, tempKey.vertexIndex1, tempKey.vertexIndex2));
            }
        }
        defineNeighbourFaces();
    }
    updateDebugObj();
    //memory.print();
}

void Mesh::cleanInfinitePoints()
{
    int i = 0;
    do
    {
        if (vertexTab[faceTab[i].getVertex(0)] == infiniteP || vertexTab[faceTab[i].getVertex(1)] == infiniteP || vertexTab[faceTab[i].getVertex(2)] == infiniteP)
        {
            faceTab.remove(i);
        }
        else
        {
            i++;
        }
    } while (i < faceTab.size());
    i = 0;
    do
    {
        if (vertexTab[i] == infiniteP)
        {
            vertexTab.remove(i);
        }
        i++;
    } while (i < vertexTab.size());
}