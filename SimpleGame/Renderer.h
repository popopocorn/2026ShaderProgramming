#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"

struct Vertex {
	float x;
	float y;
	float z;
	float mass  =1;
	float vx;
	float vy;
	float Rv;
	float Rv2;
};

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);
	void DrawSolidTriangle();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(const char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(const char* filenameVS, const char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float *newX, float *newY);
	void GenerateParticle(size_t particleSize);

	bool			m_Initialized = false;
	
	unsigned int	m_WindowSizeX = 0;
	unsigned int	m_WindowSizeY = 0;

	GLuint			m_VBORect = 0;
	GLuint			m_SolidRectShader = 0;
	GLuint			m_TriangleShader = 0;

	GLuint			m_VBOTriangle = 0;
	GLuint			m_VBOParticle = 0;
	int				m_nVertices = 0;
};

