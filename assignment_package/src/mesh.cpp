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

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        return h1 ^ h2;
    }
};

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

    // The HalfEdge map that pairs 2 vertices (previous and next) with a halfedge
    // used for builing the symmetric pointers in the half edge data structure
    // refer to: https://stackoverflow.com/questions/32685540/why-cant-i-compile-an-unordered-map-with-a-pair-as-key
    // refer to: https://en.cppreference.com/w/cpp/container/unordered_map
    std::unordered_map<std::pair<int,int>, HalfEdge*, pair_hash> heMap;

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
            count = 0; // for indexing
            for (const auto &v_idx : vert_pos_idx) // for each vertex (equal to number of edges)
            {
                // Assign the next pointers
                if (count < (num_verts - 1))
                {
                    // For indexing, note that this->halfEdges is done in order of all faces,
                    // and in order of vertices for each face!
                    this->halfEdges[int(this->halfEdges.size()) - num_verts + count]->heNext =
                            this->halfEdges[int(this->halfEdges.size()) - num_verts + count + 1].get();
                }
                else // if at the last HalfEdge for this face, must assign next to the first HalfEdge
                {
                    this->halfEdges.back()->heNext = this->halfEdges[int(this->halfEdges.size()) - num_verts].get();
                }

                // Assign the symmetric pointers (the map uses 1 indexing so I don't have to subtract 1)
                int vert1 = v_idx;
                int vert2; // need to declare in the scope outside of if/else
                if (count == 0)
                {
                    vert2 = vert_pos_idx.back(); // get the last one in the vert_pos_idx list
                }
                else
                {
                    // https://stackoverflow.com/questions/19967412/accessing-next-element-in-range-based-for-loop-before-the-next-loop
                    vert2 = *(&vert_pos_idx[count]);
                }

                // Define the key and value
                std::pair<int,int> map_key(vert1, vert2); // store two heterogeneous objects as a single unit
                HalfEdge *hePtr = this->halfEdges[halfEdges.size() - num_verts + count].get();

                // Need to provide a suitable hash function for your key type for an unordered_map!
                // and std::pair is not hashable
                // Refer: https://stackoverflow.com/questions/32685540/why-cant-i-compile-an-unordered-map-with-a-pair-as-key
                // Refer: https://stackoverflow.com/questions/69329772/why-mappairint-int-int-works-but-unordered-mappairint-int-doesnt
                // Refer: https://en.cppreference.com/w/cpp/container/unordered_map

                if (heMap.find(map_key) == heMap.end()) // if this key does NOT exist, create the key, value pair
                {
                    heMap[map_key] = hePtr;
                }
                else // if the key already exists, create the symmetric pointers!
                {
                    hePtr->heSym = heMap.find(map_key)->second; // second gives the value (HalfEdge*)
                    heMap.find(map_key)->second->heSym = hePtr;
                }

                count++;

            }

        }
    }
}

void Mesh::create()
{
    // Need vectors to store the positions and color and normals for each face
    std::vector<GLint> idx; // store the index vertices
    std::vector<glm::vec3> position;
    std::vector<glm::vec3> normal;
    std::vector<glm::vec3> color;

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
    mp_context->glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(glm::vec3), position.data(), GL_STATIC_DRAW);

    generateNor();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    mp_context->glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3), normal.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(glm::vec3), color.data(), GL_STATIC_DRAW);

    // Free up memory now that we no longer need the vertex info to be stored on the CPU
//    count.clear();

}
