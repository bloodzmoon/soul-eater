#include "Ground.h"
using namespace sf;


Ground::Ground(Vector2f size, Vector2f position)
{
	body.setSize(size);
	body.setOrigin(size / 2.0f);
	body.setFillColor(Color::White);
	body.setPosition(position);
}


Ground::~Ground()
{
}

void Ground::Draw(RenderWindow & window)
{
	window.draw(body);
}
