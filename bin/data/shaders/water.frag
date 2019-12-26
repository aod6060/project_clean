#version 400

uniform sampler2D tex0;
uniform float texScale;
uniform float timeDelta;

in vec2 v_TexCoords;
in vec3 v_Normals;

out vec4 out_Color;

void main() {

    vec2 tc = vec2(v_TexCoords.x + timeDelta, v_TexCoords.y) * texScale;
    vec2 tc2 = vec2(v_TexCoords.x + timeDelta * 2.0, v_TexCoords.y) * texScale * 0.5;
    vec2 tc3 = vec2(v_TexCoords.x + timeDelta * 4.0, v_TexCoords.y) * texScale * 0.25;

    vec3 color1 = texture(tex0, tc).rgb;
    vec3 color2 = texture(tex0, tc2).rgb;
    vec3 color3 = texture(tex0, tc3).rgb;

    vec3 color = mix(color1, color2, 0.75);
    color = mix(color, color3, 0.75);

    //out_Color = texture(tex0, tc);
    out_Color = vec4(color, 1.0);
    //out_Color = vec4(v_TexCoords, 0.0, 1.0);
}