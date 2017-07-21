//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <chrono>
#include "foundation/math/matrix4D.h"


const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=0) out vec4 Color;\n"
"uniform mat4 transMatrix;\n"
"void main()\n"
"{\n"
"	gl_Position = transMatrix*vec4(pos, 1);\n"
"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
ExampleApp::~ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;
	window->SetKeyPressFunction([this](int32, int32, int32, int32)
	{
		this->window->Close();
	});

	GLfloat buf[] =
	{
		-0.4f,	-0.25f,	-1,			// pos 0
		0.8f, 0.3f, 0, 1,	// color 0
		-0.2f,	0.15f,	-1,			// pos 1
		1,		1,		0,		1,	// color 0
		0.0f,	-0.25f,	-1,			// pos 2
		0.8f, 0.3f, 0, 1, // color 0
		0.2f, 0.15f, -1,			// pos 3
		1, 1, 0, 1,					//color 0
		0.4f, -0.25f, -1,			//pos 4
		0.8f, 0.25f, 0, 1,			//color 0
		0.0f, 0.55f, -1, 		//pos 5
		1, 1, 0, 1				//Color 0

	};

	GLuint ibuf[] =
	{
		0, 1, 2,
		2, 3, 4,
		1, 5, 4
		
	};

	if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// setup vertex shader
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLint length = std::strlen(vs);
		glShaderSource(this->vertexShader, 1, &vs, &length);
		glCompileShader(this->vertexShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// setup pixel shader
		this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		length = std::strlen(ps);
		glShaderSource(this->pixelShader, 1, &ps, &length);
		glCompileShader(this->pixelShader);

		// get error log
		shaderLogSize;
		glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// create a program object
		this->program = glCreateProgram();
		glAttachShader(this->program, this->vertexShader);
		glAttachShader(this->program, this->pixelShader);
		glLinkProgram(this->program);
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
		}

		// setup vbo

		glGenBuffers(1, &this->triangle);
		glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &this->triangle2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->triangle2);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibuf), ibuf, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ExampleApp::Run()
{
	std::chrono::high_resolution_clock::time_point before = std::chrono::high_resolution_clock::now();
	GLuint ibuf[] =
	{
		0, 1, 2,
		2, 3, 4,
		1, 5, 3
	};
	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		this->window->Update();

		// do stuff
		glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
		glUseProgram(this->program);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, NULL);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, &ibuf);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-before);
		CGMath::matrix4D mat = CGMath::matrix4D::rotZ(float( now.count() / 1000.0f));
		GLuint shaderTransMatrix = glGetUniformLocation(this->program, "transMatrix");
		glUniformMatrix4fv(shaderTransMatrix, 1, GL_FALSE, mat.getPointer());



		this->window->SwapBuffers();
	}
}

} // namespace Example