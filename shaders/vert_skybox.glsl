#version 450 core

layout(location=0) in vec3 position; 

// uniforms
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

// outputs
out vec3 texCoords; 


void main(){

    texCoords = position; // you index in the cube texture using 3d coordinates

    // compute the clip space coordinates
    vec4 clipSpacePosition = u_ProjectionMatrix * u_ViewMatrix * vec4(position, 1.0f);

    gl_Position = clipSpacePosition.xyww;   // enforce z to have 1 in the z / w component (depth)
                                            // that is, all the way in the back

}
