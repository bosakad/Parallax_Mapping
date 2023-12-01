#include "Skybox.hpp"
#include "Scene.hpp"

Skybox::Skybox(){

                                // generate the texture
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);

    // set the texture paramaters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

                                // load the data vertices
    std::vector<float> data = SkyboxData();

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);

    // set the number of vertices
    this->numVertices = data.size();

                                // create gpu shader program
    skyboxShader = new Shader("shaders/vert_skybox.glsl", "shaders/frag_skybox.glsl");

}



Skybox::~Skybox(){

    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &textureID);

    delete skyboxShader;

}

void Skybox::SetUpPipeline(){

    skyboxShader->Bind();

    // upload the view matrix
    glm::mat4 viewMatrix = gScene.MainCamera.GetViewMatrix();
    viewMatrix = glm::mat4(glm::mat3(viewMatrix)); // remove translation from the view matrix

    skyboxShader->Upload_Uniform_MAT4fv_Pipeline("u_ViewMatrix", viewMatrix);


    // upload the projection matrix
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
                                             (float)gScene.ScreenWidth/(float)gScene.ScreenHeight,
                                             0.4f,
                                             10.0f);

    skyboxShader->Upload_Uniform_MAT4fv_Pipeline("u_ProjectionMatrix", perspective);
    

    // bind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);


}

void Skybox::Draw(){

    // set up the pipeline and bind the vao
    SetUpPipeline();

    glBindVertexArray(this->VAO);

    glDrawArrays(GL_TRIANGLES, 0, this->numVertices);

    glBindVertexArray(0);
    skyboxShader->Unbind();

}


void Skybox::LoadSkyboxTextures(std::vector<std::string> facesTexPaths){

    // bind the cube texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);

    // load texture of each face
    int width, height, nrChannels;
    for (unsigned int i = 0; i < facesTexPaths.size(); i++)
    {
        unsigned char *data = stbi_load(facesTexPaths[i].c_str(), &width, &height, &nrChannels, 0);
        
        // check if data was loaded correctly
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else // print out error message if not
        {
            std::cout << "Cubemap tex failed to load at path: " << facesTexPaths[i] << std::endl;
            stbi_image_free(data);
        }
    }


}


/**
 * @brief      Gets the skybox data.
 * @return     The skybox data.
 * 
 * NOTE: taken from learnopengl.com
*/
std::vector<float> Skybox::SkyboxData(){
    
    std::vector<float> skyboxVertices = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
    };

    return skyboxVertices;

}