#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Animation.h"
#include "Collider.h"
using namespace sf;



class Player
{
public:
	Player(Texture* texture, Texture* texture2, Vector2u imageCount, float switchTime, int charactor);
	~Player();

	void Update		 (int stage, float time, int charactor);
	void loadFX      (int charactor);
	void Draw		 (RenderWindow& window);
	void OnCollision (Vector2f direction);
	void SetPosition (float x, float y);
	void SetColor    (int R, int G, int B, int A);

	Vector2f GetPosition()  { return hitBox.getPosition(); }
	Collider GetCollider()  { return Collider(sprite);     }
	Collider GetHitBox()    { return Collider(hitBox);     }
	Collider GetAttackBox() { return Collider(attackBox);  }



public:
	Animation animation;
	Vector2f  velocity;
	uint16_t fire = 0;

	bool Dead       = false;
	bool getHit     = false;
	bool Invincible = false;
	bool canSpecial = true, Dash = false;
	bool castStop   = false, STOP = false;
	bool canJump = true, jumping = false, falling = false, impactGround = false;
	bool isAttack = false, attackBoxON = false;

	bool dashDust = false, jumpDust = false, fallDust = false;
	bool faceRight, cutScene = true;
	unsigned short row, combo;
	unsigned short charactor;
	bool isPressed;

	int HP = 100;
	float lostHP = 0.f, redHP = 100.f;
	float damage, defense;

	int state = 98, bulletspeed = 800;


private:
	RectangleShape hitBox, sprite, attackBox;
	Vector2f position;
	Texture *texture, *texture2;
	Clock dashTime, stopTime, specialDelay, invincibleTime;

	SoundBuffer jumpSoundFX, fallSoundFX, dashSoundFX, attackSoundFX, stopSoundFX;
	Sound jumpFX, fallFX, dashFX, attackFX, stopFX;

	bool  bodyBlink;
	float speed = 300.0f, jumpheight = 535.0f;

	
};

