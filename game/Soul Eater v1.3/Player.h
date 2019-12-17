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

	void Update		 (Texture* texture, Texture* combo, Vector2f position, bool Invincible, float time);
	void Draw		 (RenderWindow& window);
	void OnCollision (Vector2f direction);

	Vector2f GetPosition()  { return hitBox.getPosition();}
	Collider GetCollider()  { return Collider(sprite);      }
	Collider GetHitBox()    { return Collider(hitBox);    }
	Collider GetAttackBox() { return Collider(attackBox); }



public:
	Animation animation;
	Vector2f velocity;

	bool getHit = false;
	bool Invincible;
	bool Dash, canDash;
	bool jumping, falling, canJump, impactGround;

	bool dashDust = false, jumpDust = false, fallDust = false;
	bool faceRight;

	float HP = 200.0f, Energy = 100.0f;


private:
	RectangleShape sprite, hitBox, attackBox;
	Clock dashTime;

	unsigned int row, combo;
	bool  bodyBlink, isAttack;
	float speed = 300.0f, jumpheight = 535.0f;
};

