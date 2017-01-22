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


static glm::vec3 Parallel_Light_Position = glm::vec3(1.0, 1.0, 0.0);
const float light_rotate_angle = 0.1;

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

GLuint generateFramebufferObject(GLuint* depth_texture)
{
	GLuint framebuffer = 0;

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, depth_texture);
	glBindTexture(GL_TEXTURE_2D, *depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *depth_texture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return 0;
	else
		return framebuffer;
}




int main()
{
	myWindow win("demo",800,600);
	win.show();
	egl.InitEGL(win.getMyWindow());
	GLuint program_model = GenerateProgram(".\\shader\\assimp_vertex.txt", ".\\shader\\assimp_fragment.txt");
	GLuint program_model_shadow = GenerateProgram(".\\shader\\assimp_shadow_vertex.txt", ".\\shader\\assimp_shadow_fragment.txt");
	GLuint program_floor = GenerateProgram(".\\shader\\floor_vertex.txt", ".\\shader\\floor_fragment.txt");
	
	myModel model;
	model.loadModel(".\\model\\cat.obj");
	
	myTexture texture;
	texture.load("floor.ktx");

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, floor_vertex);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, floor_texcoord);

	
	glClearColor(0.0,0.0,0.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	GLuint framebuffer, depthtex;
	framebuffer = generateFramebufferObject(&depthtex);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	

	bool exit = false;
	MSG msg;
	while (!exit) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT)
				exit = true;
			else if (win.changeSize)
			{
				glViewport(0, 0, win.width, win.height);
				win.changeSize = false;
			}
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			/////////////////////////////////////////////////////////////////////
			// generate shadow mapping
			
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glViewport(0, 0, 1024, 1024);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 light_rotate_matrix = glm::mat4(1.0);
			light_rotate_matrix = glm::rotate(light_rotate_matrix, glm::radians(light_rotate_angle), glm::vec3(0.0, 1.0, 0.0));

			Parallel_Light_Position = glm::vec3(light_rotate_matrix * glm::vec4(Parallel_Light_Position, 1.0));

			glm::mat4 Ortho = glm::ortho(-3.0, 3.0, -3.0, 3.0, 0.1, 10.0);

			glm::mat4 View = glm::lookAt(Parallel_Light_Position, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
			glm::mat4 Model = glm::mat4(1.0);
			glm::mat4 light_MVP = Ortho * View * Model;
			glUseProgram(program_model_shadow);
			GLint MVP_location = glGetUniformLocation(program_model_shadow, "MVP");
			glUniformMatrix4fv(MVP_location, 1, GL_FALSE, glm::value_ptr(light_MVP));
			model.Draw(program_model_shadow);
			
			/////////////////////////////////////////
			

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, win.width, win.height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glm::mat4 Projection = glm::perspective(glm::radians(60.0f), (float)win.width / (float)win.height, (float)0.1, (float)500.0);
			View = win.getCamera()->getViewMatrix();
 			Model = glm::mat4(1.0);
			glm::mat4 MVP = Projection * View * Model;

			
			MVP_location = glGetUniformLocation(program_model, "MVP");
			glUseProgram(program_model);
			glUniformMatrix4fv(MVP_location, 1, GL_FALSE, glm::value_ptr(MVP));
			model.Draw(program_model);
			




			MVP_location = glGetUniformLocation(program_floor, "MVP");
			GLint light_MVP_location = glGetUniformLocation(program_floor, "light_MVP");
			GLint sampler_location = glGetUniformLocation(program_floor, "sampler");
			GLint shadow_sampler_location = glGetUniformLocation(program_floor, "shadow_sampler");

			glUseProgram(program_floor);
			glUniformMatrix4fv(MVP_location, 1, GL_FALSE, glm::value_ptr(MVP));
			glUniformMatrix4fv(light_MVP_location, 1, GL_FALSE, glm::value_ptr(light_MVP));

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthtex);
			glUniform1i(shadow_sampler_location, 1);


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
	glDeleteProgram(program_model_shadow);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &depthtex);
	glDeleteFramebuffers(1, &framebuffer);
	system("pause");
	return 0;
} 