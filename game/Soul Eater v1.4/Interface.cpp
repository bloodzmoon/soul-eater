#include "Interface.h"
using namespace sf;



Interface::Interface(Texture* texture, Texture* enemyCount, Vector2u imageCount, float switchTime):
	animation(texture, imageCount, switchTime)
{
	row = 0;
	soul.setSize(Vector2f(60.0f, 90.0f));
	soul.setOrigin(soul.getSize() / 2.0f);
	soul.setTexture(texture);

	healthBar.setSize(Vector2f(200.0f,10.0f));
	healthBar.setOrigin(105.0f, 0.0f);

	energyBar.setSize(Vector2f(100.0f, 4.0f));
	energyBar.setOrigin(105.0f, 0.0f);

	EnemyIMG.setSize(Vector2f(70.0f, 70.0f));
	EnemyIMG.setOrigin(EnemyIMG.getSize() / 2.0f);
	EnemyIMG.setTexture(enemyCount);
	
	
}


Interface::~Interface()
{
}

void Interface::Update(Texture* texture, Player player, View camera, float HP, float Energy, float time, bool cutScene)
{
	if (cutScene)
	{
		soul.setPosition(400.f, 246.f);
	}
	else
	{
		if (HP >= 0)
			healthBar.setSize(Vector2f(HP, 10.0f));
		if (Energy >= 0)
			energyBar.setSize(Vector2f(Energy, 5.0f));

		soul.setPosition(camera.getCenter().x - 380.f, camera.getCenter().y - 225.f);
		healthBar.setPosition(camera.getCenter().x - 265.f, camera.getCenter().y - 200.f);
		energyBar.setPosition(camera.getCenter().x - 265.f, camera.getCenter().y - 190.f);
		EnemyIMG.setPosition(camera.getCenter().x + 315.f, camera.getCenter().y - 205.f);
		enemyCount.setPosition(camera.getCenter().x + 355.f, camera.getCenter().y - 230.f);
	}
	
	animation.Update(texture, row, time, 1);
	soul.setTextureRect(animation.uvRect);
}

void Interface::Draw(RenderWindow & window)
{
	window.draw(healthBar);
	window.draw(energyBar);
	window.draw(soul);
	window.draw(EnemyIMG);
	window.draw(enemyCount);
}
