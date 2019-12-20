#version 400

uniform sampler2D tex0;
uniform float texScale;
uniform float timeDelta;

in vec2 v_TexCoords;
in vec3 v_Normals;

out vec4 out_Color;

void main() {

    vec2 tc = vec2(v_TexCoords.x + timeDelta, v_TexCoords.y) * texScale;

    out_Color = texture(tex0, tc);
    //out_Color = vec4(v_TexCoords, 0.0, 1.0);
}