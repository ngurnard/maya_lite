# include <face.h>

unsigned int Face::count = 0; // need to initialize the static variable count

Face::Face()
    : hePtr(), color({0,0,0}), id(count++) // does this increment count?
{}

Face::Face(HalfEdge *he)
    : hePtr(he), color({0,0,0}), id(count++)
{}
