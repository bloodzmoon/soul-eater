#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Animation.h"
#include "Collider.h"
using namespace sf;



class Player
{
public:
	Player(Texture* texture, Vector2u imageCount, float switchTime, int charactor);
	~Player();

	void Update		 (Texture* texture, Texture* combo, Vector2f position, bool Invincible, float time, int charactor);
	void Draw		 (RenderWindow& window);
	void OnCollision (Vector2f direction);

	Vector2f GetPosition()  { return hitBox.getPosition(); }
	Collider GetCollider()  { return Collider(sprite);     }
	Collider GetHitBox()    { return Collider(hitBox);     }
	Collider GetAttackBox() { return Collider(attackBox);  }



public:
	Animation animation;
	RectangleShape hitBox;
	Vector2f velocity;

	bool Dead       = false;
	bool getHit     = false;
	bool Invincible = false;
	bool Dash, canDash;
	bool jumping, falling, canJump, impactGround, isAttack;
	bool attackBoxON = false;

	bool dashDust = false, jumpDust = false, fallDust = false;
	bool faceRight, cutScene = true;
	unsigned int row, combo;
	int charactor;

	float HP = 200.0f, Energy = 100.0f;


private:
	RectangleShape sprite, attackBox;
	Clock dashTime;

	bool  bodyBlink;
	float speed = 300.0f, jumpheight = 535.0f;
};

