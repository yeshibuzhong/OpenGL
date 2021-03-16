//
//  DoubleSampleFragment.fsh
//  OpenglDemo
//
//  Created by wangyahui on 2021/3/15.
//

#version 300 es

layout(location=0) in vec4 a_position;
layout(location=1) in vec2 a_texCoord;
layout(location=2) in vec4 a_color;

uniform mat4 a_mvp;

out vec2 v_texCoord;
out vec4 v_color;

void main()
{
    gl_Position = a_mvp * a_position;
    v_texCoord = a_texCoord;
    v_color = a_color;
}
