#include "halfedge_display.h"

HalfEdgeDisplay::HalfEdgeDisplay(OpenGLContext *context) : Drawable(context), he_to_disp(nullptr) {};

void HalfEdgeDisplay::create()
{
    std::vector<glm::vec4> color; // the color to be displayed for this HalfEdge
    std::vector<glm::vec4> position; // the color to be displayed for this HalfEdge
    std::vector<GLint> idx;

    // Get the before and after points and add them to position
    position.push_back(glm::vec4(he_to_disp->vert->pos,1));
    position.push_back(glm::vec4(he_to_disp->heSym->vert->pos,1));

    color.push_back(glm::vec4(1,0,0,1));
    color.push_back(glm::vec4(1,1,0,1));

    idx.push_back(0);
    idx.push_back(1);

    // Set the count variable inside the create function
    // Set "count" to the number of indices in your index VBO
    this->count = idx.size();

    // Now I want to generate the VBOs to be sent to the GPU (borrowed from scene graph project -- hw2)
    // Referece: https://www.youtube.com/watch?v=0p9VxImr7Y0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=4&ab_channel=TheCherno
    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx(); // create a buffer
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx); // selects buffer with ID bufIdx
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // the number of indices multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), idx.data(), GL_STATIC_DRAW); // put data in the buffer

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(glm::vec4), position.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(glm::vec4), color.data(), GL_STATIC_DRAW);
}

void HalfEdgeDisplay::updateHalfEdge(HalfEdge* he)
{
    this->he_to_disp = he;
}

GLenum HalfEdgeDisplay::drawMode()
{
    return GL_LINES;
}
