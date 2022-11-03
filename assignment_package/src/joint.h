#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <QTreeWidgetItem>
#include <QString>
#include <iostream>
#include <vector>
#include "la.h"
#include "smartpointerhelp.h"
#include "joint_wireframe.h"

// provide forward declarations to avoid circular dependency
class JointWireframe;

class Joint : QTreeWidgetItem {
public:

    const unsigned int id; // a unique integer to identify the vertex in the menus and while debugging
    static unsigned int count; // a counter to iterate across so that each ID is unique

    QString name;
    Joint* parent; // a pointer to the joint that is the parent of this joint
    std::vector<uPtr<Joint>> children; // a list of children pointers
    glm::vec3 pos; // position relative to the parent joint
    glm::quat rot; // quaternion that represents this joint's current orientation (relative it its parent?)
    glm::mat4 bindMat;

    JointWireframe *geom; // geometry for drawing a joint

public:

    // Constructors/Destructors
    Joint(); // default constructor
    Joint(QString name);
    Joint(QString name, Joint* parent, glm::vec3 pos, glm::quat rot); // default constructor

    glm::mat4 getLocalTransformation(); // represents the concatenation of a joint's position and rotation.
    glm::mat4 getOverallTransformation(); // represents the concatentation of this joint's local transformation with the transformations of its chain of parent joints.

    Joint& addChild(uPtr<Joint> &j); // for adding a child to the list of children
    // std::vector<uPtr<Joint>> &getChildren(); // getter for getting the children. I MADE EVERYTHING PUBLIC TO SAVE TIME. Look at scene graph HW2 for getters

    void setGeom(JointWireframe *newGeom); // set the geomtry

    friend class MyGL;

};
