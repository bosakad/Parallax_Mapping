#version 450 core

// only color out
out vec4 skyboxDiffuse;

// inputs
in vec3 texCoords;

// uniforms
uniform samplerCube skyboxTex; 

void main(){

    // outputs the skybox texture based on the position
    skyboxDiffuse = texture(skyboxTex, texCoords);

}