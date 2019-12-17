#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Collider.h"
using namespace sf;



class Player
{
public:
	Player(Texture* texture, Vector2u imageCount, float switchTime);
	~Player();

	void Update(Texture* texture, Vector2f position, float time);
	void Draw(RenderWindow& window);
	void OnCollision(Vector2f direction);

	Vector2f GetPosition() { return body.getPosition(); }
	Collider GetCollider() { return Collider(body); }


private:
	RectangleShape body;
	Animation animation;
	unsigned int row;
	bool faceRight;

	Vector2f velocity;

	float jumpheight = 1.2f;
	bool canJump, jumping, falling;
};

