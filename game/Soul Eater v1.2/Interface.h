#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
using namespace sf;


class Interface
{
public:
	Interface(Texture* texture, Texture* enemyCount, Vector2u imageCount, float switchTime);
	~Interface();

	void Update(Texture* texture, sf::Vector2f position, float HP, float Energy, float time, bool moving);
	void Draw(RenderWindow& window);


private:
	RectangleShape soul, healthBar, energyBar, EnemyCount;
	Animation animation;

	bool faceRight=1;
	unsigned int row;
	Vector2f savedCamera;
};

