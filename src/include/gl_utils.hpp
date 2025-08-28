#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include <sstream>
#include <string>
#include <stdexcept>


// -- Memory utils --

inline constexpr size_t glTypeSize(GLenum glType)
{
    switch(glType)
    {
        case GL_BYTE: 
            return sizeof(GLbyte);
        case GL_UNSIGNED_BYTE: 
            return sizeof(GLubyte);
        case GL_SHORT: 
            return sizeof(GLshort);
        case GL_UNSIGNED_SHORT: 
            return sizeof(GLushort);
        case GL_INT: 
            return sizeof(GLint);
        case GL_UNSIGNED_INT: 
            return sizeof(GLuint);
        case GL_FLOAT: 
            return sizeof(GLfloat);
        case GL_DOUBLE:
            return sizeof(GLdouble);
        case GL_HALF_FLOAT:
            return sizeof(GLhalf);
    #ifdef GL_FIXED
        case GL_FIXED:
            return sizeof(GLfixed);
    #endif
        default:
            return 0;
    }
}

// -- Error handling --

//derive from openGL doc : registry.khronos.org/OpenGL-Refpages/gl4/html/glGetError.xhtml
static const char * MSG_GL_INVALID_ENUM =                  "Error code [0x0500] :\t An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n";
static const char * MSG_GL_INVALID_VALUE =                 "Error code [0x0501] :\t A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n";
static const char * MSG_GL_INVALID_OPERATION =             "Error code [0x0502] :\t The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n";
#ifdef MSG_GL_STACK_UNDERFLOW
    static const char * MSG_GL_STACK_UNDERFLOW =           "Error code [0x0504] :\t An attempt has been made to perform an operation that would cause an internal stack to underflow.\n";
#endif
#ifdef MSG_GL_STACK_OVERFLOW
    static const char * MSG_GL_STACK_OVERFLOW =            "Error code [0x0503] :\t An attempt has been made to perform an operation that would cause an internal stack to overflow.\n";
#endif
static const char * MSG_GL_OUT_OF_MEMORY =                 "Error code [0x0505] :\t There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n";
static const char * MSG_GL_INVALID_FRAMEBUFFER_OPERATION = "Error code [0x0506] :\t The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n";
#define MSG_UNKNOWN_GL_ERROR                        "Error code [0x%04X] :\t An unknown error has been detected.\n"

// @returns the error code in case of an error, 0 if no error
inline GLenum logOnGlError(std::ostream& stream,const char * errorMsg){
    GLenum error = glGetError();
    int returnCode = 0;
    if(error != GL_NO_ERROR){
        returnCode = error;
        std::string throwError(errorMsg);
        throwError += '\n';
        do{

            switch (error)
            {
                case GL_INVALID_ENUM                    : throwError += MSG_GL_INVALID_ENUM; break; 
                case GL_INVALID_VALUE                   : throwError += MSG_GL_INVALID_VALUE; break; 
                case GL_INVALID_OPERATION               : throwError += MSG_GL_INVALID_OPERATION; break; 
                #ifdef MSG_GL_STACK_UNDERFLOW
                    case GL_STACK_OVERFLOW              : throwError += MSG_GL_STACK_OVERFLOW; break;  
                #endif        
                #ifdef MSG_GL_STACK_OVERFLOW
                    case GL_STACK_UNDERFLOW             : throwError += MSG_GL_STACK_UNDERFLOW; break; 
                #endif
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

        stream << (throwError);
    }
    return returnCode;
}

inline void throwOnGlError(const char * errorMsg){
    std::stringstream errorStream;

    if(logOnGlError(errorStream,errorMsg)){
        std::string error = errorStream.str();
        throw std::runtime_error(error);
    }
}

#define glCall(x) \
    x;{\
    std::stringstream errorStream;\
    errorStream << "GL ERROR in " << #x << "at :" << __FILE__ << "::" << __LINE__ << "\n";\
    if(logOnGlError(errorStream, ""))\
        throw std::runtime_error(errorStream.str());}

// -- Math --

template <typename T>
constexpr glm::qua<T> angleToQuaternion(T angle, glm::vec<3,T> axis){
    
    T half = angle * T(0.5);
    T sinHalf = sin(half);

    glm::qua<T> q;
    q.w = cos(half);
    q.x = axis.x * sinHalf;
    q.y = axis.y * sinHalf;
    q.z = axis.z * sinHalf;

    return q;
}

template <typename T>
constexpr glm::vec<3,T> quaternionToEuler(const glm::qua<T>& q) {
    glm::vec<3,T> angles;

    // roll (X axis)
    T sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    T cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.x = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (Y axis)
    T sinp = 2 * (q.w * q.y - q.z * q.x);
    if (std::abs(sinp) >= 1)
        angles.y = std::copysign(glm::half_pi<T>(), sinp); // clamp to 90°
    else
        angles.y = std::asin(sinp);

    // yaw (Z axis)
    T siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    T cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.z = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}


inline glm::vec3 palette(float t) {
    return 0.5f + 0.5f * cos(6.2831f * (glm::vec3(0.0, 0.33, 0.67) + t));
}