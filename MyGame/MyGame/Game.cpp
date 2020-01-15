#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "PlayerObject.h"

// Game-related State data
SpriteRenderer* Renderer;
PlayerObject* Player;

Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
	delete Renderer;
	delete Player;
}

void Game::Init()
{
	// Load shaders
	ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// Load textures
	ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("textures/spaceship.png", true, "spaceship");
	// Set render-specific controls
	Shader myShader;
	myShader = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(myShader);
	//Load level
	GameLevel MainLevel;
	MainLevel.Load("levels/MainLevel.lvl", this->Width, this->Height);
	this->Level = MainLevel;
	// Configure game objects
	glm::vec2 playerPos = glm::vec2(this->Width / 2 - 50 / 2, this->Height - 50);
	glm::vec2 playerSize(50, 50);
	Player = new PlayerObject(playerPos, playerSize, ResourceManager::GetTexture("spaceship"));
}

void Game::Update(GLfloat dt)
{
	// Check for collisions
	this->DoCollisions();
	Player->Gravity(dt, this->Height);
}

void Game::DoCollisions()
{
	for (GameObject &platform : this->Level.Platforms)
	{
		//if(CheckCollision(*Player, platform))
		//{
		//	
		//}
	}
}



void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = 500.f * dt;
		// Move player horizontally
		if(this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0) // If not on left wall
			{
				Player->Position.x -= velocity; // move left
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x) // If not on right wall
			{
				Player->Position.x += velocity; // move right
			}
		}
		if (this->Keys[GLFW_KEY_W])
		{
			if (Player->Position.y >= 0) // If not on top wall
			{
				Player->Position.y -= velocity; // move right
			}
		}
		if (this->Keys[GLFW_KEY_S])
		{
			if (Player->Position.y <= this->Height - Player->Size.y) // If not on top wall
			{
				Player->Position.y += velocity; // move right
			}
		}
	}
}

void Game::Render()
{
	Texture2D myTexture = ResourceManager::GetTexture("background");
	// Draw background
	Renderer->DrawSprite(myTexture, glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
	// Draw level
	this->Level.Draw(*Renderer);
	// Draw player
	Player->Draw(*Renderer);
}