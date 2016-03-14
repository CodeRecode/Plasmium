#version 400
in vec3 position;

out vec4 vPosition;

void main ()
{
	vPosition = vec4(position, 1.0);
}