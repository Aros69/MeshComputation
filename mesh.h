#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <QMap>
#include <QtAlgorithms>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "utils.h"

class Iterator_on_vertices;
class Circulator_on_vertices;
class Iterator_on_faces;
class Circulator_on_faces;

#define infiniteP Point(0,0,INT_MIN)

class Mesh
{
private:
    QVector<Vertex>           vertexTab;
    QVector<DebugObj>         vertexDebugTab;
    QVector<Face>             faceTab;
    QVector<DebugObj>         faceDebugTab;
    QVector<QVector<Vertex>>  voronoiCells;                     // Buffer for drawing voronoi
    QVector<Vertex>           crustCurve;
    QVector<Vector>           Laplacien;
    bool                      laplacianDone = false;
    Vector                    colorA = Vector(0.75,0.75,0.75);
    Vector                    colorB = Vector(1,0.85,0);
    Vector                    maxValueLaplacien;
    Vector                    minValueLaplacien;
    double                    maxNormLaplacian;
    double                    minNormLaplacian;
    bool                      drawVoronoi = false;

public:
    Mesh();
    ~Mesh();
    /** Set the mesh
     * vertices of the mesh
     * faces of the mesh
     */
    void setMesh(QVector<Vertex>, QVector<Face>);

    /** 
     * Draw the mesh / Voronoi cells / Highlighted face
     */
    void drawMesh();

    /** 
     * Draw mesh's Wireframe
     */
    void drawMeshWireFrame();
    /**
     * Load mesh from file
     */ 
    void meshWithFile(std::string filePath);

    /**
     * "Sew" the neibghouring faces together
     */ 
    void defineNeighbourFaces();
    
    /**
     *  Remove the Infinite points
     */ 
    void cleanInfinitePoints();

    // Getters ==============================================================================
    Vertex& getVertex(int index) { return vertexTab[index]; }
    Face& getFace(int index) { return faceTab[index]; }
    int getFaceIndex(int vertexes[3]) const;
    int getVertexID(const Vertex &m);

    // Iterators ============================================================================
    /**
     * Get an iterator pointing on the first face in the mesh's face Array
     */ 
    Iterator_on_faces     f_begin();
    /**
     * Get an iterator pointing past the face array last index
     */ 
    Iterator_on_faces     f_pend();
    /**
     * Get an iterator pointing on the first face in the mesh's vertex Array
     */ 
    Iterator_on_vertices  v_begin();
    /**
     * Get an iterator pointing past the face array last index
     */ 
    Iterator_on_vertices  v_pend();

    // Circulators ===========================================================================
    /**
     * Circulator pointing on an incident face to v
     * v :  vertex around which we circulate
     */ 
    Circulator_on_faces     incident_f(Vertex &v);

    /**
     * Circulator pointing on a vertex adjacent to v
     * v :  vertex around which we circulate
     */ 
    Circulator_on_vertices  adjacent_v(Vertex &v);

    // Mesh statistics methods ================================================================

    /**
     * Compute the voronoiCells array
     */ 
    void    computeVoronoi();

    /**
     * Return
     */
    QVector<Vertex>    getVoronoiVertices();
    /**
     * Compute the laplacian array
     */ 
    void    computeLaplacian();

    /**
     * Normalize the laplacian array between Min and Max
     */
    void    minMaxLaplacian();

    /**
     * Clamp laplacian
     * clamp : clamping value
     */
    void    clampLamplacian(int clamp);

    /**
     * get area of the face of index "index"
     * index : Index of the face to compute the area for
     */ 
    double  getFaceArea(int index);

    /**
     * Compute the cotangent from 3 vertices v1, v2, v3 centered around v1 (Get the cotangent of the angle v2 v1 v3)
     */
    double  getCot(Vertex& v1,Vertex& v2, Vertex& v3);
    
    /**
     * Tests if the vertex v is in the face with index "index"
     * index  :   Index of the face
     * v      :   vertex to test for 
     */ 
    bool    isInFace(int index,const Vertex& v);

    /**
     * True if the triangle is locally of Delaunay
     * index  : Index of the face to test
     * debug  : Highlight the face if it's not of delaunay
     * badFace: Will get the value of the neighboring face for which the test fails, if there is any
     */ 
    bool    isLocallyOfDelaunay(int index,bool debug, int& badFace);

    /**
     * True if the face contains an infinite vertex
     */
    bool    isInfinite(int index);

    /**
     * True if the face contains an infinite vertex
     */
    bool    isInfinite(Face f);
    /**
     * Call this function to toggle ON/OFF the voronoi visualization
     */ 
    void    toggleVoronoi();

    // Mesh Modification methods ================================================================
    /**
     * Flip two NEIGHBORING triangles together
     * index1 : Index of the first triangle
     * index2 : Index of the second triangle
     */
    void flip(int index1, int index2);

    /**
     * Split a triangle by adding a new point
     * faceIndex  : Face's index to split
     * newV       : Point to insert
     */
    void triangleSplit(int faceIndex, Point newV);

    /**
     * Naive insertion of a point
     * newV : Point to insert
     */
    void naiveInsertion(Point newV);

    /**
     * Transform the mesh into a Delaunay Mesh
     */
    void delaunize();
    
    /**
     * Delaunay insertion of a Vertex v
     * v  : Vertex to insert
     */
    void delaunayInsert(Vertex v);

    /**
     * Make the mesh convex starting from a vertex
     * axisVertex       : The vertex from which we start exploring
     * infiniteTriangle : Index of the infinite triangle
     */
    void convexize(int axisVertex, int infiniteTriangle);

    /**
      * Merge 2 vertices to simplify the mesh
      * vertexId1   : Vertex 1 id
      * vertexId1   : Vertex 2 id
      */
    void mergeVertices(int vertexId1, int vertexId2);

    /**
     * Simplify the mesh
     * nbOfVerticesWanted : Number of vertices we want at the end of the simplication
     */
    void simplify(int nbOfVerticesWanted);
    
    // Crust methods
    /**
     * Crust algorithm for mesh generation from points cloud
     */
    void crust2D();

    // Edge Collapse
    void edgeCollapse(int indexFace, int relativeOppositeIndex);

    //Debugging Methods ===================================================================
    /**
     * Update the debug objects that are used to highlight specific faces / vertices
     * To be called each time there is a major a change to the mesh
     */
    void updateDebugObj();

    /**
     * Print the different faces of a mesh
     */
    void printFaces();

    /**
     * Mark a specific face, the default configuration will show the face in MAGENTA
     * index  :  Index of the face to highlight
     */
    void markFace(int index);

    /**
     * Mark a specific vertex.
     * index  :  Index of the vertex to highlight.
     */
    void markVertex(int index);

    /**
     * Unmark all the faces and vertices
     */
    void unMarkAll();
    //========================================================================================================================================================
    
    /**
     * Randomly flip two neighboring triangles
     */
    void randomFlip(){
      int index1 = rand()%faceTab.size();
      int index2 = getFace(index1).getNeibFace(rand()%3);
      std::cout<<"size : "<<faceTab.size()<<" Index 1 : " <<index1<<" Index 2 : "<<index2<<std::endl;
      // TODO
      flip(9,10);
    }
    /**
     * Randomly hightlight a face
     */
    void randomFHighlight(){
      markFace(rand()%faceTab.size());
    }
};

#include "iterators.h"
#include "segmentMemory.h"
#endif // MESH_H
