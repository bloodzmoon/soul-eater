#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;



class Animation
{
public:
	Animation(Texture* texture, Vector2u imageCount, float switchTime);
	~Animation();

	void Update(Texture* texture, int row, float time, bool faceRight);


public:
	IntRect uvRect;
	Vector2u currentImage;

	bool isCombo;
	

private:
	Vector2u imageCount;
	
	float totalTime;
	float switchTime;
};

