# pragma once
# include <glm/glm.hpp>
# include <vertex.h>
# include <face.h>
# include <QListWidgetItem>

// provide forward declarations to avoid circular dependency
class Face;
class Vertex;

class HalfEdge : public QListWidgetItem {
private:
    HalfEdge *heNext; // at ptr to the next HalfEdge in the loop of HalfEdges that lie on this HalfEdges face
    HalfEdge *heSym; // a ptr to the parallel half edge on an adjacent face (symmetrical half edge)
    Face *face; // a ptr to the face in which this half edge lies
    Vertex *vert; // a ptr to the vertex between this half edge and its next half edge
    const unsigned int id; // a unique integer to the identity of the half edge in the menus and while debugging
    static unsigned int count; // a counter to iterate across so that each ID is unique

public:

    // Constructors/Destructors
    HalfEdge(); // default constructor

    friend class Mesh;
    friend class HalfEdgeDisplay;
    friend class FaceDisplay;
    friend class MyGL;
};

