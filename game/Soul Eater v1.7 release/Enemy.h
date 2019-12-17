#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Animation.h"
#include "Collider.h"
#include "Player.h"
using namespace sf;



class Enemy
{
public:
	Enemy(Texture* texture, Texture* texture2, Vector2u imageCount, float switchTime, int type);
	~Enemy();

	// type 0 = tiny
	// type 1 = giant
	// type 2 = ghost
	// type 3 = shaman
	// type 4 = darkSphere
	// type 5 = 
	// type 6 = homura
	// type 7 = bullet
	// type 8 =  bazookaa!!

	// type 44 = heal orb
	// type 55 = portal

	void Update		 (float time, Player& player, int type);
	void Draw		 (RenderWindow& window);
	void OnCollision (Vector2f direction);
	void SetPosition (float x, float y);
	void SetColor(int R, int G, int B, int A);

	void enemyHabit1(int type, int enemyState);    // tiny, giant
	void enemyHabit2(int type, int enemyState);    // ghost
	void enemyHabit3(int type, int enemyState);    // shaman
	void enemyHabit4(int type, int enemyState, Texture* texture, Texture* texture2);    // soulEater
	void enemyHabit5(int type, int enemyState, Texture* texture, Texture* texture2);    // Homura

	Vector2f GetPosition()  { return hitBox.getPosition(); }
	Collider GetCollider()  { return Collider(sprite);     }
	Collider GetHitBox()    { return Collider(hitBox);     }
	Collider GetViewBox()   { return Collider(viewBox);    }
	Collider GetAttackBox() { return Collider(attackBox);  }


public:
	Animation animation;
	Clock wait, comboWait, invincibleTime, stopTime;
	Vector2f position, velocity, dropOrb;
	RectangleShape sprite, attackBox, hitBox, viewBox;

	bool faceRight, Dead = false, blink;
	bool startShoot = false ,startShoot2 = false, startShoot3 = false, darkSphereEnd, invincible = false;
	unsigned short state;

	unsigned int row;
	float HP;

	bool STOP = false;
	float bodyDamge = 12.f, bulletDamage = 15.f;
	float damage;

	bool spawnOrb = false;

private:

	Texture *texture, *texture2;

	float tinySpeed = 80.0f, giantSpeed = 40.0f, ghostSpeed = 100.0f;
	float shamanSpeed = 60.0f, darkSphereSpeed = 400.0f;
	float bulletSpeed = 1400.f;
	float souleaterSpeed = 500.0f;

	Vector2f size;
	Vector2f waypoint;
	int repeat = 0;
	int pattern = 99;

	double zeta = 0;

	bool canJump=true, jumping=false, falling=false;
	
	SoundBuffer shootSoundFX;
	Sound shootFX;

};

