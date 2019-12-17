#include "Enemy.h"
using namespace sf;



Enemy::Enemy(Texture* texture, Vector2u imageCount, float x, float y, float switchTime, int type):
	animation(texture, imageCount, switchTime)
{
	row = 0;
	faceRight = true;

	if (type == 0)
		body.setSize(Vector2f(75.0f, 75.0f));
	else if (type == 1)
		body.setSize(Vector2f(220.0f, 170.0f));
	else if (type == 2)
		body.setSize(Vector2f(75.0f, 75.0f));
	
	body.setOrigin(body.getSize() / 2.0f);
	body.setPosition(x, y);
	body.setTexture(texture);
	position = body.getPosition();
}


Enemy::~Enemy()
{


}

void Enemy::Update(Texture* texture, float time, Vector2f playerPosition, bool enemyState, int type)
{
	if (enemyState == true)
	{
		body.move(velocity);
		velocity.x = 0.0f;
		if (type != 2)
			velocity.y += 7.0f * time;
		
		if (type == 0) // tiny Enemy
		{
			if (position.x > playerPosition.x)
				velocity.x -= 0.2f;
			if (position.x < playerPosition.x)
				velocity.x += 0.2f;
		}
		if (type == 1) // giant Enemy
		{
			if (position.x > playerPosition.x)
				velocity.x -= 0.08f;
			if (position.x < playerPosition.x)
				velocity.x += 0.08f;
		}
		if (type == 2) // ghost enemy
		{
			if (position.x > playerPosition.x)
				velocity.x -= 0.1f;
			if (position.x < playerPosition.x)
				velocity.x += 0.1f;
			if (position.y > playerPosition.y - 200.0f)
				velocity.y -= 0.0005f;
			if (position.y < playerPosition.y - 200.0f)
				velocity.y += 0.0005f;
		}
		


		if (velocity.x == 0)
			row = 0;
		else if (velocity.x != 0)
		{
			row = 1;
			if (velocity.x > 0.0f) faceRight = true;
			else faceRight = false;
		}


		animation.Update(texture, row, time, faceRight);
		body.setTextureRect(animation.uvRect);
		body.move(velocity * time);
		position += velocity;
	}
}

void Enemy::OnCollision(Vector2f direction)
{
	if (direction.x < 0.0f)
	{
		//collision on left
		velocity.x = 0.0f;
	}
	else if (direction.x > 0.0f)
	{
		//coliision on right
		velocity.x = 0.0f;
	}
	if (direction.y < 0.0f)
	{
		//collision on bottom
		velocity.y = 0.0f;
	}
	else if (direction.y > 0.0f)
	{
		//collision on top
		velocity.y = 0.0f;
	}
}

void Enemy::SetPosition(float x, float y)
{
	body.setPosition(x, y);
	position = body.getPosition();
}

void Enemy::Draw(RenderWindow & window)
{
	window.draw(body);
}
