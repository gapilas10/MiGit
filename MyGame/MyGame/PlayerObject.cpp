#include "PlayerObject.h"

PlayerObject::PlayerObject() : GameObject() { }

PlayerObject::PlayerObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite) : GameObject(pos, size, sprite, glm::vec3(1.0) ) { }

glm::vec2 PlayerObject::Gravity(GLfloat dt, GLuint window_height)
{	
	// Check if outsite window bounds and if so return
	if(this->Position.y >= window_height - this->Size.y)
	{
		return this->Position;
	}
	
	this->Position.y += 100.f * dt; // Acting as gravity
	return this->Position;
}


