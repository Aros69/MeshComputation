#include "mesh.h"

void Mesh::flip(int index1, int index2)
{
    std::cout << "Flipping Face " << index1 << "\t and Face " << index2 << std::endl;
    // markFace(index1);
    // markFace(index2);

    // Code commented to allow flip in non convex hull (convexize function)
    /*if(isInfinite(index1) || isInfinite(index2))
        return;*/

    Face fA = getFace(index1);
    Face fB = getFace(index2);
    int vA = fA.getDifferentVertex(fB); // The index of the opposite vertex on fA (-1 if disjointed triangles)
    int vB = fB.getDifferentVertex(fA); // The index of the opposite vertex on fB (-1 if disjointed triangles)
    int fA1ID = fA.getNeibFace((vA + 1) % 3);
    int fB1ID = fB.getNeibFace((vB + 1) % 3);
    Face fA1 = getFace(fA1ID); // The face near fA
    Face fB1 = getFace(fB1ID); // The face near fB

    if (vA == -1 || vB == -1 || fA1ID == -1 || fB1ID == -1 || fA1.getDifferentVertex(fA) == -1 || fB1.getDifferentVertex(fB) == -1)
    {
        std::cout << "Bad indexes \n";
        return;
    }
    std::cout << "Data : fA1 = " << fA.getNeibFace((vA + 1) % 3) << "\nfB1 = " << fB.getNeibFace((vB + 1) % 3) << std::endl;

    //Set incident faces for triangles
    vertexTab[fA.getVertex((vA + 2) % 3)].setFaceIndex(fA1ID);
    vertexTab[fB.getVertex((vB + 2) % 3)].setFaceIndex(fB1ID);

    //Set faces for neibghor's faces first
    std::cout << "Different vertex A: " << fA1.getDifferentVertex(fA) << std::endl;
    std::cout << "Different vertex B: " << fB1.getDifferentVertex(fB) << std::endl;
    fA1.setNeibFace(index2, fA1.getDifferentVertex(fA));
    fB1.setNeibFace(index1, fB1.getDifferentVertex(fB));

    //Set faces for fA / fB
    fA.setNeibFace(fB.getNeibFace((vB + 1) % 3), vA);
    fB.setNeibFace(fA.getNeibFace((vA + 1) % 3), vB);
    fA.setNeibFace(index2, (vA + 1) % 3);
    fB.setNeibFace(index1, (vB + 1) % 3);

    //Set vertices
    fA.setVertex((vA + 2) % 3, fB.getVertex(vB));
    fB.setVertex((vB + 2) % 3, fA.getVertex(vA));

    faceTab[index1] = fA;
    faceTab[index2] = fB;
    faceTab[fA1ID] = fA1;
    faceTab[fB1ID] = fB1;
}

void Mesh::triangleSplit(int faceIndex, Point newV)
{
    //std::cout<<"Begining of triangleSplit"<<std::endl;

    Vertex v(newV.x(), newV.y(), newV.z());
    //if(isInFace(faceIndex, v)){
    // Définition du nouveau Vertex
    v.setFaceIndex(faceIndex);
    vertexTab.push_back(v);
    // Fin Définition

    // Création des deux nouvelles faces

    // Face 1
    Face newFace1;
    if (orientation(v, vertexTab[faceTab[faceIndex][0]], vertexTab[faceTab[faceIndex][1]]) > 0)
    {
        newFace1 = Face(vertexTab.size() - 1, faceTab[faceIndex][0], faceTab[faceIndex][1]);
        newFace1.setNeibFace(faceTab[faceIndex].getNeibFace(2), faceIndex, faceTab.size() + 1);
    }
    else
    {
        newFace1 = Face(vertexTab.size() - 1, faceTab[faceIndex][1], faceTab[faceIndex][0]);
        newFace1.setNeibFace(faceTab[faceIndex].getNeibFace(2), faceTab.size() + 1, faceIndex);
    }
    faceTab.push_back(newFace1);
    vertexTab[faceTab[faceIndex][0]].setFaceIndex(faceTab.size());
    vertexTab[faceTab[faceIndex][1]].setFaceIndex(faceTab.size());
    // Fin Face 1

    // Face 2
    Face newFace2;
    if (orientation(v, vertexTab[faceTab[faceIndex][0]], vertexTab[faceTab[faceIndex][2]]) > 0)
    {
        newFace2 = Face(vertexTab.size() - 1, faceTab[faceIndex][0], faceTab[faceIndex][2]);
        newFace2.setNeibFace(faceTab[faceIndex].getNeibFace(1), faceIndex, faceTab.size() - 1);
    }
    else
    {
        newFace2 = Face(vertexTab.size() - 1, faceTab[faceIndex][2], faceTab[faceIndex][0]);
        newFace2.setNeibFace(faceTab[faceIndex].getNeibFace(1), faceTab.size() - 1, faceIndex);
    }
    faceTab.push_back(newFace2);
    vertexTab[faceTab[faceIndex][0]].setFaceIndex(faceTab.size());
    vertexTab[faceTab[faceIndex][2]].setFaceIndex(faceTab.size());
    // Fin Face 2
    // Fin création des deux nouvelles faces

    // Mise à jour de l'ancienne face
    //Face oldFace = faceTab[faceIndex];
    faceTab[faceIndex][0] = vertexTab.size() - 1;
    faceTab[faceIndex].setNeibFace(faceTab[faceIndex].getNeibFace(0), faceTab.size() - 1, faceTab.size() - 2);
    // Fin mise à jour de l'ancienne face

    // Mise à jour des face voisine
    defineNeighbourFaces();

    // Mise à jour du Laplacien
    // TODO !!
    // Fin mise à jour du Laplacien

    //Mise à jour des debugObj
    updateDebugObj();
    //}
}

// Possible amélioration : Au lieu de cherche à minimiser la distance pour le nouveau triangle,
// chercher les points permettant de former le triangle le plus equilateral possible (test sur les angles ?)
void Mesh::naiveInsertion(Point newV){
    //std::cout<<"Debut Insertion Naive"<<std::endl;
    Vertex v(newV.x(), newV.y(), newV.z());
    // Check if NewV is in mesh
    bool isInMesh = false;
    int indexFace = -1;
    int i = 0;
    do
    {
        if ( isInFace(i, v) )
        {
            isInMesh  = true;
            indexFace = i;
        }
        ++i;
    } while (!isInMesh && i < faceTab.size());
    // If it is it's like a split
    // Else... more complex
    if(isInMesh){
        triangleSplit(indexFace, newV);
    } else {
        // Chercher le vertex sur le bord de l'enveloppe du maillage le plus proche
            //1) Chercher le point infinis
       int indexInfinitePoint=-1;
       for(int i=0; i<vertexTab.size();++i){
           if (vertexTab[i]==infiniteP){
            indexInfinitePoint = i;
           }
       }
       if(indexInfinitePoint==-1){
            exit(-1);
        }
        else
        {
            //2) Creer circulateur sur le point infinis
                //2.1) Pour toutes les faces, calculer la distance vers le nouveau point (saut pour le point infinis
            Circulator_on_faces cf;
            Circulator_on_faces cfbegin = incident_f(*Iterator_on_vertices(indexInfinitePoint, this));
            int bestFaceIndex = 0;
            double minDeltaDistance = INT_MAX;
            double d1, d2;
            cf = cfbegin;
            do{
                std::cout << "Checking for one face" << std::endl;
                d1=0;
                d2=0;
                for(int i=0;i<3;++i){
                    if(vertexTab[(*cf).getVertex(i)]!=infiniteP){
                        if(d1==0.){
                            d1 = norm(newV - vertexTab[(*cf).getVertex(i)]);
                        } else if(d2==0.){
                            d2 = norm(newV - vertexTab[(*cf).getVertex(i)]);
                        }
                    }
                }
                //2.2) Garder la face formant le triangle plus plus isocèle possible
                if(abs(d1)+abs(d2)<minDeltaDistance){
                    minDeltaDistance = abs(d1)+abs(d2);
                    bestFaceIndex = getFaceIndex((*cf).getVertexes());
                }
                cf++;
            }while(cf != cfbegin);

        // Idée 1 pour rajouter le nouveau triangle dans le maillage
        // Simplement ajouter le vertex et la face dans leur tableau
        vertexTab.push_back(v);

        // Création de la nouvelle face
            // Selection des deux bon vertex (pas le point infinis)
        std::pair<int, int> goodVertex(0,0);
        for(int i=0;i<3;++i){
            if(vertexTab[(*cf).getVertex(i)]!=infiniteP){
                if(goodVertex.first==0){
                    goodVertex.first=i;
                } else if(goodVertex.second==0){
                    goodVertex.second=i;
                }
            }
        }
        
        Face newFace;

        if (orientation(v, vertexTab[faceTab[bestFaceIndex][goodVertex.first]], vertexTab[faceTab[bestFaceIndex][goodVertex.second]]) > 0)
        {
            newFace = Face(vertexTab.size() - 2, faceTab[bestFaceIndex][goodVertex.first], faceTab[bestFaceIndex][goodVertex.second]);
        }
        else
        {
            newFace = Face(vertexTab.size() - 2, faceTab[bestFaceIndex][goodVertex.second], faceTab[bestFaceIndex][goodVertex.first]);
        }
        
        cleanInfinitePoints();
        faceTab.push_back(newFace);
        vertexTab[vertexTab.size()-1].setFaceIndex(faceTab.size()-1);
        int indexNewFace = faceTab.size()-1;
        // Fin Nouvelle Face
        // Mettre à jour voisin
        std::cout << "Just before defining Neibghours " << std::endl;
        defineNeighbourFaces();

        convexize(faceTab[indexNewFace].getVertex(2), faceTab[indexNewFace].getNeibFace(1));
        convexize(faceTab[indexNewFace].getVertex(1), faceTab[indexNewFace].getNeibFace(2));
        
        defineNeighbourFaces();
       }
    }
    std::cout<<"Fin Insertion Naive"<<std::endl;
}

void Mesh::delaunize()
{
    int badFaceID = 0;
    //  For all triangles
    for (int i = 0 ; i < faceTab.size() ; i++)
    {
        //  Check if not deDelaunay
        if(!isLocallyOfDelaunay(i,false,badFaceID) && !isInfinite(i) && !isInfinite(badFaceID))
        {
            // Flip with bad face
            std::cout << "Flipping the faces ("<< i <<","<< badFaceID<<")" << std::endl;
            flip(i,badFaceID);
            i--;
        }
    }
}

void Mesh::delaunayInsert(Vertex v)
{
    printf("Delaunay insertion of vertex [%f][%f][%f]\n", v.x(), v.y(), v.z());
    naiveInsertion(v.getPoint());
    delaunize();
    //  Naive insertion of a point
    //  Stack of all impacted edges
    //  For each impacted edge
        // Check if triangles of the edge are locally of delaunay
            //Flip if not
            //Add newly impacted edge to stack
}

void Mesh::convexize(int axisVertex, int infiniteTriangle)
{
    Circulator_on_faces cfbegin = incident_f(getVertex(axisVertex));
    Circulator_on_faces cf = cfbegin;

    do{
        if(isInfinite(getFaceIndex((*cf).getVertexes()))
                && getFaceIndex((*cf).getVertexes()) != infiniteTriangle)
        {
            double ori1 = orientation(Vertex(infiniteP), getVertex(axisVertex),
                                      getVertex((*cf).getVertex((*cf).getDifferentVertex(getFace(infiniteTriangle)))));
            double ori2 = orientation(Vertex(infiniteP), getVertex(axisVertex),
                                      getVertex(getFace(infiniteTriangle).getVertex(getFace(infiniteTriangle).getDifferentVertex((*cf)))));
            if ((ori1 >0 && ori2 <0)
                    || (ori1<0 && ori2>0)){
                flip(infiniteTriangle, getFaceIndex((*cf).getVertexes()));
                convexize((*cf).getVertex((*cf).getDifferentVertex(getFace(infiniteTriangle))), getFaceIndex((*cf).getVertexes()));
            };
        }
        cf++;
    } while (cfbegin != cf);
}

void Mesh::mergeVertices(int vertexId1, int vertexId2, int faceId1, int faceId2){
    if(vertexId1==-1 || vertexId2==-1|| faceId1==-1 || faceId2==-1){
        fprintf(stderr, "Erreur dans les parametre de la fonction mergeVertices :" "vertexId1 = %d, vertxid2 = %d, faceId1 =%d, faceId2 = %d\n",
                vertexId1, vertexId2, faceId1, faceId2);
        exit(-1);
    }
}

void Mesh::simplify(int nbOfVerticesWanted)
{
    std::cout << "Simplifying the mesh" << std::endl;
    int vertexId1, vertexId2, faceId1, faceId2;
    // TODO remove the varibale bellow and the second bellow it
    int tempAgainstWarning = nbOfVerticesWanted+1;
    tempAgainstWarning++;
    //while(vertexTab.size()>nbOfVerticesWanted){
        // iterate on all faces
        SegmentMapSimplify segmentMap;
        for(auto face : faceTab){
            // Pour chacun des segments
            for(int i=0;i<3;++i){
                vertexId1 = face.getVertex(i);
                vertexId2 = face.getVertex((i+1)%3);
                faceId1 = getFaceIndex(face.getVertexes());
                faceId2 = face.getNeibFace((i+2)%3);
                double dist = distance(vertexTab[vertexId1].getPoint(), vertexTab[vertexId2].getPoint());
                if(!segmentMap.hashMap.contains(SegmentMapKey(vertexId1, vertexId2))){
                    // On recupère tout les voisin du vertex 1
                    std::vector<int> neighborIndexes(0);
                    Circulator_on_faces circulatorVertex1(faceId1, vertexId1, this);
                    do{

                        //std::cout<<(*circulatorVertex1).getFaceIndex()<<std::endl;
                        //std::cout<<faceTab.size()<<std::endl;
                        neighborIndexes.push_back(faceTab[circulatorVertex1.getCurrentFaceIndex()].getVertex((i+1)%3));
                        ++circulatorVertex1;
                    } while(circulatorVertex1.getCurrentFaceIndex()!=faceId1);
                    // On compare avec les voisins du second vertex (si on croise un meme voisin on arrete l'ajout de ce segment)
                    Circulator_on_faces circulatorVertex2(faceId2, vertexId2, this);
                    bool hasOneCommonNeighbor = false;
                    std::cout<<"Taille du voisinage : "<<neighborIndexes.size()<<std::endl;
                    do{
                        int neighbor = faceTab[circulatorVertex2.getCurrentFaceIndex()].getVertex((i+1)%3);
                        for(int neib : neighborIndexes){
                            printf("On compare %d et %d\n", neighbor, neib);
                            fflush(stdout);
                            if(neighbor == neib){
                                std::cout<<"Yo dog"<<std::endl;
                                hasOneCommonNeighbor=true;
                            }
                        }
                        ++circulatorVertex2;
                    } while(!hasOneCommonNeighbor && circulatorVertex2.getCurrentFaceIndex()!=faceId1);

                    // Pas de voisin commun, on ajoute a la map
                    if(hasOneCommonNeighbor){
                        std::cout<<"On insert"<<std::endl;
                        segmentMap.hashMap.insert(SegmentMapKey(vertexId1, vertexId2),
                                                  SegmentMapSimplify::SegmentMapSimplifyData(dist, faceId1, faceId2));
                    }
                }
            }
        }

        std::cout<<"Hello "<< segmentMap.hashMap.size()<<std::endl;
    // Iterate on the map and collapse edges
        faceId1 = -1;
        faceId2 = -1;
        vertexId1 = -1;
        vertexId2 = -1;
        double minDist = segmentMap.hashMap.first().dist;
        QList<SegmentMapKey> keys = segmentMap.hashMap.keys();
        int i=0;
        for(auto dataSegment : segmentMap.hashMap){
            if(dataSegment.dist<=minDist){
                minDist = dataSegment.dist;
                faceId1 = dataSegment.faceId1;
                faceId2 = dataSegment.faceId2;
                vertexId1 = keys[i].vertexIndex1;
                vertexId2 = keys[i].vertexIndex2;
            }    
            i++;
        }
        //segmentMap.print();
        mergeVertices(vertexId1, vertexId2, faceId1, faceId2);
    //}
    std::cout<<"End of simplify"<<std::endl;
}

void Mesh::crust2D(){
    // Help : cf. cours 7 slides 42
    
    printf("Crust2D computing...");
    computeVoronoi();
    QVector<Vertex> cells = getVoronoiVertices();
    crustCurve = QVector<Vertex>();
    // Inserer tout les centres de voronoi
    for(int i = 0; i < cells.size(); i++)
    {
        // naiveInsertion(cells[i].getPoint());
        delaunayInsert(cells[i]); 
        vertexTab[vertexTab.size()-1].setFlag(true);
    }
    // Pour tout les faces, prendre les aretes qui n'ont pas un vertex marqué
    for(int i = 0; i < faceTab.size(); i++)
    {
        for( int v = 0; v < 3;v++)
        {
            if(!vertexTab[(faceTab[i].getVertex(v))].getFlag())
                crustCurve.push_back(vertexTab[faceTab[i].getVertex(v)]);
        }
    }

    // Garder les edges entre deux sommets de bases

}
