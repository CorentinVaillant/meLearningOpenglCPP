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

# Main project
add_executable(MeLearningOpengl 
    src/glad.c

    src/main.cpp
    src/app.cpp
    src/Program.cpp
    src/Shader.cpp

)

target_include_directories(MeLearningOpengl PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/lib/glfw/include"
    "${PROJECT_SOURCE_DIR}/src/include"
    "${PROJECT_SOURCE_DIR}/src/headers"
)

target_link_libraries(MeLearningOpengl PUBLIC
    glfw
    compiler_flags
    dl
)
