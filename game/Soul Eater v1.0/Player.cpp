#include "Player.h"
using namespace sf;



Player::Player(Texture* texture, Vector2u imageCount, float switchTime):
	animation(texture, imageCount, switchTime)
{
	row = 0;
	faceRight = true;

	body.setSize(Vector2f(74.0f, 90.0f));
	body.setOrigin(body.getSize() / 2.0f);
	body.setPosition(500.0f, 400.0f);
	body.setTexture(texture);

}


Player::~Player()
{
}

void Player::Update(Texture* texture, Vector2f position, float time)
{
	body.move(velocity);

	velocity.x = 0.0f;
	velocity.y += 3.0f * time;
		
	if (velocity.y >= 0 and jumping)
	{
		jumping = false;
		falling = true;
	}
		
	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		velocity.x -= 0.45f;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		velocity.x += 0.45f;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up) and canJump)
	{
		canJump = false;
		jumping = true;
		velocity.y = -(jumpheight);
	}
		

	if (velocity.x == 0 and !jumping and !falling)        //idle
		row = 0;
	else if (velocity.x != 0 and !jumping and !falling)   //walking
	{
		row = 1;

		if (velocity.x > 0.0f) 
			faceRight = true;
		else 
			faceRight = false;
	}
	else if (jumping and velocity.x == 0)                 //jumping
		row = 2;
	else if (jumping and velocity.x != 0)                 //jumping + move
	{
		row = 2;
		if (velocity.x > 0.0f) 
			faceRight = true;
		else 
			faceRight = false;
	}
	else if (falling and velocity.x == 0)                 //falling
		row = 3;
	else if (falling and velocity.x != 0)                 //falling + move
	{
		row = 3;
		if (velocity.x > 0.0f) 
			faceRight = true;
		else 
			faceRight = false;
	}

	
	animation.Update(texture, row, time, faceRight);
	body.setTextureRect(animation.uvRect);
	body.move(velocity * time);
}

void Player::OnCollision(Vector2f direction)
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
		canJump = true;
		jumping = false;
		falling = false;
	}
	else if (direction.y > 0.0f)
	{
		//collision on top
		velocity.y = 0.0f;
	}
}

void Player::Draw(RenderWindow& window)
{
	window.draw(body);
}
