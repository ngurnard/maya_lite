# include <halfedge.h>

unsigned int HalfEdge::count = 0; // need to initialize the static variable count

HalfEdge::HalfEdge()
    : heNext(), heSym(), facePtr(), vertPtr(), id(count++)
{}

HalfEdge::HalfEdge(HalfEdge *next, HalfEdge *sym, Face *face, Vertex *vert)
    : heNext(next), heSym(sym), facePtr(face), vertPtr(vert), id(count++)
{}
