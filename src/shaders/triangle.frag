//Triangle Fragment Shader
#version 330 core
  
in vec2 v_uv;
uniform vec2 resolution;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

void main()
{
    vec4 text1 = texture(texture1,v_uv);
    vec4 text2 = texture(texture2,v_uv);
    FragColor = mix(text1,text2,text2.w);
}