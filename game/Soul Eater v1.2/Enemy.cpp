#include "Enemy.h"
using namespace sf;



Enemy::Enemy(Texture* texture, Vector2u imageCount, float x, float y, float switchTime, int type):
	animation(texture, imageCount, switchTime)
{
	row = 0;
	faceRight = true;

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
// Dark Sphere
	else if (type == 4)
	{
		body.setSize(Vector2f(190.0f, 40.0f));
		attackBox.setSize(Vector2f(80.0f, 20.0f));
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


void Enemy::Update(Texture* texture, float time, Vector2f position, Vector2f playerPosition, int enemyState, int type)
{

// Enemy live ----------------------
	if (enemyState != 0)
	{
		body.move(velocity);
		attackBox.setPosition(0.0f, 2000.0f);
		if (type != 2 and type != 4) velocity.y += 3.0f * time;
									 velocity.x  = 0.0f;
	// Tiny State 
		if (type == 0)
		{
			hitBox.setPosition(position);
			viewBox.setPosition(position);

		// Calm
			if (enemyState == 1)
			{
				if (position.x > playerPosition.x)
					velocity.x -= 0.1f;
				if (position.x < playerPosition.x)
					velocity.x += 0.1f;
			}
		// Alert!!
			else if (enemyState == 2)
				velocity.x = 0;
		// Chase!!!
			else if (enemyState == 3)
			{
				if (position.x > playerPosition.x)
					velocity.x -= 0.4f;
				if (position.x < playerPosition.x)
					velocity.x += 0.4f;
			}
		// Attacking !!!!
			else if (enemyState == 4 and animation.isCombo)
			{
				attackBox.setPosition(position);
				if (faceRight) velocity.x += 0.2f;
				else           velocity.x -= 0.2f;
			}
		}

	// Giant State
		if (type == 1)
		{
			if (faceRight) hitBox.setPosition(position.x - 10.0f, position.y + 30.0f);
			else     	   hitBox.setPosition(position.x + 10.0f, position.y + 30.0f);
			               viewBox.setPosition(position.x + 20.0f, position.y + 30.0f);
		// Calm
			if (enemyState == 1)
			{
				comboWait = 0;
				if (position.x > playerPosition.x)
					velocity.x -= 0.05f;
				if (position.x < playerPosition.x)
					velocity.x += 0.05f;
			}
		// Alert!
			else if (enemyState == 2)
				velocity.x = 0;
		// Wild!!!
			else if (enemyState == 3)
			{
				if (position.x > playerPosition.x)
					velocity.x -= 0.05f;
				if (position.x < playerPosition.x)
					velocity.x += 0.05f;
			}
		// Attacking !!!!!
			else if (enemyState == 4 and animation.isCombo and comboWait < 500)
			{
				comboWait++;
				velocity.x = 0;
			}
			else if (enemyState == 4 and animation.isCombo and comboWait >= 500)
			{
				velocity.x = 0;
				if (faceRight) 	attackBox.setPosition(position.x + 50, position.y + 50);
				else            attackBox.setPosition(position.x - 50, position.y + 50);
			}
		}

	// Ghost State
		if (type == 2)
		{
			hitBox.setPosition(position.x, position.y);
			attackBox.setPosition(position.x, position.y);

		// Flying
			if (position.x > playerPosition.x)
				velocity.x -= 0.1f;
			else if (position.x < playerPosition.x)
				velocity.x += 0.1f;
			if (position.y > playerPosition.y - 200.0f)
				velocity.y -= 0.0005f;
			else if (position.y < playerPosition.y - 200.0f)
				velocity.y += 0.0005f;
		}

	//Shaman State
		if (type == 3) 
		{
			hitBox.setPosition(position);
			viewBox.setPosition(position);

		// Calm
			if (enemyState == 1)
			{
				if (position.x > playerPosition.x)
					velocity.x -= 0.05f;
				if (position.x < playerPosition.x)
					velocity.x += 0.05f;
			}
		// Alert!
			else if (enemyState == 2)
				velocity.x = 0;
		// Cast!!!
			else if (enemyState == 4 and animation.isCombo and !castDarkSphere)
				castDarkSphere = true;
		}

	// Dark Sphere
		if (type == 4)
		{
			if (faceRight)
			{
				attackBox.setPosition(position.x + 50.0f, position.y);
				velocity.x += 1.0f;
			}
			else
			{
				attackBox.setPosition(position.x - 50.0f, position.y);
				velocity.x -= 1.0f;
			}
		}
	

	// Enemy Habits
		enemyHabit1(type, enemyState);   
		enemyHabit2(type, enemyState);  
		enemyHabit3(type, enemyState);

	// Animation
		animation.Update(texture, row, time, faceRight);
		body.setTextureRect(animation.uvRect);
		body.move(velocity * time);		
	}
	
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

void Enemy::enemyHabit1(int type, int enemyState)
{

// Tiny and Giant Animation
	if (type == 0 or type == 1)
	{
		if (velocity.x == 0 and enemyState != 4)
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
	}
}

void Enemy::Draw(RenderWindow & window)
{
	window.draw(viewBox);
	window.draw(hitBox);
	window.draw(attackBox);
	window.draw(body);
}
