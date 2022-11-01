#include "joint.h"

unsigned int Joint::count = 0; // need to initialize the static variable count

Joint::Joint()
    :  id(count++), name(), parent(nullptr), pos(glm::vec3(0, 0, 0)), rot(glm::quat(1, 0, 0, 0))
{};

Joint::Joint(QString name)
    : id(count++), name(name), parent(nullptr), pos(glm::vec3(0, 0, 0)), rot(glm::quat(1, 0, 0, 0))
{};

Joint::Joint(QString name, Joint* parent, glm::vec3 pos, glm::quat rot)
    : id(count++), name(name), parent(parent), pos(pos), rot(rot)
{};

glm::mat4 Joint::getLocalTransformation()
{
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), this->pos); // translate this joint to the the same space as the parent
    return glm::toMat4(glm::toQuat(trans) * this->rot); // T * R as a matrix. Represents the transformation to get from parent to this joint
}

glm::mat4 Joint::getOverallTransformation()
{
    if (parent != nullptr) {
        return this->parent->getOverallTransformation() * this->getLocalTransformation();
    } else { // if this is a base node
        return this->getLocalTransformation();
    }


}
