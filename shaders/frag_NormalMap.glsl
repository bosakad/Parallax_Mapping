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

// struct for a material
struct Material {

	vec3 Ka; // ambient
	vec3 Kd; // defuse
	vec3 Ks; // specular
	float shininess;

};

	///////////// inputs from vertex shader /////////////

// interpolated position and texture coord
in vec2 texcoord_frag;

// tangent space coords for camera, light and fragment position
in vec3 tangentCameraPos;
in vec3 tangentLightPos;
in vec3 tangentFragmentPos;

	///////////// outputs /////////////

// out color for fragment
out vec4 color;


	///////////// uniforms /////////////

// defined variables
#define MAX_LIGHTS 10

// array of lights in the scene
uniform PointLight u_Lights[MAX_LIGHTS];

uniform int usedLight;

// material struct
uniform Material objectMaterial;

// texture sampler
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;


// Entry point of program
void main()
{

	// get current light in the scene
	PointLight currentLight = u_Lights[usedLight];

					// calculate phong ligting model
	// ambient element
	vec3 ambient = objectMaterial.Ka * currentLight.lightColor;
	
	// diffuse element
	vec3 normal_frag = normalize( vec3(texture(normalTexture, texcoord_frag))*2.0 - 1.0 ); 

	vec3 lightDirection = normalize( tangentLightPos - tangentFragmentPos );
	
	float diffuseEl = max( dot(normal_frag, lightDirection) , 0);
	vec3 diffuse = diffuseEl * objectMaterial.Kd * currentLight.lightColor;


	// specular element
	vec3 specular = vec3(0.0f, 0.0f, 0.0f);
	if ( diffuseEl != 0 ){
		float alpha = objectMaterial.shininess; // shininess

		vec3 viewDirection = normalize( tangentCameraPos - tangentFragmentPos );
		vec3 reflectionLight = reflect( -lightDirection, normal_frag );
		float specularEl = pow( max(dot(viewDirection, reflectionLight), 0) , alpha );
		specular = specularEl * objectMaterial.Ks * currentLight.lightColor;
	}

					// final phong intensity
	vec3 intensity = ambient + diffuse + specular;


					// add attenuation
	// distance is not changed by orthonormal transformation - we can use tangent coordinates
	float distanceFromLight = length( tangentLightPos - tangentFragmentPos ); 
	float attenuation = 1 / (currentLight.constant
							 + currentLight.linear * distanceFromLight 
							 + currentLight.quadratic * pow(distanceFromLight, 2));
	
	attenuation = clamp(attenuation, 0, 1);

	// intensity *= 2* attenuation; // uncomment to see attenuation

	// output final color with intensity
	color = vec4(intensity, 1.0f) * texture(diffuseTexture, texcoord_frag);	
	
}

