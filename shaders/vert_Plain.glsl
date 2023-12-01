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

// Uniform variables: matrices
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix; // We'll use a perspective projection
uniform mat3 u_NormalMatrix; // matrix that transforms normals according to the objects rotation / scale


// outputs to fragment shader
out vec2 texcoord_frag;
out vec3 normalWorld;
out vec3 fragPosWorld; 


void main()
{

                        // compute projected vertex
  vec4 newPosition = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(position,1.0f);
	gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, newPosition.w); 


  // pass the world normal vectors - based on scale and rotation
  normalWorld = u_NormalMatrix * normal;
  
  // pass the texture coordinates
  texcoord_frag = texcoord;

  // get the fragment position in world space
  fragPosWorld = vec3(u_ModelMatrix * vec4(position, 1.0f));

}


