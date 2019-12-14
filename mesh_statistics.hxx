#include "mesh.h"

void Mesh::computeVoronoi()
{
    printf("Computing Voronoi...\n");
    delaunize();
    Iterator_on_vertices its;
    Circulator_on_faces cf;
    Circulator_on_faces cfbegin;
    Vertex tmpC, tmpV;
    voronoiCells.clear();
    QVector<Vertex> tmp;
    //For each vertex in a mesh
    for (its = v_begin(); its != v_pend(); ++its)
    {
        tmp.clear();

        cfbegin = incident_f(*its);
        cf = cfbegin;
        //Circulate around all the adjacent faces
        do
        {
            //add face's Circumcenter to the current Qvector<Vertex> reprensenting a cell
            tmpC = getCircumCenter(getVertex((*cf).getVertex(0)),
                                   getVertex((*cf).getVertex(1)),
                                   getVertex((*cf).getVertex(2)));

            if(!isInfinite(*cf))
                tmp.push_back(tmpC);
            cf++; 
        } while (cf != cfbegin);

        tmpC = getCircumCenter(getVertex((*cf).getVertex(0)),
                               getVertex((*cf).getVertex(1)),
                               getVertex((*cf).getVertex(2)));
        if(!isInfinite(*cf))
            tmp.push_back(tmpC);
        //Add all cells to the voronoiCells member variable
        voronoiCells.push_back(tmp);
        // printf("Added one voronoi cell\n");
    }
    // std::cout << "Voronoi computation ended..." << std::endl;
}

QVector<Vertex> Mesh::getVoronoiVertices()
{
    QVector<Vertex> v;
    for(int i = 0; i < voronoiCells.size(); i++) // For each cell
    {
        for(int j = 0; j < voronoiCells[i].size(); j++)//For each vertex in a cell
        {
            // TODO check if it's an infinit Vertex
            v.push_back(voronoiCells[i][j]);
        }
    }
    return v;
}
void Mesh::toggleVoronoi()
{
    drawVoronoi = !drawVoronoi;
}

void Mesh::computeLaplacian()
{
    //std::cout << "Computing Laplacian ===================================================================\n";

    Laplacien = QVector<Vector>(vertexTab.size());
    Iterator_on_vertices its;
    Circulator_on_faces cf;
    Circulator_on_faces cfbegin;
    int i = 0, axisGlID, axisLocID;
    float coAlpha, coBeta, area;
    Vector sum(0, 0, 0);
    Vertex axis, nextV, lastV;

    //For every vertex
    for (its = v_begin(); its != v_pend(); ++its)
    {
        coAlpha = 0;
        coBeta = 0;
        area = 0;
        sum = Vector(0, 0, 0);
        cfbegin = incident_f(*its);

        //Variables for lisibility to avoid redundancy
        axis = *its;
        axisGlID = getVertexID(axis);
        axisLocID = (*cfbegin).global2localIndex(axisGlID);
        nextV = getVertex((*cfbegin).getVertex((axisLocID + 1) % 3)); // Vertex next to axis in counter-clock wise order
        lastV = getVertex((*cfbegin).getVertex((axisLocID + 2) % 3)); // Vertex next to axis in clock wise order
        //Get the first face's alpha,
        //get first alpha
        coAlpha = getCot(nextV, axis, lastV);

        cfbegin++;
        //CIRCULATOR begins from the second face (Alpha of first face is known)
        for (cf = cfbegin, ++cf; cf != cfbegin; cf++)
        {
            //std::cout << "Circulating around axis : " << axisGlID << "\n";
            axisLocID = (*cf).global2localIndex(axisGlID);
            nextV = getVertex((*cf).getVertex((axisLocID + 1) % 3));
            lastV = getVertex((*cf).getVertex((axisLocID + 2) % 3));
            //get beta
            //std::cout << "Computing Beta...\n";
            coBeta = getCot(lastV, nextV, axis);
            //std::cout << "CoBeta = " << coBeta << "\n";

            //get area
            //std::cout << "Computing Area...\n";
            area += getArea(lastV, nextV, axis);
            //std::cout << "area = " << area << "\n";

            //Compute sigma
            sum.x += (coBeta + coAlpha) * (nextV.x() - axis.x());
            sum.y += (coBeta + coAlpha) * (nextV.y() - axis.y());
            sum.z += (coBeta + coAlpha) * (nextV.z() - axis.z());

            // std::cout << "nextV.x() = " << nextV.x() << "\t axis.x = " << axis.x() << "\n";
            // std::cout << "coBeta = " << coBeta << "\t coAlpha = " << coAlpha << "\n";
            // std::cout << "(coBeta + coAlpha) = " << (coBeta + coAlpha) << "\t(nextV.x() - axis.x()) = " <<  (nextV.x() - axis.x()) << "\n";
            // std::cout << "sum.x =" << sum.x << "\n";
            // std::cout << "sum.y = " << sum.y << "\n";
            // std::cout << "sum.z = " << sum.z << "\n";

            //get the alpha
            //std::cout << "Computing Alpha...\n";
            coAlpha = getCot(nextV, axis, lastV);
            //std::cout << "Circulated ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++on a face\n";
        }
        //std::cout << "setting Laplacian ["<< i <<"] with 1/Area = "<< 1/area << " And sum.x = "<< sum.x << std::endl;
        //std::cout << "Laplacien of x is : " << ((1 / (2 * area)) * sum.x) << "\n";
        Laplacien[i].x = (1 / (2 * area)) * sum.x;
        Laplacien[i].y = (1 / (2 * area)) * sum.y;
        Laplacien[i].z = (1 / (2 * area)) * sum.z;

        // std::cout << "Computed Laplacian for vertex ["<< i <<"] : \t ["<< Laplacien[i].x << "]["<< Laplacien[i].y <<"]["<< Laplacien[i].z <<"]" << std::endl;
        // std::cout << "2H = \t" << norm(Laplacien[i]) << std::endl;
        if (i % 1000 == 0)
        {
            std::cout << "Computed Laplacian for vertex [" << i << "] : \t [" << Laplacien[i].x << "][" << Laplacien[i].y << "][" << Laplacien[i].z << "]" << std::endl;
            std::cout << "2H = \t" << norm(Laplacien[i]) << std::endl;
        }
        i++;
    }
    clampLamplacian(50);
    minMaxLaplacian();
    laplacianDone = true;
}

void Mesh::minMaxLaplacian()
{
    maxValueLaplacien = Laplacien[0];
    minValueLaplacien = Laplacien[0];
    maxNormLaplacian = norm(Laplacien[0]);
    minNormLaplacian = norm(Laplacien[0]);
    double n;
    for (auto p : Laplacien)
    {
        if (p.x > maxValueLaplacien.x)
        {
            maxValueLaplacien.x = p.x;
        }
        if (p.y > maxValueLaplacien.y)
        {
            maxValueLaplacien.y = p.y;
        }
        if (p.z > maxValueLaplacien.z)
        {
            maxValueLaplacien.z = p.z;
        }
        if (p.x < minValueLaplacien.x)
        {
            minValueLaplacien.x = p.x;
        }
        if (p.y < minValueLaplacien.y)
        {
            minValueLaplacien.y = p.y;
        }
        if (p.z < minValueLaplacien.z)
        {
            minValueLaplacien.z = p.z;
        }
        n = norm(p);
        if (n > maxNormLaplacian)
        {
            maxNormLaplacian = n;
        }
        else if (n < minNormLaplacian)
        {
            minNormLaplacian = n;
        }
    }
}

double Mesh::getFaceArea(int FaceIndex)
{
    return getArea(getVertex(getFace(FaceIndex).getVertex(0)), getVertex(getFace(FaceIndex).getVertex(1)), getVertex(getFace(FaceIndex).getVertex(2)));
}

double Mesh::getCot(Vertex &v1, Vertex &v2, Vertex &v3)
{
    Vector vec1(v1, v2);
    Vector vec2(v1, v3);
    double sin = getSin(vec1, vec2);
    double cos = getCos(vec1, vec2);
    double clamp = 100;
    if (!sin)
        return clamp;
    double cot = cos / sin;
    if (cot > clamp)
        return clamp;
    if (cot < -clamp)
        return -clamp;
    return cot;
    //return (cos / sin) > 10000  ? 10000:(cos/sin);
}

void Mesh::clampLamplacian(int clamp)
{
    for (int i = 0; i < Laplacien.size(); i++)
    {
        if (Laplacien[i].x > clamp)
            Laplacien[i].x = clamp;
        if (Laplacien[i].y > clamp)
            Laplacien[i].y = clamp;
        if (Laplacien[i].z > clamp)
            Laplacien[i].z = clamp;

        if (Laplacien[i].x < -clamp)
            Laplacien[i].x = -clamp;
        if (Laplacien[i].y < -clamp)
            Laplacien[i].y = -clamp;
        if (Laplacien[i].z < -clamp)
            Laplacien[i].z = -clamp;
    }
}
