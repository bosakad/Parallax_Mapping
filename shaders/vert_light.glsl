#version 450 core
// From Vertex Buffer Object (VBO)
// The only thing that can come 'in', that is
// what our shader reads, the first part of the
// graphics pipeline.
layout(location=0) in vec3 position;

// Uniform variables
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix; // We'll use a perspective projection

// colors
uniform vec3 ObjectColor;
out vec3 color_v;

void main()
{

  vec4 newPosition = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(position,1.0f);

                                    // Don't forget 'w=1.0f'
	gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, newPosition.w);

  // pass the color as normal
  color_v = ObjectColor.rgb;

}


