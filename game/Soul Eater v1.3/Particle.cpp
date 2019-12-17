#include "Particle.h"



Particle::Particle(Texture* texture, Vector2u imageCount, float switchTime, int type):
	animation(texture, imageCount, switchTime)
{
	row = 0;

	if (type == 1)
		effect.setSize(Vector2f(80.0f, 100.0f));
	else if (type == 2)
		effect.setSize(Vector2f(140.0f, 100.0f));
	
	effect.setOrigin(effect.getSize() / 2.0f);
	effect.setTexture(texture);
}


Particle::~Particle()
{
}

void Particle::Update(Texture* texture, Player& player ,Vector2f position, float time, int type)
{
	// Hit Particles ------
	if (type == 1)
	{
		if (isEffect and !setPosition)
		{
			if (player.faceRight)
				effect.setPosition(position.x + 50.0f, position.y);
			else
				effect.setPosition(position.x - 50.0f, position.y);
			setPosition = true;
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
				effect.setPosition(position.x, position.y+20.0f);
			}
			else
			{
				faceRight = false;
				effect.setPosition(position.x, position.y+20.0f);
			}
			setPosition = true;
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
