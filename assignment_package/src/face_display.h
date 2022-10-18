#pragma once
#include "drawable.h"
#include "face.h"


class FaceDisplay : public Drawable {
protected:
    Face *face_to_disp; // this is the Face in which we would like to display

public:

    //constructor
    FaceDisplay(OpenGLContext *context);

    // Creates VBO data to make a visual
    // representation of the currently selected Face
    void create() override;
    // Change which Face representedFace points to
    void updateFace(Face*);

    GLenum drawMode() override;
};
