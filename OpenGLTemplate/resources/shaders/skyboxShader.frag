#version 400 core

in vec3 vColour;			// Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader

out vec4 vOutputColour;		// The output colour

uniform sampler2D sampler0;  // The texture sampler
uniform samplerCube CubeMapTex;
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
uniform bool bUseRipple;	//use ripple effect
uniform bool renderSkybox;

uniform float t;	//delta time

in vec3 worldPosition;
in float visibility;

void main()
{
	vOutputColour = texture(CubeMapTex, worldPosition);

}


