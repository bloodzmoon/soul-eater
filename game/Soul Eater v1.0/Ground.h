#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"
using namespace sf;


class Ground
{
public:
	Ground(Vector2f size, Vector2f position);
	~Ground();

	void Draw(RenderWindow& window);
	Collider GetCollider() { return Collider(body); }

private:
	RectangleShape body;
};

