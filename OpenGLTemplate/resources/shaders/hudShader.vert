#version 400 core

layout (location = 0) in vec2 inPosition;
out vec2 vTexCoord;
uniform mat4 transformationMatrix;

void main()
{
	gl_Position = transformationMatrix * vec4(inPosition, 0.0, 1.0);
	vTexCoord = vec2((inPosition.x+1.0)/2.0,1-(inPosition.y+1.0)/2.0);
}