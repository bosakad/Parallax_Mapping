#version 410 core

	///////////// structs /////////////

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

	///////////// function declarations /////////////

vec2 compute_ParallaxOffset(vec2 oldTexcoord, vec3 viewTo);
vec2 compute_Parallax_Steep(vec2 oldTexCoord, vec3 viewDir);
vec2 compute_Parallax_Occlusion(vec2 oldTexCoord, vec3 viewDir);
vec2 compute_Parallax_Occlusion_Binary(vec2 oldTexCoord, vec3 viewDir);
vec2 binary_search(vec2 texCoordL, float depthL, vec2 texCoordR, float depthR, float eps);


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


uniform int usedLight;
uniform int parallaxMethod;
uniform int continuousTexture; // if the texture is connected to another tile of the same texture

// array of lights in the scene
uniform PointLight u_Lights[MAX_LIGHTS];

// material struct
uniform Material objectMaterial;

// texture sampler
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D displacementTexture;


// debug counter
float counter = 0;
float MAX_ITER = 10;


// Entry point of program
void main()
{
	// get current light in the scene
	PointLight currentLight = u_Lights[usedLight];

					// calculate the new texture coordinates based on parallax
	vec3 viewDirection = normalize( tangentCameraPos - tangentFragmentPos );
	vec2 displacedTexCoord = vec2(0.0f, 0.0f);

	// choose parallax method
	if (parallaxMethod == 0)
		displacedTexCoord = compute_ParallaxOffset(texcoord_frag, viewDirection);
	else if (parallaxMethod == 1)
		displacedTexCoord = compute_Parallax_Steep(texcoord_frag, viewDirection);
	else if (parallaxMethod == 2)
		displacedTexCoord = compute_Parallax_Occlusion(texcoord_frag, viewDirection);
	else if (parallaxMethod == 3)
		displacedTexCoord = compute_Parallax_Occlusion_Binary(texcoord_frag, viewDirection);

					// calculate phong ligting model
	// ambient element
	vec3 ambient = objectMaterial.Ka * currentLight.lightColor;
	
	// diffuse element
	vec3 normal_frag = normalize( vec3(texture(normalTexture, displacedTexCoord))*2.0 - 1.0 ); 

	vec3 lightDirection = normalize( tangentLightPos - tangentFragmentPos );
	
	float diffuseEl = max( dot(normal_frag, lightDirection) , 0);
	vec3 diffuse = diffuseEl * objectMaterial.Kd * currentLight.lightColor;


	// specular element
	vec3 specular = vec3(0.0f, 0.0f, 0.0f);
	if ( diffuseEl != 0 ){
		float alpha = objectMaterial.shininess; // shininess

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
	color = vec4(intensity, 1.0f) * texture(diffuseTexture, displacedTexCoord);	
	// color = vec4(intensity, 1.0f) * vec4(counter / MAX_ITER, 0, 0, 1); // uncomment to see number of iterations per fragment

	
}


										// Parallax functions

// function that computes Parallax mapping with offset limiting
vec2 compute_ParallaxOffset(vec2 oldTexcoord, vec3 viewDir){

	const float height_scale = 0.08f;
	float height = texture(displacementTexture, oldTexcoord).r;

	// displace the texture coordinate based on the current height
	vec2 displacement = (height * height_scale) * viewDir.xy / viewDir.z;
	vec2 newCoords = oldTexcoord - displacement;

	// discard the fragment if we get out of texture
	if (newCoords.x > 1.0f || newCoords.x < 0.0f || newCoords.y > 1.0f || newCoords.y < 0.0f){
		discard;
	}

	return newCoords; 

}

// function that computes steep parallax mapping
vec2 compute_Parallax_Steep(vec2 oldTexCoord, vec3 viewDir){

	// define params of the method
	const float height_scale = 0.08f;
	const float numLayersMin = 15.0f;
	const float numLayerMax  = 35.0f;

	// define number of layers depending what direction we view the object
	vec3 tangentObjNormal = vec3(0.0f, 0.0f, 1.0f); // normal is always e3 since we are in tangent space
	float numberOfLayers = mix(numLayersMin, numLayerMax, max(dot(viewDir, tangentObjNormal), 0.0));

	// alloc initial current
	float currentDepth = 0.0f;
	float currentDepthMap = texture(displacementTexture, oldTexCoord).r;
	vec2 currentTexCoord = oldTexCoord;

	// alloc variables for the method
	float depthPerLayer = 1 / numberOfLayers;

	// get delta of the coords per step
	vec2 deltaCoords = -( 1 / (numberOfLayers * viewDir.z)) * viewDir.xy * height_scale;

	// step until the height map is lower
	while( currentDepth  < currentDepthMap ){

		// shift the tex coords
		currentTexCoord += deltaCoords;

		// update the heights
		currentDepth += depthPerLayer;
		currentDepthMap = texture(displacementTexture, currentTexCoord).r;

		// debug
		counter++;

	}

	// discard the fragment if we get out of texture
	if (currentTexCoord.x > 1.0f || currentTexCoord.x < 0.0f || 
					currentTexCoord.y > 1.0f || currentTexCoord.y < 0.0f){
		discard;
	}

	return currentTexCoord;
}


// function that computes parallax occlusion mapping
vec2 compute_Parallax_Occlusion(vec2 oldTexCoord, vec3 viewDir){

	// define params of the method
	const float height_scale = 0.08f;
	const float numLayersMin = 15.0f;
	const float numLayerMax  = 30.0f;

	// define number of layers depending what direction we view the object
	vec3 tangentObjNormal = vec3(0.0f, 0.0f, 1.0f); // normal is always e3 since we are in tangent space
	float numberOfLayers = mix(numLayersMin, numLayerMax, max(dot(viewDir, tangentObjNormal), 0.0));

	// alloc initial current
	float currentDepth = 0.0f;
	float currentDepthMap = texture(displacementTexture, oldTexCoord).r;
	vec2 currentTexCoord = oldTexCoord;

	// alloc variables for the method
	float depthPerLayer = 1 / numberOfLayers;

	// get delta of the coords per step
	vec2 deltaCoords = -( 1 / (numberOfLayers * viewDir.z)) * viewDir.xy * height_scale;

	// step until the height map is lower
	while( currentDepth  < currentDepthMap ){

		// shift the tex coords
		currentTexCoord += deltaCoords;

		// update the heights
		currentDepth += depthPerLayer;
		currentDepthMap = texture(displacementTexture, currentTexCoord).r;

		// debug
		counter++;

	}

								// compute occlusion mapping part

	// compute the depth differences before and after the last step
	float depthAfterDiff = currentDepth - currentDepthMap;
	
	vec2  coordsBefore = currentTexCoord - deltaCoords; // reverse the step
	float depthBefore = currentDepth - depthPerLayer;
	float depthBeforeDiff = texture(displacementTexture, coordsBefore).r - depthBefore;

	// compute the weight
	float w = depthAfterDiff / (depthAfterDiff + depthBeforeDiff);
	vec2 newTexCoord = w * coordsBefore + (1-w) * currentTexCoord;


	// discard the fragment if we get out of texture
	if (newTexCoord.x > 1.0f || newTexCoord.x < 0.0f || 
					newTexCoord.y > 1.0f || newTexCoord.y < 0.0f){
		discard;
	}

	return newTexCoord;
}


// function that computes parallax mapping using binary search
vec2 compute_Parallax_Occlusion_Binary(vec2 oldTexCoord, vec3 viewDir){

	// define params of the method
	const float height_scale = 0.08f;
	const float numLayersMin = 15.0f;
	const float numLayerMax  = 30.0f;
	const float eps 	     = 0.001f; // convergence check for binary search

	// define number of layers depending what direction we view the object
	vec3 tangentObjNormal = vec3(0.0f, 0.0f, 1.0f); // normal is always e3 since we are in tangent space
	float numberOfLayers = mix(numLayersMin, numLayerMax, max(dot(viewDir, tangentObjNormal), 0.0));

	// alloc initial current
	float currentDepth = 0.0f;
	float currentDepthMap = texture(displacementTexture, oldTexCoord).r;
	vec2 currentTexCoord = oldTexCoord;

	// alloc variables for the method
	float depthPerLayer = 1 / numberOfLayers;

	// get delta of the coords per step
	vec2 deltaCoords = -( 1 / (numberOfLayers * viewDir.z)) * viewDir.xy * height_scale;

	// step until the height map is lower
	while( currentDepth  < currentDepthMap ){

		// shift the tex coords
		currentTexCoord += deltaCoords;

		// update the heights
		currentDepth += depthPerLayer;
		currentDepthMap = texture(displacementTexture, currentTexCoord).r;

		// debug
		counter++;

	}

								// improve the point using binary search
	float depthL = currentDepth;
	vec2 texcoordL = currentTexCoord;
	
	vec2  texCoordR = currentTexCoord - deltaCoords; // reverse the step
	float depthR = currentDepth - depthPerLayer;

	vec2 newTexCoord = binary_search(texcoordL, depthL, texCoordR, depthR, eps);

	// discard the fragment if we get out of texture
	if (newTexCoord.x >= 1.0f || newTexCoord.x <= 0.0f || 
					newTexCoord.y >= 1.0f || newTexCoord.y <= 0.0f){
		if (continuousTexture == 0)
			discard;

	}

	return newTexCoord;
}

vec2 binary_search(vec2 texCoordL, float depthL, vec2 texCoordR, float depthR, float eps){

	// define variables
	vec2 diffCoords = texCoordR - texCoordL;
	float sq_norm = sqrt(dot(diffCoords, diffCoords)); // squared l2-norm

	// alloc
	vec2 midCoord = texCoordR;
	float midDepthMap;
	float midDepth;

	// until convergence
	while ( sq_norm > eps ){

		// get middle
		midCoord = (texCoordL + texCoordR) / 2;
		midDepthMap = texture(displacementTexture, midCoord).r;
		midDepth = (depthL + depthR) / 2;

		if (midDepthMap > midDepth){ // solution is in the left interval

			texCoordR = midCoord;
			depthR = midDepth;

		}
		else { // solution is on the right

			texCoordL = midCoord;
			depthL = midDepth;
		}


		// update squared l2-norm
		diffCoords = texCoordR - texCoordL;
		sq_norm = sqrt(dot(diffCoords, diffCoords)); 

		// debug
		counter++;

	}

								// compute occlusion mapping part

	// compute the depth differences before and after the last step
	float depthAfterDiff = depthL - texture(displacementTexture, texCoordL).r;
	float depthBeforeDiff = texture(displacementTexture, texCoordR).r - depthR;

	// compute the weight
	float w = depthAfterDiff / (depthAfterDiff + depthBeforeDiff);
	vec2 newTexCoord = w * texCoordR + (1-w) * texCoordL;

	return newTexCoord;

}
