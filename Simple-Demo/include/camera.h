#pragma once
#include<glm/glm.hpp>
class Camera
{
public:
	Camera();
	void setPosition(glm::vec3);
	glm::vec3 getPosition();
	void setDirection(glm::vec3);
	glm::vec3 getDirection();
	void translate(glm::vec3 translation);
	void rotate(glm::vec3 rotation);
	glm::mat4 getViewMatrix();
	
private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 delta_translation;
	glm::vec3 delta_rotation;
};