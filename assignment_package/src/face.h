# pragma once
# include <glm/glm.hpp>
# include <halfedge.h>

// provide a forward declarations to avoid circular dependencies
class HalfEdge;

class Face {
private:
    HalfEdge *hePtr; // a pointer that points to one of the half edges that lies on this face
    glm::vec3 color; // RGB color vector for this face
    const unsigned int id; // a unique integer to ideify the face in the menus while debugging
    static unsigned int count; // a counter to iterate across so that each ID is unique

public:

    // Constructors/Destructors
    Face(); // default constructor
    Face(HalfEdge *he);

    friend class Mesh;
};
