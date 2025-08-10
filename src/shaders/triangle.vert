//Triangle Vertex Shader

#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec2 TextCoord;
  
uniform mat4 viewModel;
uniform mat4 projection;

out vec2 v_uv; // specify a color output to the fragment shader

void main()
{
    gl_Position = projection * viewModel * vec4(aPos, 1.0);
    v_uv = TextCoord ;
}