/*
    test.frag

    This is a simple glsl file to test graph functionality.
    It might be the default.
*/
#version 400

//uniform sampler2D tex0;
uniform float texScale;
uniform sampler2D blendMap;
uniform sampler2D channelBlack;
uniform sampler2D channelRed;
uniform sampler2D channelGreen;
uniform sampler2D channelBlue;

in vec2 v_TexCoords;
in vec3 v_Normals;
out vec4 out_Color;

void main() {

    vec3 bm = texture(blendMap, v_TexCoords).rgb;

    vec3 colorBlack = texture(channelBlack, v_TexCoords * texScale).rgb * (1.0 - (bm.r + bm.g + bm.b));
    vec3 colorRed = texture(channelRed, v_TexCoords * texScale).rgb;
    vec3 colorGreen = texture(channelGreen, v_TexCoords * texScale).rgb;
    vec3 colorBlue = texture(channelBlue, v_TexCoords * texScale).rgb;

    vec3 color = colorRed * bm.r + colorGreen * bm.g + colorBlue * bm.b + colorBlack;

    out_Color = vec4(color, 1.0);
}