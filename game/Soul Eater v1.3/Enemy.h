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
	// type 5 = soulEater

	void Update		 (Texture* texture, Texture* texture2, float time, Vector2f position, Vector2f playerPosition, int type);
	void Draw		 (RenderWindow& window);
	void OnCollision (Vector2f direction, bool getHit);
	void SetPosition (float x, float y);

	void enemyHabit1(int type, int enemyState);    // tiny, giant
	void enemyHabit2(int type, int enemyState);    // ghost
	void enemyHabit3(int type, int enemyState);    // shaman
	void enemyHabit4(int type, int enemyState, Texture* texture, Texture* texture2);    // soulEater

	Vector2f GetPosition()  { return body.getPosition();  }
	Collider GetCollider()  { return Collider(body);      }
	Collider GetHitBox()    { return Collider(hitBox);    }
	Collider GetViewBox()   { return Collider(viewBox);   }
	Collider GetAttackBox() { return Collider(attackBox); }


public:
	Animation animation;
	Clock wait, comboWait;
	Vector2f position;

	bool faceRight;
	bool castDarkSphere, darkSphereEnd;
	unsigned int state;

private:
	RectangleShape body, attackBox, hitBox, viewBox;
	
	unsigned int row;

	float tinySpeed = 80.0f, giantSpeed = 40.0f, ghostSpeed = 100.0f;
	float shamanSpeed = 60.0f, darkSphereSpeed = 400.0f;
	float souleaterSpeed = 500.0f;

	Vector2f velocity;
	

};

