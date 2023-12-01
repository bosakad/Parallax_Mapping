#include "Scene.hpp"
#include "PointLight.hpp"
#include "Geometry.hpp"
#include "Skybox.hpp"

Scene gScene = Scene::GetInstance();    // create singleton global class


void Scene::InitializeScene(){
                                            // add walls

    float xWallShift = -2.3f;

    // define transforms
    glm::vec3 trans = glm::vec3(0.0f, 0.0f, -3.0f);
    glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
                        
    // define texture paths
    const std::string diffuseMapPath = "common/objects/brickWall/texture.jpg";
    const std::string normalMapPath = "common/objects/brickWall/bricks_normal.jpg";
    const std::string heightMapPath = "common/objects/brickWall/displacement.jpg";

    // plain wall
    Object * wall1 = CreatePlane("./shaders/vert_Plain.glsl", "./shaders/frag_Plain.glsl", 
                                    trans + glm::vec3(5*xWallShift, 0.0f, 0.0f), rot, scale, 
                                    diffuseMapPath, normalMapPath, heightMapPath, false, 0);
    

    // wall with normal texture
    Object * wall2 = CreatePlane("./shaders/vert_NormalMap.glsl", "./shaders/frag_NormalMap.glsl", 
                                    trans + glm::vec3(4*xWallShift, 0.0f, 0.0f), rot, scale, 
                                    diffuseMapPath, normalMapPath, heightMapPath, false, 0);
                                
    // wall with parallax mapping
    Object * wall3 = CreatePlane("./shaders/vert_NormalMap.glsl", "./shaders/frag_Parallax.glsl", 
                                    trans + glm::vec3(3*xWallShift, 0.0f, 0.0f), rot, scale, 
                                    diffuseMapPath, normalMapPath, heightMapPath, false, 0);
    
        // wall with steep parallax mapping
    Object * wall4 = CreatePlane("./shaders/vert_NormalMap.glsl", "./shaders/frag_Parallax.glsl", 
                                    trans + glm::vec3(2*xWallShift, 0.0f, 0.0f), rot, scale, 
                                    diffuseMapPath, normalMapPath, heightMapPath, false, 0);

            // wall with parallax occlusion mapping
    Object * wall5 = CreatePlane("./shaders/vert_NormalMap.glsl", "./shaders/frag_Parallax.glsl", 
                                    trans + glm::vec3(1*xWallShift, 0.0f, 0.0f), rot, scale, 
                                    diffuseMapPath, normalMapPath, heightMapPath, false, 0);

                // wall with parallax occlusion mapping
    Object * wall6 = CreatePlane("./shaders/vert_NormalMap.glsl", "./shaders/frag_Parallax.glsl", 
                                    trans + glm::vec3(0*xWallShift, 0.0f, 0.0f), rot, scale, 
                                    diffuseMapPath, normalMapPath, heightMapPath, false, 0);

    // set parallax mapping methods per wall
    wall1->SetParallaxMethod(-1); // no parallax (plain)
    wall2->SetParallaxMethod(-1); // no parallax (normal)
    wall3->SetParallaxMethod(0); // parallax mapping
    wall4->SetParallaxMethod(1); // steep parallax mapping
    wall5->SetParallaxMethod(2); // parallax occlusion mapping
    wall6->SetParallaxMethod(3); // parallax occlusion mapping binary

    // add walls to the scene
    this->objManager->AddObject(wall1);
    this->objManager->AddObject(wall2);
    this->objManager->AddObject(wall3);
    this->objManager->AddObject(wall4);
    this->objManager->AddObject(wall5);
    this->objManager->AddObject(wall6);


	                                // create lights
	PointLight* light1 = new PointLight(glm::vec3(-0.5f + 5*xWallShift, 0.5f, -2.5f), glm::vec3(0.9f, 0.9f, 0.9f));
	light1->Initialize_HardCoded();

    PointLight* light2 = new PointLight(glm::vec3(-0.5f + 4*xWallShift, 0.5f, -2.5f), glm::vec3(0.9f, 0.9f, 0.9f));
	light2->Initialize_HardCoded();

    PointLight* light3 = new PointLight(glm::vec3(-0.5f + 3*xWallShift, 0.5f, -2.5f), glm::vec3(0.9f, 0.9f, 0.9f));
	light3->Initialize_HardCoded();

    PointLight* light4 = new PointLight(glm::vec3(-0.5f + 2*xWallShift, 0.5f, -2.5f), glm::vec3(0.9f, 0.9f, 0.9f));
	light4->Initialize_HardCoded();

    PointLight* light5 = new PointLight(glm::vec3(-0.5f + 1*xWallShift, 0.5f, -2.5f), glm::vec3(0.9f, 0.9f, 0.9f));
	light5->Initialize_HardCoded();

    PointLight* light6 = new PointLight(glm::vec3(-0.5f + 0*xWallShift, 0.5f, -2.5f), glm::vec3(0.9f, 0.9f, 0.9f));
	light6->Initialize_HardCoded();

    // add lights to the scene
    this->lightsManager->AddLight(light1);
    this->lightsManager->AddLight(light2);
    this->lightsManager->AddLight(light3);
    this->lightsManager->AddLight(light4);
    this->lightsManager->AddLight(light5);
    this->lightsManager->AddLight(light6);

    // set lights per each wall
    wall1->SetUsedLight(0);
    wall2->SetUsedLight(1);
    wall3->SetUsedLight(2);
    wall4->SetUsedLight(3);
    wall5->SetUsedLight(4);
    wall6->SetUsedLight(5);


}

// scene 2
void Scene::InitializeScene2(){

                                // create skybox
    // define the skybox texture
    // std::vector<std::string> facesTexPaths = {   // uncomment to see normal skybox
    //     "common/textures/skyboxes/skybox/right.jpg",
    //     "common/textures/skyboxes/skybox/left.jpg",
    //     "common/textures/skyboxes/skybox/top.jpg",
    //     "common/textures/skyboxes/skybox/bottom.jpg",
    //     "common/textures/skyboxes/skybox/front.jpg",
    //     "common/textures/skyboxes/skybox/back.jpg"
    // };

    std::vector<std::string> facesTexPaths = {
        "common/textures/skyboxes/dawn/vz_dawn_right.png",
        "common/textures/skyboxes/dawn/vz_dawn_left.png",
        "common/textures/skyboxes/dawn/vz_dawn_up.png",
        "common/textures/skyboxes/dawn/vz_dawn_down.png",
        "common/textures/skyboxes/dawn/vz_dawn_front.png",
        "common/textures/skyboxes/dawn/vz_dawn_back.png"
    };
    
    this->skybox = new Skybox();
    this->skybox->LoadSkyboxTextures(facesTexPaths);


                                // define texture paths
    // define transforms
    glm::vec3 trans = glm::vec3(-5.0f, 0.0f, -13.0f);
    glm::vec3 rot = glm::vec3(-90.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    // generate dirt ground under the house
    std::string diffuseMapPath = "common/textures/dirt_path/Ground_Dirt_007_basecolor.jpg";
    std::string normalMapPath = "common/textures/dirt_path/Ground_Dirt_007_normal.jpg";
    std::string heightMapPath = "common/textures/dirt_path/Ground_Dirt_007_height.png";

    GenerateGround(5, 5, -1.25f, diffuseMapPath, normalMapPath, heightMapPath, true,
                    trans, rot, scale);

    // generate jungle path
    trans = glm::vec3(-5.0f, 0.0f, -3.0f);
    diffuseMapPath = "common/textures/jungle_path/Jungle_Floor_001_basecolor.jpg";
    normalMapPath = "common/textures/jungle_path/Jungle_Floor_001_normal.jpg";
    heightMapPath = "common/textures/jungle_path/Jungle_Floor_001_height.png";

    GenerateGround(2, 4, -1.25f, diffuseMapPath, normalMapPath, heightMapPath, true,
                    trans, rot, scale);

    trans = glm::vec3(1.0f, 0.0f, -3.0f);
    GenerateGround(2, 4, -1.25f, diffuseMapPath, normalMapPath, heightMapPath, true,
                    trans, rot, scale);


    // generate stone path
    trans = glm::vec3(-1.0f, 0.0f, -3.0f);

    diffuseMapPath = "common/textures/stone_path/Stylized_Stone_Floor_005_basecolor.jpg";
    normalMapPath = "common/textures/stone_path/Stylized_Stone_Floor_005_normal.jpg";
    heightMapPath = "common/textures/stone_path/Stylized_Stone_Floor_005_height.png";

    GenerateGround(1, 4, -1.25f, diffuseMapPath, normalMapPath, heightMapPath, true,
                    trans, rot, scale);

                                // add house
	Object* house = new Object("./shaders/vert_NormalMap.glsl", "./shaders/frag_NormalMap.glsl");

	// set affine transformation
	house->SetTranslation(glm::vec3(-0.5f, 0.15f, -7.6f));
	house->SetRotation(glm::vec3(0.0f, 40.0f, 0.0f));
	house->SetScale(glm::vec3(2.3f, 2.3f, 2.3f));

    house->LoadData_WavefrontOBJ(0, "./common/objects/house/house_obj.obj");

    // add object to the scene
    this->objManager->AddObject(house);
    house->SetUsedLight(0);

	                                // create lights
	PointLight* light1 = new PointLight(glm::vec3(15.0f, 14.0f, -4.5f), glm::vec3(0.9f, 0.9f, 0.9f));
	light1->Initialize_HardCoded();
    light1->color = glm::vec3(0.88f, 0.53f, 0.34f);

    // add lights to the scene
    this->lightsManager->AddLight(light1);


}


void Scene::PreDrawBackGround(){
	
    glEnable(GL_DEPTH_TEST);                    
    glEnable(GL_CULL_FACE); // enable culling for optimization

    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, this->ScreenWidth, this->ScreenHeight);
    glClearColor( 0.1f, 0.1f, 0.1f, 1.f );

    //Clear color buffer and Depth Buffer
  	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

}

void Scene::Render(){

    glDepthFunc(GL_LEQUAL);  // set depth function to less than AND equal for skybox depth trick.

    // render objects
    this->objManager->RenderAllObjects();

    // render lights
    this->lightsManager->RenderAllLights();

    // render the skybox last - optimization - early depth test will disregard most of the
    // skybox fragments - since they have depth 1.0f (we hardcoded its .z to w)
    if (this->skybox != nullptr){ 
        this->skybox->Draw();
    }

}

void Scene::UploadLightsToPipeline(Shader* shaderProgram){

    // upload all lights to the pipeline
    this->lightsManager->UploadLightsToPipeline(shaderProgram);

}


void Scene::GenerateGround(int width, int height, float level, 
                        const std::string diff, const std::string normal,
                        const std::string heightMap, bool inverseH,
                        glm::vec3 pos, glm::vec3 rot, glm::vec3 scale){

    const float xShift = scale[0];
    const float zShift = scale[2];

    for (unsigned int x = 0; x < width; x++){
        for (unsigned int z = 0; z < height; z++){

        
            Object * plane = CreatePlane("./shaders/vert_NormalMap.glsl", "./shaders/frag_Parallax.glsl", 
                                            pos + glm::vec3(x*2*xShift, level, z*2*zShift), rot, scale, 
                                            diff, normal, heightMap, true, 1);


                    // set parallax mapping methods per wall
            plane->SetParallaxMethod(3); // parallax occlusion mapping binary

            // add walls to the scene
            this->objManager->AddObject(plane);

                // set lights per each wall
            plane->SetUsedLight(0);

        }
    }
}


