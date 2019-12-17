#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Player.h"
using namespace sf;


class Interface
{
public:
	Interface(Texture* texture, Texture* enemyCount, Vector2u imageCount, float switchTime);
	~Interface();

	void Update(Texture* texture, Player player, View camera, float HP, float Energy, float time);
	void Draw(RenderWindow& window);


private:
	RectangleShape soul, healthBar, energyBar, EnemyCount;
	Animation animation;

	unsigned int row;
	Vector2f cameraMovement;
};

