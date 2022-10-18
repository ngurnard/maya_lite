# include <vertex.h>

unsigned int Vertex::count = 0; // need to initialize the static variable count

Vertex::Vertex()
    : pos(), halfEdge(nullptr), id(count++) // does this update count??
{
    this->setText("Vertex: " + QString::number(id));
}

Vertex::Vertex(glm::vec3 position)
    : pos(position), halfEdge(nullptr), id(count++)
{
    this->setText("Vertex: " + QString::number(id));
}
