#include "mesh.h"

Mesh::Mesh(){
    for(auto p : Laplacien){
        p.x = INT_MIN;
        p.y = INT_MIN;
        p.z = INT_MIN;
    }
    minValueLaplacien.x=INT_MIN;
}

Mesh::~Mesh(){}


// The following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
// Draw a vertex
void glVertexDraw(const Vertex & p) {
    glVertex3f(p.x(), p.y(), p.z());
}

void Mesh::drawMesh() {
    int moduloI;
    double redColor, greenColor, blueColor;
    for(int i = 0; i < faceTab.size(); i++) {


        if(minValueLaplacien.x!=INT_MIN){
            // Méthode de coloration avec moyenne et x,y,z
            /*redColor = double(Laplacien[faceTab[i][0]].x
                    + Laplacien[faceTab[i][1]].x
                    + Laplacien[faceTab[i][2]].x)/3;
            blueColor = double(Laplacien[faceTab[i][0]].y
                    + Laplacien[faceTab[i][1]].y
                    + Laplacien[faceTab[i][2]].y)/3;
            greenColor = double(Laplacien[faceTab[i][0]].z
                    + Laplacien[faceTab[i][1]].z
                    + Laplacien[faceTab[i][2]].z)/3;
            redColor = double(redColor-minValueLaplacien.x)/(maxValueLaplacien.x-minValueLaplacien.x);
            blueColor = double(blueColor-minValueLaplacien.y)/(maxValueLaplacien.y-minValueLaplacien.y);
            greenColor = double(greenColor-minValueLaplacien.z)/(maxValueLaplacien.z-minValueLaplacien.z);
            glColor3d(redColor,blueColor,greenColor);*/

            // Méthode de coloration avec norme
            double t = double(norm(Laplacien[faceTab[i][0]])
                    + norm(Laplacien[faceTab[i][1]])
                    + norm(Laplacien[faceTab[i][2]]))/3/maxNormLaplacian;
            glColor3d(t, t, t);

        } else {
            moduloI = i%4;
            if (moduloI == 0) glColor3d(1,0,0);
            else if (moduloI == 1) glColor3d(0,1,0);
            else if (moduloI == 2) glColor3d(0,0,1);
            else glColor3d(1,1,0);
        }

        glBegin(GL_TRIANGLES);
        glVertexDraw(vertexTab[faceTab[i][0]]);
        glVertexDraw(vertexTab[faceTab[i][1]]);
        glVertexDraw(vertexTab[faceTab[i][2]]);
        glEnd();
    }
}

void Mesh::drawMeshWireFrame() {
    for(int i = 0; i < faceTab.size(); i++) {
        glBegin(GL_LINES);
            glVertexDraw(vertexTab[faceTab[i][0]]);
            glVertexDraw(vertexTab[faceTab[i][1]]);
            glVertexDraw(vertexTab[faceTab[i][2]]);
            glVertexDraw(vertexTab[faceTab[i][0]]);
        glEnd();
    }
}

void Mesh::setMesh(QVector<Vertex> vertices,QVector<Face> faces){
    vertexTab = vertices;
    faceTab = faces;
}

void Mesh::meshWithFile(std::string filePath){
    //std::cout<<"Begin of creating a mesh with .off file\n";
    std::ifstream file(filePath.c_str());
    if(!file.is_open()){
        std::cout<<"ERROR : Unable to open file : \""<<filePath<<"\"\n";
    } else {
        int nbVertices, nbFaces, dump, a, b, c;
        double x, y, z;
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
    //std::cout<<"End of creating a mesh with .off file\n";
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

double dot(const Vector& v1,const Vector& v2)
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector cross(const Vector& v1,const Vector& v2)
{
  return Vector( (v1.y * v2.z) - (v1.z * v2.y),
      (v1.z * v2.x) - (v1.x * v2.z),
      (v1.x * v2.y) - (v1.y * v2.x) );
}

double norm(const Vector& v){
  return double(sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

Vector normalize(const Vector& v){
  double magnetude = norm(v);
  Vector normV(0,0,0);
  normV.x = v.x/magnetude;
  normV.y = v.y/magnetude;
  normV.z = v.z/magnetude;
  return normV;
}

double getCos(const Vector& v1,const Vector& v2){
  return dot(normalize(v1),normalize(v2));
}

double getSin(const Vector& v1,const Vector& v2){
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

double Mesh::getFaceArea(Vertex& vert1,Vertex& vert2, Vertex& vert3){
  Vector v1(vert1,vert2);
  Vector v2(vert1,vert3);
  return norm(cross(v1,v2));
}

double Mesh::getFaceArea(int FaceIndex){
  return getFaceArea(getVertex(getFace(FaceIndex).getVertex(0)),getVertex(getFace(FaceIndex).getVertex(1))
            ,getVertex(getFace(FaceIndex).getVertex(2)));
}

double Mesh::getCot(Vertex& v1,Vertex& v2, Vertex& v3){
  Vector vec1(v1,v2);
  Vector vec2(v1,v3);
  double sin = getSin(vec1,vec2);
  double cos = getCos(vec1,vec2);
  double clamp = 1;
  if(!sin)
    return clamp;
  double cot = cos/sin;
  if(cot > clamp) return clamp;
  if(cot < -clamp) return -clamp;
  return cot;
  //return (cos / sin) > 10000  ? 10000:(cos/sin);
}

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

      // std::cout << "Computed Laplacian for vertex ["<< i <<"] : \t ["<< Laplacien[i].x << "]["<< Laplacien[i].y <<"]["<< Laplacien[i].z <<"]" << std::endl;
      // std::cout << "2H = \t" << norm(Laplacien[i]) << std::endl;
      if(i%1000 == 0){
        std::cout << "Computed Laplacian for vertex ["<< i <<"] : \t ["<< Laplacien[i].x << "]["<< Laplacien[i].y <<"]["<< Laplacien[i].z <<"]" << std::endl;
        std::cout << "2H = \t" << norm(Laplacien[i]) << std::endl;
      }
      i++;
  }
  clampLamplacian(50);
  minMaxLaplacian();
}

void Mesh::minMaxLaplacian(){
    maxValueLaplacien = Laplacien[0];
    minValueLaplacien = Laplacien[0];
    maxNormLaplacian = norm(Laplacien[0]);
    minNormLaplacian = norm(Laplacien[0]);
    double n;
    for(auto p : Laplacien){
        if (p.x>maxValueLaplacien.x){
            maxValueLaplacien.x = p.x;
        }
        if (p.y>maxValueLaplacien.y){
            maxValueLaplacien.y = p.y;
        }
        if (p.z>maxValueLaplacien.z){
            maxValueLaplacien.z = p.z;
        }
        if (p.x<minValueLaplacien.x){
            minValueLaplacien.x = p.x;
        }
        if (p.y<minValueLaplacien.y){
            minValueLaplacien.y = p.y;
        }
        if (p.z<minValueLaplacien.z){
            minValueLaplacien.z = p.z;
        }
        n=norm(p);
        if(n>maxNormLaplacian){
            maxNormLaplacian=n;
        } else if(n<minNormLaplacian) {
            minNormLaplacian=n;
        }
    }
}

void Mesh::clampLamplacian(int clamp){
    for(int i = 0; i< Laplacien.size();i++){
      if(Laplacien[i].x > clamp)
        Laplacien[i].x = clamp;
      if(Laplacien[i].y > clamp)
        Laplacien[i].y = clamp;
      if(Laplacien[i].z > clamp)
        Laplacien[i].z = clamp;

      if(Laplacien[i].x < -clamp)
        Laplacien[i].x = -clamp;
      if(Laplacien[i].y < -clamp)
        Laplacien[i].y = -clamp;
      if(Laplacien[i].z < -clamp)
        Laplacien[i].z = -clamp;
    }
}

void Mesh::threadedLaplacian(){

}
