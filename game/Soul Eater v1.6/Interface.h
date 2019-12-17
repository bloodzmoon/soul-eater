#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Player.h"
using namespace sf;
using namespace std;


class Interface
{
public:
	Interface(Texture* soulTex, Texture* guiTex, Texture* blackHpTex, Texture* redHpTex, Texture* enemyCount, Font* font,Vector2u imageCount, float switchTime);
	~Interface();

	void Update(unsigned short count, unsigned int score, Texture* texture, View& camera, Player& player, float time, bool cutScene);
	void Draw(RenderWindow& window);


public:

	RectangleShape soul, playerHP;
	Font *font;
	Text counter, score;


private:

	RectangleShape gui, redHP, blackHP, counterIMG;
	Animation animation;

	bool cutScene = false;

};

