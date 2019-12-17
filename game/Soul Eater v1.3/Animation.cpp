#include "Animation.h"
using namespace sf;



Animation::Animation(Texture* texture, Vector2u imageCount, float switchTime)
{
	this->imageCount = imageCount;
	this->switchTime = switchTime;
	MaxFrame = this->imageCount;
	totalTime = 0.0f;
	currentImage.x = 0;

	uvRect.width = texture->getSize().x / float(imageCount.x);
	uvRect.height = texture->getSize().y / float(imageCount.y);
}


Animation::~Animation()
{
}

void Animation::Update(Texture* texture, int row, float time, bool faceRight)
{
	uvRect.width = texture->getSize().x / float(imageCount.x);
	uvRect.height = texture->getSize().y / float(imageCount.y);

	currentImage.y = row;
	totalTime += time;

	if (totalTime >= switchTime)
	{
		totalTime -= switchTime;

		currentImage.x++;

		if (OneTimeAnimation)
		{
			if (currentImage.x >= OneTimeFrame.x)
			{
				playingAnimation = false;
				OneTimeAnimation = false;
				currentImage.x--;
			}
		}
		else
		{
			if (currentImage.x >= MaxFrame.x)
			{
				playingAnimation = false;
				currentImage.x = 0;
			}
		}
	}

	uvRect.top = currentImage.y * uvRect.height;

	if (faceRight)
	{
		uvRect.left = currentImage.x * abs(uvRect.width);
		uvRect.width = abs(uvRect.width);
	}
	else
	{
		uvRect.left = (currentImage.x + 1) * abs(uvRect.width);
		uvRect.width = -abs(uvRect.width);
	}
}

