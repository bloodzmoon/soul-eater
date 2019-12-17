#include "Interface.h"
using namespace sf;



Interface::Interface(Texture* texture, Texture* enemyCount, Vector2u imageCount, float switchTime):
	animation(texture, imageCount, switchTime)
{
	row = 0;
	soul.setSize(Vector2f(90.0f, 90.0f));
	soul.setOrigin(soul.getSize() / 2.0f);
	soul.setTexture(texture);

	healthBar.setSize(Vector2f(200.0f,10.0f));
	healthBar.setOrigin(105.0f, 0.0f);
	healthBar.setFillColor(Color(95, 205, 228));

	energyBar.setSize(Vector2f(100.0f, 4.0f));
	energyBar.setOrigin(105.0f, 0.0f);
	energyBar.setFillColor(Color(255, 226, 0));

	EnemyCount.setSize(Vector2f(70.0f, 70.0f));
	EnemyCount.setOrigin(EnemyCount.getSize() / 2.0f);
	EnemyCount.setTexture(enemyCount);
	
	
}


Interface::~Interface()
{
}

void Interface::Update(Texture* texture, Vector2f position, float HP, float Energy, float time, bool stopMoving)
{
	if (HP >= 0)
		healthBar.setSize(Vector2f(HP, 10.0f));
	if (Energy >= 0)
		energyBar.setSize(Vector2f(Energy, 4.0f));

	if (stopMoving)
	{
		soul.setPosition(savedCamera.x, position.y - 290.0f);
		healthBar.setPosition(savedCamera.x + 115.0f, position.y - 280.0f);
		energyBar.setPosition(savedCamera.x + 115.0f, position.y - 267.0f);
		EnemyCount.setPosition(savedCamera.x + 710.0f, position.y - 280.0f);
		
	}
	else 
	{
		soul.setPosition(position.x - 360.0f, position.y - 290.0f);
		healthBar.setPosition(position.x - 245.0f, position.y - 280.0f);
		energyBar.setPosition(position.x - 245.0f, position.y - 267.0f);
		EnemyCount.setPosition(position.x + 350.0f, position.y - 280.0f);

		savedCamera.x = position.x - 360.0f;
	}
	

	animation.Update(texture, row, time, faceRight);
	soul.setTextureRect(animation.uvRect);
}

void Interface::Draw(RenderWindow & window)
{
	window.draw(healthBar);
	window.draw(energyBar);
	window.draw(soul);
	window.draw(EnemyCount);
}
