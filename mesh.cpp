#include "mesh.h"

Mesh::Mesh()
{}

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


void printSegementMemory(Mesh::SegmentMemory t){
    std::cout<<"Mémoire : vertexIndex : "<<t.vertexIndex1
            << " "<< t.vertexIndex2<<" Index de la Face "
            <<t.faceIndex<<" Index dans la face "<<t.vertexInFaceIndex<<"\n";
}
void printAllSegmentMemory(const std::list<Mesh::SegmentMemory> & m){
    for(auto ma : m){printSegementMemory(ma);}
    std::cout<<"\n";
}

void printFacesNeib(const QVector<Face> & f){
    for(int j=0;j<f.size();++j){
        std::cout<<"Face : "<<j<<" : "<<f[j].getNeibFace(0)<<" "<<f[j].getNeibFace(1)
                <<" "<<f[j].getNeibFace(2)<<"\n";
    }
}

void Mesh::defineNeighbourFaces(){
    // TODO Use a hashMap instead of a list
    std::list<Mesh::SegmentMemory> memory;
    std::list<Mesh::SegmentMemory>::iterator m, m1, m2, m3;
    bool segment1, segment2, segment3;
    for(int i=0;i<faceTab.size();++i){
        segment1 = false;
        segment2 = false;
        segment3 = false;
        m = memory.begin();
        m1 = memory.end();
        m2 = memory.end();
        m3 = memory.end();

        do{
            if((m->vertexIndex1 == faceTab[i][0] && m->vertexIndex2 == faceTab[i][1])
                    || (m->vertexIndex1 == faceTab[i][1] && m->vertexIndex2 == faceTab[i][0])) {
                faceTab[i].setNeibFace(m->faceIndex,2);
                faceTab[m->faceIndex].setNeibFace(i,m->vertexInFaceIndex);
                segment1=true;
                m1=m;
            }
            if((m->vertexIndex1 == faceTab[i][1] && m->vertexIndex2 == faceTab[i][2])
                    || (m->vertexIndex1 == faceTab[i][2] && m->vertexIndex2 == faceTab[i][1])) {
                faceTab[i].setNeibFace(m->faceIndex,0);
                faceTab[m->faceIndex].setNeibFace(i,m->vertexInFaceIndex);
                segment2=true;
                m2=m;
            }
            if((m->vertexIndex1 == faceTab[i][0] && m->vertexIndex2 == faceTab[i][2])
                    || (m->vertexIndex1 == faceTab[i][2] && m->vertexIndex2 == faceTab[i][0])) {
                faceTab[i].setNeibFace(m->faceIndex,1);
                faceTab[m->faceIndex].setNeibFace(i,m->vertexInFaceIndex);
                segment3=true;
                m3=m;
            }
            m++;
        }
        // Tant que tout les segments sont pas ok && que l'on peut avancer dans la memoire
        while((!segment1 || !segment2 || !segment3) && m!=memory.end());

        //printAllSegmentMemory(memory);
        //printFacesNeib(faceTab);

        if(!segment1){memory.push_back(Mesh::SegmentMemory(faceTab[i][0], faceTab[i][1], i, 2));} else {memory.erase(m1);}
        if(!segment2){memory.push_back(Mesh::SegmentMemory(faceTab[i][1], faceTab[i][2], i, 0));} else {memory.erase(m2);}
        if(!segment3){memory.push_back(Mesh::SegmentMemory(faceTab[i][0], faceTab[i][2], i, 1));} else {memory.erase(m3);}
    }
}

Iterator_on_faces Mesh::f_begin() { return Iterator_on_faces(0, this); }
Iterator_on_faces Mesh::f_pend() { return Iterator_on_faces(faceTab.size(), this); }
Iterator_on_vertices Mesh::v_begin() { return Iterator_on_vertices(0, this); }
Iterator_on_vertices Mesh::v_pend() { return Iterator_on_vertices(vertexTab.size(), this); }
Circulator_on_faces Mesh::incident_f(Vertex &v){return Circulator_on_faces(this->getVertexID(v),this);}
Circulator_on_vertices Mesh::adjacent_v(Vertex &v){return Circulator_on_vertices(getVertexID(v),this);}
