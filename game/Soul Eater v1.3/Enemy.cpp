#include "Enemy.h"
using namespace sf;



Enemy::Enemy(Texture* texture, Vector2u imageCount, float x, float y, float switchTime, int type):
	animation(texture, imageCount, switchTime)
{
	row = 0;
	faceRight = true;
	state = 1;

// Tiny ----------------------
	if (type == 0)
	{
		body.setSize(Vector2f(95.0f, 75.0f));
		hitBox.setSize(Vector2f(75.0f, 60.0f));
		attackBox.setSize(Vector2f(75.0f, 60.0f));
		viewBox.setSize(Vector2f(400.0f, 200.0f));
	}
// Giant ---------------------
	else if (type == 1)
	{
		body.setSize(Vector2f(540.0f, 240.0f));
		hitBox.setSize(Vector2f(130.0f, 160.0f));
		attackBox.setSize(Vector2f(280.0f, 90.0f));
		viewBox.setSize(Vector2f(400.0f, 200.0f));
	}
// Ghost ---------------------
	else if (type == 2)
	{
		body.setSize(Vector2f(75.0f, 75.0f));
		hitBox.setSize(Vector2f(55.0f, 55.0f));
		attackBox.setSize(Vector2f(55.0f, 55.0f));
	}
// Shaman --------------------
	else if (type == 3)
	{
		body.setSize(Vector2f(85.0f, 85.0f));
		hitBox.setSize(Vector2f(55.0f, 55.0f));
		viewBox.setSize(Vector2f(900.0f, 200.0f));
	}
// Dark Sphere ---------------
	else if (type == 4)
	{
		body.setSize(Vector2f(190.0f, 40.0f));
		attackBox.setSize(Vector2f(80.0f, 20.0f));
	}
// Soul Eater ----------------
	else if (type == 5)
	{
		body.setSize(Vector2f(510.0f, 320.0f));
		hitBox.setSize(Vector2f(140.0f, 320.0f));
		attackBox.setSize(Vector2f(280.0f, 90.0f));
		viewBox.setSize(Vector2f(400.0f, 200.0f));
	}
		
	body.setOrigin(body.getSize() / 2.0f);
	body.setPosition(x, y);
	body.setTexture(texture);

	hitBox.setOrigin(hitBox.getSize() / 2.0f);
	attackBox.setOrigin(attackBox.getSize() / 2.0f);
	viewBox.setOrigin(viewBox.getSize() / 2.0f);

}

Enemy::~Enemy()
{
}


void Enemy::Update(Texture* texture, Texture* texture2, float time, Vector2f position, Vector2f playerPosition, int type)
{

	// Enemy live ----------------------
	if (state != 0)
	{
		body.move(velocity * time);
		attackBox.setPosition(0.0f, 2000.0f);

		if (type != 2 and type != 4) velocity.y += 981.0f * time;
									 velocity.x  = 0.0f;

		// Tiny State 
		if (type == 0)
		{
			hitBox.setPosition(position);
			viewBox.setPosition(position);

			// Calm
			if (state == 1)
			{
				if (position.x > playerPosition.x)
					velocity.x -= tinySpeed;
				if (position.x < playerPosition.x)
					velocity.x += tinySpeed;
			}
			// Alert!!
			else if (state == 2)
				velocity.x = 0;
			// Chase!!!
			else if (state == 3)
			{
				animation.switchTime = 0.05f;
				if (position.x > playerPosition.x)
					velocity.x -= tinySpeed * 3.0f;
				if (position.x < playerPosition.x)
					velocity.x += tinySpeed * 3.0f;
			}
			// Attacking !!!!
			else if (state == 4 and animation.playingAnimation)
			{
				animation.switchTime = 0.1f;
				attackBox.setPosition(position);
				if (faceRight) velocity.x += tinySpeed * 0.5f;
				else           velocity.x -= tinySpeed * 0.5f;
			}
			// Get Hit T-T
			else if (state == 7 and animation.playingAnimation)
			{
				animation.switchTime = 0.1f;
				if (faceRight) velocity.x -= tinySpeed * 0.5f;
				else           velocity.x += tinySpeed * 0.5f;
			}
		}

		// Giant State
		if (type == 1)
		{
			if (faceRight) hitBox.setPosition (position.x - 10.0f, position.y + 30.0f);
			else     	   hitBox.setPosition (position.x + 10.0f, position.y + 30.0f);
						   viewBox.setPosition(position.x + 20.0f, position.y + 30.0f);

			// Calm
			if (state == 1)
			{
				if (position.x > playerPosition.x)
					velocity.x -= giantSpeed;
				if (position.x < playerPosition.x)
					velocity.x += giantSpeed;
			}
			// Alert!
			else if (state == 2)
				velocity.x = 0;
			// Wild!!!
			else if (state == 3)
			{
				if (position.x > playerPosition.x)
					velocity.x -= giantSpeed;
				if (position.x < playerPosition.x)
					velocity.x += giantSpeed;
			}
			// Attacking !!!!!
			else if (state == 4)
			{
				if (comboWait.getElapsedTime().asSeconds() > 1)
				{
					velocity.x = 0;
					if (faceRight) 	attackBox.setPosition(position.x + 70, position.y + 50);
					else            attackBox.setPosition(position.x - 70, position.y + 50);
				}
				else
					velocity.x = 0;
			}
			// Get Hit T-T
			else if (state == 7 and animation.playingAnimation)
				velocity.x = 0;
		}


		// Ghost State
		if (type == 2)
		{
			hitBox.setPosition(position);
			attackBox.setPosition(position);

			// Flying
			if (state == 1)
			{
				if (position.x > playerPosition.x)
					velocity.x -= ghostSpeed;
				else if (position.x < playerPosition.x)
					velocity.x += ghostSpeed;
				if (position.y > 300.0f)
					velocity.y -= ghostSpeed * 0.08f;
				else if (position.y < 320.0f)
					velocity.y += ghostSpeed * 0.08f;
			}
			// Get Hit T-T
			else if (state == 7)
			{
				if (faceRight) velocity.x -= ghostSpeed * 4.0f;
				else           velocity.x += ghostSpeed * 4.0f;
			}
		}

		//Shaman State
		if (type == 3)
		{
			hitBox.setPosition (position);
			viewBox.setPosition(position);

			// Calm
			if (state == 1)
			{
				if (position.x > playerPosition.x)
					velocity.x -= shamanSpeed;
				if (position.x < playerPosition.x)
					velocity.x += shamanSpeed;
			}
			// Alert!
			else if (state == 2)
				velocity.x = 0;
			// Cast!!!
			else if (state == 4 and animation.playingAnimation and !castDarkSphere)
				castDarkSphere = true;
			// Get Hit T-T
			else if (state == 7)
			{
				if (faceRight) velocity.x -= shamanSpeed * 0.5f;
				else           velocity.x += shamanSpeed * 0.5f;
			}
		}

		// Dark Sphere
		if (type == 4)
		{
			if (state == 99)
			{ 
				if (faceRight)
				{
					attackBox.setPosition(position.x + 50.0f, position.y);
					velocity.x += darkSphereSpeed;
				}
				else
				{
					attackBox.setPosition(position.x - 50.0f, position.y);
					velocity.x -= darkSphereSpeed;
				}
			}
			else if (state == 1)
				body.setPosition(0.0f, 2000.0f);
		}

		// Soul Eater State
		if (type == 5)
		{
			hitBox.setPosition(position);
			viewBox.setPosition(position.x + 20.0f, position.y + 30.0f);
			// Corspe
			if (state == 1)
				velocity.x = 0;
			// Resurrect
			else if (state == 2)
				velocity.x = 0;
			// idle
			else if (state == 3)
				velocity.x = 0;
			else if (state == 4)
			{
				if (faceRight) velocity.x -= souleaterSpeed;
				else           velocity.x += souleaterSpeed;
			}
			else if (state == 5)
				velocity.x = 0.0f;
			else if (state == 6)
			{
				velocity.x = 0;
			}
			else if (state == 7)
				velocity.x = 0.0f;
		}


		// Enemy Habits
		enemyHabit1(type, state);
		enemyHabit2(type, state);
		enemyHabit3(type, state);
		enemyHabit4(type, state, texture, texture2);

		// Animation
		if (state == 6)
			animation.Update(texture2, row, time, faceRight);

		else
			animation.Update(texture, row, time, faceRight);

		body.setTextureRect(animation.uvRect);


		///////////////////////////////////////
		// Show ALL hit box Press Right ALT
		if (Keyboard::isKeyPressed(Keyboard::RAlt))
		{
			hitBox.setFillColor(Color::Green);
			attackBox.setFillColor(Color::Red);
			viewBox.setFillColor(Color(120, 120, 120, 150));
		}
		else
		{
			hitBox.setFillColor(Color::Transparent);
			attackBox.setFillColor(Color::Transparent);
			viewBox.setFillColor(Color::Transparent);
		}
	}
}

void Enemy::OnCollision(Vector2f direction, bool getHit)
{
	if (getHit)
	{
		if (direction.x < 0.0f)
		{
			//collision on left
			faceRight = false;
		}
		else if (direction.x > 0.0f)
		{
			//coliision on right
			faceRight = true;
		}
		if (direction.y < 0.0f)
		{
			//collision on bottom
		}
		else if (direction.y > 0.0f)
		{
			//collision on top
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
		}
		else if (direction.y > 0.0f)
		{
			//collision on top
			velocity.y = 0.0f;
		}
	}
	
}

void Enemy::SetPosition(float x, float y)
{
	body.setPosition(x, y);
}

void Enemy::enemyHabit1(int type, int enemyState)
{

// Tiny and Giant Animation
	if (type == 0 or type == 1)
	{
		if (velocity.x == 0 and enemyState != 4 and enemyState != 7)
			row = 0;
		else if (velocity.x != 0 and enemyState == 1)
		{
			row = 1;
			if (velocity.x > 0.0f) faceRight = true;
			else                   faceRight = false;
		}
		else if (velocity.x != 0 and enemyState == 3)
		{
			row = 2;
			if (velocity.x > 0.0f) faceRight = true;
			else                   faceRight = false;
		}
		else if (enemyState == 4)
			row = 3;
		else if (enemyState == 7)
			row = 4;
	}
}

void Enemy::enemyHabit2(int type, int enemyState)
{

// Ghost Animation
	if (type == 2)
	{
		if (velocity.x != 0 and enemyState == 1)
		{
			row = 0;
			if (velocity.x > 0.0f) faceRight = true;
			else                   faceRight = false;
		}
		else if (enemyState == 7)
			row = 1;
	}
}

void Enemy::enemyHabit3(int type, int enemyState)
{

// Shaman Aniamtion
	if (type == 3)
	{
		if (velocity.x == 0 and enemyState != 4)
			row = 0;
		else if (velocity.x != 0 and enemyState == 1)
		{
			row = 0;
			if (velocity.x > 0.0f) faceRight = true;
			else                   faceRight = false;
		}
		else if (velocity.x != 0 and enemyState == 3)
		{
			row = 0;
			if (velocity.x > 0.0f) faceRight = true;
			else                   faceRight = false;
		}
		else if (enemyState == 4)
			row = 3;
		else if (enemyState == 7)
			row = 2;
	}
}

void Enemy::enemyHabit4(int type, int enemyState, Texture* normal, Texture* cast)
{

	// Soul Eater
	if (type == 5)
	{
		if (velocity.x == 0 and state == 1)
			row = 5;
		else if (velocity.x == 0 and state == 2)
			row = 4;
		else if (velocity.x == 0 and state == 3)
			row = 0;
		else if (state == 4)
			row = 2;
		else if (state == 5)
			row = 0;
		else if (state == 6)
		{
			body.setTexture(cast);
			body.setSize(Vector2f(510.0f, 520.0f));
			body.setOrigin((hitBox.getSize()/2.0f)+Vector2f(+195.0f,+125.0f));
			animation.imageCount = Vector2u(42, 1);
			row = 0;
		}
		else if (enemyState == 7)
		{
			body.setTexture(normal);
			body.setSize(Vector2f(510.0f, 320.0f));
			body.setOrigin(body.getSize() / 2.0f);
			animation.imageCount = Vector2u(12, 6);
			row = 0;
		}
	}
}

void Enemy::Draw(RenderWindow & window)
{
	window.draw(viewBox);
	window.draw(hitBox);
	window.draw(attackBox);
	window.draw(body);
}
