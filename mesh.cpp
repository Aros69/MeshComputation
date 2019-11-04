#include "mesh.h"

Mesh::Mesh() { srand(time(NULL)); }

Mesh::~Mesh() {}
// The following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
// Draw a vertex
void glVertexDraw(const Vertex &p)
{
    glVertex3f(p.x(), p.y(), p.z());
}

void Mesh::drawMesh()
{
    int moduloI;
    double t1, t2, t3;
    for (int i = 0; i < faceTab.size(); i++)
    {
        /*if(!(vertexTab[faceTab[i][0]].z()==infiniteP.z()
             ||vertexTab[faceTab[i][1]].z()==infiniteP.z()
             ||vertexTab[faceTab[i][2]].z()==infiniteP.z())){*/
        if (faceDebugTab[i].debug)
        {
            glColor3d(faceDebugTab[i].debugColor.x, faceDebugTab[i].debugColor.y, faceDebugTab[i].debugColor.z);
            glBegin(GL_TRIANGLES);
            glVertexDraw(vertexTab[faceTab[i][0]]);
            glVertexDraw(vertexTab[faceTab[i][1]]);
            glVertexDraw(vertexTab[faceTab[i][2]]);
            glEnd();
        }
        else if (laplacianDone)
        {
            t1 = norm(Laplacien[faceTab[i][0]]) / maxNormLaplacian;
            t2 = norm(Laplacien[faceTab[i][1]]) / maxNormLaplacian;
            t3 = norm(Laplacien[faceTab[i][2]]) / maxNormLaplacian;

            glBegin(GL_TRIANGLES);
            glColor3d(t1 * colorA.x - (1 - t1) * colorB.x, t1 * colorA.y - (1 - t1) * colorB.y, t1 * colorA.z - (1 - t1) * colorB.z);
            glVertexDraw(vertexTab[faceTab[i][0]]);

            glColor3d(t2 * colorA.x - (1 - t2) * colorB.x, t2 * colorA.y - (1 - t2) * colorB.y, t2 * colorA.z - (1 - t2) * colorB.z);
            glVertexDraw(vertexTab[faceTab[i][1]]);

            glColor3d(t3 * colorA.x - (1 - t3) * colorB.x, t3 * colorA.y - (1 - t3) * colorB.y, t3 * colorA.z - (1 - t3) * colorB.z);
            glVertexDraw(vertexTab[faceTab[i][2]]);
            glEnd();
        }
        else
        {

            drawVoronoi ? glColor3d(0,0,0) : glColor3d(1, 1, 1);
            glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[faceTab[i][0]]);
            glVertexDraw(vertexTab[faceTab[i][1]]);
            glVertexDraw(vertexTab[faceTab[i][2]]);
            glEnd();

            if (drawVoronoi)
            {
                glColor3d(0.5,0.5,0.5);
            }
            else
            {
                moduloI = i % 4;
                if (moduloI == 0)
                    glColor3d(1, 0, 0);
                else if (moduloI == 1)
                    glColor3d(0, 1, 0);
                else if (moduloI == 2)
                    glColor3d(0, 0, 1);
                else
                    glColor3d(1, 1, 0);
            }

            glBegin(GL_TRIANGLES);
            glVertexDraw(vertexTab[faceTab[i][0]]);
            glVertexDraw(vertexTab[faceTab[i][1]]);
            glVertexDraw(vertexTab[faceTab[i][2]]);
            glEnd();
        }
        //}
    }
    if (drawVoronoi)
    {
        // std::cout << "\n\nVoronoi drawing\n\n" << std::endl;
        for (int i = 0; i < voronoiCells.size(); i++)
        {
            //Draw a cell
            // std::cout << "Voronoi Cell drawing" << std::endl;
            glColor3d(1, 1, 1);
            glBegin(GL_LINES);

            for (int j = 0; j < voronoiCells[i].size() - 1; j++)
            {
                // printf("Drawing voronoi vertice [%f][%f][%f]\n",voronoiCells[i][j].x(),voronoiCells[i][j].y(),voronoiCells[i][j].z());
                glVertexDraw(voronoiCells[i][j] + Vector(0, 0, 0.1));
                glVertexDraw(voronoiCells[i][j + 1] + Vector(0, 0, 0.1));
            }
            glEnd();
        }
    }
}

void Mesh::drawMeshWireFrame()
{
    for (int i = 0; i < faceTab.size(); i++)
    {
        //if (!(vertexTab[faceTab[i][0]].z() == infiniteP.z() || vertexTab[faceTab[i][1]].z() == infiniteP.z() || vertexTab[faceTab[i][2]].z() == infiniteP.z()))
        //{
            glBegin(GL_LINES);
            glVertexDraw(vertexTab[faceTab[i][0]]);
            glVertexDraw(vertexTab[faceTab[i][1]]);
            glVertexDraw(vertexTab[faceTab[i][0]]);
            glVertexDraw(vertexTab[faceTab[i][2]]);
            glVertexDraw(vertexTab[faceTab[i][1]]);
            glVertexDraw(vertexTab[faceTab[i][2]]);
            glEnd();
        //}
    }
}

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

Iterator_on_faces Mesh::f_begin() { return Iterator_on_faces(0, this); }
Iterator_on_faces Mesh::f_pend() { return Iterator_on_faces(faceTab.size(), this); }
Iterator_on_vertices Mesh::v_begin() { return Iterator_on_vertices(0, this); }
Iterator_on_vertices Mesh::v_pend() { return Iterator_on_vertices(vertexTab.size(), this); }
Circulator_on_faces Mesh::incident_f(Vertex &v) { return Circulator_on_faces(this->getVertexID(v), this); }
Circulator_on_vertices Mesh::adjacent_v(Vertex &v) { return Circulator_on_vertices(getVertexID(v), this); }

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

int Mesh::getFaceIndex(int vertexes[3]) const
{
    //std::cout<<"nb Vertex, nbFace : "<<vertexTab.size()<<" "<<faceTab.size()<<std::endl;
    int res = -1;
    int i = 0;
    do
    {
        /*printf("On compare : (%d, %d, %d) et (%d, %d, %d)\n",
               vertexes[0], vertexes[1], vertexes[2],
                faceTab[i].getVertex(0), faceTab[i].getVertex(1), faceTab[i].getVertex(2));*/
        if (faceTab[i].getVertex(0) == vertexes[0] && faceTab[i].getVertex(1) == vertexes[1] && faceTab[i].getVertex(2) == vertexes[2])
        {
            res = i;
        }
        i++;
    } while (res == -1 && i < faceTab.size());
    return res;
}

int Mesh::getVertexID(const Vertex &m)
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

double Mesh::getFaceArea(int FaceIndex)
{
    return getArea(getVertex(getFace(FaceIndex).getVertex(0)), getVertex(getFace(FaceIndex).getVertex(1)), getVertex(getFace(FaceIndex).getVertex(2)));
}

double Mesh::getCot(Vertex &v1, Vertex &v2, Vertex &v3)
{
    Vector vec1(v1, v2);
    Vector vec2(v1, v3);
    double sin = getSin(vec1, vec2);
    double cos = getCos(vec1, vec2);
    double clamp = 100;
    if (!sin)
        return clamp;
    double cot = cos / sin;
    if (cot > clamp)
        return clamp;
    if (cot < -clamp)
        return -clamp;
    return cot;
    //return (cos / sin) > 10000  ? 10000:(cos/sin);
}

void Mesh::toggleVoronoi()
{
    drawVoronoi = !drawVoronoi;
}
void Mesh::computeLaplacian()
{
    //std::cout << "Computing Laplacian ===================================================================\n";

    Laplacien = QVector<Vector>(vertexTab.size());
    Iterator_on_vertices its;
    Circulator_on_faces cf;
    Circulator_on_faces cfbegin;
    int i = 0, axisGlID, axisLocID;
    float coAlpha, coBeta, area;
    Vector sum(0, 0, 0);
    Vertex axis, nextV, lastV;

    //For every vertex
    for (its = v_begin(); its != v_pend(); ++its)
    {
        coAlpha = 0;
        coBeta = 0;
        area = 0;
        sum = Vector(0, 0, 0);
        cfbegin = incident_f(*its);

        //Variables for lisibility to avoid redundancy
        axis = *its;
        axisGlID = getVertexID(axis);
        axisLocID = (*cfbegin).global2localIndex(axisGlID);
        nextV = getVertex((*cfbegin).getVertex((axisLocID + 1) % 3)); // Vertex next to axis in counter-clock wise order
        lastV = getVertex((*cfbegin).getVertex((axisLocID + 2) % 3)); // Vertex next to axis in clock wise order
        //Get the first face's alpha,
        //get first alpha
        coAlpha = getCot(nextV, axis, lastV);

        cfbegin++;
        //CIRCULATOR begins from the second face (Alpha of first face is known)
        for (cf = cfbegin, ++cf; cf != cfbegin; cf++)
        {
            //std::cout << "Circulating around axis : " << axisGlID << "\n";
            axisLocID = (*cf).global2localIndex(axisGlID);
            nextV = getVertex((*cf).getVertex((axisLocID + 1) % 3));
            lastV = getVertex((*cf).getVertex((axisLocID + 2) % 3));
            //get beta
            //std::cout << "Computing Beta...\n";
            coBeta = getCot(lastV, nextV, axis);
            //std::cout << "CoBeta = " << coBeta << "\n";

            //get area
            //std::cout << "Computing Area...\n";
            area += getArea(lastV, nextV, axis);
            //std::cout << "area = " << area << "\n";

            //Compute sigma
            sum.x += (coBeta + coAlpha) * (nextV.x() - axis.x());
            sum.y += (coBeta + coAlpha) * (nextV.y() - axis.y());
            sum.z += (coBeta + coAlpha) * (nextV.z() - axis.z());

            // std::cout << "nextV.x() = " << nextV.x() << "\t axis.x = " << axis.x() << "\n";
            // std::cout << "coBeta = " << coBeta << "\t coAlpha = " << coAlpha << "\n";
            // std::cout << "(coBeta + coAlpha) = " << (coBeta + coAlpha) << "\t(nextV.x() - axis.x()) = " <<  (nextV.x() - axis.x()) << "\n";
            // std::cout << "sum.x =" << sum.x << "\n";
            // std::cout << "sum.y = " << sum.y << "\n";
            // std::cout << "sum.z = " << sum.z << "\n";

            //get the alpha
            //std::cout << "Computing Alpha...\n";
            coAlpha = getCot(nextV, axis, lastV);
            //std::cout << "Circulated ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++on a face\n";
        }
        //std::cout << "setting Laplacian ["<< i <<"] with 1/Area = "<< 1/area << " And sum.x = "<< sum.x << std::endl;
        //std::cout << "Laplacien of x is : " << ((1 / (2 * area)) * sum.x) << "\n";
        Laplacien[i].x = (1 / (2 * area)) * sum.x;
        Laplacien[i].y = (1 / (2 * area)) * sum.y;
        Laplacien[i].z = (1 / (2 * area)) * sum.z;

        // std::cout << "Computed Laplacian for vertex ["<< i <<"] : \t ["<< Laplacien[i].x << "]["<< Laplacien[i].y <<"]["<< Laplacien[i].z <<"]" << std::endl;
        // std::cout << "2H = \t" << norm(Laplacien[i]) << std::endl;
        if (i % 1000 == 0)
        {
            std::cout << "Computed Laplacian for vertex [" << i << "] : \t [" << Laplacien[i].x << "][" << Laplacien[i].y << "][" << Laplacien[i].z << "]" << std::endl;
            std::cout << "2H = \t" << norm(Laplacien[i]) << std::endl;
        }
        i++;
    }
    clampLamplacian(50);
    minMaxLaplacian();
    laplacianDone = true;
}

void Mesh::minMaxLaplacian()
{
    maxValueLaplacien = Laplacien[0];
    minValueLaplacien = Laplacien[0];
    maxNormLaplacian = norm(Laplacien[0]);
    minNormLaplacian = norm(Laplacien[0]);
    double n;
    for (auto p : Laplacien)
    {
        if (p.x > maxValueLaplacien.x)
        {
            maxValueLaplacien.x = p.x;
        }
        if (p.y > maxValueLaplacien.y)
        {
            maxValueLaplacien.y = p.y;
        }
        if (p.z > maxValueLaplacien.z)
        {
            maxValueLaplacien.z = p.z;
        }
        if (p.x < minValueLaplacien.x)
        {
            minValueLaplacien.x = p.x;
        }
        if (p.y < minValueLaplacien.y)
        {
            minValueLaplacien.y = p.y;
        }
        if (p.z < minValueLaplacien.z)
        {
            minValueLaplacien.z = p.z;
        }
        n = norm(p);
        if (n > maxNormLaplacian)
        {
            maxNormLaplacian = n;
        }
        else if (n < minNormLaplacian)
        {
            minNormLaplacian = n;
        }
    }
}

void Mesh::clampLamplacian(int clamp)
{
    for (int i = 0; i < Laplacien.size(); i++)
    {
        if (Laplacien[i].x > clamp)
            Laplacien[i].x = clamp;
        if (Laplacien[i].y > clamp)
            Laplacien[i].y = clamp;
        if (Laplacien[i].z > clamp)
            Laplacien[i].z = clamp;

        if (Laplacien[i].x < -clamp)
            Laplacien[i].x = -clamp;
        if (Laplacien[i].y < -clamp)
            Laplacien[i].y = -clamp;
        if (Laplacien[i].z < -clamp)
            Laplacien[i].z = -clamp;
    }
}

void Mesh::triangleSplit(int faceIndex, Point newV)
{
    //std::cout<<"Begining of triangleSplit"<<std::endl;

    Vertex v(newV.x(), newV.y(), newV.z());
    //if(isInFace(faceIndex, v)){
    // Définition du nouveau Vertex
    v.setFaceIndex(faceIndex);
    vertexTab.push_back(v);
    // Fin Définition

    // Création des deux nouvelles faces

    // Face 1
    Face newFace1;
    if (orientation(v, vertexTab[faceTab[faceIndex][0]], vertexTab[faceTab[faceIndex][1]]) > 0)
    {
        newFace1 = Face(vertexTab.size() - 1, faceTab[faceIndex][0], faceTab[faceIndex][1]);
        newFace1.setNeibFace(faceTab[faceIndex].getNeibFace(2), faceIndex, faceTab.size() + 1);
    }
    else
    {
        newFace1 = Face(vertexTab.size() - 1, faceTab[faceIndex][1], faceTab[faceIndex][0]);
        newFace1.setNeibFace(faceTab[faceIndex].getNeibFace(2), faceTab.size() + 1, faceIndex);
    }
    faceTab.push_back(newFace1);
    vertexTab[faceTab[faceIndex][0]].setFaceIndex(faceTab.size());
    vertexTab[faceTab[faceIndex][1]].setFaceIndex(faceTab.size());
    // Fin Face 1

    // Face 2
    Face newFace2;
    if (orientation(v, vertexTab[faceTab[faceIndex][0]], vertexTab[faceTab[faceIndex][2]]) > 0)
    {
        newFace2 = Face(vertexTab.size() - 1, faceTab[faceIndex][0], faceTab[faceIndex][2]);
        newFace2.setNeibFace(faceTab[faceIndex].getNeibFace(1), faceIndex, faceTab.size() - 1);
    }
    else
    {
        newFace2 = Face(vertexTab.size() - 1, faceTab[faceIndex][2], faceTab[faceIndex][0]);
        newFace2.setNeibFace(faceTab[faceIndex].getNeibFace(1), faceTab.size() - 1, faceIndex);
    }
    faceTab.push_back(newFace2);
    vertexTab[faceTab[faceIndex][0]].setFaceIndex(faceTab.size());
    vertexTab[faceTab[faceIndex][2]].setFaceIndex(faceTab.size());
    // Fin Face 2
    // Fin création des deux nouvelles faces

    // Mise à jour de l'ancienne face
    //Face oldFace = faceTab[faceIndex];
    faceTab[faceIndex][0] = vertexTab.size() - 1;
    faceTab[faceIndex].setNeibFace(faceTab[faceIndex].getNeibFace(0), faceTab.size() - 1, faceTab.size() - 2);
    // Fin mise à jour de l'ancienne face

    // Mise à jour des face voisine
    defineNeighbourFaces();

    // Mise à jour du Laplacien
    // TODO !!
    // Fin mise à jour du Laplacien

    //Mise à jour des debugObj
    updateDebugObj();
    //}
}

void Mesh::flip(int index1, int index2)
{
    std::cout << "Flipping Face " << index1 << "\t and Face " << index2 << std::endl;
    // markFace(index1);
    // markFace(index2);

    // Code commented to allow flip in non convex hull (convexize function)
    /*if(isInfinite(index1) || isInfinite(index2))
        return;*/

    Face fA = getFace(index1);
    Face fB = getFace(index2);
    int vA = fA.getDifferentVertex(fB); // The index of the opposite vertex on fA (-1 if disjointed triangles)
    int vB = fB.getDifferentVertex(fA); // The index of the opposite vertex on fB (-1 if disjointed triangles)
    int fA1ID = fA.getNeibFace((vA + 1) % 3);
    int fB1ID = fB.getNeibFace((vB + 1) % 3);
    Face fA1 = getFace(fA1ID); // The face near fA
    Face fB1 = getFace(fB1ID); // The face near fB

    if (vA == -1 || vB == -1 || fA1ID == -1 || fB1ID == -1 || fA1.getDifferentVertex(fA) == -1 || fB1.getDifferentVertex(fB) == -1)
    {
        std::cout << "Bad indexes \n";
        return;
    }
    std::cout << "Data : fA1 = " << fA.getNeibFace((vA + 1) % 3) << "\nfB1 = " << fB.getNeibFace((vB + 1) % 3) << std::endl;

    //Set incident faces for triangles
    vertexTab[fA.getVertex((vA + 2) % 3)].setFaceIndex(fA1ID);
    vertexTab[fB.getVertex((vB + 2) % 3)].setFaceIndex(fB1ID);

    //Set faces for neibghor's faces first
    std::cout << "Different vertex A: " << fA1.getDifferentVertex(fA) << std::endl;
    std::cout << "Different vertex B: " << fB1.getDifferentVertex(fB) << std::endl;
    fA1.setNeibFace(index2, fA1.getDifferentVertex(fA));
    fB1.setNeibFace(index1, fB1.getDifferentVertex(fB));

    //Set faces for fA / fB
    fA.setNeibFace(fB.getNeibFace((vB + 1) % 3), vA);
    fB.setNeibFace(fA.getNeibFace((vA + 1) % 3), vB);
    fA.setNeibFace(index2, (vA + 1) % 3);
    fB.setNeibFace(index1, (vB + 1) % 3);

    //Set vertices
    fA.setVertex((vA + 2) % 3, fB.getVertex(vB));
    fB.setVertex((vB + 2) % 3, fA.getVertex(vA));

    faceTab[index1] = fA;
    faceTab[index2] = fB;
    faceTab[fA1ID] = fA1;
    faceTab[fB1ID] = fB1;
}

// Possible amélioration : Au lieu de cherche à minimiser la distance pour le nouveau triangle,
// chercher les points permettant de former le triangle le plus equilateral possible (test sur les angles ?)
void Mesh::naiveInsertion(Point newV){
    //std::cout<<"Debut Insertion Naive"<<std::endl;
    Vertex v(newV.x(), newV.y(), newV.z());
    // Check if NewV is in mesh
    bool isInMesh = false;
    int indexFace = -1;
    int i = 0;
    do
    {
        if (isInFace(i, v))
        {
            isInMesh = true;
            indexFace = i;
        }
        ++i;
    } while (!isInMesh && i < faceTab.size());
    // If it is it's like a split
    // Else... more complex
    if(isInMesh){
        triangleSplit(indexFace, newV);
    } else {
        // Chercher le vertex sur le bord de l'enveloppe du maillage le plus proche
            //1) Chercher le point infinis
       int indexInfinitePoint=-1;
       for(int i=0; i<vertexTab.size();++i){
           if (vertexTab[i]==infiniteP){
            indexInfinitePoint = i;
           }
       }
       if(indexInfinitePoint==-1){
            EXIT_FAILURE;
        }
        else
        {

            //2) Creer circulateur sur le point infinis
                //2.1) Pour toutes les faces, calculer la distance vers le nouveau point (saut pour le point infinis
            Circulator_on_faces cf;
            Circulator_on_faces cfbegin = incident_f(*Iterator_on_vertices(indexInfinitePoint, this));
            int bestFaceIndex = 0;
            double minDeltaDistance = INT_MAX;
            double d1, d2;
            cf = cfbegin;
            do{
                d1=0;
                d2=0;
                for(int i=0;i<3;++i){
                    if(vertexTab[(*cf).getVertex(i)]!=infiniteP){
                        if(d1==0){
                            d1 = norm(newV - vertexTab[(*cf).getVertex(i)]);
                        } else if(d2==0){
                            d2 = norm(newV - vertexTab[(*cf).getVertex(i)]);
                        }
                    }
                }
                //2.2) Garder la face formant le triangle plus plus isocèle possible
                if(abs(d1)+abs(d2)<minDeltaDistance){
                    minDeltaDistance = abs(d1)+abs(d2);
                    bestFaceIndex = getFaceIndex((*cf).getVertexes());
                }
                cf++;
            }while(cf != cfbegin);

        // Idée 1 pour rajouter le nouveau triangle dans le maillage
        // Simplement ajouter le vertex et la face dans leur tableau
        vertexTab.push_back(v);

        // Création de la nouvelle face
            // Selection des deux bon vertex (pas le point infinis)
        std::pair<int, int> goodVertex(0,0);
        for(int i=0;i<3;++i){
            if(vertexTab[(*cf).getVertex(i)]!=infiniteP){
                if(goodVertex.first==0){
                    goodVertex.first=i;
                } else if(goodVertex.second==0){
                    goodVertex.second=i;
                }
            }
        }
        Face newFace;
        if (orientation(v, vertexTab[faceTab[bestFaceIndex][goodVertex.first]], vertexTab[faceTab[bestFaceIndex][goodVertex.second]]) > 0)
        {
            newFace = Face(vertexTab.size() - 2, faceTab[bestFaceIndex][goodVertex.first], faceTab[bestFaceIndex][goodVertex.second]);
        }
        else
        {
            newFace = Face(vertexTab.size() - 2, faceTab[bestFaceIndex][goodVertex.second], faceTab[bestFaceIndex][goodVertex.first]);
        }
        cleanInfinitePoints();
        faceTab.push_back(newFace);
        vertexTab[vertexTab.size()-1].setFaceIndex(faceTab.size()-1);
        int indexNewFace = faceTab.size()-1;
        // Fin Nouvelle Face
        // Mettre à jour voisin
        defineNeighbourFaces();

        convexize(faceTab[indexNewFace].getVertex(2), faceTab[indexNewFace].getNeibFace(1));
        convexize(faceTab[indexNewFace].getVertex(1), faceTab[indexNewFace].getNeibFace(2));
       }
    }
    std::cout<<"Fin Insertion Naive"<<std::endl;
}

void Mesh::convexize(int axisVertex, int infiniteTriangle)
{
    Circulator_on_faces cfbegin = incident_f(getVertex(axisVertex));
    Circulator_on_faces cf = cfbegin;
    do{
        if(isInfinite(getFaceIndex((*cf).getVertexes()))
                && getFaceIndex((*cf).getVertexes()) != infiniteTriangle)
        {
            double ori1 = orientation(Vertex(infiniteP), getVertex(axisVertex),
                                      getVertex((*cf).getVertex((*cf).getDifferentVertex(getFace(infiniteTriangle)))));
            double ori2 = orientation(Vertex(infiniteP), getVertex(axisVertex),
                                      getVertex(getFace(infiniteTriangle).getVertex(getFace(infiniteTriangle).getDifferentVertex((*cf)))));
            if ((ori1 >0 && ori2 <0)
                    || (ori1<0 && ori2>0)){
                flip(infiniteTriangle, getFaceIndex((*cf).getVertexes()));
                convexize((*cf).getVertex((*cf).getDifferentVertex(getFace(infiniteTriangle))), getFaceIndex((*cf).getVertexes()));
            };
        }
        cf++;
    } while (cfbegin != cf);
}

bool Mesh::isInFace(int index, const Vertex &v)
{
    int segment[2];
    Face f = getFace(index);
    //For all segments
    for (int i = 0; i < 3; i++)
    {
        segment[0] = i;
        segment[1] = (i + 1) % 3;
        //If v and the segment are incorrectly ordered
        if (orientation(v, getVertex(f.getVertex(segment[0])), getVertex(f.getVertex(segment[1]))) < 0)
            return false;
    }
    return true;
}

bool Mesh::isLocallyOfDelaunay(int index, bool debug, int& badFace)
{
    //std::cout << "\n\n\n\n\nCheck delaunay for face n " << index << std::endl;
    Face f = getFace(index);
    Vertex D;
    Vertex A = getVertex(f.getVertex(0));
    Vertex B = getVertex(f.getVertex(1));
    Vertex C = getVertex(f.getVertex(2));

    //Circulate on all neigbhor faces
    for (int i = 0; i < 3; i++)
    {
        //Get the non adjacent vertex
        //std::cout << "Checking with face number " << f.getNeibFace(i) << std::endl;
        int tmpIndex = (getFace(f.getNeibFace(i))).global2localIndexF(index);
        //std::cout << "Checking with vertex " << tmpIndex << std::endl;
        // printFaces();
        D = getVertex( getFace(f.getNeibFace(i)).getVertex(tmpIndex) );
        if (isInCircle(A, B, C, D))
        {
            //std::cout << "Not delaunay with face n : " <<  f.getNeibFace(i) << std::endl;
            if (debug)
                markFace(f.getNeibFace(i));
            badFace = f.getNeibFace(i);
            return false;
        }
    }
    //std::cout << "Is delaunay, nothing to see here" << std::endl;
    //All the vertices are outside the circle so the triangle is locally of Delaunay
    return true;
}

void Mesh::delaunize()
{
    int badFaceID = 0;
    //  For all triangles
    for (int i = 0 ; i < faceTab.size() ; i++)
    {
        //  Check if not deDelaunay
        if(!isLocallyOfDelaunay(i,false,badFaceID) && !isInfinite(i) && !isInfinite(badFaceID))
        {
            // Flip with bad face
            std::cout << "Flipping the faces ("<< i <<","<< badFaceID<<")" << std::endl;
            flip(i,badFaceID);
            i--;
        }
    }
}

void Mesh::delaunayInsert(Vertex v)
{
    printf("Delaunay insertion of vertex [%f][%f][%f]\n", v.x(), v.y(), v.z());
    naiveInsertion(v.getPoint());
    delaunize();
    //  Naive insertion of a point
    //  Stack of all impacted edges
    //  For each impacted edge
        // Check if triangles of the edge are locally of delaunay
            //Flip if not
            //Add newly impacted edge to stack
}

void Mesh::computeVoronoi()
{
    printf("Computing Voronoi...\n");
    delaunize();
    Iterator_on_vertices its;
    Circulator_on_faces cf;
    Circulator_on_faces cfbegin;
    Vertex tmpC, tmpV;
    voronoiCells.clear();
    QVector<Vertex> tmp;
    //For each vertex in a mesh
    for (its = v_begin(); its != v_pend(); ++its)
    {
        tmp.clear();

        cfbegin = incident_f(*its);
        cf = cfbegin;
        //Circulate around all the adjacent faces
        do
        {
            //add face's Circumcenter to the current Qvector<Vertex> reprensenting a cell
            tmpC = getCircumCenter(getVertex((*cf).getVertex(0)),
                                   getVertex((*cf).getVertex(1)),
                                   getVertex((*cf).getVertex(2)));

            // if(abs(tmpC.z()) < 100)
            tmp.push_back(tmpC);
            cf++; 
        } while (cf != cfbegin);

        tmpC = getCircumCenter(getVertex((*cf).getVertex(0)),
                               getVertex((*cf).getVertex(1)),
                               getVertex((*cf).getVertex(2)));

        tmp.push_back(tmpC);
        //Add all cells to the voronoiCells member variable
        voronoiCells.push_back(tmp);
        // printf("Added one voronoi cell\n");
    }
    // std::cout << "Voronoi computation ended..." << std::endl;
}

void Mesh::crust2D(QVector<Point> points){
    // Help : cf. cours 7 slides 42
    // Detruire toute arete de Delaunay traversant le squelette
        // Trouver le squelette
}

void Mesh::edgeCollapse(unsigned int indexFace, unsigned int relativeOppositeIndex){
    Vertex v1 = getVertex(faceTab[indexFace].getVertex((relativeOppositeIndex+1)%3));
    Vertex v2 = getVertex(faceTab[indexFace].getVertex((relativeOppositeIndex+2)%3));
    std::cout<<v1.x()<<" "<<v1.y()<<" "<<v1.z()<<std::endl;
    std::cout<<v2.x()<<" "<<v2.y()<<" "<<v2.z()<<std::endl;
    //Vertex newV((v1.x()+v2.x())/2, (v1), ());
}

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
bool Mesh::isInfinite(int index)
{
    Face f = getFace(index);
    for(int i = 0;i < 3 ; i++)
    {
        if( (getVertex(f.getVertex(i))) == infiniteP )
            return true;
    }
    return false;
}
