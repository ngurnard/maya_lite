#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <QString>
#include <iostream>
#include <vector>
#include "la.h"

class Joint {
private:

    const unsigned int id; // a unique integer to identify the vertex in the menus and while debugging
    static unsigned int count; // a counter to iterate across so that each ID is unique

    QString name;
    Joint* parent; // a pointer to the joint that is the parent of this joint
    std::vector<Joint*> children; // a list of children pointers
    glm::vec3 pos; // position relative to the parent joint
    glm::quat rot; // quaternion that represents this joint's current orientation (relative it its parent?)
    glm::mat4 bindMat;

public:

    // Constructors/Destructors
    Joint(); // default constructor
    Joint(QString name);
    Joint(QString name, Joint* parent, glm::vec3 pos, glm::quat rot); // default constructor

    glm::mat4 getLocalTransformation(); // represents the concatenation of a joint's position and rotation.
    glm::mat4 getOverallTransformation(); // represents the concatentation of this joint's local transformation with the transformations of its chain of parent joints.
};
