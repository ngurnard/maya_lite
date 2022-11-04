# pragma once
# include <glm/glm.hpp>
# include <halfedge.h>
# include <QListWidgetItem>
#include "joint.h"

// provide forward declarations to avoid circular dependency
class HalfEdge;

class Vertex : public QListWidgetItem {
private:
    glm::vec3 pos; // storing the position of this vertex
    HalfEdge *halfEdge; // a pointer that points to one of the half edges that points to this vertex
    const unsigned int id; // a unique integer to identify the vertex in the menus and while debugging
    static unsigned int count; // a counter to iterate across so that each ID is unique

    // Add member variables to your vertex class that store which joints influence its transformation
    // and by how much they influence it. We only require that you allow each vertex to be influenced
    // by two joints, but you may support more influences if you like.
    std::vector<std::pair<Joint*, float>> joint_influence; // what do I do with this for now? glm::length

public:

    // Constructors/Destructors
    Vertex(); // default constructor
    Vertex(glm::vec3 position);

    friend class Mesh;
    friend class VertexDisplay;
    friend class FaceDisplay;
    friend class HalfEdgeDisplay;
    friend class MyGL;
    friend class HalfEdge;
};
