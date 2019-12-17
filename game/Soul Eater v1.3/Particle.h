#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Animation.h"
using namespace sf;
using namespace std;



class Particle
{
public:
	//type 1 = hit particle
	//type 2 = dust particle

	Particle(Texture* texture, Vector2u imageCount, float switchTime, int type);
	~Particle();

	void Update(Texture* texture, Player& player, Vector2f position, float time, int type);
	void Draw(RenderWindow& window);


public:
	Animation animation;

	bool isEffect    = false;
	bool setPosition = false;
	unsigned int row;


private:
	RectangleShape effect;
	bool faceRight;
};

