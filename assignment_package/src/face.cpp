# include <face.h>

unsigned int Face::count = 0; // need to initialize the static variable count

Face::Face()
    : halfEdge(nullptr), color(), id(count)
{
    count++;
}

Face::Face(HalfEdge *he)
    : halfEdge(he), color(), id(count)
{
    count++;
}
