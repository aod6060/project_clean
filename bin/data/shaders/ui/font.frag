#version 400

// Uniforms
uniform sampler2D font0;
uniform vec3 color;

// Varying
in vec2 v_TexCoords;
out vec4 out_Color;

void main() {

    vec4 c = texture(font0, v_TexCoords);

    if(c.a < 0.5f) {
        discard;
    }

    c.rgb = c.rgb * color;

    out_Color = c;
}