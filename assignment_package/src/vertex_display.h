#pragma once
#include "drawable.h"
#include "vertex.h"


class VertexDisplay : public Drawable {
protected:
    Vertex *vert_to_disp; // this is the vertex in which we would like to display

public:

    //constructor
    VertexDisplay(OpenGLContext *context);

    // Creates VBO data to make a visual
    // representation of the currently selected Vertex
    void create() override;
    // Change which Vertex representedVertex points to
    void updateVertex(Vertex*);

    GLenum drawMode() override;
};
