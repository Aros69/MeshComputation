#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cmath>
#include <QVector>

class Point
{
    double _x;
    double _y;
    double _z;

public:
    Point() : _x(), _y(), _z() {}
    Point(double x_, double y_, double z_) : _x(x_), _y(y_), _z(z_) {}
    // get
    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }
};

class Vertex
{
    Point point;
    int faceIndex;

public:
    Vertex() : point() {}
    Vertex(Point p) : point(p.x(), p.y(), p.z()){}
    Vertex(double x_, double y_, double z_) : point(x_, y_, z_) {}
    double x() const { return point.x(); }
    double y() const { return point.y(); }
    double z() const { return point.z(); }
    void setFaceIndex(int i) { faceIndex = i; }
    int getFaceIndex() { return faceIndex; }
    const Point & getPoint(){return point;}
    bool equals(const Vertex& m) const
    {
        if (m.x() == this->x() && m.y() == this->y() && m.z() == this->z())
        {
            return true;
        }
        return false;
    }

    bool operator==(const Point &p){
        return this->x()==p.x() && this->y()==p.y() && this->z()==p.z();
    }
    bool operator!=(const Point &p){
        return !(*this==p);
    }
};

struct Vector
{
    double x;
    double y;
    double z;
    Vector(): x(0), y(0), z(0) {}
    Vector(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

    //Init a vector going FROM p1 TO p2
    Vector(const Point& p1,const Point& p2) : x(p2.x() - p1.x()), y(p2.y() - p1.y()), z(p2.z() - p1.z()){}
    //Init a vector going FROM v1 TO v2
    Vector(const Vertex& v1,const Vertex& v2) : x(v2.x() - v1.x()), y(v2.y() - v1.y()), z(v2.z() - v1.z()){}

};
Vertex operator*(float f, const Vertex& v);
Vertex operator*(const Vertex& v, float f);
Vertex operator+(const Vertex& v1,const Vertex& v2);
Vertex operator+(const Vertex& v1,const Vector& vec);
Vertex operator+(const Vector& vec,const Vertex& v1);
Vector operator-(const Vertex& v1,const Vertex& v2);
struct DebugObj
{
  Vector debugColor;
  bool debug;
  DebugObj():debugColor(1.0,0.0,1.0),debug(false){}

};
class Face
{

    int verticesIndex[3];
    int neibFace[3];

public:
    Face() : Face(0, 0, 0) {}
    Face(int a, int b, int c)
    {
        verticesIndex[0] = a;
        verticesIndex[1] = b;
        verticesIndex[2] = c;
        neibFace[0] = -1;
        neibFace[1] = -1;
        neibFace[2] = -1;
    }

    //Setters ==============
    //Set all near face's index
    void setNeibFace(int neib1, int neib2, int neib3);
    //Set a specific near Face
    void setNeibFace(int neib, int index);
    //Set all vertices's index
    void setVertices(int v1,int v2,int v3);
    //Set specific vertex
    void setVertex(int index,int v);


    //Getters ==============
    //Get the near face with index
    int getNeibFace(int index) const { return neibFace[index]; }

    //Convert global Index to local Index
    int global2localIndex(int globalIndex);
    int global2localIndexF(int globalIndex);
    //Get vertex with index
    int getVertex(int index) const{return verticesIndex[index];}

    int* getVertexes(){return verticesIndex;}

    //Return the local index of the different vertex
    int getDifferentVertex(Face f){
      bool found = false;
      for(int i = 0;i < 3;i++){
          for (int j = 0; j < 3; j++)
          {
              // if(getVertex(i) == f.getVertex(j) && found == true){
              //   std::cout << "The triangles have more than one different vertex\n";
              //   return -1;
              // }
              if(getVertex(i) == f.getVertex(j)){
                found = true; // fA[i] is a common vertex
              }
          }
          if(!found){
            return i;
          }
          found = false;
      }
      std::cout << "No uncommon vertex..." << std::endl;
      return -1;
    }
    //Print the face's infos
    void print(int ID);

    int &operator[](int x){return verticesIndex[x];}
};

/*void printPoint(Point p){
    std::cout<<"("<<p.x()<<", "<<p.y()<<", "<<p.z()<<")"<<std::endl;
}*/
void   printFacesNeib(const QVector<Face> & f);

double  distance(const Point &p1, const Point &p2);
double  dot(const Vector& v1,const Vector& v2);
Vector  cross(const Vector& v1,const Vector& v2);
double  norm(const Vector& v);
Vector  normalize(const Vector& v);
double  getCos(const Vector& v1,const Vector& v2);
double  getSin(const Vector& v1,const Vector& v2);
//Return a positive value if v1,v2,v3 are in a trigonometric order
double  orientation(const Vertex& v1,const Vertex& v2,const Vertex& v3);
double  getArea(Vertex &vert1, Vertex &vert2, Vertex &vert3);
Vertex  getCircumCenter(Vertex A, Vertex B, Vertex C);
bool    isInCircle(Vertex A, Vertex B, Vertex C, Vertex D);
#endif
