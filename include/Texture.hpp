/** @file Texture.hpp
 *  @brief Sets up an OpenGL texture
 *  
 * Sets up an OpenGL texture.
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

#ifndef TEXTURE_HPP
#define TEXTURE_HPP


#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

#include <glad/glad.h>

class Texture{

    public:

        Texture();
        ~Texture();

        // bind texture
        void Bind(unsigned int slot);

        // load data to gpu
        void LoadData(GLuint width, GLuint height, unsigned char* data, GLenum format);

    private:

        GLuint textureID = 0;

};

#endif