#version 450 core

// struct for a point lights
struct PointLight{

	vec3 position;
	vec3 lightColor;

	// attenuation
	float constant;
	float linear;
	float quadratic;

  float ambientStrength;
  float diffusionStrengh;
  float specularStrengh;	

};


// inputs to graphics pipeline
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoord;
layout(location=3) in vec3 tangent;
layout(location=4) in vec3 bitangent;

// defined variables
#define MAX_LIGHTS 10

// Uniform variables: matrices
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix; // We'll use a perspective projection
uniform mat3 u_NormalMatrix; // matrix that transforms normals according to the objects rotation / scale

// Uniform variables: other
uniform vec3 u_CameraPos;
uniform PointLight u_Lights[MAX_LIGHTS];

// specify the used light
uniform int usedLight;

// outputs to fragment shader
out vec2 texcoord_frag;

out vec3 tangentCameraPos;
out vec3 tangentLightPos;
out vec3 tangentFragmentPos;



void main()
{

                        // compute projected vertex
  vec4 newPosition = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(position,1.0f);
	gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, newPosition.w); 

                            // Compute tangent space coordinates

  // compute world vectors - based on scale and rotation
  vec3 normalWorld    = u_NormalMatrix * normal;
  vec3 tangentWorld   = u_NormalMatrix * tangent;
  vec3 bitangentWorld = u_NormalMatrix * bitangent;

  // inverse TBN matrix
  mat3 worldToTangent = transpose(mat3(tangentWorld, bitangentWorld, normalWorld));  

  // transform camera, fragment, light into tangent space
  tangentFragmentPos = worldToTangent * vec3((u_ModelMatrix * vec4(position, 1.0f)));
  tangentCameraPos   = worldToTangent * u_CameraPos;
  tangentLightPos    = worldToTangent * u_Lights[usedLight].position;

                            // Pass texture coordinates

  // pass the texture coordinates
  texcoord_frag = texcoord;

}


