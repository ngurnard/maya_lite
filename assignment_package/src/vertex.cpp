# include <vertex.h>

unsigned int Vertex::count = 0; // need to initialize the static variable count

Vertex::Vertex()
    : pos(), hePtr(), id(count++) // does this update count??
{}

Vertex::Vertex(glm::vec3 position, HalfEdge *he)
    : pos(position), hePtr(he), id(count++)
{}
