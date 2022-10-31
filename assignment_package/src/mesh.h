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
    // loading the obj also creates the member variables of this class
    void load_obj(const char* file_name);

    // Implement virtual function create()
    void create() override;

    // Need to delete the mesh essentially if loading a new obj from the file dialog so it doesnt combine 2 objs
    void clearMesh();

    friend class MainWindow; // for displaying stuff in the vertex lists

    // attribute modification functions
//    void splitHE(HalfEdge *he);
    void splitHE(HalfEdge *he, Vertex* existing_midpt = nullptr);
    void triangulate(Face *f);

    // subdivision member functions
    void subdivide();
    void computeCentroid(Face &f);
    std::unordered_map<HalfEdge*, Vertex*> computeMidpoints(Mesh &mesh, std::unordered_map<Face*, Vertex*> &centroidMap);
    void smoothOGVerts(std::vector<Vertex*> &og_verts, std::unordered_map<Face*, Vertex*> &centroidMap);
    void quadrangulate(std::unordered_map<Face*, Vertex*> &centroidMap);
};
