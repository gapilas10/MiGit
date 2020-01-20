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
	//Player->Gravity(dt, this->Height, DoCollisions());
	DoGravity(dt);
}

glm::vec2 Game::DoGravity(GLfloat dt)
{
	if(!DoCollisions())
	{
		// Check if outsite window bounds and if so return
		if (Player->Position.y >= this->Height - Player->Size.y)
		{
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
	for (GameObject &platform : this->Level.Platforms)
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
				Player->Position.y += 1;
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
			if (Player->Position.y >= 0) // If not on top wall
			{
				// Check for collisions
				if (!DoCollisions())
				{
					velocity *= 2;
					Player->Position.y -= velocity; // move down
					Player->LastMove = UP;
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
	// Draw level
	this->Level.Draw(*Renderer);
	// Draw player
	Player->Draw(*Renderer);
}