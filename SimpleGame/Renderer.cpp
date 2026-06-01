#include "stdafx.h"
#include "Renderer.h"
#include<Windows.h>

std::random_device rd;
std::default_random_engine dre(rd());

std::uniform_real_distribution<float> urd(-1.0f, 1.0f);
std::uniform_real_distribution<float> urdzero(0.0f, 1.0f);

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_TriangleShader = CompileShaders("./Shaders/Triangle.vs", "./Shaders/Triangle.fs");
	m_FsShader = CompileShaders("./Shaders/fs.vs", "./Shaders/fs.fs");
	m_TxShader = CompileShaders("./Shaders/texture.vs", "./Shaders/texture.fs");
	m_DummyShader = CompileShaders("./Shaders/dummy.vs", "./Shaders/dummy.fs");
	m_TextureShader = CompileShaders("./Shaders/tx.vs", "./Shaders/tx.fs");

	CreateVertexBufferObjects();
	GenerateParticle(1000);
	GenerateFS();

	for (Dropinfo& d : drops)
	{
		d = {
			urd(dre), 
			urd(dre), 
			urdzero(dre), 
			urdzero(dre)
		};
	}


	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
	
	// slot0
	m_RgbTexture = CreatePngTexture("./textures/rgb.png", GL_LINEAR);

	// slot1
	m_NumsTexture = CreatePngTexture("./textures/Numbers.png", GL_LINEAR);
	m_PlagTexture = CreatePngTexture("./textures/123.png", GL_LINEAR);

	for (int i = 0; i < 10; ++i)
	{
		//slot 2~11;
		std::string path = "./textures/" + std::to_string(i) + ".png";
		m_NumTexture[i] = CreatePngTexture(path.c_str(), GL_LINEAR);
	}
	GenDummyMesh(128, 128);
	GenFBO();
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
	float centerX = 0;
	float centerY = 0;
	float size = 2;
	float mass = 1;
	float vx = 0.5, vy = 1;
	float triangle[]
		=
	{
		centerX - size / 2,	centerY - size / 2,	0, mass, vx, vy,
		centerX + size / 2,	centerY - size / 2,	0, mass, vx, vy,
		centerX + size / 2,	centerY + size / 2, 0, mass, vx, vy,

		centerX - size / 2,	centerY - size / 2,	0, mass, vx, vy,
		centerX + size / 2, centerY + size / 2, 0, mass, vx, vy,
		centerX - size / 2,	centerY + size / 2,	0, mass, vx, vy,
	};
	m_nVertices = sizeof(triangle) / (sizeof(float) * 4);
	glGenBuffers(1, &m_VBOTriangle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTriangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);


	float texRect[]
		=
	{
		-1.f, -1.f, 0.f, 
		 1.f,  1.f, 0.f, 
		-1.f , 1.f, 0.f, //Triangle1
		-1.f, -1.f, 0.f,  
		 1.f,  1.f, 0.f, 
		 1.f, -1.f, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBOTX);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTX);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texRect), texRect, GL_STATIC_DRAW);
}

void Renderer::GenerateParticle(size_t particleSize)
{
	float centerX = 0;
	float centerY = 0;
	float size = 0.025f;
	float mass = 1;
	
	//float vx = 0.5, vy = 1.0f;
	Vertex* Particles = new Vertex[particleSize * 6];
	
	for (int i = 0; i < particleSize; ++i)
	{
		float vx = urd(dre) * 2, vy = urd(dre) * 2;
		float rv = urd(dre);
		float rv2 = urd(dre);
		float lifetime = (urd(dre) + 1.0f) / 2.0f;

		Vertex* v = &Particles[i * 6];

		// 좌하 (0,0)
		v[0] = { centerX - size / 2, centerY - size / 2, 0, mass, vx, vy, rv, rv2, lifetime, 0.0f, 0.0f };
		// 우하 (1,0)
		v[1] = { centerX + size / 2, centerY - size / 2, 0, mass, vx, vy, rv, rv2, lifetime, 1.0f, 0.0f };
		// 우상 (1,1)
		v[2] = { centerX + size / 2, centerY + size / 2, 0, mass, vx, vy, rv, rv2, lifetime, 1.0f, 1.0f };

		// 좌하 (0,0)
		v[3] = { centerX - size / 2, centerY - size / 2, 0, mass, vx, vy, rv, rv2, lifetime, 0.0f, 0.0f };
		// 우상 (1,1)
		v[4] = { centerX + size / 2, centerY + size / 2, 0, mass, vx, vy, rv, rv2, lifetime, 1.0f, 1.0f };
		// 좌상 (0,1)
		v[5] = { centerX - size / 2, centerY + size / 2, 0, mass, vx, vy, rv, rv2, lifetime, 0.0f, 1.0f };
	}

	m_nVertices = particleSize * 6;
	glGenBuffers(1, &m_VBOParticle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * particleSize * 6, Particles, GL_STATIC_DRAW);

	delete[] Particles;
}

void Renderer::GenerateFS()
{
	FSVertex Rect[6];

	Rect[0] = { -1,	-1,	0, 0, 1,};
	Rect[1] = { 1,	-1,	0, 1, 1,};
	Rect[2] = { 1,	1, 0, 1, 0,};
	Rect[3] = { -1,	-1,	0,0, 1, };
	Rect[4] = { 1,	1, 0, 1, 0, };
	Rect[5] = { -1,	1,	0,0, 0, };


	glGenBuffers(1, &m_VBOFS);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFS);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FSVertex) * 6, Rect, GL_STATIC_DRAW);
}

void Renderer::GenDummyMesh(int winx, int winy)
{
	float basePosX = -0.5f;

	float basePosY = -0.5f;

	float targetPosX = 0.5f;

	float targetPosY = 0.5f;

	int pointCountX = winx;

	int pointCountY = winy;

	float width = targetPosX - basePosX;

	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];

	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];

	gDummyVertexCount = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	//Prepare points

	for (int x = 0; x < pointCountX; x++)

	{
		for (int y = 0; y < pointCountY; y++)
		{

			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));

			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));

		}
	}

	//Make triangles

	int vertIndex = 0;

	for (int x = 0; x < pointCountX - 1; x++)

	{
		for (int y = 0; y < pointCountY - 1; y++)
		{

			//Triangle part 1

			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];

			vertIndex++;

			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];

			vertIndex++;

			vertices[vertIndex] = 0.f;

			vertIndex++;

			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];

			vertIndex++;

			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];

			vertIndex++;

			vertices[vertIndex] = 0.f;

			vertIndex++;

			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 0];

			vertIndex++;

			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 1];

			vertIndex++;

			vertices[vertIndex] = 0.f;

			vertIndex++;

			//Triangle part 2

			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];

			vertIndex++;

			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];

			vertIndex++;

			vertices[vertIndex] = 0.f;

			vertIndex++;

			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 0];

			vertIndex++;

			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 1];

			vertIndex++;

			vertices[vertIndex] = 0.f;

			vertIndex++;

			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];

			vertIndex++;

			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];

			vertIndex++;

			vertices[vertIndex] = 0.f;

			vertIndex++;

		}
	}

	glGenBuffers(1, &VBO_DummyMesh);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_DummyMesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* gDummyVertexCount * 3, vertices, GL_STATIC_DRAW);

}

GLuint Renderer::CreatePngTexture(const char* filePath, GLuint samplingMethod)
{
	//Load Png

	std::vector<unsigned char> image;

	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, filePath);

	if (error != 0)

	{

		std::cout << "PNG image loading failed:" << filePath << std::endl;

		assert(0);

	}



	GLuint temp;

	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,

		GL_UNSIGNED_BYTE, &image[0]);



	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);



	return temp;
}
float ftime = 0;

void Renderer::DrawTexture(GLuint texID, float x, float y, float scale, bool flip)
{
	//ftime += 0.0016;
	int shader = m_TextureShader;
	glUseProgram(shader);

	int us = glGetUniformLocation(shader, "u_Tex");
	glUniform1i(us, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	int attribPosition = glGetAttribLocation(shader, "a_Pos");
	glEnableVertexAttribArray(attribPosition);

	int uTrans = glGetUniformLocation(shader, "u_Trans");
	glUniform4f(uTrans, x, y, 1, scale);

	int uFlip = glGetUniformLocation(shader, "u_Flip");
	glUniform1i(uFlip, flip);

	//int uTime = glGetUniformLocation(shader, "u_Time");
	//glUniform1f(uTime, ftime);


	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTX);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::GenFBO()
{
	//fbotexure 생성
	GLuint textureId; glGenTextures(1, &m_FBO_Texture);
	glBindTexture(GL_TEXTURE_2D, m_FBO_Texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, 
		GL_COLOR_ATTACHMENT0, 
		GL_TEXTURE_2D, 
		m_FBO_Texture, 0);
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER, 
		GL_DEPTH_ATTACHMENT, 
		GL_RENDERBUFFER,
		depthBuffer);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		assert(0);
	}


	GLuint textureId1; glGenTextures(1, &m_FBO_Texture_1);
	glBindTexture(GL_TEXTURE_2D, m_FBO_Texture_1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	GLuint depthBuffer1;
	glGenRenderbuffers(1, &depthBuffer1);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer1);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &m_FBO_1);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_1);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		m_FBO_Texture_1, 0);
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER,
		depthBuffer);

	GLenum status1 = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status1 != GL_FRAMEBUFFER_COMPLETE)
	{
		assert(0);
	}

	GLuint textureId2; glGenTextures(1, &m_FBO_Texture_2);
	glBindTexture(GL_TEXTURE_2D, m_FBO_Texture_2);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	GLuint depthBuffer2;
	glGenRenderbuffers(1, &depthBuffer2);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer2);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &m_FBO_2);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_2);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		m_FBO_Texture_2, 0);
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER,
		depthBuffer);

	GLenum status2 = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status2 != GL_FRAMEBUFFER_COMPLETE)
	{
		assert(0);
	}
}


void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(const char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(const char* filenameVS, const char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawSolidTriangle()
{
	ftime += 0.00016;
	//Program select
	glUseProgram(m_TriangleShader);

	int uTime = glGetUniformLocation(m_TriangleShader, "u_Time");
	

	glUniform1f(uTime, ftime);
	
	int attribPosition = glGetAttribLocation(m_TriangleShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	
	int mass = glGetAttribLocation(m_TriangleShader, "a_Mass");
	glEnableVertexAttribArray(mass);
	
	
	int v = glGetAttribLocation(m_TriangleShader, "a_Velocity");
	glEnableVertexAttribArray(v);

	int rv = glGetAttribLocation(m_TriangleShader, "a_Rv");
	glEnableVertexAttribArray(rv);

	int rv2 = glGetAttribLocation(m_TriangleShader, "a_Rv2");
	glEnableVertexAttribArray(rv2);

	int lt = glGetAttribLocation(m_TriangleShader, "a_LifeTime");
	glEnableVertexAttribArray(lt);


	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);
	glVertexAttribPointer(
		attribPosition, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(Vertex),
		0
	);

	glVertexAttribPointer(
		mass, 
		1, 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(Vertex),
		(GLvoid*)(sizeof(float) * 3)
	);

	glVertexAttribPointer(
		v, 
		2, 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(Vertex),
		(GLvoid*)(sizeof(float) * 4)
	);

	glVertexAttribPointer(
		rv,
		1,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)(sizeof(float) * 6)
	);
	glVertexAttribPointer(
		rv2,
		1,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)(sizeof(float) * 7)
	);

	glVertexAttribPointer(
		lt,
		1,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)(sizeof(float) * 8)
	);



	glDrawArrays(GL_TRIANGLES, 0, m_nVertices);

	glDisableVertexAttribArray(attribPosition);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawFS()
{
	ftime += 0.0016;
	//Program select
	glUseProgram(m_FsShader);

	int uTime = glGetUniformLocation(m_FsShader, "u_Time");
	glUniform1f(uTime, ftime);

	int DropInfo = glGetUniformLocation(m_FsShader, "u_DropInfo");
	glUniform4fv(DropInfo, drops.size(), reinterpret_cast<const float*>(drops.data()));




	int attribPosition = glGetAttribLocation(m_FsShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);

	int uv = glGetAttribLocation(m_FsShader, "a_UV");
	glEnableVertexAttribArray(uv);


	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFS);
	glVertexAttribPointer(
		attribPosition,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(FSVertex),
		0
	);

	glVertexAttribPointer(
		uv,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(FSVertex),
		(GLvoid*)(sizeof(float)*3)
	);


	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::DrawTex()
{
	ftime += 0.00016;
	//Program select
	glUseProgram(m_TxShader);


	int attribPosition = glGetAttribLocation(m_TxShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);

	int uTime = glGetUniformLocation(m_TxShader, "u_Time");
	glUniform1f(uTime, ftime);

	//Sleep(1000);
	static int cur = 0;

	int idx = glGetUniformLocation(m_TxShader, "u_Idx");
	glUniform1i(idx, cur);

	++cur;
	if (cur > 99)cur = 0;

	int uv = glGetAttribLocation(m_TxShader, "a_UV");
	glEnableVertexAttribArray(uv);

	sampler = glGetUniformLocation(m_TxShader, "uTexSampler");
	int sampler2 = glGetUniformLocation(m_TxShader, "u_CurNumTex");
	

	glUniform1i(sampler, 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RgbTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_NumsTexture);

	int tex = GL_TEXTURE1;

	for (int i = 0; i < 10; ++i)
	{
		glActiveTexture(++tex);
		glBindTexture(GL_TEXTURE_2D, m_NumTexture[i]);
	}



	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFS);
	glVertexAttribPointer(
		attribPosition,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(FSVertex),
		0
	);

	glVertexAttribPointer(
		uv,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(FSVertex),
		(GLvoid*)(sizeof(float) * 3)
	);


	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::DrawDummy()
{
	ftime += 0.0016;
	int shader = m_DummyShader;
	glUseProgram(shader);
	
	int us = glGetUniformLocation(shader, "u_sampler");
	glUniform1i(us, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_PlagTexture);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);

	int uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, ftime);

	int upts = glGetUniformLocation(shader, "u_Dropinfo");
	glUniform4fv(upts, drops.size(), reinterpret_cast<const float*>(drops.data()));


	glBindBuffer(GL_ARRAY_BUFFER, VBO_DummyMesh);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, gDummyVertexCount);

	glDisableVertexAttribArray(attribPosition);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	DrawTexture(m_RgbTexture, 0.5, 0.5, 0.1, false);
	
}

void Renderer::DrawDummyFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glViewport(0, 0, 512, 512);
	DrawDummy();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1024, 1024);
	DrawTexture(m_FBO_Texture, 0, 0, 0.5, true);
}

void Renderer::DrawAll()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glViewport(0, 0, 512, 512);
	DrawFS();



	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_1);
	glViewport(0, 0, 512, 512);
	DrawTex();

	

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_2);
	glViewport(0, 0, 512, 512);
	DrawDummy();


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1024, 1024);
	DrawTexture(m_FBO_Texture, -0.5, 0, 0.3, false);
	DrawTexture(m_FBO_Texture_1, 0, 0, 0.3, false);
	DrawTexture(m_FBO_Texture_2, 0.5, 0, 0.3, false);


}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;

	
}


