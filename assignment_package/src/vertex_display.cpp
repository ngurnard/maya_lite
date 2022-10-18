#include "vertex_display.h"

VertexDisplay::VertexDisplay(OpenGLContext *context) : Drawable(context), vert_to_disp(nullptr) {};

void VertexDisplay::create()
{
    std::vector<glm::vec4> color; // the color to be displayed for this vertex
    std::vector<glm::vec4> position; // the color to be displayed for this vertex
    std::vector<GLint> idx;

    color.push_back(glm::vec4{1,1,1,1});
    position.push_back(glm::vec4(vert_to_disp->pos, 1));
    idx.push_back(0);

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

void VertexDisplay::updateVertex(Vertex* v)
{
    this->vert_to_disp = v;
}

GLenum VertexDisplay::drawMode()
{
    return GL_POINTS;
}
