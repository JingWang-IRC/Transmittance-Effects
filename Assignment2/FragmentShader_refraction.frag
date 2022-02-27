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


void main() 
{
	vec3 finalResult = vec3(0, 0, 0);
	vec3 uNormal = normalize(normal);
	vec3 dirToCamera = normalize(viewPos - fragPos);

	// set the ratio for calculating refraction (this is not used for reflection)
	float ratio = 1.00 / 1.52;
	// the direction from the fragment to camera
	vec3 I = normalize(fragPos - viewPos);
	// get the refraction direction
	vec3 R = refract(I, normalize(uNormal), ratio);
	// get the corresponding color on the texture (skybox) as the refraction color	
	FragColor = vec4(texture(skybox, R).rgb, 1.0f);

}

