#pragma once
#include "drawable.h"
#include "halfedge.h"


class HalfEdgeDisplay : public Drawable {
protected:
    HalfEdge *he_to_disp; // this is the he in which we would like to display

public:

    //constructor
    HalfEdgeDisplay(OpenGLContext *context);

    // Creates VBO data to make a visual
    // representation of the currently selected Face
    void create() override;
    // Change which Face representedFace points to
    void updateHalfEdge(HalfEdge*);

    GLenum drawMode() override;

    std::vector<glm::vec4> position; // the position to be displayed for this HalfEdge or bone
};
