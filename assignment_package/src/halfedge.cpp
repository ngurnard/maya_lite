# include <halfedge.h>

unsigned int HalfEdge::count = 0; // need to initialize the static variable count

HalfEdge::HalfEdge()
    : heNext(nullptr), heSym(nullptr), face(nullptr), vert(nullptr), id(count++)
{
    this->setText("HalfEdge: " + QString::number(id));
}

void HalfEdge::setFace(Face *f) {
    this->face = f;
    f->halfEdge = this;
}

void HalfEdge::setVertex(Vertex *v) {
    this->vert = v;
    v->halfEdge = this;
}
