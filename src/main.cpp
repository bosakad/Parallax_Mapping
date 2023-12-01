/* Compilation on Linux: 
 g++ -std=c++17 ./src/*.cpp -o prog -I ./include/ -I./../common/thirdparty/ -lSDL2 -ldl
*/

// Image loader library
#define STB_IMAGE_IMPLEMENTATION

// Third Party Libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 

// C++ Standard Template Library (STL)
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Our libraries
#include "Camera.hpp"
#include "Object.hpp"
#include "PointLight.hpp"

// import utils - include shader parsing and error checks
#include "utils.hpp"

// Import singleton scene class
#include "Scene.hpp"

/**
* Initialization of the graphics application. Typically this will involve setting up a window
* and the OpenGL Context (with the appropriate version)
*
* @return void
*/
void InitializeProgram(){
	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		exit(1);
	}
	
	// Setup the OpenGL Context
	// Use OpenGL 4.1 core or greater
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	// We want to request a double buffer for smooth updating.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create an application window using OpenGL that supports SDL
	gScene.GraphicsApplicationWindow = SDL_CreateWindow( "Parallax Mapping",
													0,
													0,
													gScene.ScreenWidth,
													gScene.ScreenHeight,
													SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

	// Check if Window did not create.
	if( gScene.GraphicsApplicationWindow == nullptr ){
		std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
		exit(1);
	}

	// Create an OpenGL Graphics Context
	gScene.OpenGLContext = SDL_GL_CreateContext( gScene.GraphicsApplicationWindow );
	if( gScene.OpenGLContext == nullptr){
		std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
		exit(1);
	}

	// Initialize GLAD Library
	if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
		std::cout << "glad did not initialize" << std::endl;
		exit(1);
	}
	

}


/**
* Helper Function to get OpenGL Version Information
*
* @return void
*/
void getOpenGLVersionInfo(){
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}


/**
* Function called in the Main application loop to handle user input
*
* @return void
*/
void Input(){

    // Two static variables to hold the mouse position
    static int mouseX= gScene.ScreenWidth/2;
    static int mouseY= gScene.ScreenHeight/2; 

	// Event handler that handles various events in SDL
	// that are related to input and output
	SDL_Event e;
	//Handle events on queue
	while(SDL_PollEvent( &e ) != 0){
		// If users posts an event to quit
		// An example is hitting the "x" in the corner of the window.
		if(e.type == SDL_QUIT){
			std::cout << "Leaving MainApplicationLoop()" << std::endl;
			gScene.Quit = true;
		}
        if(e.type == SDL_KEYDOWN){

			// edge case all keys
			if(e.key.keysym.sym == SDLK_ESCAPE){
				gScene.Quit = true;
			}

			// wireframe mode
			if(e.key.keysym.sym == SDLK_TAB){ 
				if (gScene.WireFrameMode){
					glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
					gScene.WireFrameMode = false;
				}
				else{
					glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
					gScene.WireFrameMode = true;
				}
			}
			if(e.key.keysym.sym == SDLK_q){	// quit on q	
				gScene.Quit = true;
			}
			

        }
        if(e.type==SDL_MOUSEMOTION){
            // Capture the change in the mouse position
            mouseX+=e.motion.xrel;
            mouseY+=e.motion.yrel;
            gScene.MainCamera.MouseLook(mouseX,mouseY);
        }
	}

    // Retrieve keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);
	float cameraSpeed = 0.1f / 2;	
    if (state[SDL_SCANCODE_W]) {
        gScene.MainCamera.MoveForward(cameraSpeed);
    }
    if (state[SDL_SCANCODE_S]) {
        gScene.MainCamera.MoveBackward(cameraSpeed);
    }
    if (state[SDL_SCANCODE_A]) {
        gScene.MainCamera.MoveLeft(cameraSpeed);
    }
    if (state[SDL_SCANCODE_D]) {
        gScene.MainCamera.MoveRight(cameraSpeed);
    }
	if (state[SDL_SCANCODE_SPACE]) {
        gScene.MainCamera.MoveUp(cameraSpeed);
    }
	if (state[SDL_SCANCODE_LCTRL]) {
        gScene.MainCamera.MoveDown(cameraSpeed);
    }

}


/**
* Main Application Loop
* This is an infinite loop in our graphics application
*
* @return void
*/
void MainLoop(){

    SDL_WarpMouseInWindow(gScene.GraphicsApplicationWindow, gScene.ScreenWidth/2, gScene.ScreenHeight/2);
    SDL_SetRelativeMouseMode(SDL_TRUE);


	unsigned int milisecondsPerFrame = 16; // results in 60 frames per second

	// While application is running
	while(!gScene.Quit){
		
		// Type of start of frame
		Uint32 start = SDL_GetTicks();

		// Handle Input
		Input();
		
		// render background
		gScene.PreDrawBackGround();

		// render objects
		gScene.Render();

		Uint32 elapsedTime = SDL_GetTicks() - start;
		if(elapsedTime < milisecondsPerFrame){
			SDL_Delay(milisecondsPerFrame - elapsedTime); // fast computers should wait
		}

		//Update screen of our specified window
		SDL_GL_SwapWindow(gScene.GraphicsApplicationWindow);
	}
}



/**
* The last function called in the program
* This functions responsibility is to destroy any global
* objects in which we have create dmemory.
*
* @return void
*/
void CleanUp(){
	//Destroy our SDL2 Window
	SDL_DestroyWindow(gScene.GraphicsApplicationWindow );

	gScene.DeleteScene();

	//Quit SDL subsystems
	SDL_Quit();
}

/**
* The entry point into our C++ programs.
*
* @return program status
*/
int main( int argc, char** argv ){
    std::cout << "Mouse to rotate, WASD to move around, tab for wireframe, q/ESC to exit\n";

	// 1. Setup the graphics program
	InitializeProgram();

	// 2. setup the scene
	// gScene.InitializeScene();	// uncomment this line to see wall scene
	gScene.InitializeScene2();	// uncomment this line to see Parallax at Dawn
	
	// 3. Call the main application loop
	MainLoop();	

	// 4. Call the cleanup function when our program terminates
	CleanUp();

	return 0;
}
