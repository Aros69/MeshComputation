#include "mesh.h"

Mesh::Mesh()
{
    
    // int faces[12]={
    //     0,1,2, // 1
    //     1,3,2, // 2
    //     3,0,2, // 3
    //     0,1,3  // 4
    // }; // To be replaced by a vector of faces
}

// The following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
// Draw a vertex
void glVertexDraw(const Vertex & p) {
    glVertex3f(p.x(), p.y(), p.z());
}

//Example with a tetraedra
void Mesh::drawMesh() {
    int moduloI;
    for(int i = 0; i < faceTab.size(); i++) {

        moduloI = i%4;
        if (moduloI == 0) glColor3d(1,0,0);
        else if (moduloI == 1) glColor3d(0,1,0);
        else if (moduloI == 2) glColor3d(0,0,1);
        else glColor3d(1,1,0);

        glBegin(GL_TRIANGLES);
        glVertexDraw(vertexTab[faceTab[i][0]]);
        glVertexDraw(vertexTab[faceTab[i][1]]);
        glVertexDraw(vertexTab[faceTab[i][2]]);
        glEnd();
    }
}

//Example with a wireframe tedraedra
void Mesh::drawMeshWireFrame() {
    for(int i = 0; i < faceTab.size(); i++) {
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[faceTab[i][0]]);
            glVertexDraw(vertexTab[faceTab[i][1]]);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[faceTab[i][1]]);
            glVertexDraw(vertexTab[faceTab[i][2]]);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[faceTab[i][0]]);
            glVertexDraw(vertexTab[faceTab[i][2]]);
        glEnd();
    }

}

void Mesh::setMesh(QVector<Vertex> vertices,QVector<Face> faces){
    vertexTab = vertices;
    faceTab = faces;
}

void Mesh::meshWithFile(std::string filePath){
    std::cout<<"Begin of creating a mesh with .off file\n";
    std::ifstream file(filePath.c_str());
    if(!file.is_open()){
        std::cout<<"ERROR : Unable to open file : \""<<filePath<<"\"\n";
    } else {
        int nbVertices, nbFaces, dump, a, b, c;
        float x, y, z;
        std::string temp;
        file >> nbVertices >> nbFaces >> dump;

        vertexTab.clear();
        for(int i=0;i<nbVertices;++i){
            file >> x >> y >> z;
            vertexTab.push_back(Vertex(x,y,z));
        }
        faceTab.clear();
        for(int i=0;i<nbFaces;++i){
            //TODO use the first element to check if we create triangle or another polygon
            file >> dump >> a >> b >> c;
            vertexTab[a].setFaceIndex(i);
            vertexTab[b].setFaceIndex(i);
            vertexTab[c].setFaceIndex(i);
            faceTab.push_back(Face(a,b,c));
        }
        defineNeighbourFaces();
        file.close();
    }
    std::cout<<"End of creating a mesh with .off file\n";
}

void Mesh::defineNeighbourFaces(){
    // TODO Use a hashMap instead of a list
    std::list<Mesh::segmentMemory> memory;
    std::list<Mesh::segmentMemory>::iterator m, m1, m2, m3;
    //Mesh::segmentMemory* m;
    bool segment1, segment2, segment3;
    int i = 0;
    for(auto face : faceTab){
        segment1 = false;
        segment2 = false;
        segment3 = false;
        m = memory.begin();
        do{
            if(face.getNeibFace(2) == -1
                    && ((m->vertexIndex1 == face[0] && m->vertexIndex2 == face[1])
                    || (m->vertexIndex1 == face[1] && m->vertexIndex2 == face[0]))) {
                face.setNeibFace(m->faceIndex,2);
                faceTab[m->faceIndex].setNeibFace(i,m->vertexInFaceIndex);
                segment1=true;
                m1=m;
            }

            if((m->vertexIndex1 == face[1] && m->vertexIndex2 == face[2])
                    || (m->vertexIndex1 == face[2] && m->vertexIndex2 == face[1])) {
                face.setNeibFace(m->faceIndex,0);
                faceTab[m->faceIndex].setNeibFace(i,m->vertexInFaceIndex);
                segment2=true;
                m2=m;
            }

            if((m->vertexIndex1 == face[0] && m->vertexIndex2 == face[2])
                    || (m->vertexIndex1 == face[2] && m->vertexIndex2 == face[0])) {
                face.setNeibFace(m->faceIndex,1);
                //std::cout<<faceTab[m->faceIndex].getNeibFace(m->vertexInFaceIndex)<<"\n";
                faceTab[m->faceIndex].setNeibFace(i,m->vertexInFaceIndex);
                //std::cout<<faceTab[m->faceIndex].getNeibFace(m->vertexInFaceIndex)<<"\n";
                segment3=true;
                m3=m;
            }
            m++;
        // Tant que tout les segments sont pas ok && que l'on peut avancer dans la memoire
        }while((!segment1 || !segment2 || !segment3) && m!=memory.end());
        if(!segment1){memory.push_back(Mesh::segmentMemory(face[0], face[1], i, 2));} //else {memory.erase(m1);}
        if(!segment2){memory.push_back(Mesh::segmentMemory(face[1], face[2], i, 0));} //else {memory.erase(m2);}
        if(!segment3){memory.push_back(Mesh::segmentMemory(face[0], face[2], i, 1));} //else {memory.erase(m3);}
        ++i;
        std::cout<<i<<" "<<memory.size()<<"\n";
    }

    i=0;
    for(auto f : faceTab){
        std::cout<<f.getNeibFace(0)<<" "<<f.getNeibFace(1)<<" "<<f.getNeibFace(2)<<"\n";
        if(f.getNeibFace(0)==-1){
            ++i;
        }
        if(f.getNeibFace(1)==-1){
            ++i;
        }
        if(f.getNeibFace(2)==-1){
            ++i;
        }
    }
    std::cout<<i<<"\n";
}
