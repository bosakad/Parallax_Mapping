#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>



// vvvvvvvvvvvvvvvvvvv Error Handling Routines vvvvvvvvvvvvvvv
void GLClearAllErrors();

// Returns true if we have an error
bool GLCheckErrorStatus(const char* function, int line);

#define GLCheck(x) GLClearAllErrors(); x; GLCheckErrorStatus(#x,__LINE__);
// ^^^^^^^^^^^^^^^^^^^ Error Handling Routines ^^^^^^^^^^^^^^^

#endif
