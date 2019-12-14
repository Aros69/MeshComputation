#include "mesh.h"

bool Mesh::isInFace(int index, const Vertex &v)
{
    int segment[2];
    Face f = getFace(index);
    //For all segments
    for (int i = 0; i < 3; i++)
    {
        segment[0] = i;
        segment[1] = (i + 1) % 3;
        //If v and the segment are incorrectly ordered
        if (orientation(v, getVertex(f.getVertex(segment[0])), getVertex(f.getVertex(segment[1]))) < 0)
            return false;
    }
    return true;
}

bool Mesh::isLocallyOfDelaunay(int index, bool debug, int& badFace)
{
    //std::cout << "\n\n\n\n\nCheck delaunay for face n " << index << std::endl;
    Face f = getFace(index);
    Vertex D;
    Vertex A = getVertex(f.getVertex(0));
    Vertex B = getVertex(f.getVertex(1));
    Vertex C = getVertex(f.getVertex(2));

    //Circulate on all neigbhor faces
    for (int i = 0; i < 3; i++)
    {
        //Get the non adjacent vertex
        //std::cout << "Checking with face number " << f.getNeibFace(i) << std::endl;
        int tmpIndex = (getFace(f.getNeibFace(i))).global2localIndexF(index);
        //std::cout << "Checking with vertex " << tmpIndex << std::endl;
        // printFaces();
        D = getVertex( getFace(f.getNeibFace(i)).getVertex(tmpIndex) );
        if (isInCircle(A, B, C, D))
        {
            //std::cout << "Not delaunay with face n : " <<  f.getNeibFace(i) << std::endl;
            if (debug)
                markFace(f.getNeibFace(i));
            badFace = f.getNeibFace(i);
            return false;
        }
    }
    //std::cout << "Is delaunay, nothing to see here" << std::endl;
    //All the vertices are outside the circle so the triangle is locally of Delaunay
    return true;
}

bool Mesh::isInfinite(int index)
{
    Face f = getFace(index);
    for(int i = 0;i < 3 ; i++)
    {
        if( (getVertex(f.getVertex(i))) == infiniteP )
            return true;
    }
    return false;
}

bool Mesh::isInfinite(Face f)
{
    for(int i = 0;i < 3 ; i++)
    {
        if( (getVertex(f.getVertex(i))) == infiniteP )
            return true;
    }
    return false;
}