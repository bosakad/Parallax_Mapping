/** @file Geometry.hpp
 *  @brief Sets up an OpenGL object using geometry
 *  
 *  Sets up an OpenGL object using geometry.
 * 
 *  @author Adam
 *  @bug No known bugs.
 */


#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "Object.hpp"

// STL
#include <string>
#include <iostream>
#include <vector>
#include <fstream>


// create plane 
Object * CreatePlane(const std::string& vertexShaderPath, const std::string& fragmentShaderPath,
                     const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale,
                     const std::string& texturePath, const std::string& normalMapPath, 
                     const std::string& heightMapPath, bool inverseH, int continousTexture);



#endif