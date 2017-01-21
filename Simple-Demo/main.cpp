#include"myWindow.h"
#include"fileops.hpp"
#include"myEGL.h"
#include"myTexture.h"
#include"myModel.h"
#include"myParticle.h"

#include<GLES3/gl3.h>
#include<glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
static myEGL egl;

enum Log_Type
{
	SHADER,
	PROGRAM
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



GLuint GenerateProgram(const char* vertex, const char* fragment)
{
	GLuint vertexshader, fragmentshader, program = 0;
	if (!GenerateShader(vertex, vertexshader, GL_VERTEX_SHADER))
	{
		glDeleteShader(vertexshader);
		return 0;
	}
	if (!GenerateShader(fragment, fragmentshader, GL_FRAGMENT_SHADER))
	{
		glDeleteShader(vertexshader);
		glDeleteShader(fragmentshader);
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
		return 0;
	}
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	return program;
}






GLfloat floor_vertex[] = {
	-5.0,0.0,5.0,
	-5.0,0.0,-5.0,
	5.0,0.0,5.0,
	5.0,0.0,-5.0
}; 

GLfloat floor_texcoord[] = {
	0.0,5.0,
	0.0,0.0,
	5.0,5.0,
	5.0,0.0
};


GLuint floor_index[] = {
	0,1,2,
	1,2,3
};






int main()
{
	myWindow win("demo",800,600);
	win.show();
	egl.InitEGL(win.getMyWindow());
	GLuint program_model = GenerateProgram(".\\shader\\assimp_vertex.txt", ".\\shader\\assimp_fragment.txt");
	
	GLuint program_floor = GenerateProgram(".\\shader\\floor_vertex.txt", ".\\shader\\floor_fragment.txt");
	
	glUseProgram(program_model);
	myModel model;
	model.loadModel(".\\model\\cat.obj");
	
	glUseProgram(program_floor);
	myTexture texture;
	texture.load("floor.ktx");

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, floor_vertex);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, floor_texcoord);

	
	glClearColor(0.0,0.0,0.0, 1.0);
	glViewport(0, 0, win.width, win.height);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
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
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glm::mat4 Projection = glm::perspective(glm::radians(60.0f), (float)win.width / (float)win.height, (float)0.1, (float)500.0);
			glm::mat4 View = win.getCamera()->getViewMatrix();
 			glm::mat4 Model = glm::mat4(1.0);
			glm::mat4 MVP = Projection * View * Model;

			
			GLint MVP_location = glGetUniformLocation(program_model, "MVP");
			glUseProgram(program_model);
			glUniformMatrix4fv(MVP_location, 1, GL_FALSE, glm::value_ptr(MVP));
			model.Draw(program_model);
			
			MVP_location = glGetUniformLocation(program_floor, "MVP");
			GLint sampler_location = glGetUniformLocation(program_floor, "sampler");
			glUseProgram(program_floor);
			glUniformMatrix4fv(MVP_location, 1, GL_FALSE, glm::value_ptr(MVP));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture.getTextureHandle());
			glUniform1i(sampler_location, 0);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, floor_index);

			egl.SwapBuffer();
		}
	}

	glUseProgram(0);
	glDeleteProgram(program_model);
	glDeleteProgram(program_floor);
	system("pause");
	return 0;
} 