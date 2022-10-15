# include <mesh.h>
# include <random>
# include <fstream> // for I/O file streams
# include <iostream> // for debugging
# include <sstream> // make a string an object (stringstream). https://www.tutorialspoint.com/stringstream-in-cplusplus
# include "smartpointerhelp.h"

Mesh::Mesh(OpenGLContext *context)
    : Drawable(context), faces(), vertices(), halfEdges()
{}

glm::vec3 genRandColor()
{
    // This is to generate random numbers for colors
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 255); // define the range
    return glm::vec3(distr(gen), distr(gen), distr(gen));
}

void Mesh::load_obj(const char* file_name)
{
    std::ifstream input_file(file_name); // input file object

    // Make sure the file is read correctly, else throw error
    if (!input_file.is_open())
    {
        std::cout << "OBJ file read incorrectly in loadobj.h" << std::endl;
        throw "OBJ file read incorrectly";
    }

    std::stringstream strstream; // initialize empty string stream object
    std::string line = ""; // current line in reading the file
    std::string prefix = ""; // the first bit of the line (such as v, vt, f) from the OBJ

    // This is temporary
    glm::vec3 temp_pos;
    GLint temp_glint = 0;

    // While not at the end of the file, read line by line
    while(std::getline(input_file, line))
    {
        // Reset the faces vertex idx vector
        std::vector<GLint> vert_pos_idx; // position coords indices

        // Set up the string stream with the current line
        strstream.clear(); // be sure to clear the string stream for each line
        strstream.str(line); // set the current line as the string stream
        strstream >> prefix; // set the prefix

        // Check what the prefix is, and do something based on the prefix. If unrecognized, do nothing
        if (prefix == "v") // if it is a vertex prefix
        {
            // Populate the temp vec3 for position
            strstream >> temp_pos.x >> temp_pos.y >> temp_pos.z; // get the x,y,z of the vertex pos
            // Put the vertex in the verts uPtr vector
            uPtr<Vertex> vptr = mkU<Vertex>(temp_pos); // instantiate vert and make unique ptr to it
            this->vertices.push_back(std::move(vptr)); // put the uPtr in the verts list
        }
        else if (prefix == "f") // if it is a face prefix
        {
            // Instatiate a face object
            uPtr<Face> f = mkU<Face>();

            int counter = 0; // to only grab the first part
            while (strstream >> temp_glint)
            {
                if (counter == 0)
                {
                    // Populate the indices vectors with the appropriate GLint
                    vert_pos_idx.push_back(temp_glint);
                }

                // Increment the counter such that we only get the fist part of each str for face
                if (strstream.peek() == '/')
                {
                    ++counter;
                    strstream.ignore(1, '/');
                }
                else if (strstream.peek() == ' ')
                {
                    ++counter;
                    strstream.ignore(1, ' ');
                }
                // Reset the counter
                if (counter > 2)
                {
                    counter = 0;
                }
            }

            // Make HalfEdges for all of the vertex indices that were just made
            for (const auto &v_idx : vert_pos_idx) // for each vertex
            {
                // For the following, recall .get() returns a raw ptr so everything is rewritten each loop!
                uPtr<HalfEdge> he = mkU<HalfEdge>(); // instatiate HalfEdge object
                he->vert = this->vertices[v_idx - 1].get(); // Assign the Vertex it points to (obj is 1 indexing, c++ is 0 indexing)
                this->vertices[v_idx - 1]->halfEdge = he.get(); // Vertex stores 1 ptr to an arbitrary half-edge that points to it
                he->face = f.get(); // assign all created HalfEdges to this Face
                f->halfEdge = he.get(); // assign this Face one of these HalfEdges
                this->halfEdges.push_back(std::move(he)); // Store HalfEdge in list of HalfEdges
            }

            // Assign Face a random color
            f->color = genRandColor();

            // Now this face is fully populated, so push into faces list
            this->faces.push_back(std::move(f));

            // HalfEdges need their next HE assigned and their symmetric HE assigned
            int num_verts = vert_pos_idx.size(); // this is how many vertices/edges are in this face
            for (int i = 0; i < int(vert_pos_idx.size()); i++) // for each vertex (equal to number of edges)
            {
                // Assign the next pointers
                if (i < (num_verts - 1))
                {
                    // For indexing, note that this->halfEdges is done in order of all faces,
                    // and in order of vertices for each face!
                    this->halfEdges[int(this->halfEdges.size()) - num_verts + i]->heNext =
                            this->halfEdges[int(this->halfEdges.size()) - num_verts + i + 1].get();
                }
                else // if at the last HalfEdge for this face, must assign next to the first HalfEdge
                {
                    this->halfEdges.back()->heNext = this->halfEdges[int(this->halfEdges.size()) - num_verts].get();
                }

                // Assign the symmetric pointers

            }

        }
    }
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
        HalfEdge *curr = face->halfEdge; // make a raw pointer to the halfEdge for this face

        int numVerts = 0; // number of vertices on the current face
        do {
                // Add the position
                position.push_back(curr->vert->pos); // put the position in the position vector
                // Add the color by talking the current faces color
                color.push_back(curr->face->color);
                // Add the normal ( with the cross product of half edges )
                normal.push_back(glm::cross((curr->vert->pos - (curr->heSym->vert->pos)),
                           (curr->heNext->vert->pos - curr->vert->pos)));

                // Update the current half edge to the next half edge
                curr = curr->heNext; // get the next half edge

                // Update the current vertex number
                ++numVerts;
        } while (curr != face->halfEdge);

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
}
