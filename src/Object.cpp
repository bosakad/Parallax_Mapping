#include "Object.hpp"
#include "ObjectParser.hpp"
#include "Scene.hpp"
#include "utils.hpp"
#include "PointLight.hpp"
#include "ObjectParser.hpp"

// define constructor
Object::Object(const std::string& vertexShaderPath, const std::string& fragmentShaderPath){

    // create graphics pipeline for the object

    this->shader = new Shader(vertexShaderPath, fragmentShaderPath);

    // generate VAO
    glGenVertexArrays(1, &this->VertexArrayObject);
    
    // generate VBOs and EBOs
    glGenBuffers(1, &this->VertexBufferObject);
    glGenBuffers(1, &this->ElementBufferObject);

    // connect attributes to VAO and VBO
    glBindVertexArray(this->VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, this->VertexBufferObject);

    unsigned int stride = 14;

    glEnableVertexAttribArray(0);   // vertex attrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride*sizeof(GL_FLOAT), (void *)0);

    glEnableVertexAttribArray(1);   // normals attrib
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride*sizeof(GL_FLOAT), (void *) (3*sizeof(GLfloat)));

    glEnableVertexAttribArray(2);   // texture attrib
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride*sizeof(GL_FLOAT), (void *) (6*sizeof(GLfloat)));

    glEnableVertexAttribArray(3);   // tanget attrib
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride*sizeof(GL_FLOAT), (void *) (8*sizeof(GLfloat)));

    glEnableVertexAttribArray(4);   // bitanget attrib
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride*sizeof(GL_FLOAT), (void *) (11*sizeof(GLfloat)));

    glBindVertexArray(0);   // unbind for clean code
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

                        // generate textures

    diffuseTex = new Texture();
    normalTex = new Texture();
    heightTex = new Texture();

    // set object number to null and number of elements of the object to 0
    objectNumber = 0;
    numberOfElements = 0;

}

// define destructor
Object::~Object(){

    // free buffers
	glDeleteVertexArrays(1, &VertexArrayObject);
	glDeleteBuffers(1, &VertexBufferObject);
	glDeleteBuffers(1, &ElementBufferObject);

    delete diffuseTex;
    delete normalTex;
    delete heightTex;
    
    // delete pipeline
    delete this->shader;

}


// set up graphics pipeline - upload uniform variables to shader pipeline
void Object::SetUpPipeline(){

        // Use our shader
    this->shader->Bind();

    // upload material components
    this->Upload_Material_Pipeline(this->shader);

    // set texture sampler ID uniform 
    shader->Upload_Uniform1i_Pipeline("diffuseTexture", 0);
    shader->Upload_Uniform1i_Pipeline("normalTexture", 1);
    shader->Upload_Uniform1i_Pipeline("displacementTexture", 2);

    // set used light and used method
    shader->Upload_Uniform1i_Pipeline("usedLight", this->usedLight);
    if (this->parallaxMethod != -1){ // if parallax method is set
        shader->Upload_Uniform1i_Pipeline("parallaxMethod", this->parallaxMethod);

        // set continuous texture to false / true
        if (continuousTexture == 1){
            shader->Upload_Uniform1i_Pipeline("continuousTexture", 1);
        }
        else{
            shader->Upload_Uniform1i_Pipeline("continuousTexture", 0);
        }

    }


    // Model transformation by translating our object into world space
    glm::mat4 model = glm::mat4(1.0f);

    // translation
    model = glm::translate(model, glm::vec3(this->translation.x, this->translation.y, this->translation.z));

    // rotation
    model = glm::rotate(model, glm::radians(this->rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f)); // x
    model = glm::rotate(model, glm::radians(this->rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f)); // y
    model = glm::rotate(model, glm::radians(this->rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f)); // z
    
    // scale 
    model = glm::scale(model, this->scale);


    // Retrieve our location of our Model Matrix
    shader->Upload_Uniform_MAT4fv_Pipeline("u_ModelMatrix", model);

    // upload normal matrix - based on model matrix
    glm::mat3 normalMatrix = glm::transpose(glm::inverse( glm::mat3(model)));
    shader->Upload_Uniform_MAT3fv_Pipeline("u_NormalMatrix", normalMatrix);

    // upload point lights
    gScene.UploadLightsToPipeline(shader);


    // upload camera position
    shader->Upload_Uniform3f_Pipeline("u_CameraPos", gScene.MainCamera.GetEyeXPosition(),
                         gScene.MainCamera.GetEyeYPosition(), gScene.MainCamera.GetEyeZPosition());


    // Update the View Matrix
    glm::mat4 viewMatrix = gScene.MainCamera.GetViewMatrix();
    shader->Upload_Uniform_MAT4fv_Pipeline("u_ViewMatrix", viewMatrix);

    // Projection matrix (in perspective) 
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
                                             (float)gScene.ScreenWidth/(float)gScene.ScreenHeight,
                                             0.1f,
                                             40.0f);

    // Retrieve our location of our perspective matrix uniform 
    shader->Upload_Uniform_MAT4fv_Pipeline("u_ProjectionMatrix", perspective);

}

// define draw function
void Object::Draw(){

    // set up pipeline
    SetUpPipeline();

    // bind textures and VAO
    diffuseTex->Bind(0);
    normalTex->Bind(1);
    heightTex->Bind(2);

    glBindVertexArray(this->VertexArrayObject);

    glDrawElements(GL_TRIANGLES, this->numberOfElements, GL_UNSIGNED_INT, 0);

    // unbind VAO and texture
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // stop using current pipeline
    this->shader->Unbind();

}


// load data from obj file
void Object::LoadData_WavefrontOBJ(unsigned int objNumber, std::string filePath){

    // alloc vectors
    std::vector<GLfloat> data;
    std::vector<GLuint> indices; 
    std::string MTL_Path = "";


    // parse the file and store the data into data array and indices array
    ObjectParser::Parse_WavefrontOBJ(data, indices, filePath, MTL_Path);

    // bind VAO
    glBindVertexArray(this->VertexArrayObject); 

    // load data (vertices, normals and textures) to GPU
    glBindBuffer(GL_ARRAY_BUFFER, this->VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GL_FLOAT), data.data(), GL_STATIC_DRAW);

    // load indices to GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GL_UNSIGNED_INT), indices.data(), GL_STATIC_DRAW);

    // unbind VAO
    glBindVertexArray(0);

    // load texture if the model has one
    if (MTL_Path != ""){
        
        int width, height;

        // uint8_t* diffuseTextureData;
        // uint8_t * normalTextureData;
        std::vector<uint8_t> diffuseTextureData;
        std::vector<uint8_t> normalTextureData;

        ObjectParser::Load_Textures(MTL_Path, &width, &height,
                                    this->Ka, this->Kd, this->Ks, &this->shininess,
                                    diffuseTextureData, normalTextureData);


        // upload diffuse texture to GPU
        diffuseTex->LoadData(width, height, diffuseTextureData.data(), GL_RGB);

        // upload normal texture to GPU
        normalTex->LoadData(width, height, normalTextureData.data(), GL_RGB);


    }

    // set index of the object
    this->objectNumber = objNumber;
    this->numberOfElements = indices.size();

}



// upload generated data to GPU
void Object::UploadVertices(std::vector<GLfloat> data, std::vector<GLuint> indices){

        // bind VAO
    glBindVertexArray(this->VertexArrayObject); 

    // load data (vertices, normals and textures) to GPU
    glBindBuffer(GL_ARRAY_BUFFER, this->VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GL_FLOAT), data.data(), GL_STATIC_DRAW);

    // load indices to GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GL_UNSIGNED_INT), indices.data(), GL_STATIC_DRAW);

    // unbind VAO
    glBindVertexArray(0);
    
}


/**
 * Load texture from a file and upload it to GPU
 * 
 * @param texturePath path to the texture file
 * @param textureType type of the texture (diffuse=0, normal=1, height=2)
 * 
 * @return void
*/
void Object::LoadTexture(const std::string& texturePath, unsigned int textureType, bool inverseH){

    // load the texture
    int width, height, nrComponents;
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrComponents, 0);

    // specify the ID
    Texture *targetTexture = nullptr;
    if (textureType == 0)
        targetTexture = this->diffuseTex;
    else if (textureType == 1)
        targetTexture = this->normalTex;
    else if (textureType == 2)
        targetTexture = this->heightTex;

    // check if the texture was loaded correctly
    if (data)
    {
        // check what format the texture is
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        if (inverseH){ // should inverse the heightmap
            for (int i = 0; i < width * height * nrComponents; i += nrComponents)
            {
                data[i] = 1 - data[i];
            }
        
        }

        // load data to gpu
        targetTexture->LoadData(width, height, data, format);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << texturePath << std::endl;
        stbi_image_free(data);
    }

}



////////////////////////////////////// pipeline uploaders ///////////////////////////////////////

void Object::Upload_Material_Pipeline(Shader* shader){

    // upload ambient, diffusion and specular
    shader->Upload_Uniform3f_Pipeline("objectMaterial.Ka", this->Ka.x, 
                                                           this->Ka.y, 
                                                           this->Ka.z);

    shader->Upload_Uniform3f_Pipeline("objectMaterial.Kd", this->Kd.x, 
                                                             this->Kd.y, 
                                                             this->Kd.z);

    shader->Upload_Uniform3f_Pipeline("objectMaterial.Ks", this->Ks.x, 
                                                             this->Ks.y, 
                                                             this->Ks.z);

    // upload shininess
    shader->Upload_Uniform1f_Pipeline("objectMaterial.shininess", this->shininess);
    
}

////////////////////////////////////////////////////// setters //////////////////////////////////////////

void Object::SetTranslation(const glm::vec3 &newTranslation){

    this->translation.x = newTranslation.x;
    this->translation.y = newTranslation.y;
    this->translation.z = newTranslation.z;

}


void Object::SetRotation(const glm::vec3 &rotation){

    this->rotation.x = rotation.x;
    this->rotation.y = rotation.y;
    this->rotation.z = rotation.z;

}

void Object::SetScale(const glm::vec3 &scale){
    
    this->scale.x = scale.x;
    this->scale.y = scale.y;
    this->scale.z = scale.z;

}


glm::vec3 & Object::GetTranslation(){

    return this->translation;

}

glm::vec3 & Object::GetRotation(){
    return this->rotation;
}

glm::vec3 & Object::GetScale(){
    return this->scale;
}

void Object::SetNumberOfElements(unsigned int numberOfElements){
    this->numberOfElements = numberOfElements;
}


void Object::SetUsedLight(int usedLight){
    this->usedLight = usedLight;

}


void Object::SetParallaxMethod(int parallaxMethod){
    this->parallaxMethod = parallaxMethod;

}

void Object::SetContinuousTexture(int continuousTexture){
    this->continuousTexture = continuousTexture;
}