cmake_minimum_required(VERSION 3.15)

project(LearnOpengl VERSION 1.0)

# Specify the C++ standard
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_20)

# Compiler warning flags
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>;-g>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# Add GLFW
add_subdirectory(lib/glfw)

# Add glm
add_subdirectory(lib/glm)

# Main project
add_executable(MeLearningOpengl 
    src/glad.c

    src/main.cpp

    src/App.cpp
    src/Program.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/Movable.cpp
    src/Texture.cpp

)

target_include_directories(MeLearningOpengl PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/lib/"
    "${PROJECT_SOURCE_DIR}/lib/glfw/include"
    "${PROJECT_SOURCE_DIR}/lib/glm"

    "${PROJECT_SOURCE_DIR}/src/include"
    "${PROJECT_SOURCE_DIR}/src/headers"
)

target_link_libraries(MeLearningOpengl PUBLIC
    glfw
    glm
    compiler_flags
    dl
)

# Shaders source
set(SHADER_DIR "${PROJECT_SOURCE_DIR}/src/shaders")
set(GENERATED_DIR "${CMAKE_BINARY_DIR}/src/generated_shaders")
file(MAKE_DIRECTORY "${GENERATED_DIR}")

function(embed_shader shader_file var_name)
    get_filename_component(shader_name ${shader_file} NAME)
    set(output_file "${GENERATED_DIR}/${shader_name}.cpp")

    file(READ ${shader_file} shader_content)
    # Escape backslashes and quotes for C++ string
    string(REPLACE "\\" "\\\\" shader_content "${shader_content}")
    string(REPLACE "\"" "\\\"" shader_content "${shader_content}")
    string(REPLACE "\n" "\\n\"\n\"" shader_content "${shader_content}")

    file(WRITE ${output_file} "const char* ${var_name} = \"//${var_name} \\n\"\n\"${shader_content}\";\n")
    set_source_files_properties(${output_file} PROPERTIES GENERATED TRUE)
    target_sources(MeLearningOpengl PRIVATE ${output_file})    
endfunction()

embed_shader("${SHADER_DIR}/cube.frag" "cubeFragShaderSrc")
embed_shader("${SHADER_DIR}/cube.vert" "cubeVertShaderSrc")

embed_shader("${SHADER_DIR}/light.frag" "lightFragShaderSrc")