#include "Player.h"



Player::Player(Texture* texture, Texture* texture2, Vector2u imageCount, float switchTime, int charactor):
	animation(texture, imageCount, switchTime)
{
	this->texture  = texture;
	this->texture2 = texture2;

	row = 0;
	faceRight = true;

	sprite.setSize(Vector2f(128.0f, 113.0f));
	sprite.setOrigin(sprite.getSize() / 2.0f);
	sprite.setTexture(texture);
	
	hitBox.setSize(Vector2f(50.0f, 95.0f));
	hitBox.setOrigin(hitBox.getSize() / 2.0f);

	attackBox.setSize(Vector2f(50.0f, 90.0f));
	attackBox.setOrigin(attackBox.getSize() / 2.0f);
	attackBox.setPosition(0.f, 9000.f);

	fallSoundFX.loadFromFile("Res/sound/fall.wav"); fallFX.setBuffer(fallSoundFX);
	dashSoundFX.loadFromFile("Res/sound/dash.wav"); dashFX.setBuffer(dashSoundFX);
	jumpSoundFX.loadFromFile("Res/sound/jump.wav");	jumpFX.setBuffer(jumpSoundFX);

	if (charactor == 0)
	{
		attackSoundFX.loadFromFile("Res/sound/swing.wav");
		attackFX.setBuffer(attackSoundFX);
	}
	else if (charactor == 3)
	{
		stopSoundFX.loadFromFile("Res/sound/stopTime.wav");
		stopFX.setBuffer(stopSoundFX);
	}
		
}

Player::~Player()
{
}


void Player::Update(float time, int charactor)
{
	if (charactor != 90)
	{
		// ALL Movement -----------
		if (!cutScene)
		{
			if (Keyboard::isKeyPressed(Keyboard::RShift) and HP < 100 and redHP < 100) { HP += 5; redHP += 5; Dead = false; }
			if (Keyboard::isKeyPressed(Keyboard::Left) and !getHit and !Dead)
			{
				if (charactor == 0 or charactor == 1)
				{
					if ((isAttack or Dash) and (!jumping and !falling))
						velocity.x -= speed / 6.f;
					else if (!Dash)
						velocity.x -= speed;
				}
				else if (charactor == 2)
				{
					if ((isAttack or Dash) and (!jumping and !falling))
						velocity.x = 0.f;
					else if (isAttack)
						velocity.x -= speed / 2.f;
					else if (!Dash)
						velocity.x -= speed;
				}
				else if (charactor == 3)
				{
					if (castStop)
						velocity.x = 0.f;
					else if (!castStop)
						velocity.x -= speed;
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Right) and !getHit and !Dead)
			{
				if (charactor == 0 or charactor == 1)
				{
					if ((isAttack or Dash) and (!jumping and !falling))
						velocity.x += speed / 6.f;
					else if (!Dash)
						velocity.x += speed;
				}
				else if (charactor == 2)
				{
					if ((isAttack or Dash) and (!jumping and !falling))
						velocity.x = 0.f;
					else if (isAttack)
						velocity.x += speed / 2.f;
					else if (!Dash)
						velocity.x += speed;
				}
				else if (charactor == 3)
				{
					if (castStop)
						velocity.x = 0.f;
					else if (!castStop)
						velocity.x += speed;
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Up) and canJump and !isAttack and !getHit and !Dead)
			{
				jumpFX.play();
				canJump = false;
				jumping = true;
				impactGround = true;
				velocity.y = -(jumpheight);
				combo = 0;
				animation.currentImage.x = 0;
			}
			if (Keyboard::isKeyPressed(Keyboard::Z) and !isAttack and !getHit and !Dead)
			{
				attackFX.play();
				isAttack = true;
				animation.OneTimeAnimation = true;
				animation.playingAnimation = true;
				animation.currentImage.x = 0;

				if (Dash)                    combo = 3;
				else if (jumping or falling) combo = 4;
				else
				{
					combo++;
					if (combo > 2) combo = 1;
				}

				if (faceRight) 	attackBox.setPosition(position.x + 7, position.y);
				else     		attackBox.setPosition(position.x - 7, position.y);
			}
			if (charactor == 0 or charactor == 2)
			{
				if (Keyboard::isKeyPressed(Keyboard::X) and canSpecial and !animation.playingAnimation and !Dead)
				{
					dashFX.play();
					canSpecial = false;
					Dash = true;
					dashTime.restart().asSeconds();
				}
			}
			else if (charactor == 3)
			{
				if (Keyboard::isKeyPressed(Keyboard::X) and canSpecial and !animation.playingAnimation and !Dead)
				{
					canSpecial = false;
					castStop = true;
					specialDelay.restart().asSeconds();
					animation.OneTimeAnimation = true;
					animation.playingAnimation = true;
					animation.OneTimeFrame.x = 7;
					animation.currentImage.x = 0;
					animation.switchTime = 0.1f;
				}
			}
		}


		// Player status -------------------
		// fix min max HP
		if (HP < 0) HP = 0; if (redHP < 0) redHP = 0;
		if (HP > 100) HP = 100; if (redHP > 100) redHP = 100;

		// get hit
		if (getHit and !animation.playingAnimation) getHit = false;
		if (redHP > 0 and lostHP > 0)
		{
			redHP -= 0.3f;
			lostHP -= 0.3f;
		}

		// Invincible
		if (getHit and !Invincible)
		{
			Invincible = true;
			invincibleTime.restart().asSeconds();
		}
		if (Invincible and invincibleTime.getElapsedTime().asSeconds() > 1.5f) Invincible = false;
		if (!cutScene)
		{
			if (!Invincible)                      sprite.setFillColor(Color(255, 255, 255, 255));
			else if (Invincible and  bodyBlink) { sprite.setFillColor(Color(255, 255, 255, 0)); bodyBlink = false; }
			else if (Invincible and !bodyBlink) { sprite.setFillColor(Color(255, 255, 255, 255)); bodyBlink = true; }
		}

		// Dash
		if (charactor == 0 or charactor == 2)
		{
			if (Dash and dashTime.getElapsedTime().asSeconds() > 0.25f and !isAttack)
			{
				animation.switchTime = 0.1f;
				Dash = false;
				specialDelay.restart().asSeconds();
			}
			else if (Dash)
			{
				if (charactor == 0 or charactor == 2)
				{
					if (isAttack)
					{
						if (faceRight) velocity.x += speed * 1.5f;
						else           velocity.x -= speed * 1.5f;
					}
					else
					{
						animation.switchTime = 0.06f;
						if (faceRight) velocity.x += speed * 3.f;
						else           velocity.x -= speed * 3.f;
					}
				}
			}
			if (!Dash and specialDelay.getElapsedTime().asSeconds() > 1.f) canSpecial = true;
		}

		// Stop time
		if (charactor == 3)
		{
			if (castStop and !animation.playingAnimation and !STOP)
			{
				castStop = false;
				stopFX.play();
				STOP = true;
				stopTime.restart().asSeconds();
			}
			if (STOP and stopTime.getElapsedTime().asSeconds() > 2)
			{
				animation.switchTime = 0.1f;
				STOP = false;
			}
			if (!STOP and specialDelay.getElapsedTime().asSeconds() > 6) canSpecial = true;
		}

		// start falling
		if (velocity.y >= 0 and jumping)
		{
			jumping = false;
			falling = true;

			if (!isAttack) animation.currentImage.x = 0;
		}

		// stop attacking
		if (isAttack and !animation.playingAnimation)
		{
			isAttack = false;
			animation.currentImage.x = 0;
		}

		// Attcking!!!
		if (charactor == 0)
		{
			if (isAttack and combo == 1)	  animation.OneTimeFrame.x = 3;
			else if (isAttack and combo == 2) animation.OneTimeFrame.x = 5;
			else if (isAttack and combo == 3) animation.OneTimeFrame.x = 10;
			else if (isAttack and combo == 4) animation.OneTimeFrame.x = 6;
		}
		else if (charactor == 2)
		{
			if (isAttack and combo == 1)	  animation.OneTimeFrame.x = 9;
			else if (isAttack and combo == 2) animation.OneTimeFrame.x = 9;
			else if (isAttack and combo == 3) animation.OneTimeFrame.x = 12;
			else if (isAttack and combo == 4) animation.OneTimeFrame.x = 12;
		}
		else if (charactor == 3)
		{
			if (isAttack and combo == 1)	  animation.OneTimeFrame.x = 5;
			else if (isAttack and combo == 2) animation.OneTimeFrame.x = 5;
			else if (isAttack and combo == 3) animation.OneTimeFrame.x = 7;
			else if (isAttack and combo == 4) animation.OneTimeFrame.x = 6;
		}

		// Dead
		if (HP <= 0.f and !Dead)
		{
			Dead = true;
			animation.OneTimeAnimation = true;
			animation.playingAnimation = true;
			animation.OneTimeFrame.x = 9;
			animation.currentImage.x = 0;
		}
		if (Dead and !animation.playingAnimation) animation.currentImage.x = 8;


		// Set Animation ---------------------
		// Dead
		if (Dead)
		{
			row = 5;
			if (faceRight) faceRight = true;
			else           faceRight = false;
		}
		// Get Hit
		else if (getHit)
		{
			row = 4;
			isAttack = false;
			if (charactor == 0 or charactor == 2) animation.MaxFrame.x = 5;
			else if (charactor == 3) animation.MaxFrame.x = 4;
			if (faceRight) velocity.x -= speed * 0.5f;
			else           velocity.x += speed * 0.5f;
		}
		// idle
		else if (velocity.x == 0 and !jumping and !falling and !Dash and !castStop)
		{
			row = 0;
			animation.switchTime = 0.1f;
			if (charactor == 0)      animation.MaxFrame.x = 10;
			else if (charactor == 2) animation.MaxFrame.x = 6;
			else if (charactor == 3) animation.MaxFrame.x = 8;
		}
		// walking
		else if (velocity.x != 0 and !jumping and !falling and !Dash and !castStop)
		{
			row = 1;
			animation.switchTime = 0.1f;
			if (charactor == 0)		 animation.MaxFrame.x = 8;
			else if (charactor == 2) animation.MaxFrame.x = 8;
			else if (charactor == 3) animation.MaxFrame.x = 8;
			if (velocity.x > 0.0f) faceRight = true;
			else        		   faceRight = false;
		}
		// special
		else if (Dash or castStop)
		{
			row = 6;
			if (charactor == 0 or charactor == 2) animation.MaxFrame.x = 6;
			else if (charactor == 3) animation.MaxFrame.x = 7;
		}
		// jumping
		else if (jumping and velocity.x == 0)
		{
			row = 2;
			if (charactor == 0)		 animation.MaxFrame.x = 9;
			else if (charactor == 2) animation.MaxFrame.x = 8;
			else if (charactor == 3) animation.MaxFrame.x = 8;
		}
		else if (jumping and velocity.x != 0)
		{
			row = 2;
			if (charactor == 0)      animation.MaxFrame.x = 9;
			else if (charactor == 2) animation.MaxFrame.x = 8;
			else if (charactor == 3) animation.MaxFrame.x = 8;
			if (velocity.x > 0.0f) faceRight = true;
			else				   faceRight = false;
		}
		// falling
		else if (falling and velocity.x == 0)
		{
			row = 3;
			if (charactor == 0 or charactor == 2) animation.MaxFrame.x = 3;
			else if (charactor == 3) animation.MaxFrame.x = 2;

		}
		else if (falling and velocity.x != 0)
		{
			row = 3;
			if (charactor == 0 or charactor == 2) animation.MaxFrame.x = 3;
			else if (charactor == 3) animation.MaxFrame.x = 2;
			if (velocity.x > 0.0f) faceRight = true;
			else 		           faceRight = false;
		}



		// Animation Update ---------------------

		if (charactor == 0)
		{
			if (isAttack)
			{
				sprite.setTexture(texture2);
				animation.imageCount = Vector2u(11, 4);
				sprite.setSize(Vector2f(170.0f, 115.0f));
				sprite.setOrigin(sprite.getSize() / 2.0f);

				animation.Update(texture2, combo - 1, time, faceRight);
			}
			else
			{
				sprite.setTexture(texture);
				animation.imageCount = Vector2u(10, 7);
				sprite.setSize(Vector2f(128.0f, 113.0f));
				sprite.setOrigin(sprite.getSize() / 2.0f);

				animation.Update(texture, row, time, faceRight);
			}
		}
		else if (charactor == 2)
		{
			if (isAttack)
			{
				sprite.setTexture(texture2);
				animation.imageCount = Vector2u(12, 4);
				sprite.setSize(Vector2f(189.0f, 135.0f));
				sprite.setOrigin(sprite.getSize() / 2.0f);

				animation.Update(texture2, combo - 1, time, faceRight);
			}
			else
			{
				sprite.setTexture(texture);
				animation.imageCount = Vector2u(12, 7);
				sprite.setSize(Vector2f(188.0f, 120.0f));
				sprite.setOrigin(sprite.getSize() / 2.0f);

				animation.Update(texture, row, time, faceRight);
			}
		}
		else if (charactor == 3)
		{
			if (isAttack)
			{
				sprite.setTexture(texture2);
				animation.imageCount = Vector2u(7, 4);
				sprite.setSize(Vector2f(150.0f, 117.0f));
				sprite.setOrigin(sprite.getSize() / 2.0f);

				animation.Update(texture2, combo - 1, time, faceRight);
			}
			else
			{
				sprite.setTexture(texture);
				animation.imageCount = Vector2u(13, 7);
				sprite.setSize(Vector2f(165.0f, 125.0f));
				sprite.setOrigin(sprite.getSize() / 2.0f);

				animation.Update(texture, row, time, faceRight);
			}
		}

		sprite.setTextureRect(animation.uvRect);



		// Set HitBox --------------
		if (faceRight) sprite.setPosition(position.x - 10.f, position.y - 10.f);
		else           sprite.setPosition(position.x + 10.f, position.y - 10.f);

		if (!attackBoxON) attackBox.setPosition(0.0f, 9000.0f);
		else if (attackBoxON and combo == 1)
		{
			if (charactor == 0)
			{
				attackBox.setSize(Vector2f(50.0f, 90.0f));
				attackBox.setOrigin(attackBox.getSize() / 2.f);
				if (animation.currentImage.x >= 1)
				{
					if (faceRight) attackBox.setPosition(position.x + 60.0f, position.y);
					else		   attackBox.setPosition(position.x - 60.0f, position.y);
				}
			}
			else if (charactor == 2)
			{
				attackBox.setSize(Vector2f(160.0f, 50.0f));
				attackBox.setOrigin(attackBox.getSize() / 2.f);
				if (animation.currentImage.x >= 7)
				{
					if (faceRight) attackBox.setPosition(position.x + 140.f, position.y);
					else		   attackBox.setPosition(position.x - 140.f, position.y);
				}
			}
			else if (charactor == 3)
			{
				attackBox.setSize(Vector2f(40.f, 15.f));
				attackBox.setOrigin(attackBox.getSize() / 2.f);
				if (animation.currentImage.x >= 3)
				{
					if (faceRight) attackBox.setPosition(position.x + 140.f, position.y);
					else		   attackBox.setPosition(position.x - 140.f, position.y);
				}
			}
		}
		else if (attackBoxON and combo == 2)
		{
			if (charactor == 0)
			{
				attackBox.setSize(Vector2f(50.0f, 90.0f));
				attackBox.setOrigin(attackBox.getSize() / 2.f);
				if (animation.currentImage.x >= 1)
				{
					if (faceRight) attackBox.setPosition(position.x + 60.0f, position.y);
					else		   attackBox.setPosition(position.x - 60.0f, position.y);
				}
			}
			else if (charactor == 2)
			{
				attackBox.setSize(Vector2f(160.0f, 50.0f));
				attackBox.setOrigin(attackBox.getSize() / 2.f);
				if (animation.currentImage.x >= 6)
				{
					if (faceRight) attackBox.setPosition(position.x + 140.0f, position.y);
					else		   attackBox.setPosition(position.x - 140.0f, position.y);
				}
			}
			else if (charactor == 3)
			{
				attackBox.setSize(Vector2f(40.f, 15.f));
				attackBox.setOrigin(attackBox.getSize() / 2.f);
				if (animation.currentImage.x >= 3)
				{
					if (faceRight) attackBox.setPosition(position.x + 140.f, position.y);
					else		   attackBox.setPosition(position.x - 140.f, position.y);
				}
			}
		}
		else if (attackBoxON and combo == 3)
		{
			if (charactor == 0)
			{
				attackBox.setSize(Vector2f(140.0f, 90.0f));
				attackBox.setOrigin(attackBox.getSize() / 2.f);
				if (animation.currentImage.x >= 7)
				{
					if (faceRight) attackBox.setPosition(position.x + 20.0f, position.y);
					else		   attackBox.setPosition(position.x - 20.0f, position.y);
				}
			}
			else if (charactor == 2)
			{
				attackBox.setSize(Vector2f(340.0f, 50.0f));
				attackBox.setOrigin(attackBox.getSize() / 2.f);
				if (animation.currentImage.x >= 7)
				{
					attackBox.setPosition(position.x, position.y);
				}
			}
			else if (charactor == 3)
			{
				attackBox.setSize(Vector2f(40.f, 15.f));
				attackBox.setOrigin(attackBox.getSize() / 2.f);
				if (animation.currentImage.x >= 3)
				{
					if (faceRight) attackBox.setPosition(position.x + 140.f, position.y);
					else		   attackBox.setPosition(position.x - 140.f, position.y);
				}
			}

		}
		else if (attackBoxON and combo == 4)
		{
			if (charactor == 0)
			{
				attackBox.setSize(Vector2f(50.0f, 90.0f));
				attackBox.setOrigin(attackBox.getSize() / 2.f);
				if (animation.currentImage.x >= 1)
				{
					if (faceRight) attackBox.setPosition(position.x + 60.0f, position.y);
					else		   attackBox.setPosition(position.x - 60.0f, position.y);
				}
			}
			else if (charactor == 2)
			{
				attackBox.setSize(Vector2f(90.0f, 90.0f));
				attackBox.setOrigin(attackBox.getSize() / 2.f);
				if (animation.currentImage.x >= 7)
				{
					if (faceRight) attackBox.setPosition(position.x + 60.0f, position.y);
					else		   attackBox.setPosition(position.x - 60.0f, position.y);
				}
			}
			else if (charactor == 3)
			{
				attackBox.setSize(Vector2f(40.f, 15.f));
				attackBox.setOrigin(attackBox.getSize() / 2.f);
				if (animation.currentImage.x >= 3)
				{
					if (faceRight) attackBox.setPosition(position.x + 140.f, position.y);
					else		   attackBox.setPosition(position.x - 140.f, position.y);
				}
			}
		}


		position = hitBox.getPosition();
		hitBox.move(velocity * time);
		velocity.y += 981.f * time;
		velocity.x = 0;
	}
	else
	{
		if (state == 98) hitBox.setPosition(-500, -500);
		else if (state == 1)
		{
			if (dashTime.getElapsedTime().asSeconds() > 5) state = 98;
			else
			{
				if (faceRight) velocity.x += bulletspeed;
				else velocity.x -= bulletspeed;
			}
		}

		sprite.setTextureRect(animation.uvRect);
		sprite.setPosition(position);
		position = hitBox.getPosition();
		hitBox.move(velocity * time);
		velocity.x = 0;
	}

	/////////////////////////
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
		if (!canJump) fallFX.play();

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

void Player::SetPosition(float x, float y)
{
	hitBox.setPosition(x, y);
}

void Player::SetColor(int R, int G, int B, int A)
{
	sprite.setFillColor(Color(R, G, B, A));
}

void Player::Draw(RenderWindow& window)
{
	window.draw(hitBox);
	window.draw(attackBox);
	window.draw(sprite);
}
