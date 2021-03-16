//
//  DoubleSampleFragment.fsh
//  OpenglDemo
//
//  Created by wangyahui on 2021/3/15.
//

#version 300 es

precision mediump float;

in vec2 v_texCoord;
in vec4 v_color;
uniform sampler2D s_texture;
uniform sampler2D s_texture1;
layout(location = 0) out vec4 outColor;

void main() {
    vec4 baseColor;
    vec4 baseColor1;
    baseColor = texture(s_texture, v_texCoord);
    baseColor1 = texture(s_texture1, v_texCoord);
    outColor = baseColor * (baseColor1 + 0.25) * v_color;
}
