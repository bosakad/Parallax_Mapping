#include "Texture.hpp"


Texture::Texture(){

    // generate diffuse texture
    glGenTextures(1, &this->textureID);

    glBindTexture(GL_TEXTURE_2D, this->textureID);

            // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}


    // delete texture
Texture::~Texture(){

    glDeleteTextures(1, &this->textureID);

}

void Texture::Bind(unsigned int slot){

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->textureID);

}

// upload the data to GPU and generate mipmaps
void Texture::LoadData(GLuint width, GLuint height, unsigned char* data, GLenum format){
        
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

}