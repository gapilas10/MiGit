#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include <iostream>

// Game-related State data
SpriteRenderer* Renderer;
PlayerObject* Player;

Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height), Level(0)
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
	ResourceManager::LoadTexture("textures/lava_block.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("textures/spaceship.png", true, "spaceship");
	// Set render-specific controls
	Shader myShader;
	myShader = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(myShader);
	//Load level
	GameLevel MainLevel;	MainLevel.Load("levels/MainLevel.lvl", this->Width, this->Height);
	GameLevel SecondLevel;	SecondLevel.Load("levels/SecondLevel.lvl", this->Width, this->Height);
	GameLevel ThirdLevel;	ThirdLevel.Load("levels/ThirdLevel.lvl", this->Width, this->Height);
	GameLevel FinalLevel;	FinalLevel.Load("levels/FinalLevel.lvl", this->Width, this->Height);
	this->Levels.push_back(MainLevel);
	this->Levels.push_back(SecondLevel);
	this->Levels.push_back(ThirdLevel);
	this->Levels.push_back(FinalLevel);
	this->Level = 0;
	// Configure game objects
	glm::vec2 playerPos = glm::vec2(this->Width / 2 - 50 / 2, this->Height - 50);
	glm::vec2 playerSize(50, 50);
	Player = new PlayerObject(playerPos, playerSize, ResourceManager::GetTexture("spaceship"));
}

void Game::Update(GLfloat dt)
{
	DoGravity(dt);
	Refuel();
}

void Game::Refuel()
{
	if(Player->Fuel < 30)
	{
		Player->Fuel += 0.1;
	}
}

glm::vec2 Game::DoGravity(GLfloat dt)
{
	if(!DoCollisions())
	{
		// Check if outsite window bounds
		if (Player->Position.y >= this->Height - Player->Size.y)
		{
		// Check level
			switch (this->Level)
			{
			case 1:
				this->Level--;
				Player->Position.y = 0;
				break;
			case 2:
				this->Level--;
				Player->Position.y = 0;
				break;
			case 3:
				this->Level--;
				Player->Position.y = 0;
				break;
			}
			return Player->Position;
		}

		Player->Position.y += Player->playerSpeed * dt / 2; // Acting as gravity
		Player->LastMove = NONE;
		return Player->Position;
	}
}

GLboolean CheckCollision(GameObject& one, GameObject& two);

bool Game::DoCollisions()
{
	bool collision = false;
	for (GameObject &platform : this->Levels[this->Level].Platforms)
	{
		if(CheckCollision(*Player, platform))
		{
			//There was a collision
			//find out and 'uncollide'

			switch (Player->LastMove)
			{
			case LEFT:
				Player->State = COLLISION_LEFT;
				Player->Position.x += 1;
				break;
			case RIGHT:
				Player->State = COLLISION_RIGHT;
				Player->Position.x -= 1;
				break;
			case UP:
				Player->State = COLLISION_TOP;
				if (Player->Position.y > 0) // If not on top of the screen
				{
					Player->Position.y += 1;
				}
				break;
			case NONE:
				Player->State = COLLISION_BOTTOM;
				Player->Position.y -= 1;
				break;
			}
			collision = true;
		}
		else
		{
			Player->State = COLLISION_NONE;
		}
	}
	return collision;
}

GLboolean CheckCollision(GameObject &one, GameObject &two)
{
	// Collision x-axis?
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// Collision y-axis?
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// Collision only if on both axes
	return collisionX && collisionY;
}



void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = Player->playerSpeed* dt;
		// Move player left
		if(this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0) // If not on left wall
			{
				// Check for collisions
				if (!DoCollisions())
				{
					Player->Position.x -= velocity; // move left
					Player->LastMove = LEFT;
				}
			}
		}
		// Move player right
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x) // If not on right wall
			{
				// Check for collisions
				if (!DoCollisions())
				{
					Player->Position.x += velocity; // move right
					Player->LastMove = RIGHT;
				}
			}
		}
		// Move player up
		if (this->Keys[GLFW_KEY_W])
		{
			if (Player->Fuel >= 0)
			{ // If the ship has enough fuel				
				if (Player->Position.y >= 0) // If not on top wall
				{
					// Check for collisions
					if (!DoCollisions())
					{
						velocity *= 2;
						Player->Position.y -= velocity; // move down
						Player->LastMove = UP;
						Player->Fuel -= 1;
					}
				}
				else
				{
					Player->Fuel = 30;
					switch (this->Level) //Check current level and change accordingly
					{
					case 0: 
						this->Level++;
						Player->Position.y = Height - Player->Size.y; //"Reset so that the player appears at the bottom of the screen as if the ship teleported there
						break;
					case 1:
						this->Level++;
						Player->Position.y = Height - Player->Size.y;
						break;
					case 2:
						this->Level++;
						Player->Position.y = Height - Player->Size.y;
						this->Render();
						break;
					case 3:
						break;
					}
				}
			}
		}
	}
}

void Game::Render()
{
	Texture2D myTexture = ResourceManager::GetTexture("background");
	// Draw background
	Renderer->DrawSprite(myTexture, glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
	// Draw current level
	this->Levels[this->Level].Draw(*Renderer);
	// Draw player
	Player->Draw(*Renderer);
}