#version 430 core

layout (location = 0) in vec3 g_position;
layout (location = 1) in vec2 g_texcoord;

uniform mat4 u_mvp;

out VS_OUT {
  vec2 texcoord;
}vs_out;

void main()
{
   vs_out.texcoord = g_texcoord * 5.0f;
   gl_Position = u_mvp * vec4(g_position, 1.0f);
}