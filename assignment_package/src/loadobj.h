# pragma once

# include <drawable.h>
# include <glm/glm.hpp>
//# include "mesh.h"
# include <algorithm> // for random colors

# include <fstream> // for I/O file streams
# include <iostream> // for debugging
# include <sstream> // make a string an object (stringstream). https://www.tutorialspoint.com/stringstream-in-cplusplus

struct LoadOBJ
{
    std::vector<glm::vec3> position;
    std::vector<glm::vec3> color;
    std::vector<glm::vec2> uv_coord;
    std::vector<glm::vec3> normal;

    void load_obj(const char* file_name); // populate the member variable with this function

    friend class Mesh; // Mesh can access all of Camera’s private and protected members now
};

