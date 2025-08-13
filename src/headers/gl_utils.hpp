#pragma once

#include <glad/glad.h>

#include <string>
#include <stdexcept>

//derive from openGL doc : registry.khronos.org/OpenGL-Refpages/gl4/html/glGetError.xhtml
static const char * MSG_GL_INVALID_ENUM =                  "Error code [0x0500] :\t An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n";
static const char * MSG_GL_INVALID_VALUE =                 "Error code [0x0501] :\t A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n";
static const char * MSG_GL_INVALID_OPERATION =             "Error code [0x0502] :\t The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n";
// static const char * MSG_GL_STACK_OVERFLOW =                "Error code [0x0503] :\t An attempt has been made to perform an operation that would cause an internal stack to overflow.\n";
// static const char * MSG_GL_STACK_UNDERFLOW =               "Error code [0x0504] :\t An attempt has been made to perform an operation that would cause an internal stack to underflow.\n";
static const char * MSG_GL_OUT_OF_MEMORY =                 "Error code [0x0505] :\t There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n";
static const char * MSG_GL_INVALID_FRAMEBUFFER_OPERATION = "Error code [0x0506] :\t The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n";
#define MSG_UNKNOWN_GL_ERROR                        "Error code [0x%04X] :\t An unknown error has been detected.\n"

inline void throwOnGlError(const char * errorMsg){
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::string throwError(errorMsg);
        throwError += '\n';
        do{

            switch (error)
            {
                case GL_INVALID_ENUM                    : throwError += MSG_GL_INVALID_ENUM; break; 
                case GL_INVALID_VALUE                   : throwError += MSG_GL_INVALID_VALUE; break; 
                case GL_INVALID_OPERATION               : throwError += MSG_GL_INVALID_OPERATION; break; 
                // case GL_STACK_OVERFLOW                  : throwError += MSG_GL_STACK_OVERFLOW; break;             
                // case GL_STACK_UNDERFLOW                 : throwError += MSG_GL_STACK_UNDERFLOW; break; 
                case GL_OUT_OF_MEMORY                   : throwError += MSG_GL_OUT_OF_MEMORY; break; 
                case GL_INVALID_FRAMEBUFFER_OPERATION   : throwError += MSG_GL_INVALID_FRAMEBUFFER_OPERATION; break; 
                default: 
                    char buffer[128];
                    snprintf(buffer, sizeof(buffer),MSG_UNKNOWN_GL_ERROR, error);
                    throwError += buffer;
                    break;
            }
            error = glGetError();
        }while(error != GL_NO_ERROR);   

        throw std::runtime_error(throwError);
    }
}
