#include<glm/glm.hpp>
#include <GLES3/gl3.h>
//#include<list>
#include<vector>
#define MAX_GENERATED_PARTICLES_PER_FRAME 1
#define MAX_LIFE 60
#define MAX_VELOCITY_COMPONENT 0.1f

struct Particle
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 velocity;
	unsigned int life;
};





class myParticles
{
public:
	myParticles();
	glm::vec3 main_light_position;
	void drawParticles(GLuint program,GLuint texture);

private:
//	std::list<Particle> particles;
	void updateParticles();
	std::vector<Particle> particles;
	GLuint buffer;
};