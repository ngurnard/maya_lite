# include <halfedge.h>

unsigned int HalfEdge::count = 0; // need to initialize the static variable count

HalfEdge::HalfEdge()
    : heNext(nullptr), heSym(nullptr), face(nullptr), vert(nullptr), id(count)
{
    count++;
}

//HalfEdge::HalfEdge(HalfEdge *next, HalfEdge *sym, Face *face, Vertex *vert)
//    : heNext(next), heSym(sym), face(face), vert(vert), id(count)
//{
//    count++;
//}
