#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Animation.h"
#include "Collider.h"
#include "Player.h"
using namespace sf;



class Weapon
{
public:
	Weapon(Texture* texture, Vector2u imageCount, float switchTime, int type);
	~Weapon();

	void Update(float time, int type);
	void Draw(RenderWindow& window);

	Collider GetCollider() { return Collider(box); }
	Vector2f GetPosition() { return box.getPosition(); }
	void SetPosition(float x, float y);


public:
	Animation animation;
	Clock wait;
	int state;
	bool faceRight;

private:
	RectangleShape box, sprite;
	Texture *texture;
	Vector2f velocity;

	float speed = 1800.f;
	unsigned short row;
};

