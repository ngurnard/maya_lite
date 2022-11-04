# pragma once
# include <drawable.h>
# include <smartpointerhelp.h>
# include <face.h>
# include <vertex.h>
# include <halfedge.h>
# include <unordered_map>

class Mesh : public Drawable {
private:

    std::vector<uPtr<Face>> faces; // stores the faces
    std::vector<uPtr<Vertex>> vertices; // stores the vertices
    std::vector<uPtr<HalfEdge>> halfEdges; // stores the halfedges
    std::vector<Joint*> skeletonJoints; // skeleton joints are obtained with helper function getSkeletonJoints

public:

    bool bound_to_skeleton = false; // a bool to say if skinning has occured
    std::vector<glm::mat4> skeletonBindMats; // skeleton bindMats are obtained with helper function getSkeletonJoints
    std::vector<glm::mat4> skeletonOverallTransforms; // skeleton bindMats are obtained with helper function getSkeletonJoints

    // Constructors/Destructors
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
    void splitHE(HalfEdge *he, Vertex* existing_midpt = nullptr);
    void triangulate(Face *f);

    // subdivision member functions
    void subdivide();
    void computeCentroid(Face &f);
    std::unordered_map<HalfEdge*, Vertex*> computeMidpoints(Mesh &mesh, std::unordered_map<Face*, Vertex*> &centroidMap);
    void smoothOGVerts(std::vector<Vertex*> &og_verts, std::unordered_map<Face*, Vertex*> &centroidMap);
    void quadrangulate(std::unordered_map<Face*, Vertex*> &centroidMap);
    void bindToSkeleton(Joint *root);
    void getSkeletonJoints(Joint *root);
};
