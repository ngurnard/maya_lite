# pragma once
# include <drawable.h>
# include <smartpointerhelp.h>
# include <face.h>
# include <vertex.h>
# include <halfedge.h>

class Mesh : public Drawable {
private:
    std::vector<uPtr<Face>> faces; // stores the faces
    std::vector<uPtr<Vertex>> verts; // stores the vertices
    std::vector<uPtr<HalfEdge>> hes; // stores the halfedges

public:

    // Constructor/Destructor

    // Implement virtual function create()


};
