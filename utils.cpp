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
int Face::global2localIndexF(int globalIndex){
  for (int i = 0; i < 3; i++)
    {
        if (neibFace[i] == globalIndex)
        {
            return i;
        }
    }
    std::cout << "Face not part of this triangle\n";
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

double distance(const Point &p1, const Point &p2){
    return sqrt(pow(p2.x()-p1.x(), 2) + pow(p2.y()-p1.y(),2) + pow(p2.z()-p1.z(),2));
}

Vertex operator*(float f, const Vertex& v)
{
    return Vertex(v.x() * f, v.y() * f,  v.z() * f);
}

Vertex operator*(const Vertex& v, float f)
{
    return Vertex(v.x() * f, v.y() * f,  v.z() * f );
}

Vertex operator+(const Vertex& v1,const Vertex& v2)
{
  return Vertex(v1.x() + v2.x(),  v1.y() + v2.y(),  v1.z() + v2.z());
}

Vertex operator+(const Vertex& v1,const Vector& vec)
{
  return Vertex(v1.x() +  vec.x, v1.y() +  vec.y, v1.z() +  vec.z);
}

Vertex operator+(const Vector& vec,const Vertex& v1)
{
  return Vertex(v1.x() +  vec.x, v1.y() +  vec.y, v1.z() +  vec.z);
}
Vector operator-(const Vertex& v1,const Vertex& v2)
{
  return Vector(v1.x() - v2.x(),  v1.y() - v2.y(),  v1.z() - v2.z());
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
  return norm(cross(v1,v2)) / (norm(v1)*norm(v2));
}

double getTan(const Vector& v1,const Vector& v2) {
  return norm(cross(v1,v2)) / std::max(dot(v1,v2),0.1);
}

double orientation(const Vertex& v1,const Vertex& v2,const Vertex& v3) {
  Vector vec1(v1,v2);
  Vector vec2(v1,v3);
  Vector oz(0,0,1);
  return dot(cross(normalize(vec1),normalize(vec2)),oz);
}

double getArea(Vertex &vert1, Vertex &vert2, Vertex &vert3)
{
    Vector v1(vert1, vert2);
    Vector v2(vert1, vert3);
    return norm(cross(v1, v2))/2;
}

Vertex getCircumCenter(Vertex A, Vertex B, Vertex C)
{
  Vertex center;
  double tanC = getTan( A - C, B - C);
  double tanA = getTan( C - A, B - A);
  double tanB = getTan( A - B, C - B);
  Vertex P = 0.5 * A + 0.5 * B;
  Vertex M = 0.5 * C + 0.5 * B;
  Vertex N = 0.5 * A + 0.5 * C;
  // printf("P [%f][%f][%f] M [%f][%f][%f] N [%f][%f][%f]\n",P.x(),P.y(),P.z(),M.x(),M.y(),M.z(),N.x(),N.y(),N.z());
  // printf("Delaunay tanA[%f]tanB[%f]tanC[%f]\n",tanA,tanB,tanC);
  return (P * tanC + M * tanA + N * tanB) * (1/(tanC+tanA+tanB));
  // return A * (0.5*tanC + 0.5*tanB) + B * (0.5 * tanC + 0.5 * tanA) + C * (0.5 * tanA + 0.5 * tanB);
}

bool isInCircle(Vertex A, Vertex B, Vertex C, Vertex D)
{
  Vertex center = getCircumCenter(A,B,C);
  return norm(center-D) <= norm(center-A);
}
