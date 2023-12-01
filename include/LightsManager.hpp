
/** @file LightsManager.hpp
 *  @brief Class for maintaining lights in the scene
 *  
 *  Class for maintaining lights in the scene.
 *
 *  @author Adam Bosak
 *  @bug No known bugs.
 */

#ifndef LIGHTSMANAGER_HPP
#define LIGHTSMANAGER_HPP

#include <vector>
#include "Light.hpp"
#include "PointLight.hpp"
#include "Shader.hpp"

class LightsManager{

public:

    LightsManager();
    ~LightsManager();

    void RenderAllLights();

    void AddLight(Light *light);

    void UploadLightsToPipeline(Shader* shaderProgram);
    
private:
    
    // self defined instanceof function
    template<typename Base, typename T>
    inline bool instanceof(const T *ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
    }

private: 

    std::vector<Light *> lights;

};



#endif