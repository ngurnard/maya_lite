#include <joint_wireframe.h>

JointWireframe::JointWireframe(OpenGLContext* context)
    : Drawable(context)
{}

void JointWireframe::create()
{
    std::vector<glm::vec4> color; // the color to be displayed for this joint
    std::vector<glm::vec4> position; // the color to be displayed for this joint
    std::vector<GLint> idx;
    float radius = 0.5; // the sphere radius
    float num_points = 15; // the resolution of the circle to draw
    float deg = glm::radians(360.f / num_points); // degrees in terms of radians

    // Represent this joint in terms of the world space via a transform matrix
    // glm::mat4 transform = this->joint_to_disp->getOverallTransformation();

    // Draw the wireframe sphere
    int my_counter = 0;
    // x circle
    // Vertex positions
    glm::vec4 offset(0, 0, radius, 1); // how far to offset from joint to draw the circle. Offset in z for around the x axis
    for (int i = 0; i < num_points; i++)
    {
        glm::mat4 rotMat = glm::rotate(glm::mat4(1.f), i * deg, glm::vec3(1, 0, 0)); // rotate around x axis
        position.push_back(rotMat * offset); // draw this offset position in terms of the world space
        if (highlighted == false)
        {
            color.push_back(glm::vec4(1, 0, 0, 1)); // red for x
        } else {
            color.push_back(glm::vec4(1, 1, 1, 1)); // white for weeeeeeeeeeeeeeee
        }
    }
    for (int i = 0; i < int(position.size()); i++)
    {
        if (i < int(position.size()) - 1)
        {
            idx.push_back(i);
            idx.push_back(i + 1);
        } else {
            idx.push_back(i);
            idx.push_back(0);
        }
    }
    my_counter += position.size();

    // y circle
    // Vertex positions
    offset = glm::vec4(radius, 0, 0, 1); // how far to offset from joint to draw the circle. Offset in x for around the y axis
    for (int i = 0; i < num_points; i++)
    {
        glm::mat4 rotMat = glm::rotate(glm::mat4(1.f), i * deg, glm::vec3(0, 1, 0)); // rotate around y axis
        position.push_back(rotMat * offset); // draw this offset position in terms of the world space
        if (highlighted == false)
        {
            color.push_back(glm::vec4(0, 1, 0, 1)); // green for y
        } else {
            color.push_back(glm::vec4(1, 1, 1, 1)); // white for weeeeeeeeeeeeeeee
        }
    }

    for (int i = 0; i < int(position.size()); i++)
    {
        if (i < int(position.size()) - 1)
        {
            idx.push_back(i + my_counter);
            idx.push_back(i + 1 + my_counter);
        } else {
            idx.push_back(i + my_counter);
            idx.push_back(0 + my_counter);
        }
    }
    my_counter += position.size();

    // z circle
    offset = glm::vec4(radius, 0, 0, 1); // how far to offset from joint to draw the circle. Offset in x for around the z axis
    for (int i = 0; i < num_points; i++)
    {
        glm::mat4 rotMat = glm::rotate(glm::mat4(1.f), i * deg, glm::vec3(0, 0, 1)); // rotate around z axis
        position.push_back(rotMat * offset); // draw this offset position in terms of the world space
        if (highlighted == false)
        {
            color.push_back(glm::vec4(0, 0, 1, 1)); // blue for z
        } else {
            color.push_back(glm::vec4(1, 1, 1, 1)); // white for weeeeeeeeeeeeeeee
        }
    }

    for (int i = 0; i < int(position.size()); i++)
    {
        if (i < int(position.size()) - 1)
        {
            idx.push_back(i + my_counter);
            idx.push_back(i + 1 + my_counter);
        } else {
            idx.push_back(i + my_counter);
            idx.push_back(0 + my_counter);
        }
    }
    my_counter += position.size();

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

GLenum JointWireframe::drawMode()
{
    return GL_LINES; // wireframe mode
}

void JointWireframe::setJoint(Joint *joint)
{
    joint_to_disp = joint;
}

