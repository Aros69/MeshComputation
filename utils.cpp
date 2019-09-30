#include "utils.h"

void Face::setNeibFace(int neib1, int neib2, int neib3)
{
    neibFace[0] = neib1;
    neibFace[1] = neib2;
    neibFace[2] = neib3;
}

void Face::setNeibFace(int neib, int index)
{
    neibFace[index] = neib;
}

void Face::setVertex(int index,int v)
{
  verticesIndex[index] = v;
}

void Face::setVertices(int v1,int v2,int v3)
{
  verticesIndex[0] = v1;
  verticesIndex[1] = v2;
  verticesIndex[2] = v3;
}

int Face::global2localIndex(int globalIndex)
{
    for (int i = 0; i < 3; i++)
    {
        if (verticesIndex[i] == globalIndex)
        {
            return i;
        }
    }
    std::cout << "vertex not part of this triangle\n";
    std::cout << "indexes are ["<< verticesIndex [0] <<"] ["<< verticesIndex[1] <<"] and ["<< verticesIndex[2]<<"\n";
    std::cout << " Provided was " << globalIndex << "\n";
    return -1;
}

void Face::print(int ID){
    std::cout << "Face ["<< ID <<"] : \n";
    std::cout << "Vertices Index :\n 1 :\t ["<< verticesIndex [0] <<"]\t 2 : ["<< verticesIndex[1]
              <<"]\t 3 :["<< verticesIndex[2]<<"]\n";
    std::cout << "Near Faces Index :\n 1 :\t ["<< neibFace[0] <<"]\t 2 : ["<< neibFace[1]
              <<"]\t 3 :["<< neibFace[2]<<"]\n";
}

void printFacesNeib(const QVector<Face> & f){
    std::cout<<"Affichages des faces voisines"<<std::endl;
    for(int j=0;j<f.size();++j){
        std::cout<<"Face : "<<j<<" : ("<<f[j].getVertex(0)<<", "
                <<f[j].getNeibFace(0)<<") (" <<f[j].getVertex(1)
                <<", "<<f[j].getNeibFace(1) <<") ("
                <<f[j].getVertex(2)<<", "<<f[j].getNeibFace(2)<<")\n";
    }
    std::cout<<"Fin affichages des faces voisines"<<std::endl;
}

double dot(const Vector& v1,const Vector& v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector cross(const Vector& v1,const Vector& v2) {
  return Vector( (v1.y * v2.z) - (v1.z * v2.y),
      (v1.z * v2.x) - (v1.x * v2.z),
      (v1.x * v2.y) - (v1.y * v2.x) );
}

double norm(const Vector& v) {
  return double(sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

Vector normalize(const Vector& v) {
  double magnetude = norm(v);
  Vector normV(0,0,0);
  normV.x = v.x/magnetude;
  normV.y = v.y/magnetude;
  normV.z = v.z/magnetude;
  return normV;
}

double getCos(const Vector& v1,const Vector& v2) {
  return dot(normalize(v1),normalize(v2));
}

double getSin(const Vector& v1,const Vector& v2) {
  return cross(normalize(v1),normalize(v2)).x;
}

int orientation(const Vertex& v1,const Vertex& v2,const Vertex& v3) {
  Vector vec1(v1,v2);
  Vector vec2(v1,v3);
  Vector oz(0,0,1);
  return dot(normalize(vec1),normalize(vec2));
}
