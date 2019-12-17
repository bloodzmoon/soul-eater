#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Collider.h"
using namespace sf;



class Enemy
{
public:
	Enemy(Texture* texture, Vector2u imageCount, float x, float y, float switchTime, int type);
	~Enemy();

	// type 0 = tiny
	// type 1 = giant
	// type 2 = ghost
	// type 3 = shaman
	// type 4 = darkSphere

	void Update		 (Texture* texture, float time, Vector2f position, Vector2f playerPosition, int enemyState, int type);
	void Draw		 (RenderWindow& window);
	void OnCollision (Vector2f direction);
	void SetPosition (float x, float y);

	void enemyHabit1(int type, int enemyState);    // tiny, giant
	void enemyHabit2(int type, int enemyState);    // ghost
	void enemyHabit3(int type, int enemyState);    // shaman

	Vector2f GetPosition()  { return body.getPosition();  }
	Collider GetCollider()  { return Collider(body);      }
	Collider GetHitBox()    { return Collider(hitBox);    }
	Collider GetViewBox()   { return Collider(viewBox);   }
	Collider GetAttackBox() { return Collider(attackBox); }


public:
	Animation animation;

	bool faceRight;
	bool castDarkSphere, darkSphereEnd;

private:
	RectangleShape body, attackBox, hitBox, viewBox;
	
	unsigned int row;
	int comboWait;

	Vector2f velocity;
	Vector2f position;

};

