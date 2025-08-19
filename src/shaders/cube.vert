#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
uniform mat4 viewModel;
uniform mat4 projection;

void main()
{
    gl_Position = projection * viewModel * vec4(aPos, 1.0);
    // gl_Position = vec4(aPos, 1.0);
}