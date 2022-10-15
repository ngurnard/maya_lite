# include <mesh.h>
# include <random>
# include <fstream> // for I/O file streams
# include <iostream> // for debugging
# include <sstream> // make a string an object (stringstream). https://www.tutorialspoint.com/stringstream-in-cplusplus
# include "smartpointerhelp.h"

Mesh::Mesh(OpenGLContext *context)
    : Drawable(context), faces(), verts(), hes()
{}

void Mesh::load_obj(const char* file_name)
{
    // This is to generate random numbers for colors
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 255); // define the range

    // vertex data
    std::vector<glm::vec3> vert_pos; // position coords
    std::vector<glm::vec2> vert_uv; // texture coords
    std::vector<glm::vec3> vert_nor; // normal coords

    // face data
    std::vector<GLint> vert_pos_idx; // position coords indices
    std::vector<GLint> vert_uv_idx; // texture coords indices
    std::vector<GLint> vert_nor_idx; // normal coords indices

    std::ifstream input_file(file_name); // input file object

    // Make sure the file is read correctly, else throw error
    if (!input_file.is_open())
    {
        std::cout << "OBJ file read incorrectly in loadobj.h" << std::endl;
        throw "OBJ file read incorrectly";
    }

    std::stringstream strstream; // initialize empty string stream object
    std::string current_line = ""; // current line in reading the file
    std::string prefix = ""; // the first bit of the line (such as v, vt, f) from the OBJ

    // This is temporary
    glm::vec3 temp_pos;
    glm::vec3 temp_nor;
    glm::vec2 temp_uv;
    GLint temp_glint = 0;

    // These are purely for storing stuff in load_obj. Not for use outside of load_obj
    // Position is the only one used in this assignment
    std::vector<glm::vec3> position;
    std::vector<glm::vec3> color;
    std::vector<glm::vec2> uv_coord;
    std::vector<glm::vec3> normal;


    // While not at the end of the file, read line by line
    while(std::getline(input_file, current_line))
    {

        // Set up the string stream with the current line
        strstream.clear(); // be sure to clear the string stream for each line
        strstream.str(current_line); // set the current line as the string stream
        strstream >> prefix; // set the prefix

        // Check what the prefix is, and do something based on the prefix. If unrecognized, do nothing
        if (prefix == "v") // if it is a vertex prefix
        {
            strstream >> temp_pos.x >> temp_pos.y >> temp_pos.z; // get the x,y,z of the vertex pos
            position.push_back(temp_pos); // store in the vector
        }
        else if (prefix == "vt") // if it is a texture prefix
        {
            strstream >> temp_uv.x >> temp_uv.y;
            uv_coord.push_back(temp_uv);
        }
        else if (prefix == "vn") // if it is a normal prefix
        {
            strstream >> temp_nor.x >> temp_nor.y >> temp_nor.z; // get the x,y,z of the vertex normal
            normal.push_back(temp_nor); // put into the vertex normal vector
        }
        else if (prefix == "f") // if it is a face prefix
        {
            int count = 0;
            while (strstream >> temp_glint)
            {
                // Populate the indices vectors with the appropriate GLint
                if (count == 0) // if the first part of the face X/X/X it is a vert pos
                {
                    vert_pos_idx.push_back(temp_glint);
                }
                else if (count == 1) // if the second part of the face X/X/X it is a uv coord
                {
                    vert_uv_idx.push_back(temp_glint);
                }
                else if (count == 2) // if the third part of the face X/X/X it is a vert nor
                {
                    vert_nor_idx.push_back(temp_glint);
                }

                // Ignore the slash cahracters in the faces
                if (strstream.peek() == '/') // peek looks ahead without popping from the stream
                {
                    ++count; // increment the count
                    strstream.ignore(1, '/');
                }
                else if (strstream.peek() == ' ') // if at the end of the face reset the count
                {
                    count = 0; // reset the count
                    strstream.ignore(1, ' ');
                }
            }

            // Assign each face a random color
            color.push_back(glm::vec3(distr(gen), distr(gen), distr(gen)));

        }
        else
        {
            // do nothing
        }

    }

    // Now that the obj is loaded, create the half mesh data structure
    // NOTE: For this assignment we actually only really need the vertices since
    // the colors and texture coordinates are not constant once we modify the mesh in the gui

}

void Mesh::create()
{
    // Need vectors to store the positions and color and normals for each face
    std::vector<glm::vec3> position;
    std::vector<glm::vec3> color;
    std::vector<glm::vec3> normal;
    std::vector<GLint> idx; // store the index vertices

    int totVerts = 0; // initialize the total amount of vertices
    for (const auto &face : this->faces) // for each face
    {
        // Iterate over the half edges to get vertex position info
        HalfEdge *curr = face->hePtr; // make a raw pointer to the faces smart pointer

        int numVerts = 0; // number of vertices on the current face
        do {
                // Add the position
                position.push_back(curr->vertPtr->pos); // put the position in the position vector
                // Add the color by talking the current faces color
                color.push_back(curr->facePtr->color);
                // Add the normal ( with the cross product of half edges )
                normal.push_back(glm::cross((curr->vertPtr->pos - (curr->heSym->vertPtr->pos)),
                           (curr->heNext->vertPtr->pos - curr->vertPtr->pos)));

                // Update the current half edge to the next half edge
                curr = curr->heNext; // get the next half edge

                // Update the current vertex number
                ++numVerts;
        } while (curr != face->hePtr);

        // Assign indices to the current faces vertices
        for (int i = 0; i < numVerts - 2; ++i)
        {
            idx.push_back(totVerts); // each face starts incrementing at the total number of verts
            idx.push_back(i + 1 + totVerts);
            idx.push_back(i + 2 + totVerts);
        }

        totVerts += numVerts; // update the total number of indices seen
    }

    // Set the count variable inside the create function
    // Set "count" to the number of indices in your index VBO
    count = idx.size();

    // Make the HalfEdge data structure
    // Populate the vertex unique pointers list
    for (glm::vec3 &v : position)
    {
        uPtr<Vertex> vptr = mkU<Vertex>(v); // instantiate vert and make unique bitch to it
        this->verts.push_back(vptr); // put in the thing

        uPtr<HalfEdge> heptr = mkU<HalfEdge>(v);
    }
    // Create all of the HalfEdges
//    for (glm::vec3 &v : position)
//    {
//        uPtr<HalfEdge> hePtr = mkU<HalfEdge>(*(&v + 1) - v);
//        this->verts.push_back(vptr);
//    }

}
