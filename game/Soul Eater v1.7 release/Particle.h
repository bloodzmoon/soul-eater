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
	//type 3 = atk particle

	Particle(Texture* texture, Vector2u imageCount, float switchTime, int type);
	~Particle();

	void Update(Texture* texture, Player& player, float time, int type);
	void Draw(RenderWindow& window);


public:
	Animation animation;
	RectangleShape effect;

	bool isEffect    = false;
	bool setPosition = false;
	unsigned int row;


private:
	bool faceRight;
};

