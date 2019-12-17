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

	void Update(Texture* texture, Texture* combo, Vector2f position, bool getHit, float time);
	void Draw(RenderWindow& window);
	void OnCollision(Vector2f direction);

	Vector2f GetPosition() { return body.getPosition(); }
	Collider GetCollider() { return Collider(body); }
	Collider GetAttackBox() { return Collider(attackBox); }

	bool Dash, canDash;
	Vector2f velocity;


private:
	RectangleShape body, attackBox;
	Animation animation;
	Clock dashTime;
	unsigned int row, combo;
	bool faceRight;


	float jumpheight = 1.2f;
	bool canJump, jumping, falling;
};

