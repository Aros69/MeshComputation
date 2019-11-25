#include "mesh.h"

Iterator_on_faces Mesh::f_begin() { return Iterator_on_faces(0, this); }
Iterator_on_faces Mesh::f_pend() { return Iterator_on_faces(faceTab.size(), this); }
Iterator_on_vertices Mesh::v_begin() { return Iterator_on_vertices(0, this); }
Iterator_on_vertices Mesh::v_pend() { return Iterator_on_vertices(vertexTab.size(), this); }
Circulator_on_faces Mesh::incident_f(Vertex &v) { return Circulator_on_faces(this->getVertexID(v), this); }
Circulator_on_vertices Mesh::adjacent_v(Vertex &v) { return Circulator_on_vertices(getVertexID(v), this); }