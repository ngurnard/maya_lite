#include "face_display.h"

FaceDisplay::FaceDisplay(OpenGLContext *context) : Drawable(context), face_to_disp(nullptr) {};

void FaceDisplay::create()
{
    std::vector<glm::vec4> color; // the color to be displayed for this Face
    std::vector<glm::vec4> position; // the color to be displayed for this Face
    std::vector<GLint> idx;

    HalfEdge *curr = face_to_disp->halfEdge; // make a raw pointer to the halfEdge for this face
    // Loop through the half edges
    int numVerts = 0;
    do {
        // Add the position
        position.push_back(glm::vec4(curr->vert->pos, 1)); // put the position in the position vector
        // Add the color by talking the current faces color
        color.push_back(glm::vec4{1,1,1,1} - glm::vec4(face_to_disp->color, 1));
        // Update the current half edge to the next half edge
        curr = curr->heNext; // get the next half edge
        ++numVerts;
    } while (curr != face_to_disp->halfEdge);

    // Assign indices to the current faces vertices
    for (int i = 0; i < numVerts; ++i)
    {
        if (i == numVerts - 1)
        {
            idx.push_back(i); // each face starts incrementing at the total number of verts
            idx.push_back(0);
        } else {
            idx.push_back(i); // each face starts incrementing at the total number of verts
            idx.push_back(i + 1);
        }

    }

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

void FaceDisplay::updateFace(Face* f)
{
    this->face_to_disp = f;
}

GLenum FaceDisplay::drawMode()
{
    return GL_LINES;
}
