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
    std::cout<<vertexTab.size() << "\n";
    for(int i = 0; i < faceTab.size(); i++) {

        if (i < 1) glColor3d(1,0,0);
        else if (i < 2) glColor3d(0,1,0);
        else if (i < 3) glColor3d(0,0,1);
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