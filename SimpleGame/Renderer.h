#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <array>
#include <assert.h>

#include "Dependencies\glew.h"
#include "LoadPng.h"

struct Vertex {
	float x;
	float y;
	float z;
	float mass = 1;
	float vx;
	float vy;
	float Rv;
	float Rv2;
	float lifetime;
	float uvx;
	float uvy;
};

struct FSVertex {
	float x;
	float y;
	float z;
	float tx;
	float ty;
};

struct Dropinfo {
	float x;
	float y;
	float start;
	float life;
};

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);
	void DrawSolidTriangle();
	void DrawFS();
	void DrawTex();
	void DrawDummy();
	void DrawDummyFBO();
	void DrawAll();
private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(const char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(const char* filenameVS, const char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float *newX, float *newY);
	void GenerateParticle(size_t particleSize);
	void GenerateFS();
	void GenDummyMesh(int winx, int winy);
	GLuint CreatePngTexture(const char* filePath, GLuint samplingMethod);
	void DrawTexture(GLuint texID, float x, float y, float scale, bool flip);
	void GenFBO();

	bool			m_Initialized = false;
	
	unsigned int	m_WindowSizeX = 0;
	unsigned int	m_WindowSizeY = 0;

	GLuint			m_VBORect = 0;
	GLuint			m_SolidRectShader = 0;
	GLuint			m_TriangleShader = 0;
	GLuint			m_FsShader = 0;
	GLuint			m_TxShader = 0;
	GLuint			m_DummyShader = 0;
	GLuint			m_TextureShader = 0;
	



	GLuint			m_VBOTriangle = 0;
	GLuint			m_VBOParticle = 0;
	GLuint			VBO_DummyMesh = 0;
	int				m_nVertices = 0;
	int				gDummyVertexCount=0;
	GLuint			m_VBOFS = 0;
	GLuint			m_VBOTX = 0;


	std::array<Dropinfo, 1000> drops;

	GLuint			sampler = 0;
	GLuint			m_RgbTexture = 0;
	std::array<GLuint, 10> m_NumTexture;
	GLuint			m_NumsTexture = 0;
	GLuint			m_PlagTexture = 0;

	//fbo
	GLuint			m_FBO = 0;
	GLuint			m_FBO_Texture = 0;

	GLuint			m_FBO_1 = 0;
	GLuint			m_FBO_Texture_1 = 0;

	GLuint			m_FBO_2 = 0;
	GLuint			m_FBO_Texture_2 = 0;

};

