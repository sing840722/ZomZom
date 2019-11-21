#version 400 core

struct LightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
	vec3 direction;
	float exponent;
	float cutoff;
	vec3 attenuation;
};

// Structure holding material information:  its ambient, diffuse, and specular colours, and shininess
struct MaterialInfo
{
	vec3 Ma;
	vec3 Md;
	vec3 Ms;
	float shininess;
};


uniform sampler2D sampler0;  // The texture sampler
uniform samplerCube CubeMapTex;
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
uniform bool bUseRipple;	//use ripple effect
uniform bool renderSkybox;
uniform float t;	//delta time

uniform LightInfo light1; 
uniform LightInfo light2; 
uniform LightInfo light3; 
uniform LightInfo light4;
uniform LightInfo light5;
uniform LightInfo light6;
uniform MaterialInfo material1; 

in vec3 viewPosition;
in vec3 vEyeNorm;
in vec4 vEyePosition;
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader
in vec3 worldPosition;
in float visibility;
out vec4 vOutputColour;		// The output colour


vec3 BlinnPhongSpotlightModel(LightInfo light, vec4 p, vec3 n)
{
	float distance = length(light.position - p);
	float attFactor = light.attenuation.x + (light.attenuation.y * distance) + (light.attenuation.z * distance * distance);
	vec3 s = normalize(vec3(light.position - p));
	float angle = acos(dot(-s, light.direction));
	float cutoff = radians(clamp(light.cutoff, 0.0, 90.0));
	vec3 ambient = light.La * material1.Ma;
	if (angle < cutoff) {
		float spotFactor = pow(dot(-s, light.direction), light.exponent);
		vec3 v = normalize(p.xyz);
		vec3 h = normalize(v + s);
		float sDotN = max(dot(s, n), 0.0);
		vec3 diffuse = (light.Ld * material1.Md * sDotN) /attFactor;
		vec3 specular = vec3(0.0);
		if (sDotN > 0.0)
		specular = (light.Ls * material1.Ms * pow(max(dot(h, n), 0.0),
			material1.shininess)) /attFactor;
		return ambient + spotFactor * (diffuse + specular);
	} else
	return ambient;
}

void main()
{
	vec3 vColour = BlinnPhongSpotlightModel(light1, vEyePosition, vEyeNorm) +
				BlinnPhongSpotlightModel(light2, vEyePosition, vEyeNorm) +
				BlinnPhongSpotlightModel(light3, vEyePosition, vEyeNorm) +
	 			BlinnPhongSpotlightModel(light4, vEyePosition, vEyeNorm) +
				BlinnPhongSpotlightModel(light5, vEyePosition, vEyeNorm) +
				BlinnPhongSpotlightModel(light6, vEyePosition, vEyeNorm);


	if (renderSkybox) {
		vec4 vTexColour = texture(CubeMapTex, worldPosition);
		vOutputColour = vTexColour*vec4(vColour, 1.0f);	// Combine object colour and texture 

	} else {
		// Get the texel colour from the texture sampler
		vec4 vTexColour = texture(sampler0, vTexCoord);	
		if(vTexColour.a<0.2){	//if alpha > 0.5, do not render 
			discard;
		}

		if (bUseTexture){
			vOutputColour = vTexColour*vec4(vColour, 1.0f);	// Combine object colour and texture 
		}
		else{
			vOutputColour = vec4(vColour, 1.0f);	// Just use the colour instead
		}
	}

	//vec4 vFogColour = vec4(0.81f ,0.81f ,0.81f ,0.0f );
	//vOutputColour = mix(vOutputColour,vFogColour, visibility);
}


