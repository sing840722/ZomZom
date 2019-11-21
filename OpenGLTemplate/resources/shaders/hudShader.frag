#version 400 core

in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader
uniform sampler2D sampler0;  // The texture sampler
out vec4 vOutputColour;
uniform float opacity;

uniform float mouseX;
uniform float mouseY;

void main()
{	
	vec4 vTexColour = texture(sampler0, vTexCoord);


	if(opacity >= 0 && vTexColour.a >= 0.15){
		vTexColour.a = opacity;
	}
	
	if(vTexColour.a<0.15){	
		//discard;
	}

	vOutputColour = vTexColour;
}
