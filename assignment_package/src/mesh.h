# pragma once
#include "loadobj.h"
# include <drawable.h>
# include <smartpointerhelp.h>
# include <face.h>
# include <vertex.h>
# include <halfedge.h>

class Mesh : public Drawable {
private:

    std::vector<uPtr<Face>> faces; // stores the faces
    std::vector<uPtr<Vertex>> vertices; // stores the vertices
    std::vector<uPtr<HalfEdge>> halfEdges; // stores the halfedges

public:

    // Constructors/Destructors
//    Mesh(OpenGLContext*); // default constructor
    Mesh(OpenGLContext* context); // default constructor

    // Need to invoke the loading on an obj
//    LoadOBJ load_obj(const char* file_name); // class implementation (DEPRECATED)
    void load_obj(const char* file_name);


    // Implement virtual function create()
    void create() override;

};
