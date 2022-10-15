# pragma once
# include <glm/glm.hpp>
# include <halfedge.h>

// provide forward declarations to avoid circular dependency
class HalfEdge;

class Vertex {
private:
    glm::vec3 pos; // storing the position of this vertex
    HalfEdge *hePtr; // a pointer that points to one of the half edges that points to this vertex
    const unsigned int id; // a unique integer to identify the vertex in the menus and while debugging
    static unsigned int count; // a counter to iterate across so that each ID is unique

public:

    // Constructors/Destructors
    Vertex(); // default constructor
    Vertex(glm::vec3 position);
    Vertex(glm::vec3 position, HalfEdge *he);

    friend class Mesh;
};
