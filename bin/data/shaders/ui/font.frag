#version 400

// Uniforms
uniform sampler2D font0;
// Varying
in vec2 v_TexCoords;
out vec4 out_Color;

void main() {
    out_Color = texture(font0, v_TexCoords);
}