#include "Interface.h"
using namespace sf;



Interface::Interface(Texture* soulTex, Texture* guiTex, Texture* blackHpTex, Texture* redHpTex, Texture* counterTex, Font* font, Vector2u imageCount, float switchTime):
	animation(soulTex, imageCount, switchTime)
{
	this->font = font;
	counter.setFont(*font);
	counter.setCharacterSize(36);
	counter.setStyle(Text::Bold | Text::Italic);

	score.setFont(*font);
	score.setCharacterSize(22);

	soul.setSize(Vector2f(70.f, 100.f));
	soul.setOrigin(soul.getSize() / 2.f);
	soul.setTexture(soulTex);

	gui.setSize(Vector2f(292.3f, 49.7f));
	gui.setPosition(1000, 0);
	gui.setTexture(guiTex);

	blackHP.setSize(Vector2f(292.3f, 49.7f));
	blackHP.setPosition(1000, 0);
	blackHP.setTexture(blackHpTex);

	redHP.setSize(Vector2f(234.9f, 22.9f));
	redHP.setPosition(1000, 0);
	redHP.setTexture(redHpTex);

	playerHP.setSize(Vector2f(234.9f, 22.9f));
	playerHP.setPosition(1000, 0);

	counterIMG.setSize(Vector2f(1167.f / 5.f, 460.f/ 5.f));
	counterIMG.setOrigin(counterIMG.getSize() / 2.0f);
	counterIMG.setPosition(1000, 0);
	counterIMG.setTexture(counterTex);

}


Interface::~Interface()
{
}

void Interface::Update(unsigned short count, unsigned int score,Texture* soulTex, View& camera, Player& player, float time, bool cutScene)
{
	this->cutScene = cutScene;
	if (cutScene)
	{
		soul.setPosition(400.f, 246.f);
	}
	else
	{
		if (player.HP >= 0)
		{
			playerHP.setSize(Vector2f(player.HP * 2.349f, 22.9f));
			redHP   .setSize(Vector2f(player.redHP * 2.349f, 22.9f));
		}
			

		soul      .setPosition(camera.getCenter().x - 380.f, camera.getCenter().y - 225.f);
		gui       .setPosition(camera.getCenter().x - 405.f, camera.getCenter().y - 230.f);
		blackHP   .setPosition(camera.getCenter().x - 405.f, camera.getCenter().y - 230.f);
		redHP     .setPosition(camera.getCenter().x - 358.f, camera.getCenter().y - 213.f);
		playerHP  .setPosition(camera.getCenter().x - 358.f, camera.getCenter().y - 213.f);
		counterIMG.setPosition(camera.getCenter().x + 290.f, camera.getCenter().y - 200.f);
		this->score.setPosition(camera.getCenter().x -360.f, camera.getCenter().y - 185.f);
		this->score.setString("score "+ to_string(score));


		if      (count < 10)
		counter   .setPosition(camera.getCenter().x + 353.f, camera.getCenter().y - 225.f);
		else if (count < 20)
		counter   .setPosition(camera.getCenter().x + 349.f, camera.getCenter().y - 225.f);
		else
		counter   .setPosition(camera.getCenter().x + 345.f, camera.getCenter().y - 225.f);
		counter.setString(to_string(count));
	}
	
	animation.Update(soulTex, 0, time, 1);
	soul.setTextureRect(animation.uvRect);
}

void Interface::Draw(RenderWindow& window)
{
	if (!cutScene)
	{
		window.draw(blackHP);
		window.draw(redHP);
		window.draw(playerHP);
		window.draw(gui);
		window.draw(soul);

		window.draw(counterIMG);
		window.draw(counter);
		window.draw(score);
	}
	else window.draw(soul);
	
}
