/** @file Skybox.hpp
 *  @brief Skybox Class, can initialize the load skybox textures and draw the skybox
 *  
 *
 *  @author Adam Bosak
 *  @bug No known bugs.
 */


#ifndef SKYBOX_HPP
#define SKYBOX_HPP


#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Shader.hpp"
#include <vector>
#include <iostream>

class Skybox{

public:

    Skybox();
    ~Skybox();

    void Draw();
    void LoadSkyboxTextures(std::vector<std::string> facesTexPaths);
    void SetUpPipeline();

    std::vector<float> SkyboxData();

private:

    GLuint textureID;
    GLuint VBO, VAO;
    Shader * skyboxShader;

    // number of vertices
    int numVertices;

};


#endif