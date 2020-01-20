#include "PlayerObject.h"

PlayerObject::PlayerObject() = default;

PlayerObject::PlayerObject(glm::vec2 pos, glm::vec2 size,Texture2D sprite) : GameObject(pos, size, sprite ) , playerSpeed(500.0f), State(COLLISION_NONE),LastMove(NONE) { }








