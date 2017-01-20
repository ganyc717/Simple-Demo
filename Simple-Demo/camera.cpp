#include"camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera():position(glm::vec3(0.0)),direction(glm::vec3(0.0,0.0,1.0)),
					delta_rotation(glm::vec3(0.0)),delta_translation(glm::vec3(0.0))
{}

void Camera::setPosition(glm::vec3 Position)
{
	position = Position;
	direction = glm::normalize(glm::vec3(0.0) - position);
}
glm::vec3 Camera::getPosition()
{
	return position;
}
void Camera::setDirection(glm::vec3 Direction)
{
	direction = glm::normalize(Direction);
}
glm::vec3 Camera::getDirection()
{
	return direction;
}
void Camera::translate(glm::vec3 translation)
{
	delta_translation += translation;
}
void Camera::rotate(glm::vec3 rotation)
{
	delta_rotation += rotation;
}

glm::mat4 Camera::getViewMatrix()
{
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	position += delta_translation;
	delta_translation = glm::vec3(0.0);
	glm::mat4 rotate_matrix = glm::mat4(1.0);
	rotate_matrix = glm::rotate(rotate_matrix, delta_rotation.x, glm::vec3(1.0, 0.0, 0.0));
	rotate_matrix = glm::rotate(rotate_matrix, delta_rotation.y, glm::vec3(0.0, 1.0, 0.0));
	rotate_matrix = glm::rotate(rotate_matrix, delta_rotation.z, glm::vec3(0.0, 0.0, 1.0));
	direction = rotate_matrix * glm::vec4(direction, 1.0);
	direction = glm::normalize(direction);
	delta_rotation = glm::vec3(0.0);
	return glm::lookAt(position, position + direction, up);
}
