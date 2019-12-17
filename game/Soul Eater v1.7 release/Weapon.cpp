#include "Weapon.h"



Weapon::Weapon(Texture* texture, Vector2u imageCount, float switchTime, int type):
	animation(texture, imageCount, switchTime)
{
	this->texture = texture;

	row = 0;
	state = 99;
	faceRight = true;

	if (type == 0)
	{
		box.setSize(Vector2f(40, 15));
		sprite.setSize(Vector2f(50, 25));
	}
	else if (type == 1)
	{
		box.setSize(Vector2f(40, 15));
		sprite.setSize(Vector2f(50, 25));
	}
	else if (type == 2)
	{
		box.setSize(Vector2f(60, 35));
		sprite.setSize(Vector2f(60, 35));
	}

	box.setOrigin(box.getSize() / 2.f);
	sprite.setOrigin(sprite.getSize() / 2.f);
	box.setPosition(0,9000);
	sprite.setPosition(0, 9000);
	sprite.setTexture(texture);
}


Weapon::~Weapon()
{
}

void Weapon::Update(float time, int type)
{
	
	sprite.setPosition(box.getPosition());
	sprite.setTextureRect(animation.uvRect);
	animation.Update(texture, row, time, faceRight);

	if (state == 1)
	{
		box.move(velocity * time);
		velocity = Vector2f(0,0);

		if (faceRight) velocity.x += speed;
		else velocity.x -= speed;

		if (wait.getElapsedTime().asSeconds() > 5) state = 99;
	}
	else if (state == 99)
		box.setPosition(0, 9000);


	//// show hit box /////
	if (Keyboard::isKeyPressed(Keyboard::RAlt))
		box.setFillColor(Color::Blue);
	else box.setFillColor(Color::Transparent);
}

void Weapon::Draw(RenderWindow & window)
{
	window.draw(box);
	window.draw(sprite);
}

void Weapon::SetPosition(float x, float y)
{
	box.setPosition(x, y);
}
