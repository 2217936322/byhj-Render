#version 430 core

void main(void)
{
   vec4 VertexData[4] = vec4[](vec4(-0.5, -0.5, 0.5, 1.0),
                               vec4( 0.5, -0.5, 0.5, 1.0),
                               vec4(-0.5,  0.5, 0.5, 1.0),
                               vec4( 0.5,  0.5, 0.5, 1.0) );
    gl_Position = VertexData[gl_VertexID];

}