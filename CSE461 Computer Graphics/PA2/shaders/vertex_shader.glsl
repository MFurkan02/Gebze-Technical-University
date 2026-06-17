#version 330 core

layout(location = 0) in vec3 aPos;
// Input vertex attribute: position of the vertex (x, y, z), bound to location 0

layout(location = 1) in vec3 aNormal;
// Input vertex attribute: normal vector at the vertex, used for lighting calculations, bound to location 1

layout(location = 2) in vec2 aTexCoords;
// Input vertex attribute: texture coordinates (u, v), bound to location 2

out vec2 TexCoords;
// Output to the fragment shader: interpolated texture coordinates

out vec3 FragPos;
// Output to the fragment shader: the vertex position in world space (used for lighting)

out vec3 Normal;
// Output to the fragment shader: the transformed normal vector (in world space)

uniform mat4 model;
// The model matrix transforms object coordinates to world coordinates

uniform mat4 view;
// The view matrix transforms world coordinates to view (camera) space

uniform mat4 projection;
// The projection matrix transforms view space to clip space (for perspective projection)

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    // Convert the object-space vertex position to world-space using the model matrix
    // Store the result in FragPos to use in lighting calculations in the fragment shader

    Normal = mat3(transpose(inverse(model))) * aNormal;
    // Compute the correct normal vector in world space by transforming it with the
    // inverse-transpose of the model matrix (handles non-uniform scaling properly)
    // mat3() removes the translation part of the matrix, which is irrelevant for normals

    TexCoords = aTexCoords;
    // Pass texture coordinates directly to the fragment shader

    gl_Position = projection * view * vec4(FragPos, 1.0);
    // This determines where the vertex will appear on the screen
}
