#pragma once

// - Meshes cst -

float vertices[] = {
    //Positions         //Texture coord
     0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f    // top left 
};


unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  


// - Shaders cst -
extern const char* triangleFragShaderSrc;
extern const char* triangleVertShaderSrc;
