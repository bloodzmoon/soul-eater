#include "Particle.h"



Particle::Particle(Texture* texture, Vector2u imageCount, float switchTime, int type):
	animation(texture, imageCount, switchTime)
{
	row = 0;

	if (type == -1)
		effect.setSize(Vector2f(69.f / 2.f, 69.f / 2.f));
	if (type == 0)
		effect.setSize(Vector2f(138.f / 2.f, 69.f / 2.f));
	else if (type == 1)
		effect.setSize(Vector2f(207.0f, 166.0f));
	else if (type == 2)
		effect.setSize(Vector2f(140.0f, 100.0f));
	else if (type == 3)
		effect.setSize(Vector2f(161.0f, 127.0f));
	else if (type == 4)
		effect.setSize(Vector2f(261.0f, 177.0f));
	
	
	effect.setOrigin(effect.getSize() / 2.0f);
	effect.setTexture(texture);
}


Particle::~Particle()
{
}

void Particle::Update(Texture* texture, Player& player, float time, int type)
{
	// Hit Particles ------
	if (type == 0 or type == -1)
	{
		effect.setPosition(player.GetPosition().x, player.GetPosition().y - 80.f);
	}
	else if (type == 1)
	{
		if (player.charactor == 0) effect.setSize(Vector2f(207.0f, 166.0f));
		else if (player.charactor == 1) effect.setSize(Vector2f(207.0f, 166.0f));
		else if (player.charactor == 2) effect.setSize(Vector2f(319.0f/1.6, 343.0f/1.6));
		effect.setOrigin(effect.getSize() / 2.f);
		if (isEffect and !setPosition)
		{
			
		}
		else if (!isEffect)
			effect.setPosition(0.0f, 2000.0f);
	}
	// Dust Particles -------
	else if (type == 2)
	{
		if (isEffect and !setPosition)
		{
			if (player.faceRight)
			{
				faceRight = true;
				effect.setPosition(player.GetPosition().x, player.GetPosition().y+20.0f);
			}
			else
			{
				faceRight = false;
				effect.setPosition(player.GetPosition().x, player.GetPosition().y+20.0f);
			}
			setPosition = true;
		}
		else if (!isEffect)
			effect.setPosition(0.0f, 2000.0f);
	}
	else if (type == 3)
	{
		if (isEffect and !setPosition)
		{
			if (player.charactor == 0)
			{
				if (player.faceRight)
				{
					faceRight = true;
					effect.setPosition(player.GetPosition().x + 60.f, player.GetPosition().y + 5.0f);
				}
				else
				{
					faceRight = false;
					effect.setPosition(player.GetPosition().x - 60.f, player.GetPosition().y + 5.0f);
				}
			}
			else if (player.charactor == 1)
			{
				if (player.faceRight)
				{
					faceRight = true;
					effect.setPosition(player.GetPosition().x, player.GetPosition().y);
				}
				else
				{
					faceRight = false;
					effect.setPosition(player.GetPosition().x, player.GetPosition().y);
				}
			}
			else if (player.charactor == 2)
			{
				if (player.faceRight)
				{
					faceRight = true;
					effect.setPosition(player.GetPosition().x + 20.f, player.GetPosition().y - 10.f);
				}
				else
				{
					faceRight = false;
					effect.setPosition(player.GetPosition().x - 20.f, player.GetPosition().y - 10.f);
				}
			}
			setPosition = true;
		}
		else if (!isEffect)
			effect.setPosition(0.0f, 2000.0f);
	}
	else if (type == 4)
	{
		setPosition = true;
		if (isEffect)
		{
			if (player.charactor == 0)
			{
				if (player.faceRight)
					faceRight = true;
				else
					faceRight = false;
				effect.setSize(Vector2f(261.0f, 177.0f));
				effect.setOrigin(effect.getSize() / 2.f);
			}
			else if (player.charactor == 1)
			{
				if (player.faceRight)
				{
					faceRight = true;
					effect.setPosition(player.GetPosition().x, player.GetPosition().y);
				}
				else
				{
					faceRight = false;
					effect.setPosition(player.GetPosition().x, player.GetPosition().y);
				}
				effect.setSize(Vector2f(240.f, 230.f));
				effect.setOrigin(effect.getSize() / 2.f);
			}
			else if (player.charactor == 2)
			{
				if (player.faceRight)
				{
					faceRight = true;
					effect.setPosition(player.GetPosition().x + 10.f, player.GetPosition().y - 20.f);
				}
				else
				{
					faceRight = false;
					effect.setPosition(player.GetPosition().x - 10.f, player.GetPosition().y - 20.f);
				}
				effect.setSize(Vector2f(594.0f / 1.5f, 146.0f / 1.5f));
			}
		}
		else if (!isEffect)
			effect.setPosition(0.0f, 2000.0f);
		
	}



	animation.Update(texture, row, time, faceRight);
	effect.setTextureRect(animation.uvRect);
}

void Particle::Draw(RenderWindow & window)
{
	window.draw(effect);
}
