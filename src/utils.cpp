#include "utils.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include "Light.hpp"
#include "PointLight.hpp"



// vvvvvvvvvvvvvvvvvvv Error Handling Routines vvvvvvvvvvvvvvv
void GLClearAllErrors(){
    while(glGetError() != GL_NO_ERROR){
    }
}

// Returns true if we have an error
bool GLCheckErrorStatus(const char* function, int line){
    while(GLenum error = glGetError()){
        std::cout << "OpenGL Error:" << error 
                  << "\tLine: " << line 
                  << "\tfunction: " << function << std::endl;
        return true;
    }
    return false;
}

#define GLCheck(x) GLClearAllErrors(); x; GLCheckErrorStatus(#x,__LINE__);
// ^^^^^^^^^^^^^^^^^^^ Error Handling Routines ^^^^^^^^^^^^^^^


