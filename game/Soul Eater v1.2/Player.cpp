#include "Player.h"
using namespace sf;



Player::Player(Texture* texture, Vector2u imageCount, float switchTime):
	animation(texture, imageCount, switchTime)
{
	row = 0;
	faceRight = true;

	body.setSize(Vector2f(70.0f, 90.0f));
	body.setOrigin(body.getSize() / 2.0f);
	body.setTexture(texture);
	
	hitBox.setSize(Vector2f(70.0f, 90.0f));
	hitBox.setOrigin(hitBox.getSize() / 2.0f);
	hitBox.setPosition(500.0f, 400.0f);

	attackBox.setSize(Vector2f(110.0f, 90.0f));
	attackBox.setOrigin(attackBox.getSize() / 2.0f);
}

Player::~Player()
{
}


void Player::Update(Texture* texture, Texture* comboTexture, Texture* dashTexture, Vector2f position, bool Invincible, float time)
{

// ALL Movement -----------
	
	hitBox.move(velocity);
	velocity.y += 3.0f * time;
	velocity.x  = 0.0f;

	if (Keyboard::isKeyPressed(Keyboard::Left) and !animation.isCombo)
	{
		combo = 0;
		velocity.x -= 0.45f;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right) and !animation.isCombo)
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
		animation.currentImage.x = 0;

		combo++;
		if (combo > 3) combo = 0;

		if (faceRight) 	
			attackBox.setPosition(position.x+7, position.y);
		else 
			attackBox.setPosition(position.x-7, position.y);

		body.setSize(Vector2f(90.0f, 90.0f));
		body.setTexture(comboTexture);
	}
	if (Keyboard::isKeyPressed(Keyboard::X) and !Dash and canDash and dashTime.getElapsedTime().asSeconds() > 1.2f) 
	{
		Dash = true;
		dashTime.restart().asSeconds();

		body.setTexture(comboTexture);
		body.setSize(Vector2f(90.0f, 90.0f));
	}
	
	

// Player status -------------------

	// Dash
	if (Dash and dashTime.getElapsedTime().asSeconds() > 0.225f)
		Dash = false;
	else if (Dash)
	{
		if (faceRight) velocity.x += 1.5;
		else           velocity.x -= 1.5;
	}
	
	// Invincible
	if (Invincible and bodyBlink)
	{	body.setFillColor(Color(255, 255, 255, 0));	 bodyBlink = false;	}
	else if (Invincible and !bodyBlink)
	{	body.setFillColor(Color(255, 255, 255, 255)); bodyBlink = true;	}
	else
		body.setFillColor(Color(255, 255, 255, 255));

	// start falling
	if (velocity.y >= 0 and jumping)
	{
		jumping = false;
		falling = true;
	}


// Set Animation ---------------------

	// idle
	if (velocity.x == 0 and !jumping and !falling) 
		row = 0;

	// walking
	else if (velocity.x != 0 and !jumping and !falling) 
	{
		row = 1;

		if (velocity.x > 0.0f) faceRight = true;
		else        		   faceRight = false;
	}

	// jumping
	else if (jumping and velocity.x == 0)                
		row = 2;
	else if (jumping and velocity.x != 0)    
	{
		row = 2;

		if (velocity.x > 0.0f) faceRight = true;
		else				   faceRight = false;
	}

	// falling
	else if (falling and velocity.x == 0)                
		row = 3;
	else if (falling and velocity.x != 0)                
	{
		row = 3;

		if (velocity.x > 0.0f) faceRight = true;
		else 		           faceRight = false;
	}

// Animation Update ---------------------

	// attacking
	if (animation.isCombo)
	{
		body.setSize(Vector2f(108.0f, 90.0f));
		body.setTexture(comboTexture);
		body.setOrigin(body.getSize() / 2.0f);
		animation.Update(comboTexture, combo, time, faceRight);
	}
	else
	{	
		// dashing
		if (Dash)
		{
			body.setSize(Vector2f(220.0f, 90.0f));
			body.setTexture(dashTexture);
			body.setOrigin(body.getSize() / 2.0f);
			animation.Update(dashTexture, row, time, faceRight);
		}
		// normal
		else
		{
			body.setSize(Vector2f(70.0f, 90.0f));
			body.setTexture(texture);
			body.setOrigin(body.getSize() / 2.0f);
			animation.Update(texture, row, time, faceRight);
		}
	}

	body.setTextureRect(animation.uvRect);
	hitBox.move(velocity * time);

// Set HitBox --------------
	body.setPosition(position);
	if (!animation.isCombo) attackBox.setPosition(0.0f, 2000.0f);

	/////////////////////
	// Show hit box
	if (Keyboard::isKeyPressed(Keyboard::RAlt))
	{
		hitBox.setFillColor(Color::Green);
		attackBox.setFillColor(Color::Blue);
	}
	else
	{
		hitBox.setFillColor(Color::Transparent);
		attackBox.setFillColor(Color::Transparent);
	}

}

void Player::OnCollision(Vector2f direction, bool getHit)
{

	if (getHit and !Invincible /*and !onBoder*/)
	{
		if (direction.x < 0.0f)
		{
			//collision on left
			faceRight = false;
			Invincible = true;
			hitBox.move(+15.0f, -5.0f);
		}
		else if (direction.x > 0.0f)
		{
			//coliision on right
			faceRight = true;
			Invincible = true;
			hitBox.move(-15.0f, -5.0f);
		}
		if (direction.y < 0.0f)
		{
			//collision on bottom
			if (faceRight) hitBox.move(-15.0f, -5.0f);
			else           hitBox.move(+15.0f, -5.0f);
			Invincible = true;
		}
		else if (direction.y > 0.0f)
		{
			//collision on top
			if (faceRight) hitBox.move(-15.0f, -5.0f);
			else           hitBox.move(+15.0f, -5.0f);
			Invincible = true;
		}
	}
	else
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
}

void Player::Draw(RenderWindow& window)
{
	window.draw(hitBox);
	window.draw(attackBox);
	window.draw(body);
}
