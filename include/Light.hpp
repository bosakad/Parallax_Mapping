
/** @file Light.hpp
 *  @brief Abstract class for lights in the scene
 *  
 *
 *  @author Adam Bosak
 *  @bug No known bugs.
 */


#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "utils.hpp"

class Light{

public: 

    // pos and color
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 color = glm::vec3(0.2f, 0.2f, 0.2f);

    // general Phong light properties
    float ambientStrength = 0.1f;
    float diffusionStrengh = 1.0f;
    float specularStrengh = 0.5f;

public:

    void virtual Draw(){ }; 

};

#endif