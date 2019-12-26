#version 400

// Uniforms
uniform vec3 color;

// Varyings
out vec4 out_Color;

void main() {
    out_Color = vec4(color, 1.0);
}