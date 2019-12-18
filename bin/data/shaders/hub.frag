#version 400


uniform sampler2D tex0;

in vec2 v_TexCoords;
out vec4 out_Color;

void main() {
    out_Color = texture(tex0, v_TexCoords);    
    //out_Color = vec4(v_TexCoords, 0.0f, 1.0f);
}