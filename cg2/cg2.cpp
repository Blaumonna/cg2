#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include "pipeline.h"

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;
static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWorld;                                                                \n\
out vec4 Color;                                                                     \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWorld * vec4(Position, 1.0);                                     \n\
	Color = vec4(clamp(Position, 0.0, 1.0), 0.0);                                   \n\
}";

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
in vec4 Color;                                                                      \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = Color;                                                              \n\
}";

void RenderSceneCB() {
	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0f;
	Scale += 0.1f;

	Pipeline p;
	p.setRotation(Scale, Scale, 0);
	p.setScale(1, 1, 1);
	p.setPosition(0.0f, 0.0f, 10);
	p.setCamera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	p.setProjection(60.0f, 600, 600, 1.0f, 100.0f);

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.getTransformations());

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
}

static void CreateVertexBuffer()
{
	glm::vec3 Vertices[8];
	/*Vertices[0] = glm::vec3(-1.0f, -1.0f, 1.0f);
	Vertices[1] = glm::vec3(0.0f, -1.0f, 3.0f);
	Vertices[2] = glm::vec3(1.0f, -1.0f, 1.0f);
	Vertices[3] = glm::vec3(0.0f, 1.0f, 1.0f);
	*/
	Vertices[0] = glm::vec3(-1.0f, 1.0f, -1.0f);
	Vertices[1] = glm::vec3(1.0f, 1.0f, -1.0f);
	Vertices[2] = glm::vec3(1.0f, -1.0f, -1.0f);
	Vertices[3] = glm::vec3(-1.0f, -1.0f, -1.0f);
	Vertices[4] = glm::vec3(-1.0f, 1.0f, 1.0f);
	Vertices[5] = glm::vec3(1.0f, 1.0f, 1.0f);
	Vertices[6] = glm::vec3(1.0f, -1.0f, 1.0f);
	Vertices[7] = glm::vec3(-1.0f, -1.0f, 1.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}
static void CreateIndexBuffer()
{
	/*unsigned int Indices[] = { 0, 3, 1,
								1, 3, 2,
								2, 3, 0,
								0, 2, 1 };
	*/
	unsigned int Indices[] =
	{ 0, 1, 2,
	   0, 2, 3,
	   2, 1, 5,
	   2, 5, 6,
	   3, 2, 6,
	   3, 6, 7,
	   0, 3, 7,
	   0, 7, 4,
	   1, 0, 4,
	   1, 4, 5,
	   6, 5, 4,
	   6, 4, 7, };

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}
static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
	AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);

	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	assert(gWorldLocation != 0xFFFFFFFF);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("***");

	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);

	glClearColor(0.8f, 0.5f, 0.9f, 0.0f);

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	CreateVertexBuffer();
	CreateIndexBuffer();
	CompileShaders();

	glutMainLoop();

	return 0;
}