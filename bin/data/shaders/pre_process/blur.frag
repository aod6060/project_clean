#version 400

// Uniforms
uniform sampler2D inputTex0;
uniform float size;

// Varying
in vec2 v_TexCoords;
out vec4 out_Color;

vec3 hblur(sampler2D samp, vec2 tc, float blurSize) {
    vec3 sum = vec3(0.0);
    float iBlurSize = 1.0 / blurSize;

    sum += texture(samp, vec2(tc.x - 4.0 * iBlurSize, tc.y)).rgb * 0.125;
    sum += texture(samp, vec2(tc.x - 3.0 * iBlurSize, tc.y)).rgb * 0.125;
    sum += texture(samp, vec2(tc.x - 2.0 * iBlurSize, tc.y)).rgb * 0.125;
    sum += texture(samp, vec2(tc.x - 1.0 * iBlurSize, tc.y)).rgb * 0.125;
    
    sum += texture(samp, vec2(tc.x + 1.0 * iBlurSize, tc.y)).rgb * 0.125;
    sum += texture(samp, vec2(tc.x + 2.0 * iBlurSize, tc.y)).rgb * 0.125;
    sum += texture(samp, vec2(tc.x + 3.0 * iBlurSize, tc.y)).rgb * 0.125;
    sum += texture(samp, vec2(tc.x + 4.0 * iBlurSize, tc.y)).rgb * 0.125;

    return sum;
}

vec3 vblur(sampler2D samp, vec2 tc, float blurSize) {
    vec3 sum = vec3(0.0);
    float iBlurSize = 1.0 / blurSize;

    sum += texture(samp, vec2(tc.x, tc.y - 4.0 * iBlurSize)).rgb * 0.125;
    sum += texture(samp, vec2(tc.x, tc.y - 3.0 * iBlurSize)).rgb * 0.125;
    sum += texture(samp, vec2(tc.x, tc.y - 2.0 * iBlurSize)).rgb * 0.125;
    sum += texture(samp, vec2(tc.x, tc.y - 1.0 * iBlurSize)).rgb * 0.125;

    sum += texture(samp, vec2(tc.x, tc.y + 1.0 * iBlurSize)).rgb * 0.125;
    sum += texture(samp, vec2(tc.x, tc.y + 2.0 * iBlurSize)).rgb * 0.125;
    sum += texture(samp, vec2(tc.x, tc.y + 3.0 * iBlurSize)).rgb * 0.125;
    sum += texture(samp, vec2(tc.x, tc.y + 4.0 * iBlurSize)).rgb * 0.125;

    return sum;
}

void main() {
    
    vec3 hb = hblur(inputTex0, v_TexCoords, size);
    vec3 vb = vblur(inputTex0, v_TexCoords, size);
    vec3 i = texture(inputTex0, v_TexCoords).rgb;

    vec3 o = (hb + vb + i) / 3.0;
    

    //out_Color = vec4(1.0, 0.0, 0.0, 1.0);
    out_Color = vec4(o, 1.0);
}