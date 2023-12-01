/** @file ObjectParser.hpp
 *  @brief Includes parsing function for .obj files
 *  
 * Class with static method parse obj file.
 * Might include other parsers in the future.
 * 
 *  @note might be extended to return also textures and normals 
 *  @author Adam
 *  @bug No known bugs.
 */

#ifndef OBJECT_PARSER_HPP
#define OBJECT_PARSER_HPP

// STD libs
#include <string>
#include <iostream>
#include <tuple>
#include <vector>
#include <fstream>
#include "PPM.hpp"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

// GLAD
#include <glad/glad.h>


/////////////////////////////////////////  Helper structs /////////////////////////////////////

// vertex struct
struct Vertex
{
    float x, y, z;
    Vertex(float _x, float _y, float _z): x(_x),y(_y),z(_z) { } // constructor
};

// normal struct
struct Normal{
    float vx, vy, vz;
    Normal(float _vx, float _vy, float _vz): vx(_vx),vy(_vy),vz(_vz) { } // constructor
};

// texture struct
struct TextureCoords{
    float s, t;
    TextureCoords(float _s, float _t): s(_s), t(_t) { } // constructor
};

//////////////////////////////////////////// Class definition ////////////////////////////

class ObjectParser{

public:

        // function that parses .obj file and save it to verties, indices and normals vector
    static void Parse_WavefrontOBJ(std::vector<GLfloat> &data, 
                                    std::vector<GLuint> &indices, std::string &filePath,
                                    std::string &texturePath);

    // function that loads texture from a given path, also load phong lighting model properties
    static void Load_Textures(std::string &mtlFilePath, int *width, int *height, 
                                    glm::vec3 &Ka, glm::vec3 &Kd, glm::vec3 &Ks, float *shininess,
    std::vector<uint8_t> &diffuseTextureData, std::vector<uint8_t> &normalTextureData); // return values

private:

    static void Compute_tangents_bitangets(std::vector<GLfloat> &data, std::vector<GLuint> &indices);

};




#endif