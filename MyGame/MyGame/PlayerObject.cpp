#include "PlayerObject.h"

PlayerObject::PlayerObject() = default;

PlayerObject::PlayerObject(glm::vec2 pos, glm::vec2 size,Texture2D sprite) : GameObject(pos, size, sprite ) , playerSpeed(500.0f) { }

glm::vec2 PlayerObject::Gravity(GLfloat dt, GLuint window_height)
{	
	// Check if outsite window bounds and if so return
	if(this->Position.y >= window_height - this->Size.y)
	{
		return this->Position;		
	}

		this->Position.y += this->playerSpeed * dt /2; // Acting as gravity
	
}






