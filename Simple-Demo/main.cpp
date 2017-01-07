#include"myWindow.h"
#include"fileops.hpp"
#include"myEGL.h"
#include"myTexture.h"
#include<GLES3/gl3.h>
#include<glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

enum Log_Type
{
	SHADER,
	PROGRAM
};



GLfloat Vertex[] = {
	-3.0,3.0,0.0,
	3.0,3.0,0.0,
	3.0,-3.0,0.0,
	-3.0,-3.0,0.0
};


GLuint Index[] = {
	0,1,2,
	2,3,0
};

GLfloat Texcoord[] = {
	0.0,0.0,
	1.0,0.0,
	1.0,1.0,
	0.0,1.0
};

void PrintLog(GLuint handle,Log_Type type)
{
	GLint log_length = 0;
	char * log;
	if (type == PROGRAM)
	{
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);
		log = new char[log_length + 1];
		glGetProgramInfoLog(handle, log_length + 1, 0, log);
		cout << "program link failed." << log << endl;
		delete[] log;
	}
	else if (type == SHADER)
	{
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);
		log = new char[log_length + 1];
		glGetShaderInfoLog(handle, log_length + 1, 0, log);
		cout << "shader compile failed." << log << endl;
		delete[] log;
	}
	else
	{
		cout << "Wrong Log Type" << endl;
	}
}


bool GenerateShader(const char* name, GLuint& shader, int shadertype)
{
	bool ret = true;
	GLint status = GL_FALSE;
	shader = glCreateShader(shadertype);
	string s;
	ret = fileops::readTextFromFile(name, s);
	if (!ret) return ret;
	const char* str = s.c_str();
	glShaderSource(shader, 1, &str, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		ret = false;
		PrintLog(shader, SHADER);
	}
	return ret;
}

int main()
{
	myWindow win("demo",800,600);
	win.show();
	myEGL egl;
	egl.InitEGL(win.getMyWindow());

	GLuint vertexshader, fragmentshader, program = 0;
	if (!GenerateShader(".\\shader\\vertex.txt", vertexshader, GL_VERTEX_SHADER))
	{
		glDeleteShader(vertexshader);
		system("pause");
		return 0;
	}
	if (!GenerateShader(".\\shader\\fragment.txt", fragmentshader, GL_FRAGMENT_SHADER))
	{
		glDeleteShader(vertexshader);
		glDeleteShader(fragmentshader);
		system("pause");
		return 0;
	}
	program = glCreateProgram();
	glAttachShader(program, vertexshader);
	glAttachShader(program, fragmentshader);
	glLinkProgram(program);

	GLint status = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		PrintLog(program, PROGRAM);
		glUseProgram(0);
		glDeleteShader(vertexshader);
		glDeleteShader(fragmentshader);
		glDeleteProgram(program);
		system("pause");
		return 0;
	}
	glUseProgram(program);
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	GLint texcoord_location = glGetAttribLocation(program, "cord");
	GLint position_location = glGetAttribLocation(program, "position");
	GLint sampler_location = glGetUniformLocation(program, "sampler");
	GLuint index_buffer;

	glVertexAttribPointer(position_location, 3, GL_FLOAT, 0, 0, Vertex);
	glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, 0, 0, Texcoord);
	glEnableVertexAttribArray(position_location);
	glEnableVertexAttribArray(texcoord_location);
	myTexture texture;
	texture.load(".\\texture\\texture.ktx");
	GLuint tex = texture.getTextureHandle();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(sampler_location, 0);

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, Index, GL_STATIC_DRAW);
	glClearColor(1.0, 0.4, 0.6, 1.0);
	glViewport(0, 0, win.width, win.height);
	bool exit = false;
	MSG msg;
	while (!exit) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT)
				exit = true;
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glm::mat4 Projection = glm::perspective(glm::radians(60.0f), (float)win.width / (float)win.height, (float)0.1, (float)100.0);
			glm::mat4 View = glm::lookAt(win.getCamera()->position, glm::vec3(0.0, 0.0, 0.0), win.getCamera()->up);
			glm::mat4 Model = glm::mat4(1.0);
			glm::mat4 MVP = Projection * View * Model;

			GLint MVP_location = glGetUniformLocation(program, "MVP");
			glUniformMatrix4fv(MVP_location, 1, GL_FALSE, glm::value_ptr(MVP));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
			egl.SwapBuffer();
		}
	}

	glUseProgram(0);
	glDeleteProgram(program);
	system("pause");
	return 0;
} 