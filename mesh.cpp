#include "mesh.h"
#include "mesh_creation.hxx"
#include "mesh_iterators.hxx"
#include "mesh_predicate.hxx"
#include "mesh_statistics.hxx"
#include "mesh_modification.hxx"
#include "mesh_debug.hxx"

Mesh::Mesh() {
    srand(time(NULL));
}

Mesh::~Mesh() {}
// The following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
// Draw a vertex
void glVertexDraw(const Vertex &p)
{
    glVertex3f(p.x(), p.y(), p.z());
}

void Mesh::drawMesh()
{
    //glDrawArrays(GL_TRIANGLES, 0, vertexTab.size());
    int moduloI;
    double t1, t2, t3;
    for (int i = 0; i < faceTab.size(); i++)
    {
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
    }
    if (drawVoronoi)
    {
        // std::cout << "\n\nVoronoi drawing\n\n" << std::endl;
        for (int i = 0; i < voronoiCells.size(); i++)
        {
            //  Draw a cell
            //  std::cout << "Voronoi Cell drawing" << std::endl;
            glColor3d(1, 0, 1);
            glLineWidth(3);
            glBegin(GL_LINES);

            for (int j = 0; j < voronoiCells[i].size() - 1; j++)
            {
                // printf("Drawing voronoi vertice [%f][%f][%f]\n",voronoiCells[i][j].x(),voronoiCells[i][j].y(),voronoiCells[i][j].z());
                glVertexDraw(voronoiCells[i][j] + Vector(0, 0, 0.4));
                glVertexDraw(voronoiCells[i][j + 1] + Vector(0, 0, 0.4));
            }
            glEnd();
        }
    }
    // Drawing crustCurve
    if(drawCrust){
        for (int i = 0; i < crustCurve.size()-1; i = i+2)
        {
            glColor3d(1, 1, 1);
            glLineWidth(1);
            glBegin(GL_LINES);
            glVertexDraw(crustCurve[i] + Vector(0, 0, 0.9));
            glVertexDraw(crustCurve[i+1] + Vector(0, 0, 0.9));
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
int Mesh::getVertexCount(){return vertexTab.size();}
int Mesh::getFaceCount(){return faceTab.size();}
