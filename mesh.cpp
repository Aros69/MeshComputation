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

void printFacesNeib(const QVector<Face> & f){
    for(int j=0;j<f.size();++j){
        std::cout<<"Face : "<<j<<" : ("<<f[j].getVertex(0)<<", "
                <<f[j].getNeibFace(0)<<") (" <<f[j].getVertex(1)
                <<", "<<f[j].getNeibFace(1) <<") ("
                <<f[j].getVertex(2)<<", "<<f[j].getNeibFace(2)<<")\n";
    }
}

void Mesh::defineNeighbourFaces(){
    SegmentMemory memory;

    for(int i=0;i<faceTab.size();++i){
        int toTest[2] {faceTab[i][0], faceTab[i][1]};
        if(memory.contain(toTest)){
            faceTab[i].setNeibFace(memory.faceIndex(toTest),2);
            faceTab[memory.faceIndex(toTest)]
                    .setNeibFace(i,memory.vertexInFaceIndex(toTest));
            memory.deleteSegment(toTest);
        } else {
            memory.addSegment(faceTab[i][0], faceTab[i][1], i, 2);
        }
        int toTest2[2] {faceTab[i][1], faceTab[i][2]};
        if(memory.contain(toTest2)){
            faceTab[i].setNeibFace(memory.faceIndex(toTest2),0);
            faceTab[memory.faceIndex(toTest2)]
                    .setNeibFace(i,memory.vertexInFaceIndex(toTest2));
            memory.deleteSegment(toTest2);
        } else {
            memory.addSegment(faceTab[i][1], faceTab[i][2], i, 0);
        }
        int toTest3[2] {faceTab[i][0], faceTab[i][2]};
        if(memory.contain(toTest3)){
            faceTab[i].setNeibFace(memory.faceIndex(toTest3),1);
            faceTab[memory.faceIndex(toTest3)]
                    .setNeibFace(i,memory.vertexInFaceIndex(toTest3));
            memory.deleteSegment(toTest3);
        } else {
            memory.addSegment(faceTab[i][0], faceTab[i][2], i, 1);
        }
    }
    //printFacesNeib(faceTab);
    //memory.print();
}

Iterator_on_faces Mesh::f_begin() { return Iterator_on_faces(0, this); }
Iterator_on_faces Mesh::f_pend() { return Iterator_on_faces(faceTab.size(), this); }
Iterator_on_vertices Mesh::v_begin() { return Iterator_on_vertices(0, this); }
Iterator_on_vertices Mesh::v_pend() { return Iterator_on_vertices(vertexTab.size(), this); }
Circulator_on_faces Mesh::incident_f(Vertex &v){return Circulator_on_faces(this->getVertexID(v),this);}
Circulator_on_vertices Mesh::adjacent_v(Vertex &v){return Circulator_on_vertices(getVertexID(v),this);}

float dot(const Vector& v1,const Vector& v2)
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector cross(const Vector& v1,const Vector& v2)
{
  return Vector( (v1.y * v2.z) - (v1.z * v2.y),
      (v1.z * v2.x) - (v1.x * v2.z),
      (v1.x * v2.y) - (v1.y * v2.x) );
}

float norm(const Vector& v){
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector normalize(const Vector& v){
  float magnetude = norm(v);
  Vector normV(0,0,0);
  normV.x = v.x/magnetude;
  normV.y = v.y/magnetude;
  normV.z = v.z/magnetude;
  return normV;
}

float getCos(const Vector& v1,const Vector& v2){
  return dot(normalize(v1),normalize(v2));
}

float getSin(const Vector& v1,const Vector& v2){
  return cross(normalize(v1),normalize(v2)).x;
}

void Mesh::printFaces(){
        Iterator_on_faces itf;
        int i = 0;
        for(itf = f_begin();itf  != f_pend() ; itf++){
            (*itf).print(i);
            i++;
        }
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

float Mesh::getFaceArea(Vertex& vert1,Vertex& vert2, Vertex& vert3){
  Vector v1(vert1,vert2);
  Vector v2(vert1,vert3);
  return norm(cross(v1,v2));
}

float Mesh::getFaceArea(int FaceIndex){
  return getFaceArea(getVertex(getFace(FaceIndex).getVertex(0)),getVertex(getFace(FaceIndex).getVertex(1))
            ,getVertex(getFace(FaceIndex).getVertex(2)));
}

float Mesh::getCot(Vertex& v1,Vertex& v2, Vertex& v3){
  Vector vec1(v1,v2);
  Vector vec2(v1,v3);
  double sin = getSin(vec1,vec2);
  double cos = getCos(vec1,vec2);
  if(!sin)
    return 10000;
  double cot = cos/sin;
  if(cot > 10000) return 10000;
  if(cot < -10000) return -10000;
  return cot;
  //return (cos / sin) > 10000  ? 10000:(cos/sin);
}
//TODO
void Mesh::computeLaplacian(){
  //std::cout << "Computing Laplacian ===================================================================\n";

  Laplacien = QVector<Vector>(vertexTab.size());
  Iterator_on_vertices its;
  Circulator_on_faces cf;
  int i = 0;

  //For every vertex
  for (its = v_begin(); its != v_pend(); ++its)
  {
      float coAlpha = 0;
      float coBeta = 0;
      float area = 0;
      Vector sum(0,0,0);
      Circulator_on_faces cfbegin = incident_f(*its);

      //Variables for lisibility to avoid redundancy
      Vertex axis = *its;
      int axisGlID = getVertexID(axis);
      int axisLocID = (*cfbegin).global2localIndex(axisGlID);
      Vertex nextV = getVertex((*cfbegin).getVertex( (axisLocID+1) %3)); // Vertex next to axis in counter-clock wise order
      Vertex lastV = getVertex((*cfbegin).getVertex( (axisLocID+2) %3)); // Vertex next to axis in clock wise order
      //Get the first face's alpha,
      //get first alpha
      coAlpha = getCot( nextV, axis, lastV );

      cfbegin++;
      //CIRCULATOR begins from the second face (Alpha of first face is known)
      for (cf = cfbegin, ++cf; cf != cfbegin; cf++){
        //std::cout << "Circulating around axis : " << axisGlID << "\n";
        axisLocID = (*cf).global2localIndex(axisGlID);
        nextV = getVertex((*cf).getVertex( (axisLocID+1) %3));
        lastV = getVertex((*cf).getVertex( (axisLocID+2) %3));
        //get beta
        //std::cout << "Computing Beta...\n";
        coBeta = getCot( lastV, nextV, axis );
        //std::cout << "CoBeta = " << coBeta << "\n";

        //get area
        //std::cout << "Computing Area...\n";
        area += getFaceArea( lastV, nextV, axis );
        //std::cout << "area = " << area << "\n";

        //Compute sigma
        sum.x += (coBeta + coAlpha) * (nextV.x() - axis.x());
        sum.y += (coBeta + coAlpha) * (nextV.y() - axis.y() );
        sum.z += (coBeta + coAlpha) * (nextV.z() - axis.z() );

        // std::cout << "nextV.x() = " << nextV.x() << "\t axis.x = " << axis.x() << "\n";
        // std::cout << "coBeta = " << coBeta << "\t coAlpha = " << coAlpha << "\n";
        // std::cout << "(coBeta + coAlpha) = " << (coBeta + coAlpha) << "\t(nextV.x() - axis.x()) = " <<  (nextV.x() - axis.x()) << "\n";
        // std::cout << "sum.x =" << sum.x << "\n";
        // std::cout << "sum.y = " << sum.y << "\n";
        // std::cout << "sum.z = " << sum.z << "\n";

        //get the alpha
        //std::cout << "Computing Alpha...\n";
        coAlpha = getCot( nextV, axis, lastV );
        //std::cout << "Circulated ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++on a face\n";

      }
      //std::cout << "setting Laplacian ["<< i <<"] with 1/Area = "<< 1/area << " And sum.x = "<< sum.x << std::endl;
      //std::cout << "Laplacien of x is : " << ((1 / (2 * area)) * sum.x) << "\n";
      Laplacien[i].x = (1 / (2 * area)) * sum.x;
      Laplacien[i].y = (1 / (2 * area)) * sum.y;
      Laplacien[i].z = (1 / (2 * area)) * sum.z;

      std::cout << "Computed Laplacian for vertex ["<< i <<"] : \t ["<< Laplacien[i].x << "]["<< Laplacien[i].y <<"]["<< Laplacien[i].z <<"]\n";
      i++;
  }
}
