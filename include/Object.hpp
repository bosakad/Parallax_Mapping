
/** @file Object.hpp
 *  @brief Sets up an OpenGL object
 *  
 *  Sets up an OpenGL object. Can load object from .obj file.
 *  Enables drawing
 * 
 *  @author Adam
 *  @bug No known bugs.
 */



#ifndef OBJECT_HPP
#define OBJECT_HPP

// texture loading library
#include "stb_image.h"

// glad
#include <glad/glad.h>

// glm lib
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 

// STL
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

// our libraries
#include <Camera.hpp>
#include "PointLight.hpp"
#include "Shader.hpp"
#include "Texture.hpp"


class Object{

public: 

    // constructor
    Object(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    // destructor
    ~Object();

    // draw object
    void Draw();

    // set up pipeline
    void SetUpPipeline();

    // load data from obj file
    void LoadData_WavefrontOBJ(unsigned int objNumber, std::string filePath);

    // upload generated data to GPU
    void UploadVertices(std::vector<GLfloat> data, std::vector<GLuint> indices);

    // load texture
    void LoadTexture(const std::string& texturePath, unsigned int textureType, bool inverseH = true);


    // set affine transforms
    void SetTranslation(const glm::vec3 &translation);
    void SetRotation(const glm::vec3 &translation);
    void SetScale(const glm::vec3 &scale);
    void Upload_Material_Pipeline(Shader* shader);

    // setter for numberOfElements
    void SetNumberOfElements(unsigned int numberOfElements);

    // setter for usedLight
    void SetUsedLight(int usedLight);
    void SetParallaxMethod(int parallaxMethod);
    void SetContinuousTexture(int continuousTexture);

    // getters
    glm::vec3 & GetTranslation();
    glm::vec3 & GetRotation();
    glm::vec3 & GetScale(); 


private:

    // shader
    Shader *shader;

    // buffers
    GLuint VertexArrayObject = 0;
    GLuint VertexBufferObject = 0;
    GLuint ElementBufferObject = 0;

    // textures
    Texture * diffuseTex; 
    Texture * normalTex;
    Texture * heightTex;
    
    // properties
    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    // numberOfVertices
    unsigned int numberOfElements = 0;

    // specify the used light
    int usedLight = 0;
    int parallaxMethod = -1; // -1 = no parallax, 0 = parallax mapping,
                            //  1 = steep parallax mapping, 2 = parallax occlusion mapping

    int continuousTexture = 0; // 0 = no, 1 = yes (ground tiles)

public: 

    // object number that is beeing rendered
    u_int8_t objectNumber = 0;

    // phong lighting model properties
    glm::vec3 Ka = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 Kd = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 Ks = glm::vec3(0.5f, 0.5f, 0.5f);
    float shininess = 32.0f;    

};


#endif