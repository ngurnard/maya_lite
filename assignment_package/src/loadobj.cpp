# include "loadobj.h"

# include "vertex.h"
# include "face.h"
# include <random>

void LoadOBJ::load_obj(const char* file_name)
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

    glm::vec3 temp_pos;
    glm::vec3 temp_nor;
    glm::vec2 temp_uv;
    GLint temp_glint = 0;


    // While not at the end of the file, read line by line
    while(input_file.good())
    {

        // Set up the string stream with the current line
        strstream.clear(); // be sure to clear the string stream for each line
        strstream.str(current_line); // set the current line as the string stream
        strstream >> prefix; // set the prefix

        // Check what the prefix is, and do something based on the prefix. If unrecognized, do nothing
        if (prefix == "v") // if it is a vertex prefix
        {
            strstream >> temp_pos.x >> temp_pos.y >> temp_pos.z; // get the x,y,z of the vertex pos
            this->position.push_back(temp_pos); // store in the vector
        }
        else if (prefix == "vt") // if it is a texture prefix
        {
            strstream >> temp_uv.x >> temp_uv.y;
            this->uv_coord.push_back(temp_uv);
        }
        else if (prefix == "vn") // if it is a normal prefix
        {
            strstream >> temp_nor.x >> temp_nor.y >> temp_nor.z; // get the x,y,z of the vertex normal
            this->normal.push_back(temp_nor); // put into the vertex normal vector
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
            this->color.push_back(glm::vec3(distr(gen), distr(gen), distr(gen)));

        }
        else
        {
            // do nothing
        }

    }

}

