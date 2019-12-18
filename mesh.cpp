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
                    glColor3d(1, 0.306, 0.455);
                else if (moduloI == 1)
                    glColor3d(0.455, 1, 0.306);
                else if (moduloI == 2)
                    glColor3d(0.306, 0.608, 1);
                else
                    glColor3d(1, 0.584, 0.306);
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
            switch(i%3)
            {
            case 0:glColor3d(1, 0, 1);
                break;
            case 1: glColor3d(1, 1, 0);
                break;
            case 2 : glColor3d(0, 1, 1);
                break;
            }
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
            switch((i/2)%2)
            {
                case 0 : glColor3d(1, 0.467, 0);
                break;
                case 1 : glColor3d(0, 0.169, 1);
                break;
            }
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

int Mesh::oppositeVertexGlobal(int localVertexId, int faceId){
    int opppositeFace = faceTab[faceId].getNeibFace(localVertexId);
    for(int i=0;i<3;++i){
        if(faceTab[opppositeFace].getNeibFace(i)==faceId){return faceTab[opppositeFace].getVertex(i);}
    }
    return -1;
}

int Mesh::oppositeVertexLocal(int localVertexId, int faceId){
    int opppositeFace = faceTab[faceId].getNeibFace(localVertexId);
    for(int i=0;i<3;++i){
        if(faceTab[opppositeFace].getNeibFace(i)==faceId){return i;}
    }
    return -1;
}

void Mesh::eraseVertex(int vertexId){
    vertexTab.remove(vertexId);
    vertexDebugTab.remove(vertexId);
    for (int j=0;j<faceTab.size();++j) {
        for(int i=0;i<3;++i){
            if(faceTab[j].getVertex(i)>=vertexId){
                faceTab[j].setVertex(i, faceTab[j].getVertex(i)-1);
            } else if(faceTab[j].getVertex(i)==vertexId){
                fprintf(stderr, "Il y a encore des liens vers le vertex que l'on veut supprimer.\n");
            }
        }
    }
}
void Mesh::eraseFace(int faceId){
    //std::cout<<"On supprime la face "<<faceId<<std::endl;
    faceTab.remove(faceId);
    faceDebugTab.remove(faceId);
    for (int j=0;j<faceTab.size();++j) {
        for(int i=0;i<3;++i){
            if(faceTab[j].getNeibFace(i)>faceId){
                faceTab[j].setNeibFace(faceTab[j].getNeibFace(i)-1, i);
            } else if(faceTab[j].getNeibFace(i)==faceId){
                fprintf(stderr, "Il y a encore des liens face %d à face %d vers la face que l'on veut supprimer a cause de vertex %d.\n",
                        j, faceId, faceTab[j].getVertex(i));
            }

        }
    }
    for(int i=0;i<vertexTab.size();++i){
        if(vertexTab[i].getFaceIndex()>faceId){
            vertexTab[i].setFaceIndex(vertexTab[i].getFaceIndex()-1);
        } else if(vertexTab[i].getFaceIndex()==faceId){
            fprintf(stderr, "Il y a encore des liens vertex a face vers la face que l'on veut supprimer.\n");
        }
    }
}
