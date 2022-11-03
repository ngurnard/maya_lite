#include "joint.h"

unsigned int Joint::count = 0; // need to initialize the static variable count

Joint::Joint()
    :  id(count++), name("Name Not Set"), parent(nullptr), pos(glm::vec3(0, 0, 0)), rot(glm::quat(1, 0, 0, 0))
{
    QTreeWidgetItem::setText(0, name);
};

Joint::Joint(QString name)
    : id(count++), name(name), parent(nullptr), pos(glm::vec3(0, 0, 0)), rot(glm::quat(1, 0, 0, 0))
{
    QTreeWidgetItem::setText(0, name);
};

Joint::Joint(QString name, Joint* parent, glm::vec3 pos, glm::quat rot)
    : id(count++), name(name), parent(parent), pos(pos), rot(rot)
{
    QTreeWidgetItem::setText(0, name);
};

glm::mat4 Joint::getLocalTransformation()
{
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), this->pos); // translate this joint to the the same space as the parent
    return trans * glm::toMat4(this->rot); // T * R as a matrix. Represents the transformation to get from parent to this joint
}

glm::mat4 Joint::getOverallTransformation()
{
    if (parent != nullptr) {
        return this->parent->getOverallTransformation() * this->getLocalTransformation(); // represent this joint in terms of the world space
    } else { // if this is a base node
        return this->getLocalTransformation();
    }
}

Joint& Joint::addChild(uPtr<Joint> &j)
{
    QTreeWidgetItem::addChild((QTreeWidgetItem*)j.get()); // add to the tree
    Joint &ref = *j; // reference to the object
    this->children.push_back(std::move(j)); // add to the children vector
    return ref;
}

void Joint::setGeom(JointWireframe *newGeom)
{
    geom = newGeom;
}
