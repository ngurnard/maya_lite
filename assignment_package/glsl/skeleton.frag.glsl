#version 150

in vec4 fs_Col; // input color from the create functions
out vec4 out_Col; // This is the final output color that you will see on your
                  // screen for the pixel that is currently being processed.

// note: storing 2 bing matrices (bind1,bind2) forces us to use a VBO for the bind matrices. Lets not do that for the sake of passing less data
uniform mat4[30] u_bindMats; // support skeletons up to 30 joints. Change this number to support bigger skelly's
uniform mat4[30] u_overallTransforms; // the overall transforms for each joint
void main(void)
{
    // Compute final shaded color (I dont want to change the color -- ie no shading)
    out_Col = fs_Col;

    // debugggin
    // if any is zero everywhere, make it magenta
}
