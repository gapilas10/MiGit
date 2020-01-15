#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

// PlayerObject holds the state of the player inheriting relevant state data from GameObject.
// Constains some extra functionality specific to the player's behaviour 

class PlayerObject : public GameObject
{
public:
	GLfloat playerSpeed{};
	bool isFalling;
	// Constructor(s)
	PlayerObject();
	PlayerObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite);
	// Gravity pulls object down
	glm::vec2	Gravity(GLfloat dt ,GLuint window_height);
	// Perform 'Bounce'
	void Bounce(GLfloat dt);
};

