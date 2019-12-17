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

	attackBox.setSize(Vector2f(110.0f, 90.0f));
	attackBox.setOrigin(attackBox.getSize() / 2.0f);
	attackBox.setFillColor(Color::Transparent);


}


Player::~Player()
{
}

void Player::Update(Texture* texture, Texture* comboTexture, Vector2f position, bool getHit, float time)
{
	body.move(velocity);
	attackBox.setPosition(0.0f, 2000.0f);

	velocity.x = 0.0f;
	velocity.y += 3.0f * time;

	if (velocity.y >= 0 and jumping)
	{
		jumping = false;
		falling = true;
	}
		
		
	if (Keyboard::isKeyPressed(Keyboard::Left) and position.x > 0 and !animation.isCombo)
	{
		combo = 0;
		velocity.x -= 0.45f;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right) and position.x < 3400 and !animation.isCombo)
	{
		combo = 0;
		velocity.x += 0.45f;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up) and canJump and !animation.isCombo)
	{
		canJump = false;
		jumping = true;
		velocity.y = -(jumpheight);
	}
	if (Keyboard::isKeyPressed(Keyboard::Z) and !animation.isCombo)
	{
		animation.isCombo = true;
		body.setSize(Vector2f(90.0f, 90.0f));
		body.setOrigin(body.getSize() / 2.0f);
		body.setTexture(comboTexture);
		combo++;
		if (combo > 3) combo = 0;
		if (faceRight) 	attackBox.setPosition(position.x+25, position.y);
		else attackBox.setPosition(position.x-25, position.y);
	}
	if (Keyboard::isKeyPressed(Keyboard::X) and !Dash and canDash and dashTime.getElapsedTime().asSeconds() > 1.5f)
	{
		Dash = true;
		dashTime.restart().asSeconds();
	}
		
	if (Dash and dashTime.getElapsedTime().asSeconds() > 0.25f)
		Dash = false;
	else if (Dash)
	{
		if (faceRight)
			velocity.x += 1.5;
		else
			velocity.x -= 1.5;
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

	if (animation.isCombo)
	{
		body.setSize(Vector2f(108.0f, 90.0f));
		body.setOrigin(body.getSize() / 2.0f);
		body.setTexture(comboTexture);
		animation.Update(comboTexture, combo, time, faceRight);
	}
		
	else
	{
		body.setSize(Vector2f(74.0f, 90.0f));
		body.setOrigin(body.getSize() / 2.0f);
		body.setTexture(texture);
		animation.Update(texture, row, time, faceRight);
	}
		
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
	window.draw(attackBox);
}
