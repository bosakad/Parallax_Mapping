#include "LightsManager.hpp"
#include "Shader.hpp"


LightsManager::LightsManager(){ }

LightsManager::~LightsManager(){ 

    for (auto &light : lights){   // delete all lights
        delete light;
    }

}

void LightsManager::RenderAllLights(){

    for (auto &light : lights){   // render all lights
        light->Draw();
    }

}


void LightsManager::AddLight(Light *light){

    lights.push_back(light);
}



void LightsManager::UploadLightsToPipeline(Shader* shaderProgram){

    unsigned int lightIndex = 0;

    for (auto &light : lights){   // upload all lights to the pipeline
        
        if ( instanceof<PointLight>(light) ){
            ((PointLight *)light)->Upload_PointLight_Pipeline(shaderProgram, lightIndex); 
        }

        lightIndex++;
    }

}
