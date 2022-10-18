# include <face.h>

unsigned int Face::count = 0; // need to initialize the static variable count

Face::Face()
    : halfEdge(nullptr), color(), id(count++)
{
    this->setText("Face: " + QString::number(id));
}
