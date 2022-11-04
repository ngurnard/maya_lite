#version 150

in vec4 fs_Col; // input color from the create functions
out vec4 out_Col; // This is the final output color that you will see on your
                  // screen for the pixel that is currently being processed.

void main(void)
{
    // Compute final shaded color (I dont want to change the color -- ie no shading)
    out_Col = fs_Col;
}
