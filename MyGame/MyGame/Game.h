#pragma once

#include <gl/glew.h>
#include <glfw/glfw3.h>

#include "GameLevel.h"

// Represents the current state of the game
enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_END
};

//// Initial size of the player paddle
//const glm::vec2 PLAYER_SIZE(50, 50);
//// Initial velocity of the player paddle
//const GLfloat PLAYER_VELOCITY(500.0f);

class Game
{
public:
	//Game state
	GameState	State;
	GLboolean	Keys[1024];
	GLuint		Width, Height;
	std::vector<GameLevel>		Levels;
	GLuint Level;
	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	//Initialize game state (load all shaders/textures/levels)
	void Init();
	//Game Loop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
	bool DoCollisions();
	static void Refuel();
	glm::vec2 DoGravity(GLfloat dt);
};

