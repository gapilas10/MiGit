#pragma once


#include <gl/glew.h>
#include <glfw/glfw3.h>

// Represents the current state of the game
enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_END
};


class Game
{
public:
	//Game state
	GameState	State;
	GLboolean	Keys[1024];
	GLuint		Width, Height;
	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	//Initialize game state (load all shaders/textures/levels)
	void Init();
	//Game Loop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
};
