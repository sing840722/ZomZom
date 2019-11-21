#version 400 core

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;
out vec3 worldPosition;	// used for skybox
out vec2 vTexCoord;	// Texture coordinate
mat4 modelViewMatrix; 
mat4 projMatrix;

void main()
{	
// Save the world position for rendering the skybox
	worldPosition = inPosition;

	vec4 positionRelativeToCam = modelViewMatrix * vec4(inPosition, 1.0f);
	gl_Position = projMatrix * positionRelativeToCam;

	vTexCoord = inCoord;
} 
	