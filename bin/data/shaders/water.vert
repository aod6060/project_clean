#version 400


layout(location=0) in vec3 vertices;
layout(location=1) in vec2 texCoords;
layout(location=2) in vec3 normals;


uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec2 v_TexCoords;
out vec3 v_Normals;

void main() {
    gl_Position = proj * view * model * vec4(vertices, 1.0);
    v_TexCoords = texCoords;
    v_Normals = normals;
}