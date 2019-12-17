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

	void Update(Texture* texture, float time, Vector2f playerPosition, bool enemyState, int type);
	void Draw(RenderWindow& window);
	void OnCollision(Vector2f direction);
	void SetPosition(float x, float y);

	Vector2f GetPosition() { return body.getPosition(); }
	Collider GetCollider() { return Collider(body); }

private:
	RectangleShape body;
	Animation animation;
	unsigned int row;
	bool faceRight;

	Vector2f velocity;
	Vector2f position;

};

