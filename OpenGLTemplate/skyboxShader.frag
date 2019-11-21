#version 400 core

in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader
out vec4 vOutputColour;		// The output colour

uniform samplerCube CubeMapTex;
in vec3 worldPosition;

void main()
{
	vOutputColour = texture(CubeMapTex, worldPosition);
}


