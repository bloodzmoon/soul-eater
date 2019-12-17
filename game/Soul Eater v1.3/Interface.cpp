#include "Interface.h"
using namespace sf;



Interface::Interface(Texture* texture, Texture* enemyCount, Vector2u imageCount, float switchTime):
	animation(texture, imageCount, switchTime)
{
	row = 0;
	soul.setSize(Vector2f(60.0f, 90.0f));
	soul.setOrigin(soul.getSize() / 2.0f);
	soul.setTexture(texture);
	soul.setPosition(-380.0f, -210.0f);

	healthBar.setSize(Vector2f(200.0f,10.0f));
	healthBar.setOrigin(105.0f, 0.0f);
	healthBar.setFillColor(Color(95, 205, 228));
	healthBar.setPosition(-260.0f, -195.0f);

	energyBar.setSize(Vector2f(100.0f, 4.0f));
	energyBar.setOrigin(105.0f, 0.0f);
	energyBar.setFillColor(Color(255, 226, 0));
	energyBar.setPosition(-265.0f, -180.0f);

	EnemyCount.setSize(Vector2f(70.0f, 70.0f));
	EnemyCount.setOrigin(EnemyCount.getSize() / 2.0f);
	EnemyCount.setTexture(enemyCount);
	EnemyCount.setPosition(380.0f, -210.0f);
	
	
}


Interface::~Interface()
{
}

void Interface::Update(Texture* texture, Player player, View camera, float HP, float Energy, float time)
{
	if (HP >= 0)
		healthBar.setSize(Vector2f(HP, 10.0f));
	if (Energy >= 0)
		energyBar.setSize(Vector2f(Energy, 4.0f));

	cameraMovement = player.GetPosition() - camera.getCenter();
	cameraMovement += Vector2f(0.0f, -90.0f);

	soul.move(cameraMovement * time * 2.5f);
	healthBar.move(cameraMovement * time * 2.5f);
	energyBar.move(cameraMovement * time * 2.5f);
	EnemyCount.move(cameraMovement * time * 2.5f);
	

	animation.Update(texture, row, time, 1);
	soul.setTextureRect(animation.uvRect);
}

void Interface::Draw(RenderWindow & window)
{
	window.draw(healthBar);
	window.draw(energyBar);
	window.draw(soul);
	window.draw(EnemyCount);
}
