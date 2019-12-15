/*
    test.frag

    This is a simple glsl file to test graph functionality.
    It might be the default.
*/
#version 400

uniform sampler2D tex0;

in vec2 v_TexCoords;
in vec3 v_Normals;
out vec4 out_Color;

void main() {
    //out_Color = vec4(v_TexCoords, 0.0, 1.0);
    out_Color = texture(tex0, v_TexCoords);
}