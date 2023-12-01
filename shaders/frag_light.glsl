#version 450 core

in vec3 color_v;

out vec4 color;

// Entry point of program
void main()
{

	color = vec4(color_v.r, color_v.g, color_v.b, 1.0f);
}

