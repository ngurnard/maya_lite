#version 150

uniform mat4 u_Model; // The matrix that defines the transformation of the object we're rendering.
uniform mat4 u_ViewProj; // The matrix that defines the camera's transformation.

// note: storing 2 bing matrices (bind1,bind2) forces us to use a VBO for the bind matrices. Lets not do that for the sake of passing less data
uniform mat4[30] u_bindMats; // support skeletons up to 30 joints. Change this number to support bigger skelly's
uniform mat4[30] u_overallTransforms; // the overall transforms for each joint

in vec2 jointWeights; // the 2 joint weights that correspong to the nearest joints
in ivec2 jointIDs; // the 2 nearest jointIDs

uniform mat4 viewProj;

in vec4 vs_Pos;
in vec4 vs_Col;
//in vec4 vs_Nor;

out vec3 fs_Pos;
out vec4 fs_Col;
//out vec4 fs_Nor;

void main(void)
{
    // glUniformMatrix4fv lets you specify the number of mat4s to pass in

    mat4 bind1 = u_bindMats[jointIDs[0]];
    mat4 bind2 = u_bindMats[jointIDs[1]];

    mat4 T1 = u_overallTransforms[jointIDs[0]];
    mat4 T2 = u_overallTransforms[jointIDs[1]];

    // What is the position of this vert in world space, given the joints current pos?
    vec4 joint1WorldPos = T1 * bind1 * vs_Pos;
    vec4 joint2WorldPos = T2 * bind2 * vs_Pos;

    vec4 blend = jointWeights[0] * joint1WorldPos + jointWeights[1] * joint2WorldPos; // the linear blend

    // Dont forget to apply the model transofmration
    vec4 modelPosition = u_Model * blend; // takes us from model coordinates to world coordinates
    gl_Position = viewProj * modelPosition;

    // color to pass to the fragment shader
    fs_Col = vs_Col;

    // surface normals -- only if doing lambert shading?
    // inverse scale
    // same rotate
    // inverse then transpose

//    vec3 nor1 = normalize(transpose(inverse(mat3(T1))) * mat3(bind1) * vs_Nor.xyz);
//    vec3 nor2 = normalize(transpose(inverse(mat3(T2))) * mat3(bind2) * vs_Nor.xyz);
//    fs_Nor = mix(nor1, nor2, jointWeights[0]);

}
