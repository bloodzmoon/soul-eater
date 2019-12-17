#include "Enemy.h"



Enemy::Enemy(Texture* texture, Texture* texture2, Vector2u imageCount, float switchTime, int type):
	animation(texture, imageCount, switchTime)
{
	this->texture  = texture;
	this->texture2 = texture2;

	row = 0;
	state = 98;
	faceRight = true;

// Tiny ----------------------
	if (type == 0)
	{
		sprite.setSize(Vector2f(95.0f, 75.0f));
		hitBox.setSize(Vector2f(75.0f, 60.0f));
		attackBox.setSize(Vector2f(75.0f, 60.0f));
		viewBox.setSize(Vector2f(400.0f, 200.0f));
		HP = 40.f;
		damage = 15.f;
	}
// Giant ---------------------
	else if (type == 1)
	{
		sprite.setSize(Vector2f(540.0f, 240.0f));
		hitBox.setSize(Vector2f(130.0f, 160.0f));
		attackBox.setSize(Vector2f(280.0f, 90.0f));
		viewBox.setSize(Vector2f(400.0f, 200.0f));
		HP = 100.f;
		damage = 30.f;
	}
// Ghost ---------------------
	else if (type == 2)
	{
		sprite.setSize(Vector2f(75.0f, 75.0f));
		hitBox.setSize(Vector2f(55.0f, 55.0f));
		attackBox.setSize(Vector2f(55.0f, 55.0f));
		HP = 20.f;
		damage = 12.f;
	}
// Shaman --------------------
	else if (type == 3)
	{
		sprite.setSize(Vector2f(85.0f, 85.0f));
		hitBox.setSize(Vector2f(55.0f, 55.0f));
		viewBox.setSize(Vector2f(900.0f, 200.0f));
		HP = 30;
	}
// Dark Sphere ---------------
	else if (type == 4)
	{
		sprite.setSize(Vector2f(190.0f, 40.0f));
		attackBox.setSize(Vector2f(80.0f, 20.0f));
		damage = 15.f;
	}

// Homura --------------------
	else if (type == 6)
	{
		HP = 300.f;
		sprite.setSize(Vector2f(165.0f, 125.0f));
		hitBox.setSize(Vector2f(50.0f, 95.0f));
		attackBox.setSize(Vector2f(50.0f, 95.0f));

		shootSoundFX.loadFromFile("Res/sound/shoot.wav");
		shootFX.setBuffer(shootSoundFX);
	}
// Bullet --------------------
	else if (type == 7)
	{
		sprite.setSize(Vector2f(50.f, 25.f));
		attackBox.setSize(Vector2f(40.f, 15.f));
	}
// Rocket --------------------
	else if (type == 8)
	{
		sprite.setSize(Vector2f(60.f, 35.f));
		attackBox.setSize(Vector2f(60.f, 35.f));
	}

// heal orb ------------------
	else if (type == 44)
	{
		sprite.setSize(Vector2f(25, 55));
		hitBox.setSize(Vector2f(30, 30));
		sprite.setFillColor(Color(255, 255, 255, 150));
	}
// Portal --------------------
	else if (type == 55)
	{
		sprite.setSize(Vector2f(320.f, 320.f));
		hitBox.setSize(Vector2f(320.f, 320.f));
	}
		

	sprite.setOrigin(sprite.getSize() / 2.0f);
	sprite.setTexture(texture);

	hitBox.setOrigin(hitBox.getSize() / 2.0f);
	hitBox.setPosition(0, 1800);

	attackBox.setOrigin(attackBox.getSize() / 2.0f);
	attackBox.setPosition(500, 2000);
	viewBox  .setOrigin(viewBox.getSize() / 2.0f);

	size = sprite.getSize();

}

Enemy::~Enemy()
{
}


void Enemy::Update(float time, Player& player, int type)
{

	// Enemy live ----------------------
	if (state != 0)
	{
		position = hitBox.getPosition();
		hitBox.move(velocity * time);
		attackBox.setPosition(500, 2000);

		if (type != 2 and type != 4 and type != 55 and type != 7 and type != 8 and state != 101 and state != 98)
		{
			if (velocity.y >= 0 and jumping)
			{
				jumping = false;
				falling = true;
			}
			if(type == 44) velocity.y += 681.f * time;
			else     	   velocity.y += 981.f * time;
		}

		velocity.x  = 0.f;

		// Tiny State 
		if (type == 0)
		{
			sprite.setPosition(position.x, position.y - 10.f);
			viewBox.setPosition(position);

			// Calm
			if (state == 1)
			{
				if (position.x > player.GetPosition().x)
					velocity.x -= tinySpeed;
				if (position.x < player.GetPosition().x)
					velocity.x += tinySpeed;
			}
			// Alert!!
			else if (state == 2)
				velocity.x = 0;
			// Chase!!!
			else if (state == 3)
			{
				animation.switchTime = 0.04f;
				if (position.x > player.GetPosition().x)
					velocity.x -= tinySpeed * 3.f;
				if (position.x < player.GetPosition().x)
					velocity.x += tinySpeed * 3.f;
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
			// Dead X.X
			else if (state == 100)
			{
				velocity.x = 0;
				if (size.x > 0.f)
					size.x -= 4.f;
				if (size.y < 300.f)
					size.y += 20.f;
				if (size.x <= 0.f and size.y >= 300.f)
					Dead = true;
				sprite.setSize(Vector2f(size.x, size.y));
				sprite.setOrigin(sprite.getSize().x / 2.f, sprite.getSize().y);
				sprite.setPosition(position.x, position.y + 40.f);
			}
			else if (state == 101)
			{
				hitBox.setPosition(0.f, 1800.f);
				sprite.setSize(Vector2f(95.0f, 75.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				size = sprite.getSize();
				HP = 50.f;
				Dead = false;
				state = 98;
			}	
		}

		// Giant State
		if (type == 1)
		{
			if (faceRight) sprite.setPosition(position.x, position.y - 40.f);
			else     	   sprite.setPosition(position.x, position.y - 40.f);
						   viewBox.setPosition(position.x + 20.0f, position.y);

			// Calm
			if (state == 1)
			{
				if (position.x > player.GetPosition().x)
					velocity.x -= giantSpeed;
				if (position.x < player.GetPosition().x)
					velocity.x += giantSpeed;
			}
			// Alert!
			else if (state == 2)
				velocity.x = 0;
			// Wild!!!
			else if (state == 3)
			{
				if (position.x > player.GetPosition().x)
					velocity.x -= giantSpeed;
				if (position.x < player.GetPosition().x)
					velocity.x += giantSpeed;
			}
			// Attacking !!!!!
			else if (state == 4)
			{
				if (comboWait.getElapsedTime().asSeconds() > 1)
				{
					velocity.x = 0;
					if (faceRight) 	attackBox.setPosition(position.x + 75, position.y + 30);
					else            attackBox.setPosition(position.x - 75, position.y + 30);
				}
				else
					velocity.x = 0;
			}
			// Get Hit T-T
			else if (state == 7 and animation.playingAnimation)
				velocity.x = 0;
			// Dead X.X
			else if (state == 100)
			{
				velocity.x = 0;
				if (size.x > 0.f)
					size.x -= 30.f;
				if (size.y < 600.f)
					size.y += 45.f;
				if (size.x <= 0.f and size.y >= 600.f)
					Dead = true;
				sprite.setSize(Vector2f(size.x, size.y));
				sprite.setOrigin(sprite.getSize().x / 2.f, sprite.getSize().y);
				sprite.setPosition(position.x, position.y + 60.f);
			}
			else if (state == 101)
			{
				hitBox.setPosition(0.f, 1800.f);
				sprite.setSize(Vector2f(540.0f, 240.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				size = sprite.getSize();
				HP = 150.f;
				Dead = false;
				state = 98;
			}
		}


		// Ghost State
		if (type == 2)
		{
			sprite.setPosition(position);
			attackBox.setPosition(position);

			// Flying
			if (state == 1)
			{
				if (position.x > player.GetPosition().x)
					velocity.x -= ghostSpeed;
				else if (position.x < player.GetPosition().x)
					velocity.x += ghostSpeed;
				if (position.y > player.GetPosition().y - 200.f)
					velocity.y -= ghostSpeed * 0.08f;
				else if (position.y < player.GetPosition().y - 180.f)
					velocity.y += ghostSpeed * 0.08f;
			}
			// Get Hit T-T
			else if (state == 7)
			{
				if (faceRight) velocity.x -= ghostSpeed * 4.0f;
				else           velocity.x += ghostSpeed * 4.0f;
			}
			// Dead X.X
			else if (state == 100)
			{
				velocity.x = 0;
				if (size.x > 0.f)
					size.x -= 4.f;
				if (size.y < 300.f)
					size.y += 20.f;
				if (size.x <= 0.f and size.y >= 300.f)
					Dead = true;
				sprite.setSize(Vector2f(size.x, size.y));
				sprite.setOrigin(sprite.getSize().x / 2.f, sprite.getSize().y);
				sprite.setPosition(position.x, position.y);
			}
			else if (state == 101)
			{
				hitBox.setPosition(0.f, 1800.f);
				sprite.setSize(Vector2f(75.0f, 75.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				size = sprite.getSize();
				HP = 20.f;
				Dead = false;
				state = 98;
			}
		}

		//Shaman State
		if (type == 3)
		{
			sprite.setPosition(position.x, position.y - 10.f);
			viewBox.setPosition(position);

			// Calm
			if (state == 1)
			{
				if (position.x > player.GetPosition().x)
					velocity.x -= shamanSpeed;
				if (position.x < player.GetPosition().x)
					velocity.x += shamanSpeed;
			}
			// Alert!
			else if (state == 2)
				velocity.x = 0;
			// Cast!!!
			else if (state == 4 and animation.playingAnimation and !startShoot)
				startShoot = true;
			// Get Hit T-T
			else if (state == 7)
			{
				if (faceRight) velocity.x -= shamanSpeed * 0.5f;
				else           velocity.x += shamanSpeed * 0.5f;
			}
			// Dead X.X
			else if (state == 100)
			{
				velocity.x = 0;
				if (size.x > 0.f)
					size.x -= 4.f;
				if (size.y < 300.f)
					size.y += 20.f;
				if (size.x <= 0.f and size.y >= 300.f)
					Dead = true;
				sprite.setSize(Vector2f(size.x, size.y));
				sprite.setOrigin(sprite.getSize().x / 2.f, sprite.getSize().y);
				sprite.setPosition(position.x, position.y + 40.f);
			}
			else if (state == 101)
			{
				hitBox.setPosition(0.f, 1800.f);
				sprite.setSize(Vector2f(85.0f, 85.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				size = sprite.getSize();
				HP = 30.f;
				Dead = false;
				state = 98;
			}
		}

		// Dark Sphere
		if (type == 4)
		{
			sprite.setPosition(position);
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
				hitBox.setPosition(0.0f, 2000.0f);
		}


		// Homura State
		if (type == 6)
		{
			sprite.setPosition(position.x, position.y - 10.f);
			attackBox.setPosition(500, 2000);
			//printf("pattern = %d   state = %d\n", pattern, state);

			if (invincible and !blink)
			{
				sprite.setFillColor(Color(255,255,255,0));
				blink = true;
			}
			else if (invincible and blink)
			{
				sprite.setFillColor(Color(255, 255, 255, 255));
				blink = false;
			}
			if (STOP and stopTime.getElapsedTime().asSeconds() > 3)
				STOP = false;

			if (HP <= 0 and !Dead)
			{
				Dead = true;
				pattern = 100;
				animation.currentImage.x = 0;
				animation.OneTimeAnimation = true;
				animation.playingAnimation = true;
			}
			if (Dead and !animation.playingAnimation)
				animation.currentImage.x = 13;
			// movement
			if (pattern == 99)
			{
				if (state != 1) attackBox.setPosition(hitBox.getPosition());
				// 1 == wait
				if (state == 1 and wait.getElapsedTime().asSeconds() > 2)
				{
					if (HP > 200)
						state = 2;
					else if (HP > 100)
						state = 5;
					else if (HP > 0)
						state = 8;
				}
				// 2 == try to move
				else if (state == 2)
				{
					if (player.GetPosition().x < position.x and player.GetPosition().x >250)
					{
						waypoint.x = player.GetPosition().x - 200;
						state = 3;
					}
					else if (player.GetPosition().x < position.x and player.GetPosition().x < 250)
					{
						waypoint.x = player.GetPosition().x + 200;
						state = 3;
					}
					else if (player.GetPosition().x > position.x and player.GetPosition().x < 620)
					{
						waypoint.x = player.GetPosition().x + 200;
						state = 4;
					}
					else if (player.GetPosition().x > position.x and player.GetPosition().x > 620)
					{
						waypoint.x = player.GetPosition().x - 200;
						state = 4;
					}
				}
				// 3 == move left
				else if (state == 3)
				{
					if (position.x > waypoint.x)
						velocity.x -= 300.f;
					else
					{
						if (position.x < player.GetPosition().x)
						{
							if (!player.canJump)
								pattern = 1;
							else pattern = 0;

							state = 1;
							wait.restart().asSeconds();
							faceRight = true;
						}
						else
						{
							if (!player.canJump)
								pattern = 1;
							else pattern = 0;

							state = 1;
							wait.restart().asSeconds();
							faceRight = false;
						}
					}	
				}
				// 4 == move right
				else if (state == 4)
				{
					if (position.x < waypoint.x)
						velocity.x += 300.f;
					else
					{
						if (position.x < player.GetPosition().x)
						{
							if (!player.canJump)
								pattern = 1;
							else pattern = 0;

							state = 1;
							wait.restart().asSeconds();
							faceRight = true;
						}
						else
						{
							if (!player.canJump)
								pattern = 1;
							else pattern = 0;

							state = 1;
							wait.restart().asSeconds();
							faceRight = false;
						}
					}
				}
				// 5 == try to move 2
				else if (state == 5)
				{
					if (player.GetPosition().x < position.x and player.GetPosition().x >250)
					{
						waypoint.x = player.GetPosition().x - 200;
						state = 6;
					}
					else if (player.GetPosition().x < position.x and player.GetPosition().x < 250)
					{
						waypoint.x = player.GetPosition().x + 200;
						state = 3;
					}
					else if (player.GetPosition().x > position.x and player.GetPosition().x < 620)
					{
						waypoint.x = player.GetPosition().x + 200;
						state = 7;
					}
					else if (player.GetPosition().x > position.x and player.GetPosition().x > 620)
					{
						waypoint.x = player.GetPosition().x - 200;
						state = 4;
					}
				}
				// 6 == jump left
				else if (state == 6)
				{
					if (canJump)
					{
						canJump = false;
						jumping = true;
						hitBox.setPosition(position.x, position.y - 20);
						velocity.y -= 535.f;
					}
					if (position.x > waypoint.x)
						velocity.x -= 300.f;
					else
					{
						if (position.x < player.GetPosition().x)
						{
							pattern = 2;
							state = 1;
							wait.restart().asSeconds();
							faceRight = true;
						}
						else
						{
							pattern = 2;
							state = 1;
							wait.restart().asSeconds();
							faceRight = false;
						}
					}
				}
				// 7 == jump right
				else if (state == 7)
				{
					if (canJump)
					{
						canJump = false;
						jumping = true;
						hitBox.setPosition(position.x, position.y - 20);
						velocity.y -= 535.f;
					}
					if (position.x < waypoint.x)
						velocity.x += 300.f;
					else
					{
						if (position.x < player.GetPosition().x)
						{
							pattern = 2;
							state = 1;
							wait.restart().asSeconds();
							faceRight = true;
						}
						else
						{
							pattern = 2;
							state = 1;
							wait.restart().asSeconds();
							faceRight = false;
						}
					}
				}
				// 8 == try to move 3
				else if (state == 8)
				{
					if (stopTime.getElapsedTime().asSeconds() > 15)
					{
						state = 1;
						pattern = 3;
					}
					else if (player.GetPosition().x < position.x and player.GetPosition().x >250)
					{
						waypoint.x = player.GetPosition().x - 200;
						state = 6;
					}
					else if (player.GetPosition().x < position.x and player.GetPosition().x < 250)
					{
						waypoint.x = player.GetPosition().x + 200;
						state = 3;
					}
					else if (player.GetPosition().x > position.x and player.GetPosition().x < 620)
					{
						waypoint.x = player.GetPosition().x + 200;
						state = 7;
					}
					else if (player.GetPosition().x > position.x and player.GetPosition().x > 620)
					{
						waypoint.x = player.GetPosition().x - 200;
						state = 4;
					}
					
				}
			}

			// burst 3 shots ------
			else if (pattern == 0)
			{
				// 1 == prepare
				if (state == 1 and wait.getElapsedTime().asSeconds() > 1)
				{
					state = 2;
				}
				// 2 == prepare
				else if (state == 2)
				{
					animation.OneTimeFrame.x = 5;
					animation.playingAnimation = true;
					animation.OneTimeAnimation = true;
					animation.currentImage.x = 0;
					state = 3;
					shootFX.play();
					startShoot = true;
				}
				// 3 == shoot
				else if (state == 3 and !animation.playingAnimation)
				{
					repeat++;
					if (repeat < 3)
					{
						animation.OneTimeFrame.x = 5;
						animation.playingAnimation = true;
						animation.OneTimeAnimation = true;
						animation.currentImage.x = 0;
						shootFX.play();
						startShoot = true;
					}
					else
					{
						state = 1;
						pattern = 99;
						wait.restart().asSeconds();
						repeat = 0;
					}
				}
			}

			// jump shoot -----------
			else if (pattern == 1)
			{
				if (state == 3) attackBox.setPosition(hitBox.getPosition());
				// 1 == shoot
				if (state == 1)
				{
					animation.playingAnimation = true;
					animation.OneTimeAnimation = true;
					animation.currentImage.x = 0;
					shootFX.play();
					startShoot = true;
					state = 2;
				}
				// 2 == shoot
				else if (state == 2 and !animation.playingAnimation)
				{
					canJump = false;
					jumping = true;
					hitBox.setPosition(position.x,position.y-20);
					velocity.y -= 535.f;

					state = 3;
				}
				// 3 == jump
				else if (state == 3 and falling)
				{
					animation.playingAnimation = true;
					animation.OneTimeAnimation = true;
					animation.currentImage.x = 0;
					shootFX.play();
					startShoot = true;
					state = 4;
				}
				// jump shoot
				else if (state == 4 and !animation.playingAnimation and canJump)
				{
					state = 1;
					pattern = 99;
					wait.restart().asSeconds();
				}
			}

			// machine gun --------
			else if (pattern == 2)
			{
				//prepare
				if (state == 1)
				{
					animation.OneTimeAnimation = true;
					animation.playingAnimation = true;
					animation.currentImage.x = 0;
					state = 2;
				}
				else if (state == 2 and !animation.playingAnimation)
				{
					wait.restart().asSeconds();
					state = 3;
				}
				//shooting
				else if (state == 3 and wait.getElapsedTime().asSeconds() > 2)
				{
					state = 1;
					pattern = 99;
					wait.restart().asSeconds();
				}
				else if (state == 3 and comboWait.getElapsedTime().asSeconds() > 0.25)
				{
					shootFX.play();
					startShoot2 = true;
					comboWait.restart().asSeconds();
				}
			}

			// special -----
			else if (pattern == 3)
			{
				//cast ability
				if (state == 1)
				{
					animation.OneTimeFrame.x = 7;
					animation.playingAnimation = true;
					animation.OneTimeAnimation = true;
					animation.currentImage.x = 0;
					state = 2;
				}
				else if (state == 2 and !animation.playingAnimation)
				{
					STOP = true;
					stopTime.restart().asSeconds();
					comboWait.restart().asSeconds();
					if (position.x < 400)
					{
						waypoint.x = 50;
						state = 3;
						faceRight = false;
					}
					else
					{
						waypoint.x = 810;
						state = 4;
						faceRight = true;
					}
				}
				else if (state == 3)
				{
					if (position.x > waypoint.x)
						velocity.x -= 300.f;
					else
					{
						faceRight = true;
						state = 1;
						pattern = 4;
					}
				}
				else if (state == 4)
				{
					if (position.x < waypoint.x)
						velocity.x += 300.f;
					else
					{
						faceRight = false;
						state = 1;
						pattern = 4;
					}
				}
				
			}

			// bazooka -------------------
			else if (pattern == 4)
			{
				// 1 == prepare
				if (state == 1)
				{
					animation.OneTimeAnimation = true;
					animation.playingAnimation = true;
					animation.currentImage.x = 0;
					state = 2;
				}
				// 2 == prepare
				else if (state == 2 and !animation.playingAnimation)
				{
					animation.OneTimeAnimation = true;
					animation.playingAnimation = true;
					animation.currentImage.x = 0;
					shootFX.play();
					startShoot3 = true;
					state = 3;
				}
				// 3 == shoot
				else if (state == 3 and !animation.playingAnimation)
				{
					canJump = false;
					jumping = true;
					hitBox.setPosition(position.x, position.y -20);
					velocity.y -= 535.f;

					state = 4;
				}
				// 4 == jump
				else if (state == 4 and falling)
				{
					animation.OneTimeAnimation = true;
					animation.playingAnimation = true;
					animation.currentImage.x = 0;
					shootFX.play();
					startShoot = true;
					state = 5;
				}
				// 5 == jump shoot
				else if (state == 5 and !animation.playingAnimation and canJump)
				{
					state = 1;
					pattern = 99;
					wait.restart().asSeconds();
				}
			}
		}

		// Bullet
		if (type == 7)
		{
			sprite.setPosition(position);
			if (state == 99)
			{
				if (faceRight)
				{
					attackBox.setPosition(position);
					velocity.x += bulletSpeed;
				}
				else
				{
					attackBox.setPosition(position);
					velocity.x -= bulletSpeed;
				}
			}
			else if (state == 1)
				attackBox.setPosition(0.0f, 2000.0f);
		}
		// Bazooka
		if (type == 8)
		{
			sprite.setPosition(position);
			if (state == 99)
			{
				if (faceRight)
				{
					//attackBox.setPosition(position.x + 50.0f, position.y);
					velocity.x += bulletSpeed / 2.f;
				}
				else
				{
					//attackBox.setPosition(position.x - 50.0f, position.y);
					velocity.x -= bulletSpeed / 2.f;
				}
			}
			else if (state == 1)
				hitBox.setPosition(0.0f, 2000.0f);
		}


		// Heal orb
		if (type == 44)
		{
			if (state == 98)
			{
				sprite.setPosition(0, 1800);
				hitBox.setPosition(0, 1800);
			}
			else
			{
				if (wait.getElapsedTime().asSeconds() > 0.01)
				{
					sprite.setPosition(hitBox.getPosition().x, (hitBox.getPosition().y - 10) + sin(zeta) * 7);
					zeta += 0.1f;
					wait.restart().asSeconds();
				}
			}	
		}
		// Portal
		if (type == 55)
		{
			sprite.setPosition(hitBox.getPosition());
		}


		// Enemy Habits
		enemyHabit1(type, state);
		enemyHabit2(type, state);
		enemyHabit3(type, state);
		enemyHabit4(type, state, texture, texture2);
		enemyHabit5(type, state, texture, texture2);

		// Animation
		if (type == 6) // homura
		{
			if (pattern == 99 or pattern == 3 or pattern == 100)
			{
				animation.Update(texture, row, time, faceRight);
			}
			else if (pattern == 0 or pattern == 1 or pattern == 2 or pattern == 4)
			{
				animation.Update(texture2, row, time, faceRight);
			}
			
		}
		else
		{
			if (state == 6)
				animation.Update(texture2, row, time, faceRight);

			else
				animation.Update(texture, row, time, faceRight);
		}
		
		sprite.setTextureRect(animation.uvRect);


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
		canJump = true;
		falling = false;
		jumping = false;
	}
	else if (direction.y > 0.0f)
	{
		//collision on top
		velocity.y = 0.0f;
	}
}

void Enemy::SetPosition(float x, float y)
{
	hitBox.setPosition(x, y);
}

void Enemy::SetColor(int R, int G, int B, int A)
{
	sprite.setFillColor(Color(R, G, B, A));
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
			sprite.setTexture(cast);
			sprite.setSize(Vector2f(510.0f, 520.0f));
			sprite.setOrigin((hitBox.getSize()/2.0f)+Vector2f(+195.0f,+125.0f));
			animation.imageCount = Vector2u(42, 1);
			row = 0;
		}
		else if (enemyState == 7)
		{
			sprite.setTexture(normal);
			sprite.setSize(Vector2f(510.0f, 320.0f));
			sprite.setOrigin(sprite.getSize() / 2.0f);
			animation.imageCount = Vector2u(12, 6);
			row = 0;
		}
	}
}

void Enemy::enemyHabit5(int type, int enemyState, Texture* normal, Texture* ATK)
{

	// Homura Animation
	if (type == 6)
	{
		//Dead
		if (pattern == 100)
		{
			animation.OneTimeFrame.x = 13;
			sprite.setTexture(normal);
			sprite.setSize(Vector2f(165.0f, 125.0f));
			sprite.setOrigin(sprite.getSize() / 2.f);
			animation.imageCount = Vector2u(13, 7);
			row = 5;
		}
		// movement
		if (pattern == 99)
		{
			if (velocity.x == 0 or enemyState == 98)
			{
				animation.MaxFrame.x = 8;
				sprite.setTexture(normal);
				sprite.setSize(Vector2f(165.0f, 125.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(13, 7);
				row = 0;
			}
			else if (velocity.x != 0)
			{
				animation.MaxFrame.x = 8;
				sprite.setTexture(normal);
				sprite.setSize(Vector2f(165.0f, 125.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(13, 7);
				row = 1;
				if (velocity.x < 0)
					faceRight = false;
				else if (velocity.x > 0)
					faceRight = true;
			}
		}
		// burst 3 shot
		else if (pattern == 0)
		{
			if (enemyState == 1 or enemyState == 2)
			{
				animation.MaxFrame.x = 12;
				sprite.setTexture(ATK);
				sprite.setSize(Vector2f(150.0f, 117.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(12, 8);
				row = 0;
			}
			else if (enemyState == 3)
			{
				animation.OneTimeFrame.x = 5;
				sprite.setTexture(ATK);
				sprite.setSize(Vector2f(150.0f, 117.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(12, 8);
				row = 1;
			}
		}
		// jump shoot
		else if (pattern == 1)
		{
			if (enemyState == 1 or enemyState == 2)
			{
				animation.OneTimeFrame.x = 5;
				sprite.setTexture(ATK);
				sprite.setSize(Vector2f(150.0f, 117.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(12, 8);
				row = 1;
			}
			else if (enemyState == 3)
			{
				animation.MaxFrame.x = 12;
				sprite.setTexture(ATK);
				sprite.setSize(Vector2f(150.0f, 117.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(12, 8);
				row = 2;
			}
			else if (enemyState == 4)
			{
				animation.MaxFrame.x = 6;
				sprite.setTexture(ATK);
				sprite.setSize(Vector2f(150.0f, 117.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(12, 8);
				row = 3;
			}
		}
		// machine gun
		else if (pattern == 2)
		{
			if (enemyState == 1 or enemyState == 2)
			{
				animation.OneTimeFrame.x = 6;
				sprite.setTexture(ATK);
				sprite.setSize(Vector2f(150.0f, 117.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(12, 8);
				row = 4;
			}
			else if (enemyState == 3)
			{
				animation.MaxFrame.x = 2;
				sprite.setTexture(ATK);
				sprite.setSize(Vector2f(150.0f, 117.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(12, 8);
				row = 5;
			}
		}
		//special
		else if (pattern == 3)
		{
			if (enemyState == 1 or enemyState == 2)
			{
				animation.OneTimeFrame.x = 7;
				sprite.setTexture(normal);
				sprite.setSize(Vector2f(165.0f, 125.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(13, 7);
				row = 6;
			}
			else if (enemyState == 3 or enemyState == 4)
			{
				animation.MaxFrame.x = 8;
				sprite.setTexture(normal);
				sprite.setSize(Vector2f(165.0f, 125.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(13, 7);
				row = 1;
			}
		}
		// bazooka
		else if (pattern == 4)
		{
			if (enemyState == 1 or enemyState == 2)
			{
				animation.MaxFrame.x = 6;
				sprite.setTexture(ATK);
				sprite.setSize(Vector2f(150.0f, 117.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(12, 8);
				row = 6;
			}
			else if (enemyState == 3)
			{
				animation.OneTimeFrame.x = 6;
				sprite.setTexture(ATK);
				sprite.setSize(Vector2f(150.0f, 117.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(12, 8);
				row = 7;
			}
			else if (enemyState == 4)
			{
				animation.MaxFrame.x = 12;
				sprite.setTexture(ATK);
				sprite.setSize(Vector2f(150.0f, 117.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(12, 8);
				row = 2;
			}
			else if (enemyState == 5)
			{
				animation.OneTimeFrame.x = 6;
				sprite.setTexture(ATK);
				sprite.setSize(Vector2f(150.0f, 117.0f));
				sprite.setOrigin(sprite.getSize() / 2.f);
				animation.imageCount = Vector2u(12, 8);
				row = 3;
			}
		}
		
	}
}

void Enemy::Draw(RenderWindow & window)
{
	window.draw(viewBox);
	window.draw(hitBox);
	window.draw(attackBox);
	window.draw(sprite);
}
