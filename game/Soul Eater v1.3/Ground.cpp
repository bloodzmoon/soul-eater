#include "Ground.h"
using namespace sf;


Ground::Ground(Vector2f size, Vector2f position)
{
	body.setSize(size);
	body.setOrigin(size / 2.0f);
	body.setPosition(position);
}


Ground::~Ground()
{
}

void Ground::Draw(RenderWindow & window)
{
	if (Keyboard::isKeyPressed(Keyboard::RAlt))
		body.setFillColor(Color::White);
	else
		body.setFillColor(Color::Transparent);
	window.draw(body);
}
