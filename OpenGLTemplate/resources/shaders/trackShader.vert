#version 400 core

uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix;
	mat3 normalMatrix;
} matrices;


layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

out vec2 vTexCoord;
out vec3 vEyeNorm;
out vec4 vEyePosition;

void main()
{
	vec4 positionRelativeToCam = matrices.modelViewMatrix * vec4(inPosition, 1.0f);
	gl_Position = matrices.projMatrix * positionRelativeToCam;
	
	vEyeNorm = normalize(matrices.normalMatrix * inNormal);
	vEyePosition = matrices.modelViewMatrix * vec4(inPosition, 1.0f);

	vTexCoord = inCoord;
}