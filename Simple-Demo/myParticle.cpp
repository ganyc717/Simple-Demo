#include"myParticle.h"
#include <ctime>
#include <cstdlib>

#define RANDOM_COMPONENT ((float)(rand() % 256)/255.0f)
#define TICKS 0.1f
#define GRAVITY -0.098f

myParticles::myParticles()
{
	main_light_position = glm::vec3(0.0,1.0,0.0);
	particles.resize(0);
	srand(unsigned(time(0)));
	glGenBuffers(1, &buffer);
}

void myParticles::updateParticles()
{
	for (auto it = particles.begin(); it != particles.end(); it++)
	{
		it->life--;
		if (it->life <= 0)
		{
			it = particles.erase(it);
			if (it != particles.end())
				continue;
			else
				break;
		}
		it->position += it->velocity * TICKS;
		it->velocity += GRAVITY * TICKS;
	}

	int count = rand() % (MAX_GENERATED_PARTICLES_PER_FRAME + 1);
	for (int i = 0; i < count; i++)
	{
		Particle particle;
		particle.color = glm::vec3(RANDOM_COMPONENT, RANDOM_COMPONENT, RANDOM_COMPONENT);
		particle.position = main_light_position;
		particle.life = rand() % (MAX_LIFE) + 1;
		particle.velocity = glm::vec3(RANDOM_COMPONENT, RANDOM_COMPONENT, RANDOM_COMPONENT) * MAX_VELOCITY_COMPONENT;
		particles.push_back(particle);
	}
}

void myParticles::drawParticles(GLuint program,GLuint texture = 0)
{
	glUseProgram(program);
	updateParticles();
	if (particles.size() == 0)
		return;
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), &particles[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(Particle), (void *)offsetof(Particle, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, 0, sizeof(Particle), (void *)offsetof(Particle, color));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	if (texture != 0)
	{
		GLint sampler_location = glGetUniformLocation(program,"sampler");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(sampler_location, 0);
	}
	glDrawArrays(GL_POINTS,0,particles.size());
}

/*
class myParticles
{
public:
	glm::vec3 main_light_position;
	void drawParticles(GLuint program);
	void generatedNewParticles();
private:
	std::vector<particle> particles;
};*/