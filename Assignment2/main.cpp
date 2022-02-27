#define GLEW_STATIC
#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <math.h>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "LightDirectional.h"
//#include "LightPoint.h"
//#include "LightSpot.h"
#include "Mesh.h"
#include "Model.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#pragma region Model Data
float vertices_ground[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   100.0f, 100.0f,   // top right
	 0.5f, -0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   100.0f, 0.0f,   // bottom right
	 0.5f, -0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // bottom left
	-0.5f, -0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 100.0f    // top left 
};

unsigned int indices_ground[] = {
	0, 1, 2,
	2, 3, 0
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

#pragma endregion

#pragma region Light Declaration
LightDirectional lightD(glm::vec3(10.0f, 30.0f, 20.0f), glm::vec3(glm::radians(110.0f), glm::radians(30.0f), 0), glm::vec3(1.0f, 0.95f, 0.8f));
#pragma endregion

#pragma region Camera Declaration
// Initialize camera class
//Camera camera(glm::vec3(0, 0, 3.0f), glm::vec3(0, -1.0f, 0), glm::vec3(0, 1.0f, 0));
Camera camera(glm::vec3(0, 3.0f, 20.0f), glm::radians(0.0f), glm::radians(-90.0f), glm::vec3(0, 1.0f, 0));
#pragma endregion

#pragma region Texture Loading Function
unsigned int LoadTextureImageToGPU(const char* filename, GLint internalFormat, GLenum format, int textureSlot)
{
	unsigned int TexBuffer;
	glGenTextures(1, &TexBuffer);

	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, TexBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannel;
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Texture image " << filename << " load failed." << endl;
	}
	stbi_image_free(data);

	//cout << filename << "Slot:  " << TexBuffer << endl;

	return TexBuffer;
}
#pragma endregion

#pragma region Camera Operation Funtions
double previous_xPos, previous_yPos;
bool first_initialise_mouse = true;

void processInput_camera(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.SpeedZ = 1.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.SpeedZ = -1.0;
	}
	else
	{
		camera.SpeedZ = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.SpeedX = 1.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.SpeedX = -1.0;
	}
	else
	{
		camera.SpeedX = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.SpeedY = 1.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.SpeedY = -1.0;
	}
	else
	{
		camera.SpeedY = 0;
	}
}

void mouse_callback_camera(GLFWwindow* window, double xPos, double yPos)
{
	if (first_initialise_mouse == true)
	{
		previous_xPos = xPos;
		previous_yPos = yPos;
		first_initialise_mouse = false;
	}

	double Delta_x = xPos - previous_xPos;
	double Delta_y = yPos - previous_yPos;

	previous_xPos = xPos;
	previous_yPos = yPos;

	//cout << Delta_x << "   " << Delta_y << endl;
	camera.ProcessMouseMovement(Delta_x, Delta_y);
}
#pragma endregion

#pragma region Load Sky Box

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			std::cout << "Cubemap tex succeeded to load at path: " << faces[i] << std::endl;
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

#pragma endregion

int main(int argc, char* argv[])
{
	std::string exePath = argv[0];

#pragma region Open a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1400, 1000, "My openGL game", NULL, NULL);

	if (window == NULL)
	{
		cout << "Open window failed." << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		cout << "Init GlEW failed." << endl;
		glfwTerminate();
		return -1;
	}

	glfwSetCursorPosCallback(window, mouse_callback_camera);
	//glfwSetCursorPosCallback(window, mouse_callback_camera_ThirdPersonView);
	//glfwSetScrollCallback(window, scroll_callback_camera_ThirdPersonView);

	//glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#pragma endregion

#pragma region Init Shader Program
	Shader* shader_ground = new Shader("VertexShader_ground.vert", "FragmentShader_ground.frag");
	Shader* shader_reflection = new Shader("Vertexshader_reflection.vert", "Fragmentshader_reflection.frag");
	Shader* shader_refraction = new Shader("Vertexshader_refraction.vert", "Fragmentshader_refraction.frag");
	Shader* shader_fresnel_onlySpecular = new Shader("Vertexshader_fresnel_onlySpecular.vert", "Fragmentshader_fresnel_onlySpecular.frag");
	Shader* shader_fresnel = new Shader("Vertexshader_fresnel.vert", "Fragmentshader_fresnel.frag");
	Shader* shader_all = new Shader("Vertexshader_all.vert", "Fragmentshader_all.frag");
	Shader* shader_chromatic = new Shader("Vertexshader_chromatic.vert", "Fragmentshader_chromatic.frag");
	Shader* shader_skybox = new Shader("VertexShader_skybox.vert", "FragmentShader_skybox.frag");
#pragma endregion 

#pragma region Pass light to shaders;
	shader_refraction->use();
	shader_refraction->SetUniform3f("lightD.pos", glm::vec3(lightD.position.x, lightD.position.y, lightD.position.z));
	shader_refraction->SetUniform3f("lightD.color", glm::vec3(lightD.color.x, lightD.color.y, lightD.color.z));
	shader_refraction->SetUniform3f("lightD.dirToLight", glm::vec3(lightD.direction.x, lightD.direction.y, lightD.direction.z));

	shader_reflection->use();
	shader_reflection->SetUniform3f("lightD.pos", glm::vec3(lightD.position.x, lightD.position.y, lightD.position.z));
	shader_reflection->SetUniform3f("lightD.color", glm::vec3(lightD.color.x, lightD.color.y, lightD.color.z));
	shader_reflection->SetUniform3f("lightD.dirToLight", glm::vec3(lightD.direction.x, lightD.direction.y, lightD.direction.z));

	shader_fresnel_onlySpecular->use();
	shader_fresnel_onlySpecular->SetUniform3f("lightD.pos", glm::vec3(lightD.position.x, lightD.position.y, lightD.position.z));
	shader_fresnel_onlySpecular->SetUniform3f("lightD.color", glm::vec3(lightD.color.x, lightD.color.y, lightD.color.z));
	shader_fresnel_onlySpecular->SetUniform3f("lightD.dirToLight", glm::vec3(lightD.direction.x, lightD.direction.y, lightD.direction.z));
#pragma endregion

#pragma region Load Models

	Model bunny(exePath.substr(0, exePath.find_last_of('\\')) + "\\bunny\\bunny.obj");
	Model teapot(exePath.substr(0, exePath.find_last_of('\\')) + "\\teapot\\teapot.obj");
	Model cuboid(exePath.substr(0, exePath.find_last_of('\\')) + "\\cuboid\\cuboid.obj");

	// skybox VAO VBO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// cube VAO VBO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// ground VAO VBO
	unsigned int VBO_ground;
	glGenBuffers(1, &VBO_ground);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ground);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_ground), vertices_ground, GL_STATIC_DRAW);

	unsigned int VAO_ground;
	glGenVertexArrays(1, &VAO_ground);
	glBindVertexArray(VAO_ground);

	unsigned int EBO_ground;
	glGenBuffers(1, &EBO_ground);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ground);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_ground), indices_ground, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
#pragma endregion

#pragma region Init and Load Texture;
	stbi_set_flip_vertically_on_load(true);
	unsigned int TexBuffer_ground;
	TexBuffer_ground = LoadTextureImageToGPU("ground.jpg", GL_RGB, GL_RGB, 0);
	//unsigned int TexBufferB;
	//TexBufferB = LoadTextureImageToGPU("awesomeface.png", GL_RGBA, GL_RGBA, 1);
#pragma endregion

#pragma region Init Skybox Texture;
	vector<std::string> faces
	{
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\church\\posx.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\church\\negx.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\church\\posy.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\church\\negy.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\church\\posz.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\church\\negz.jpg"
	};
	stbi_set_flip_vertically_on_load(false);
	unsigned int cubemapTexture = loadCubemap(faces);
#pragma endregion

#pragma region Prepare MVP matrices
	//glm::mat4 scale;  glm::scale(transMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
	glm::mat4 trans;
	glm::mat4 modelMat;
	//modelMat = glm::rotate(modelMat, glm::radians(0.0f), glm::vec3(1.0f, 0, 0));
	glm::mat4 viewMat;
	glm::mat4 projMat;
	projMat = glm::perspective(glm::radians(45.0f), 1400.0f / 1000.0f, 0.1f, 300.0f);
#pragma endregion


	// Start drawing
	while (!glfwWindowShouldClose(window))
	{

		float currentTime = glfwGetTime();

		// Mouse Action
		processInput_camera(window);
		//cout << 1 << endl;

		// Clear Screen
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, 1400, 1000);
		viewMat = camera.GetViewMatrix();

#pragma region Draw ground
		// Ground
		/*shader_ground->use();
		trans = glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1000.0f, 1000.0f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		shader_ground->SetMatrix("transform", trans);
		shader_ground->SetMatrix("modelMat", modelMat);
		shader_ground->SetMatrix("viewMat", viewMat);
		shader_ground->SetMatrix("projMat", projMat);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexBuffer_ground);
		shader_ground->SetUniform1i("groundTexture_diffuse", 0);

		glBindVertexArray(VAO_ground);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ground);
		glDrawElements(GL_TRIANGLES, sizeof(indices_ground), GL_UNSIGNED_INT, 0);*/
#pragma endregion

#pragma region Draw Teapot
		shader_refraction->use();
		trans = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(-24.0f, 0.0f, -5.0f));
		shader_refraction->SetMatrix("transform", trans);
		shader_refraction->SetMatrix("modelMat", modelMat);
		shader_refraction->SetMatrix("viewMat", viewMat);
		shader_refraction->SetMatrix("projMat", projMat);
		shader_refraction->SetUniform1i("skybox", 0);
		shader_refraction->SetUniform3f("viewPos", camera.Position);

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		teapot.Draw(shader_refraction);


		shader_reflection->use();
		trans = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 0.0f, -5.0f));
		shader_reflection->SetMatrix("transform", trans);
		shader_reflection->SetMatrix("modelMat", modelMat);
		shader_reflection->SetMatrix("viewMat", viewMat);
		shader_reflection->SetMatrix("projMat", projMat);
		shader_reflection->SetUniform1i("skybox", 0);
		shader_reflection->SetUniform3f("viewPos", camera.Position);

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		teapot.Draw(shader_reflection);


		shader_fresnel->use();
		trans = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, 0.0f, -5.0f));
		shader_fresnel->SetMatrix("transform", trans);
		shader_fresnel->SetMatrix("modelMat", modelMat);
		shader_fresnel->SetMatrix("viewMat", viewMat);
		shader_fresnel->SetMatrix("projMat", projMat);
		shader_fresnel->SetUniform1i("skybox", 0);
		shader_fresnel->SetUniform3f("viewPos", camera.Position);

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		teapot.Draw(shader_fresnel);


		shader_chromatic->use();
		trans = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(24.0f, 0.0f, -5.0f));
		shader_chromatic->SetMatrix("transform", trans);
		shader_chromatic->SetMatrix("modelMat", modelMat);
		shader_chromatic->SetMatrix("viewMat", viewMat);
		shader_chromatic->SetMatrix("projMat", projMat);
		shader_chromatic->SetUniform1i("skybox", 0);
		shader_chromatic->SetUniform3f("viewPos", camera.Position);

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		teapot.Draw(shader_chromatic);


		shader_all->use();
		trans = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		trans = glm::rotate(trans, currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, 0.0f, -5.0f));
		shader_all->SetMatrix("transform", trans);
		shader_all->SetMatrix("modelMat", modelMat);
		shader_all->SetMatrix("viewMat", viewMat);
		shader_all->SetMatrix("projMat", projMat);
		shader_all->SetUniform1i("skybox", 0);
		shader_all->SetUniform3f("viewPos", camera.Position);

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		teapot.Draw(shader_all);
#pragma endregion

#pragma region Draw Cuboid

		shader_fresnel_onlySpecular->use();
		trans = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, 0.0f, 40.0f));
		shader_fresnel_onlySpecular->SetMatrix("transform", trans);
		shader_fresnel_onlySpecular->SetMatrix("modelMat", modelMat);
		shader_fresnel_onlySpecular->SetMatrix("viewMat", viewMat);
		shader_fresnel_onlySpecular->SetMatrix("projMat", projMat);
		shader_fresnel_onlySpecular->SetUniform1i("skybox", 0);
		shader_fresnel_onlySpecular->SetUniform3f("viewPos", camera.Position);

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		cuboid.Draw(shader_fresnel_onlySpecular);


		shader_fresnel->use();
		trans = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 0.0f, 40.0f));
		shader_fresnel->SetMatrix("transform", trans);
		shader_fresnel->SetMatrix("modelMat", modelMat);
		shader_fresnel->SetMatrix("viewMat", viewMat);
		shader_fresnel->SetMatrix("projMat", projMat);
		shader_fresnel->SetUniform1i("skybox", 0);
		shader_fresnel->SetUniform3f("viewPos", camera.Position);

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		cuboid.Draw(shader_fresnel_onlySpecular);

#pragma endregion

#pragma region Draw Skybox

		glDepthFunc(GL_LEQUAL);
		shader_skybox->use();
		viewMat = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.04f, 0.0f));

		shader_skybox->SetMatrix("modelMat", modelMat);
		shader_skybox->SetMatrix("viewMat", viewMat);
		shader_skybox->SetMatrix("projMat", projMat);
		shader_skybox->SetUniform1i("skybox", 0);

		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

#pragma endregion

		// Clean up, prepare for next render loop
		glfwSwapBuffers(window);
		glfwPollEvents();
		camera.UpdateCameraPos();
	}


}
