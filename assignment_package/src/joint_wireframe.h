#pragma once
#include "joint.h"
#include "drawable.h"
#include "smartpointerhelp.h"
#include <vector>

// provide forward declarations to avoid circular dependency
class Joint;

class JointWireframe : public Drawable {

protected:
    Joint *joint_to_disp; // this is the he in which we would like to display

public:

    // Constructors/Destructors
    JointWireframe(OpenGLContext* context); // default constructor

    void create() override; // Need to implemnent the create() virtual function from the drawable class
    // make a second create for a selected joint??

    GLenum drawMode() override;

    void setJoint(Joint *joint);
};
