#ifndef SCENE_HPP
#define SCENE_HPP

#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

#include <glad/glad.h>
#include "Object.hpp"
#include "PointLight.hpp"
#include "ObjectManager.hpp"
#include "LightsManager.hpp"
#include "Shader.hpp"
#include "Skybox.hpp"

// Scene is a singleton class
class Scene{

// public functions
public:

    static Scene& GetInstance(){ // singleton initialization

        static Scene instance;

        return instance;
    }

    void DeleteScene(){

        // delete objects
        delete Target;
        delete lights;

        delete objManager;
        delete lightsManager;
        
        if (skybox != nullptr)
            delete skybox;

        // delete window
        this->GraphicsApplicationWindow = nullptr;

    }

    // function that creates the scene
    void InitializeScene();
    void InitializeScene2(); // second scene

    // render the whole scene
    void Render();

    void PreDrawBackGround();

    // generates ground
    void GenerateGround(int width, int height, float level, 
                        const std::string diff, const std::string normal,
                        const std::string heightMap, bool inverseH,
                        glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

    // upload all lights to the pipeline
    void UploadLightsToPipeline(Shader* shaderProgram);

// private functions
private: 

    Scene(){};  // inaccessible constructor

// public variables
public:

    // Screen Dimensions
    int ScreenWidth 						= 2560; // set to i.e. 640 
    int ScreenHeight 						= 1440; // set to i.e. 480
    SDL_Window* GraphicsApplicationWindow 	= nullptr;
    SDL_GLContext OpenGLContext			= nullptr;

    // pointer to args
    char ** ArgsPointer = nullptr;
    int NumArgs = 0;

    // Main loop flag
    bool Quit = false; // If this is quit = 'true' then the program terminates.

    // objects
    Object *Target = nullptr;
    ObjectManager *objManager = new ObjectManager();
    LightsManager *lightsManager = new LightsManager();


    // Camera
    Camera MainCamera;

    bool WireFrameMode = false;

    // lights
    PointLight * lights = nullptr;

    // skybox
    Skybox *skybox = nullptr;


};

// External linkage such that the
// global variable is available
// everywhere.
extern Scene gScene;


#endif


