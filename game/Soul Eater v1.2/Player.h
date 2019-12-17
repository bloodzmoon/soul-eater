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

	void Update		 (Texture* texture, Texture* combo, Texture* dash, Vector2f position, bool Invincible, float time);
	void Draw		 (RenderWindow& window);
	void OnCollision (Vector2f direction, bool getHit);

	Vector2f GetPosition()  { return hitBox.getPosition();  }
	Collider GetCollider()  { return Collider(body);      }
	Collider GetHitBox()    { return Collider(hitBox);    }
	Collider GetAttackBox() { return Collider(attackBox); }

	bool Dash, canDash;
	bool Invincible;
	bool onBoder;
	Vector2f velocity;


private:
	RectangleShape body, hitBox, attackBox;
	Animation animation;
	Clock dashTime;

	unsigned int row, combo;
	bool faceRight;
	bool  canJump, jumping, falling;
	bool  bodyBlink;

	float jumpheight = 1.3f;
};

