/** @file Shader.hpp
 *  @brief Sets up an OpenGL shader
 *  
 * Sets up an OpenGL shader. Can load shader from .glsl file.
 * 
 *  @author Adam
 *  @bug No known bugs.
 */

#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

#include <glad/glad.h>
#include <string>   
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#ifndef SHADER_HPP
#define SHADER_HPP

class Shader{

public:

    Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    ~Shader();

    // binds the shader
    void Bind();

    // unbinds the shader
    void Unbind();


    /**
     *  Uploads to a current pipeline shaderID a uniform variable under "name" a 1 int value
     * 
     * @param name
     * @param elementValue
    */
    void Upload_Uniform1i_Pipeline(const char * name, int elementValue);

    /**
     *  Uploads to a current pipeline shaderID a uniform variable under "name" a 1 float value
     * 
     * @param name
     * @param elementValue
    */
    void Upload_Uniform1f_Pipeline(const char * name, float elementValue);

    /**
     *  Uploads to a current pipeline shaderID a uniform variable under "name" a 3 float value
     * 
     * @param name
     * @param ev1
     * @param ev2
     * @param ev3
     * 
    */
    void Upload_Uniform3f_Pipeline(const char * name, float ev1, float ev2, float ev3);


    /**
     *  Uploads to a current pipeline shaderID a uniform variable under "name" a mat4 matrix
     * 
     * @param name
     * @param martix
     * 
    */
    void Upload_Uniform_MAT4fv_Pipeline(const char * name, glm::mat4 &matrix);

    /**
     *  Uploads to a current pipeline shaderID a uniform variable under "name" a mat3 matrix
     * 
     * @param name
     * @param martix
     * 
    */
    void Upload_Uniform_MAT3fv_Pipeline(const char * name, glm::mat3 &matrix);

private:

    // load shader and return it as a string
    std::string LoadShaderAsString(const std::string& filename);

    // compile shader
    GLuint CompileShader(GLuint type, const std::string& source);

    // create shader program
    GLuint CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);


public: 

    GLuint shaderID;

};


#endif

