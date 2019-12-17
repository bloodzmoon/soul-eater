#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;



class Animation
{
public:
	Animation(Texture* texture, Vector2u imageCount, float switchTime);
	~Animation();

	void Update(Texture* texture, int row, float time, bool faceRight);

	bool isCombo;

public:
	IntRect uvRect;

private:
	Vector2u imageCount;
	Vector2u currentImage;

	float totalTime;
	float switchTime;
};

