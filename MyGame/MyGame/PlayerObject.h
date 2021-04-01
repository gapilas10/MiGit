#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

// PlayerObject holds the state of the player inheriting relevant state data from GameObject.
// Constains some extra functionality specific to the player's behaviour
//

enum CollisionState
{
	COLLISION_TOP,
	COLLISION_BOTTOM,
	COLLISION_LEFT,
	COLLISION_RIGHT,
	COLLISION_NONE
};
enum LastMove
{
	LEFT,
	UP,
	RIGHT,
	NONE
};

class PlayerObject : public GameObject
{
public:
	CollisionState State;
	LastMove LastMove;
	GLfloat playerSpeed{};
	GLfloat Fuel;
	
	// Constructor(s)
	PlayerObject();
	PlayerObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite);

};

