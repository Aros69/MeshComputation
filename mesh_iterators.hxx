#include "mesh.h"

Iterator_on_faces Mesh::f_begin() { return Iterator_on_faces(0, this); }
Iterator_on_faces Mesh::f_pend() { return Iterator_on_faces(faceTab.size(), this); }
Iterator_on_vertices Mesh::v_begin() { return Iterator_on_vertices(0, this); }
Iterator_on_vertices Mesh::v_pend() { return Iterator_on_vertices(vertexTab.size(), this); }
Circulator_on_faces Mesh::incident_f(Vertex &v) { return Circulator_on_faces(this->getVertexID(v), this); }
Circulator_on_vertices Mesh::adjacent_v(Vertex &v) { return Circulator_on_vertices(getVertexID(v), this); }

bool Mesh::isStable(){
    bool somethingNotOK = false;
    Iterator_on_faces faceIte = f_begin();
    int circulatorIterationLimit = 50;
    do {
        for(int i=0;i<3;++i){
            int circulatorIterationCount = 0;
            //fprintf(stdout, "On circule en partant de la face %d sur le vertex %d\n", faceIte.getIndex(), (*faceIte).getVertex(i));
            Circulator_on_faces faceCircu(faceIte.getIndex(), (*faceIte).getVertex(i), this);
            do {
                ++circulatorIterationCount;
                ++faceCircu;
            } while(faceCircu.getCurrentFaceIndex()!=faceIte.getIndex() && circulatorIterationCount < circulatorIterationLimit);
            if(circulatorIterationCount==circulatorIterationLimit){
                somethingNotOK=true;
                fprintf(stderr, "Probleme d'iteration en partant de la face %d, pour le vertex %d. On est bloqué sur la face %d\n",
                        faceIte.getIndex(), (*faceIte).getVertex(i), faceCircu.getCurrentFaceIndex());
            }
        }
        ++faceIte;
    } while(faceIte!=f_pend());


    return !somethingNotOK;
}
