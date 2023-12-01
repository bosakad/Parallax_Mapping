#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "Light.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Object.hpp"

// Class representing point light in the scene. Is involved in rendering of some object - 'bulb'

class PointLight : public Light {

// public functions
public: 

    PointLight(glm::vec3 position, glm::vec3 color);
    ~PointLight();

    // draw function
    void Draw() override;

    // function that can be called after setting up OpenGL - hardcoded cube
    void Initialize_HardCoded();

    // rotation
    glm::mat4 RotateAroundPoint(glm::vec3 center, float radius);

    // uploads the struct to a pipeline
    void Upload_PointLight_Pipeline(Shader* targetShader, unsigned int lightIndex);

// private functions
private:

    void SetUpPipeLine();

// public variables
public: 

    // rendering attributes
    Shader * shader = nullptr;
    GLuint VAO = 0;
    GLuint VBO = 0;

    // number of elements to draw
    unsigned int numberOfElements = 0;

    // scale
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    // rotation
    float rotation = 0;

    // attenuation properties
    float constant = 1.0f;
    float linear = 0.3f;
    float quadratic = 0.4f;

};

#endif