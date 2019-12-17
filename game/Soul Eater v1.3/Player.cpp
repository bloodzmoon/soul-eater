#include "Player.h"
using namespace sf;



Player::Player(Texture* texture, Vector2u imageCount, float switchTime):
	animation(texture, imageCount, switchTime)
{
	row = 0;
	faceRight = true;

	sprite.setSize(Vector2f(128.0f, 113.0f));
	sprite.setOrigin(sprite.getSize() / 2.0f);
	sprite.setTexture(texture);
	
	hitBox.setSize(Vector2f(60.0f, 95.0f));
	hitBox.setOrigin(hitBox.getSize() / 2.0f);
	hitBox.setPosition(500.0f, 400.0f);

	attackBox.setSize(Vector2f(50.0f, 90.0f));
	attackBox.setOrigin(attackBox.getSize() / 2.0f);
}

Player::~Player()
{
}


void Player::Update(Texture* texture, Texture* comboTexture, Vector2f position, bool Invincible, float time)
{

// ALL Movement -----------

	hitBox.move(velocity * time);
	velocity.y += 981.0f * time;
	velocity.x  = 0.0f;

	if (Keyboard::isKeyPressed(Keyboard::Left) and !Dash)
	{
		if (isAttack)
			velocity.x -= speed / 6.0f;
		else
			velocity.x -= speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right) and !Dash)
	{
		if (isAttack)
			velocity.x += speed / 6.0f;
		else
			velocity.x += speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up) and canJump and !isAttack)
	{
		canJump = false;
		jumping = true;
		impactGround = true;
		velocity.y = -(jumpheight);
		combo = 0;
		animation.currentImage.x = 0;
	}
	if (Keyboard::isKeyPressed(Keyboard::Z) and !isAttack and !getHit)
	{
		isAttack = true;
		animation.OneTimeAnimation = true;
		animation.playingAnimation = true;
		animation.currentImage.x = 0;

		if (Dash)
			combo = 3;
		else if (jumping or falling)
			combo = 4;
		else
		{ 
			combo++;
			if (combo > 2)combo = 1;
		}
		
		if (faceRight) 	
			attackBox.setPosition(position.x+7, position.y);
		else 
			attackBox.setPosition(position.x-7, position.y);
	}
	if (Keyboard::isKeyPressed(Keyboard::X) and !Dash and canDash and dashTime.getElapsedTime().asSeconds() > 1.2f and !animation.playingAnimation) 
	{
		Dash = true;
		dashTime.restart().asSeconds();
	}
	

// Player status-------------------

	// get hit
	if (getHit and !animation.playingAnimation)
		getHit = false;

	// Dash
	if (Dash and dashTime.getElapsedTime().asSeconds() > 0.25f)
	{
		if (!isAttack)
		{
			animation.switchTime = 0.1f;
			Dash = false;
		}
	}
	else if (Dash)
	{
		animation.switchTime = 0.06f;
		if (faceRight) velocity.x += speed * 3.0f;
		else           velocity.x -= speed * 3.0f;
	}

	// Invincible
	if (Invincible and bodyBlink)
	{
		sprite.setFillColor(Color(255, 255, 255, 0));	 bodyBlink = false;
	}
	else if (Invincible and !bodyBlink)
	{
		sprite.setFillColor(Color(255, 255, 255, 255)); bodyBlink = true;
	}
	else
		sprite.setFillColor(Color(255, 255, 255, 255));

	// start falling
	if (velocity.y >= 0 and jumping)
	{
		jumping = false;
		falling = true;
		if (!isAttack)
			animation.currentImage.x = 0;
	}

	if (isAttack and !animation.playingAnimation)
	{
		isAttack = false;
		animation.currentImage.x = 0;
	}
	if (isAttack and combo == 1)	  animation.OneTimeFrame.x = 3;
	else if (isAttack and combo == 2) animation.OneTimeFrame.x = 5;
	else if (isAttack and combo == 3) animation.OneTimeFrame.x = 10;
	else if (isAttack and combo == 4) animation.OneTimeFrame.x = 6;


// Set Animation ---------------------

	// get hit
	if (getHit)
	{
		isAttack = false;
		row = 4;
		if (faceRight) velocity.x -= speed * 0.5f;
		else           velocity.x += speed * 0.5f;
	}
	// idle
	else if (velocity.x == 0 and !jumping and !falling and !Dash)
	{
		animation.switchTime = 0.1f;
		row = 0;
		animation.MaxFrame.x = 10;
	}
	// walking
	else if (velocity.x != 0 and !jumping and !falling and !Dash) 
	{
		row = 1;
		animation.MaxFrame.x = 8;
		if (velocity.x > 0.0f) faceRight = true;
		else        		   faceRight = false;
	}
	// dashing
	else if (Dash)
	{
		row = 6;
		animation.MaxFrame.x = 6;
	}
	// jumping
	else if (jumping and velocity.x == 0)                
	{
		row = 2;
		animation.MaxFrame.x = 9;
	}
	else if (jumping and velocity.x != 0)    
	{
		row = 2;
		animation.MaxFrame.x = 9;
		if (velocity.x > 0.0f) faceRight = true;
		else				   faceRight = false;
	}
	// falling
	else if (falling and velocity.x == 0)
	{
		row = 3;
		animation.MaxFrame.x = 3;
	}
	else if (falling and velocity.x != 0)                
	{
		row = 3;
		animation.MaxFrame.x = 3;
		if (velocity.x > 0.0f) faceRight = true;
		else 		           faceRight = false;
	}

	

// Animation Update ---------------------

	if (isAttack)
	{
		animation.imageCount = Vector2u(11, 4);
		sprite.setTexture(comboTexture);
		sprite.setSize(Vector2f(170.0f, 115.0f));
		sprite.setOrigin(sprite.getSize() / 2.0f);
		animation.Update(comboTexture, combo - 1, time, faceRight);
	}
	else
	{
		animation.imageCount = Vector2u(10, 7);
		sprite.setTexture(texture);
		sprite.setSize(Vector2f(128.0f, 113.0f));
		sprite.setOrigin(sprite.getSize() / 2.0f);
		animation.Update(texture, row, time, faceRight);
	}
	sprite.setTextureRect(animation.uvRect);

// Set HitBox --------------
	sprite.setPosition(position.x, position.y - 10.0f);
	if (!isAttack) 
		attackBox.setPosition(0.0f, 2000.0f);
	if (isAttack and combo != 3)
	{
		attackBox.setSize(Vector2f(50.0f, 90.0f));
		if (faceRight) attackBox.setPosition(position.x + 60.0f, position.y);
		else		   attackBox.setPosition(position.x - 60.0f, position.y);
	}
	else if (isAttack and combo == 3)
	{
		attackBox.setSize(Vector2f(90.0f, 90.0f));
		if (faceRight) attackBox.setPosition(position.x + 20.0f, position.y);
		else		   attackBox.setPosition(position.x - 20.0f, position.y);
	}


	/////////////////////
	// Show hit box
	if (Keyboard::isKeyPressed(Keyboard::RAlt))
	{
		hitBox.setFillColor   (Color::Green);
		attackBox.setFillColor(Color::Blue);
	}
	else
	{
		hitBox.setFillColor   (Color::Transparent);
		attackBox.setFillColor(Color::Transparent);
	}

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
	window.draw(hitBox);
	window.draw(attackBox);
	window.draw(sprite);
}
