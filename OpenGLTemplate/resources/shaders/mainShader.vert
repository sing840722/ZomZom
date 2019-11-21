#version 400 core

// Structure for matrices
uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix; 
	mat3 normalMatrix;
} matrices;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;



out vec2 vTexCoord;	// Texture coordinate
out vec3 worldPosition;	// skybox

out vec3 vEyeNorm;
out vec4 vEyePosition;

out vec3 viewPosition;

out float visibility;
const float density = 0.01f;

void main()
{	
	worldPosition = inPosition;

	// Transform the vertex spatial position using 
	vec4 positionRelativeToCam = matrices.modelViewMatrix * vec4(inPosition, 1.0f);
	gl_Position = matrices.projMatrix * positionRelativeToCam;

	//viewPosition = gl_Position;

	// Get the vertex normal and vertex position in eye coordinates
	vEyeNorm = normalize(matrices.normalMatrix * inNormal);
	vEyePosition = matrices.modelViewMatrix * vec4(inPosition, 1.0f);
		
	vTexCoord = inCoord;

	float distance = length(positionRelativeToCam.xyz);		
	visibility = distance * density;
	visibility = clamp(visibility, 0.0f, 1.0f);
} 
	