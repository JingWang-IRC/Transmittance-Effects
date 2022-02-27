#version 330 core

struct Material{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct LightDirectional{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;
};

struct LightPoint{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;
	float constant;
	float linear;
	float quadratic;
};

struct LightSpot{
vec3 pos;
	vec3 color;
	vec3 dirToLight;
	//float constant;
	//float linear;
	//float quadratic;
	float cosInner;
	float cosOuter;
};

//in vec3 vertexColor;
//in vec2 TexCoord;
in vec3 fragPos;
in vec4 fragPosLightSpace;
in vec3 normal;
in vec2 texCoord;

uniform Material material;
uniform LightDirectional lightD;
uniform LightDirectional lightD1;
uniform LightPoint lightP0;
uniform LightPoint lightP1;
uniform LightPoint lightP2;
uniform LightPoint lightP3;
uniform LightSpot lightS;
uniform vec3 objectColor;
uniform vec3 ambientColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 lightDirUniform;
uniform vec3 viewPos;
uniform sampler2D shadowMap;
uniform samplerCube skybox;

out vec4 FragColor;

// give red/green/blue light different ratios for calculating refraction
const float etaR = 0.65;
const float etaG = 0.67;
const float etaB = 0.69;


void main() 
{
	vec3 finalResult = vec3(0, 0, 0);
	vec3 uNormal = normalize(normal);
	vec3 dirToCamera = normalize(viewPos - fragPos);

	// set the ratio for calculating refraction (this is not used for reflection)
	float ratio = 1.00 / 1.52;
	// the direction from the fragment to camera
	vec3 I = normalize(fragPos - viewPos);

	// calculate the refraction of lights of different colors
	vec3 RefractR = refract(I, normalize(uNormal), etaR);
	vec3 RefractG = refract(I, normalize(uNormal), etaG);
	vec3 RefractB = refract(I, normalize(uNormal), etaB);

	// combine the red/green/blue lights to make the final refraction results
	vec3 Refract;
	Refract.r = texture(skybox, RefractR).r;
	Refract.g = texture(skybox, RefractG).g;
	Refract.b = texture(skybox, RefractB).b;

	// output the color
	FragColor += vec4(Refract, 1.0f);

}

