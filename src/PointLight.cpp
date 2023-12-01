#include "PointLight.hpp"
#include "Scene.hpp"
#include "Shader.hpp"

// Constructor
PointLight::PointLight(glm::vec3 position, glm::vec3 color){

    // set position
    this->position.x = position.x;
    this->position.y = position.y;
    this->position.z = position.z;

    // set color
    this->color.x = color.x;
    this->color.y = color.y;
    this->color.z = color.z;

    // set object number to null and number of elements of the object to 0
    numberOfElements = 0;

}

// destructor
PointLight::~PointLight(){

    // free buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    // delete pipeline
    delete shader;

}

void PointLight::Initialize_HardCoded(){

    // create graphics pipeline for the light
    this->shader = new Shader("./shaders/vert_light.glsl", "./shaders/frag_light.glsl");

    // Draw a cube to represent the light
    const std::vector<GLfloat> vertices{
            -0.1f,-0.1f,-0.1f, // triangle 1 : begin
            -0.1f,-0.1f, 0.1f,
            -0.1f, 0.1f, 0.1f, // triangle 1 : end
            0.1f, 0.1f,-0.1f, // triangle 2 : begin
            -0.1f,-0.1f,-0.1f,
            -0.1f, 0.1f,-0.1f, // triangle 2 : end
            0.1f,-0.1f, 0.1f,
            -0.1f,-0.1f,-0.1f,
            0.1f,-0.1f,-0.1f,
            0.1f, 0.1f,-0.1f,
            0.1f,-0.1f,-0.1f,
            -0.1f,-0.1f,-0.1f,
            -0.1f,-0.1f,-0.1f,
            -0.1f, 0.1f, 0.1f,
            -0.1f, 0.1f,-0.1f,
            0.1f,-0.1f, 0.1f,
            -0.1f,-0.1f, 0.1f,
            -0.1f,-0.1f,-0.1f,
            -0.1f, 0.1f, 0.1f,
            -0.1f,-0.1f, 0.1f,
            0.1f,-0.1f, 0.1f,
            0.1f, 0.1f, 0.1f,
            0.1f,-0.1f,-0.1f,
            0.1f, 0.1f,-0.1f,
            0.1f,-0.1f,-0.1f,
            0.1f, 0.1f, 0.1f,
            0.1f,-0.1f, 0.1f,
            0.1f, 0.1f, 0.1f,
            0.1f, 0.1f,-0.1f,
            -0.1f, 0.1f,-0.1f,
            0.1f, 0.1f, 0.1f,
            -0.1f, 0.1f,-0.1f,
            -0.1f, 0.1f, 0.1f,
            0.1f, 0.1f, 0.1f,
            -0.1f, 0.1f, 0.1f,
            0.1f,-0.1f, 0.1f
    };  

    // update number of elements
    this->numberOfElements = vertices.size();

    // generate VAO
    glGenVertexArrays(1, &this->VAO);

    // Vertex Arrays Object (VAO) Setup
    glBindVertexArray(this->VAO);

    // generate VBOs   
    glGenBuffers(1, &this->VBO);
    
    // Vertex Buffer Object (VBO) creation
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    // Setup Vertex Attributes
    // position (x,y,z)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0,(void*)0);

    // Unbind our currently bound Vertex Array Object
    glBindVertexArray(0);
    // Disable any attributes we opened in our Vertex Attribute Arrray,
    // as we do not want to leave them open. 
    glDisableVertexAttribArray(0);

}

// Model transformation by translating our object into world space - rotates round a center with radius r
glm::mat4 PointLight::RotateAroundPoint(glm::vec3 center, float radius){

    float speed = 12.0f;
    this->rotation += 0.1f * speed;    

    glm::mat4 model = glm::mat4(1.0f);

    // // move to the center - last operation
    model = glm::translate(model, center);

    // // rotation
    model = glm::rotate(model, glm::radians(this->rotation), glm::vec3(0.0f, 1.0f, 0.0f)); // y

    // translate by a radius - first operation
    model = glm::translate(model, glm::vec3(radius, 0, 0));

    return model;
}

void PointLight::SetUpPipeLine(){

        // Use our shader
    shader->Bind();

    // upload object color
    shader->Upload_Uniform3f_Pipeline("ObjectColor", this->color.r, this->color.g, this->color.b);

        // update light position - comment out not to have moving light
    // glm::mat4 model = RotateAroundPoint(glm::vec3(0.0, 0.0, -2.0), 3.0f); // uncomment to see the light rotate
    glm::mat4 model = glm::translate(glm::mat4(1.0f), this->position);

    this->position = glm::vec3(model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    // Retrieve our location of our Model Matrix
    shader->Upload_Uniform_MAT4fv_Pipeline("u_ModelMatrix", model);


    // Update the View Matrix
    glm::mat4 viewMatrix = gScene.MainCamera.GetViewMatrix();
    shader->Upload_Uniform_MAT4fv_Pipeline("u_ViewMatrix", viewMatrix);

    // Projection matrix (in perspective) 
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
                                             (float)gScene.ScreenWidth/(float)gScene.ScreenHeight,
                                             0.1f,
                                             20.0f);

    // Retrieve our location of our perspective matrix uniform 
    shader->Upload_Uniform_MAT4fv_Pipeline("u_ProjectionMatrix", perspective);

}


void PointLight::Draw() {

    // setup pipeline 
    SetUpPipeLine();

    // bind vao
    glBindVertexArray(this->VAO);

    // draw
    glDrawArrays(GL_TRIANGLES, 0, this->numberOfElements);

    // stop using pipeline
    glBindVertexArray(0);
    shader->Unbind();


}


////////////////////////////////////// pipeline uploaders ///////////////////////////////////////

// Uploads point light to a scene
// @note : only one light is supported for now
void PointLight::Upload_PointLight_Pipeline(Shader* targetShader, unsigned int lightIndex){

    std::string lightName = std::string("u_Lights[") + std::to_string(lightIndex) +
                             std::string("].");

    // upload ambient, diffusion and specular
    targetShader->Upload_Uniform1f_Pipeline((lightName + std::string(".ambientStrength")).c_str(), this->ambientStrength);
    targetShader->Upload_Uniform1f_Pipeline((lightName + std::string("diffusionStrengh")).c_str(), this->diffusionStrengh);
    targetShader->Upload_Uniform1f_Pipeline((lightName + std::string("specularStrengh")).c_str(), this->specularStrengh);

    // // upload attenuation
    targetShader->Upload_Uniform1f_Pipeline((lightName + std::string("constant")).c_str(), this->constant);
    targetShader->Upload_Uniform1f_Pipeline((lightName + std::string("linear")).c_str(), this->linear);
    targetShader->Upload_Uniform1f_Pipeline((lightName + std::string("quadratic")).c_str(), this->quadratic);

    // upload position and color
    targetShader->Upload_Uniform3f_Pipeline((lightName + std::string("position")).c_str(), this->position.x, 
                                                                this->position.y, 
                                                                this->position.z);


    targetShader->Upload_Uniform3f_Pipeline((lightName + std::string("lightColor")).c_str(), this->color.r, 
                                                                  this->color.g, 
                                                                  this->color.b);

}